/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 * Copyright (C) 2019-2021  Saiyans King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "chat.h"
#include "game.h"
#include "pugicast.h"
#include "tasks.h"

extern Chat* g_chat;

bool PrivateChatChannel::isInvited(const uint32_t guid) const
{
    if (guid == getOwner()) {
        return true;
    }
    return invites.find(guid) != invites.end();
}

bool PrivateChatChannel::removeInvite(const uint32_t guid)
{
    return invites.erase(guid) != 0;
}

void PrivateChatChannel::invitePlayer(const Player& player, Player& invitePlayer)
{
    const auto result = invites.emplace(invitePlayer.getGUID(), &invitePlayer);
    if (!result.second) {
        return;
    }

    std::stringExtended ss(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + 42);
    ss << player.getName() << " invites you to " << (player.getSex() == PLAYERSEX_FEMALE ? "her" : "his") << " private chat channel.";
    invitePlayer.sendTextMessage(MESSAGE_INFO_DESCR, ss);

    ss.clear();
    ss << invitePlayer.getName() << " has been invited.";
    player.sendTextMessage(MESSAGE_INFO_DESCR, ss);

#if GAME_FEATURE_CHAT_PLAYERLIST > 0
    for (const auto& it : users) {
        it.second->sendChannelEvent(id, invitePlayer.getName(), CHANNELEVENT_INVITE);
    }
#endif
}

void PrivateChatChannel::excludePlayer(const Player& player, Player& excludePlayer)
{
    if (!removeInvite(excludePlayer.getGUID())) {
        return;
    }

    removeUser(excludePlayer);

    std::stringExtended ss(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + 32);
    ss << excludePlayer.getName() << " has been excluded.";
    player.sendTextMessage(MESSAGE_INFO_DESCR, ss);

    excludePlayer.sendClosePrivate(id);

#if GAME_FEATURE_CHAT_PLAYERLIST > 0
    for (const auto& it : users) {
        it.second->sendChannelEvent(id, excludePlayer.getName(), CHANNELEVENT_EXCLUDE);
    }
#endif
}

void PrivateChatChannel::closeChannel() const
{
    for (const auto& it : users) {
        it.second->sendClosePrivate(id);
    }
}

bool ChatChannel::addUser(Player& player)
{
    if (users.find(player.getID()) != users.end()) {
        return false;
    }

    if (!executeOnJoinEvent(player)) {
        return false;
    }

    // TODO: Move to script when guild channels can be scripted
    if (id == CHANNEL_GUILD) {
        const Guild* guild = player.getGuild();
        if (guild && !guild->getMotd().empty()) {
            g_dispatcher.addEvent(150, [ObjectPtr = &g_game, capture0 = player.getID()] {
                ObjectPtr->sendGuildMotd(capture0);
            });
        }
    }

#if GAME_FEATURE_CHAT_PLAYERLIST > 0
    if (!publicChannel) {
        for (const auto& it : users) {
            it.second->sendChannelEvent(id, player.getName(), CHANNELEVENT_JOIN);
        }
    }
#endif

    users[player.getID()] = &player;
    return true;
}

bool ChatChannel::removeUser(const Player& player)
{
    const auto iter = users.find(player.getID());
    if (iter == users.end()) {
        return false;
    }

    users.erase(iter);

#if GAME_FEATURE_CHAT_PLAYERLIST > 0
    if (!publicChannel) {
        for (const auto& it : users) {
            it.second->sendChannelEvent(id, player.getName(), CHANNELEVENT_LEAVE);
        }
    }
#endif

    executeOnLeaveEvent(player);
    return true;
}

bool ChatChannel::hasUser(const Player& player) {
    return users.find(player.getID()) != users.end();
}

void ChatChannel::sendToAll(const std::string& message, const SpeakClasses type) const
{
    for (const auto& it : users) {
        it.second->sendChannelMessage("", message, type, id);
    }
}

bool ChatChannel::talk(const Player& fromPlayer, const SpeakClasses type, const std::string& text)
{
    if (users.find(fromPlayer.getID()) == users.end()) {
        return false;
    }

    for (const auto& it : users) {
        it.second->sendToChannel(&fromPlayer, type, text, id);
    }
    return true;
}

bool ChatChannel::executeCanJoinEvent(const Player& player) const
{
    if (canJoinEvent == -1) {
        return true;
    }

    //canJoin(player)
    LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
    if (!scriptInterface->reserveScriptEnv()) {
        std::cout << "[Error - CanJoinChannelEvent::execute] Call stack overflow" << std::endl;
        return false;
    }

    ScriptEnvironment* env = scriptInterface->getScriptEnv();
    env->setScriptId(canJoinEvent, scriptInterface);

    lua_State* L = scriptInterface->getLuaState();

    scriptInterface->pushFunction(canJoinEvent);
    LuaScriptInterface::pushUserdata(L, &player);
    LuaScriptInterface::setMetatable(L, -1, "Player");

    return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnJoinEvent(const Player& player) const
{
    if (onJoinEvent == -1) {
        return true;
    }

    //onJoin(player)
    LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
    if (!scriptInterface->reserveScriptEnv()) {
        std::cout << "[Error - OnJoinChannelEvent::execute] Call stack overflow" << std::endl;
        return false;
    }

    ScriptEnvironment* env = scriptInterface->getScriptEnv();
    env->setScriptId(onJoinEvent, scriptInterface);

    lua_State* L = scriptInterface->getLuaState();

    scriptInterface->pushFunction(onJoinEvent);
    LuaScriptInterface::pushUserdata(L, &player);
    LuaScriptInterface::setMetatable(L, -1, "Player");

    return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnLeaveEvent(const Player& player) const
{
    if (onLeaveEvent == -1) {
        return true;
    }

    //onLeave(player)
    LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
    if (!scriptInterface->reserveScriptEnv()) {
        std::cout << "[Error - OnLeaveChannelEvent::execute] Call stack overflow" << std::endl;
        return false;
    }

    ScriptEnvironment* env = scriptInterface->getScriptEnv();
    env->setScriptId(onLeaveEvent, scriptInterface);

    lua_State* L = scriptInterface->getLuaState();

    scriptInterface->pushFunction(onLeaveEvent);
    LuaScriptInterface::pushUserdata(L, &player);
    LuaScriptInterface::setMetatable(L, -1, "Player");

    return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnSpeakEvent(const Player& player, SpeakClasses& type, const std::string& message) const
{
    if (onSpeakEvent == -1) {
        return true;
    }

    //onSpeak(player, type, message)
    LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
    if (!scriptInterface->reserveScriptEnv()) {
        std::cout << "[Error - OnSpeakChannelEvent::execute] Call stack overflow" << std::endl;
        return false;
    }

    ScriptEnvironment* env = scriptInterface->getScriptEnv();
    env->setScriptId(onSpeakEvent, scriptInterface);

    lua_State* L = scriptInterface->getLuaState();

    scriptInterface->pushFunction(onSpeakEvent);
    LuaScriptInterface::pushUserdata(L, &player);
    LuaScriptInterface::setMetatable(L, -1, "Player");

    lua_pushnumber(L, type);
    LuaScriptInterface::pushString(L, message);

    bool result = false;
    const int size0 = lua_gettop(L);
    const int ret = scriptInterface->protectedCall(L, 3, 1);
    if (ret != 0) {
        LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
    } else if (lua_gettop(L) > 0) {
        if (lua_isboolean(L, -1)) {
            result = LuaScriptInterface::getBoolean(L, -1);
        } else if (lua_isnumber(L, -1)) {
            result = true;
            type = LuaScriptInterface::getNumber<SpeakClasses>(L, -1);
        }
        lua_pop(L, 1);
    }

    if (lua_gettop(L) + 4 != size0) {
        LuaScriptInterface::reportError(nullptr, "Stack size changed!");
    }
    scriptInterface->resetScriptEnv();
    return result;
}

Chat::Chat() :
    scriptInterface("Chat Interface"),
    dummyPrivate(CHANNEL_PRIVATE, "Private Chat Channel")
{
    scriptInterface.initState();
}

bool Chat::load()
{
    pugi::xml_document doc;
    const pugi::xml_parse_result result = doc.load_file("data/chatchannels/chatchannels.xml");
    if (!result) {
        printXMLError("Error - Chat::load", "data/chatchannels/chatchannels.xml", result);
        return false;
    }

    for (auto channelNode : doc.child("channels").children()) {
        uint16_t channelId = 0;
        if (pugi::xml_attribute attr = channelNode.attribute("hid")) {
            std::string tmpStrValue = asLowerCaseString(attr.as_string());
            if (!tfs_strcmp(tmpStrValue.c_str(), "help")) {
#if CLIENT_VERSION >= 871
                channelId = 7;
#elif CLIENT_VERSION == 870
                channelId = 6;
#elif CLIENT_VERSION >= 840
                channelId = 9;
#elif CLIENT_VERSION >= 790
                channelId = 8;
#else
                channelId = 7;
#endif
            } else if (!tfs_strcmp(tmpStrValue.c_str(), "advertising")) {
#if CLIENT_VERSION >= 871
                channelId = 5;
#else
                channelId = 4;
#endif
            } else if (!tfs_strcmp(tmpStrValue.c_str(), "advertising-rookgaard")) {
#if CLIENT_VERSION >= 871
                channelId = 6;
#else
                channelId = 5;
#endif
            } else if (!tfs_strcmp(tmpStrValue.c_str(), "rvr")) {
#if GAME_FEATURE_RULEVIOLATION > 0
                channelId = 3;
#else
                continue;
#endif
            }
        } else {
            channelId = pugi::cast<uint16_t>(channelNode.attribute("id").value());
        }
        std::string channelName = channelNode.attribute("name").as_string();
        const bool isPublic = channelNode.attribute("public").as_bool();
        pugi::xml_attribute scriptAttribute = channelNode.attribute("script");

        auto it = normalChannels.find(channelId);
        if (it != normalChannels.end()) {
            ChatChannel& channel = it->second;
            channel.publicChannel = isPublic;
            channel.name = channelName;

            if (scriptAttribute) {
                if (scriptInterface.loadFile("data/chatchannels/scripts/" + std::string(scriptAttribute.as_string())) == 0) {
                    channel.onSpeakEvent = scriptInterface.getEvent("onSpeak");
                    channel.canJoinEvent = scriptInterface.getEvent("canJoin");
                    channel.onJoinEvent = scriptInterface.getEvent("onJoin");
                    channel.onLeaveEvent = scriptInterface.getEvent("onLeave");
                } else {
                    std::cout << "[Warning - Chat::load] Can not load script: " << scriptAttribute.as_string() << std::endl;
                }
            }

            UsersMap tempUserMap = std::move(channel.users);
            for (const auto& pair : tempUserMap) {
                channel.addUser(*pair.second);
            }
            continue;
        }

        auto ret = normalChannels.emplace(std::piecewise_construct, std::forward_as_tuple(channelId), std::forward_as_tuple(channelId, channelName));
        ChatChannel& channel = (*ret.first).second;
        channel.publicChannel = isPublic;

        if (scriptAttribute) {
            if (scriptInterface.loadFile("data/chatchannels/scripts/" + std::string(scriptAttribute.as_string())) == 0) {
                channel.onSpeakEvent = scriptInterface.getEvent("onSpeak");
                channel.canJoinEvent = scriptInterface.getEvent("canJoin");
                channel.onJoinEvent = scriptInterface.getEvent("onJoin");
                channel.onLeaveEvent = scriptInterface.getEvent("onLeave");
            } else {
                std::cout << "[Warning - Chat::load] Can not load script: " << scriptAttribute.as_string() << std::endl;
            }
        }
        cachedChannels.push_back(channelId);
    }
    cachedChannels.shrink_to_fit();
    return true;
}

ChatChannel* Chat::createChannel(const Player& player, uint16_t channelId)
{
    if (getChannel(player, channelId)) {
        return nullptr;
    }

    switch (channelId) {
        case CHANNEL_GUILD: {
            const Guild* guild = player.getGuild();
            if (guild) {
                const auto ret = guildChannels.emplace(std::piecewise_construct, std::forward_as_tuple(guild->getId()), std::forward_as_tuple(channelId, guild->getName()));
                return &ret.first->second;
            }
            break;
        }

        case CHANNEL_PARTY: {
            Party* party = player.getParty();
            if (party) {
                const auto ret = partyChannels.emplace(std::piecewise_construct, std::forward_as_tuple(party), std::forward_as_tuple(channelId, "Party"));
                return &ret.first->second;
            }
            break;
        }

        case CHANNEL_PRIVATE: {
            //only 1 private channel for each premium player
            if (!player.isPremium() || getPrivateChannel(player)) {
                return nullptr;
            }

            //find a free private channel slot
            for (uint16_t i = 100; i < 10000; ++i) {
                const auto ret = privateChannels.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(i, player.getName() + "'s Channel"));
                if (ret.second) { //second is a bool that indicates that a new channel has been placed in the map
                    auto& newChannel = (*ret.first).second;
                    newChannel.setOwner(player.getGUID());
                    return &newChannel;
                }
            }
            break;
        }

        default:
            break;
    }
    return nullptr;
}

bool Chat::deleteChannel(const Player& player, const uint16_t channelId)
{
    switch (channelId) {
        case CHANNEL_GUILD: {
            const Guild* guild = player.getGuild();
            if (!guild) {
                return false;
            }

            const auto it = guildChannels.find(guild->getId());
            if (it == guildChannels.end()) {
                return false;
            }

            guildChannels.erase(it);
            break;
        }

        case CHANNEL_PARTY: {
            Party* party = player.getParty();
            if (!party) {
                return false;
            }

            const auto it = partyChannels.find(party);
            if (it == partyChannels.end()) {
                return false;
            }

            partyChannels.erase(it);
            break;
        }

        default: {
            const auto it = privateChannels.find(channelId);
            if (it == privateChannels.end()) {
                return false;
            }

            it->second.closeChannel();

            privateChannels.erase(it);
            break;
        }
    }
    return true;
}

ChatChannel* Chat::addUserToChannel(Player& player, const uint16_t channelId)
{
    ChatChannel* channel = getChannel(player, channelId);
    if (channel && channel->addUser(player)) {
        return channel;
    }
    return nullptr;
}

bool Chat::removeUserFromChannel(const Player& player, const uint16_t channelId)
{
    ChatChannel* channel = getChannel(player, channelId);
    if (!channel || !channel->removeUser(player)) {
        return false;
    }

    if (channel->getOwner() == player.getGUID()) {
        deleteChannel(player, channelId);
    }
    return true;
}

void Chat::removeUserFromAllChannels(const Player& player)
{
    for (auto& it : normalChannels) {
        it.second.removeUser(player);
    }

    for (auto& it : partyChannels) {
        it.second.removeUser(player);
    }

    for (auto& it : guildChannels) {
        it.second.removeUser(player);
    }

    auto it = privateChannels.begin();
    while (it != privateChannels.end()) {
        PrivateChatChannel* channel = &it->second;
        channel->removeInvite(player.getGUID());
        channel->removeUser(player);
        if (channel->getOwner() == player.getGUID()) {
            channel->closeChannel();
            it = privateChannels.erase(it);
        } else {
            ++it;
        }
    }
}

bool Chat::talkToChannel(const Player& player, SpeakClasses type, const std::string& text, const uint16_t channelId)
{
    ChatChannel* channel = getChannel(player, channelId);
    if (!channel) {
        return false;
    }

    if (channelId == CHANNEL_GUILD) {
        const GuildRank* rank = player.getGuildRank();
        if (rank && rank->level > 1) {
            type = TALKTYPE_CHANNEL_O;
        } else if (type != TALKTYPE_CHANNEL_Y) {
            type = TALKTYPE_CHANNEL_Y;
        }
    } else if (type != TALKTYPE_CHANNEL_Y && (channelId == CHANNEL_PRIVATE || channelId == CHANNEL_PARTY)) {
        type = TALKTYPE_CHANNEL_Y;
    }

    if (!channel->executeOnSpeakEvent(player, type, text)) {
        return false;
    }

    return channel->talk(player, type, text);
}

void Chat::openChannelsByServer(Player* player)
{
    const ChannelList& list = getChannelList(*player);
    for (ChatChannel* channel : list) {
        if (channel->hasUser(*player)) {
            const auto privateChannel = dynamic_cast<PrivateChatChannel*>(channel);
            if (privateChannel && privateChannel->getOwner() == player->getGUID()) {
                player->sendCreatePrivateChannel(channel->getId(), channel->getName());
            } else {
                const InvitedMap* invitedUsers = channel->getInvitedUsers();
                const UsersMap* users;
                if (!channel->isPublicChannel()) {
                    users = &channel->getUsers();
                } else {
                    users = nullptr;
                }

#if GAME_FEATURE_RULEVIOLATION > 0
                if (channel->getId() == 3) {
                    player->sendRuleViolationChannel(channel->getId());
                    continue;
                }
#endif
                player->sendChannel(channel->getId(), channel->getName(), users, invitedUsers);
            }
        }
    }
}

ChannelList Chat::getChannelList(const Player& player)
{
    ChannelList list;
    ChannelList privates;
    privates.reserve(8);

    bool hasPrivate = false;
    for (auto& it : privateChannels) {
        if (PrivateChatChannel* channel = &it.second) {
            const uint32_t guid = player.getGUID();
            if (channel->isInvited(guid)) {
                privates.push_back(channel);
            }

            if (channel->getOwner() == guid) {
                hasPrivate = true;
            }
        }
    }

    list.reserve(cachedChannels.size() + privates.size() + 3);
    if (!hasPrivate && player.isPremium()) {
        list.push_back(&dummyPrivate);
    }

    if (player.getGuild()) {
        ChatChannel* channel = getChannel(player, CHANNEL_GUILD);
        if (channel) {
            list.push_back(channel);
        } else {
            channel = createChannel(player, CHANNEL_GUILD);
            if (channel) {
                list.push_back(channel);
            }
        }
    }

    if (player.getParty()) {
        ChatChannel* channel = getChannel(player, CHANNEL_PARTY);
        if (channel) {
            list.push_back(channel);
        } else {
            channel = createChannel(player, CHANNEL_PARTY);
            if (channel) {
                list.push_back(channel);
            }
        }
    }

    for (const uint16_t channelId : cachedChannels) {
        ChatChannel* channel = getChannel(player, channelId);
        if (channel) {
            list.push_back(channel);
        }
    }

    if (!privates.empty()) {
        list.insert(list.end(), privates.begin(), privates.end());
    }
    return list;
}

ChatChannel* Chat::getChannel(const Player& player, const uint16_t channelId)
{
    switch (channelId) {
        case CHANNEL_GUILD: {
            const Guild* guild = player.getGuild();
            if (guild) {
                const auto it = guildChannels.find(guild->getId());
                if (it != guildChannels.end()) {
                    return &it->second;
                }
            }
            break;
        }

        case CHANNEL_PARTY: {
            Party* party = player.getParty();
            if (party) {
                const auto it = partyChannels.find(party);
                if (it != partyChannels.end()) {
                    return &it->second;
                }
            }
            break;
        }

        default: {
            const auto it = normalChannels.find(channelId);
            if (it != normalChannels.end()) {
                ChatChannel& channel = it->second;
                if (!channel.executeCanJoinEvent(player)) {
                    return nullptr;
                }
                return &channel;
            }
            const auto it2 = privateChannels.find(channelId);
            if (it2 != privateChannels.end() && it2->second.isInvited(player.getGUID())) {
                return &it2->second;
            }
            break;
        }
    }
    return nullptr;
}

ChatChannel* Chat::getGuildChannelById(const uint32_t guildId)
{
    const auto it = guildChannels.find(guildId);
    if (it == guildChannels.end()) {
        return nullptr;
    }
    return &it->second;
}

ChatChannel* Chat::getChannelById(const uint16_t channelId)
{
    const auto it = normalChannels.find(channelId);
    if (it == normalChannels.end()) {
        return nullptr;
    }
    return &it->second;
}

PrivateChatChannel* Chat::getPrivateChannel(const Player& player)
{
    for (auto& it : privateChannels) {
        if (it.second.getOwner() == player.getGUID()) {
            return &it.second;
        }
    }
    return nullptr;
}