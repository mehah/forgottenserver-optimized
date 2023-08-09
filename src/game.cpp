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

#include "pugicast.h"
#include "decay.h"

#include "modules.h"
#include "actions.h"
#include "bed.h"
#include "configmanager.h"
#include "creature.h"
#include "creatureevent.h"
#include "databasetasks.h"
#include "events.h"
#include "game.h"
#include "globalevent.h"
#include "iologindata.h"
#include "iomarket.h"
#include "items.h"
#include "monster.h"
#include "movement.h"
#include "server.h"
#include "spells.h"
#include "talkaction.h"
#include "weapons.h"
#include "script.h"
#include "tasks.h"

extern ConfigManager g_config;
extern Modules g_modules;
extern Actions* g_actions;
extern Chat* g_chat;
extern TalkActions* g_talkActions;
extern Spells* g_spells;

extern GlobalEvents* g_globalEvents;
extern CreatureEvents* g_creatureEvents;
extern Events* g_events;
extern Monsters g_monsters;
extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;
extern Scripts* g_scripts;

Game::Game()
{
    offlineTrainingWindow.defaultEnterButton = 1;
    offlineTrainingWindow.defaultEscapeButton = 0;
    offlineTrainingWindow.choices.emplace_back("Sword Fighting and Shielding", SKILL_SWORD);
    offlineTrainingWindow.choices.emplace_back("Axe Fighting and Shielding", SKILL_AXE);
    offlineTrainingWindow.choices.emplace_back("Club Fighting and Shielding", SKILL_CLUB);
    offlineTrainingWindow.choices.emplace_back("Distance Fighting and Shielding", SKILL_DISTANCE);
    offlineTrainingWindow.choices.emplace_back("Magic Level and Shielding", SKILL_MAGLEVEL);
    offlineTrainingWindow.choices.shrink_to_fit();
    offlineTrainingWindow.buttons.emplace_back("Okay", offlineTrainingWindow.defaultEnterButton);
    offlineTrainingWindow.buttons.emplace_back("Cancel", offlineTrainingWindow.defaultEscapeButton);
    offlineTrainingWindow.buttons.shrink_to_fit();
    offlineTrainingWindow.priority = true;
}

Game::~Game()
{
    for (const auto& it : guilds) {
        delete it.second;
    }
}

void Game::start(ServiceManager* manager)
{
    serviceManager = manager;

    g_dispatcher.addEvent(EVENT_LIGHTINTERVAL, [this] { checkLight(); });
    g_dispatcher.addEvent(EVENT_CREATURE_THINK_INTERVAL, [this] { checkCreatures(0); });
}

GameState_t Game::getGameState() const
{
    return gameState;
}

void Game::setWorldType(const WorldType_t type)
{
    worldType = type;
}

void Game::setGameState(const GameState_t newState)
{
    if (gameState == GAME_STATE_SHUTDOWN) {
        return;    //this cannot be stopped
    }

    if (gameState == newState) {
        return;
    }

    gameState = newState;
    switch (newState) {
        case GAME_STATE_INIT: {
            loadExperienceStages();

            groups.load();
            g_chat->load();

            map.spawns.startup();

            raids.loadFromXml();
            raids.startup();

            quests.loadFromXml();
#if GAME_FEATURE_MOUNTS > 0
            mounts.loadFromXml();
#endif

            const auto maxPlayers = static_cast<size_t>(g_config.getNumber(ConfigManager::MAX_PLAYERS));
            if (maxPlayers > 0) {
                players.reserve(maxPlayers);
                mappedPlayerNames.reserve(maxPlayers);
            }

            loadMotdNum();
            loadPlayersRecord();

            g_globalEvents->startup();
            break;
        }

        case GAME_STATE_SHUTDOWN: {
            g_globalEvents->execute(GLOBALEVENT_SHUTDOWN);

            //kick all players that are still online
            auto it = players.begin();
            while (it != players.end()) {
                it->second->kickPlayer(true);
                it = players.begin();
            }

            saveMotdNum();
            saveGameState();

            g_dispatcher.addTask([this] { shutdown(); });

            g_databaseTasks.stop();
            g_dispatcher.stop();
            break;
        }

        case GAME_STATE_CLOSED: {
            /* kick all players without the CanAlwaysLogin flag */
            auto it = players.begin();
            while (it != players.end()) {
                if (!it->second->hasFlag(PlayerFlag_CanAlwaysLogin)) {
                    it->second->kickPlayer(true);
                    it = players.begin();
                } else {
                    ++it;
                }
            }

            saveGameState();
            break;
        }

        default:
            break;
    }
}

void Game::saveGameState()
{
    if (gameState == GAME_STATE_NORMAL) {
        setGameState(GAME_STATE_MAINTAIN);
    }

    std::cout << "Saving server..." << std::endl;

    for (const auto& it : players) {
        it.second->loginPosition = it.second->getPosition();
        IOLoginData::savePlayer(it.second);
    }

    Map::save();

    g_databaseTasks.flush();

    if (gameState == GAME_STATE_MAINTAIN) {
        setGameState(GAME_STATE_NORMAL);
    }
}

bool Game::loadMainMap(const std::string& filename)
{
    Monster::despawnRange = g_config.getNumber(ConfigManager::DEFAULT_DESPAWNRANGE);
    Monster::despawnRadius = g_config.getNumber(ConfigManager::DEFAULT_DESPAWNRADIUS);
    return map.loadMap("data/world/" + filename + ".otbm", true);
}

void Game::loadMap(const std::string& path)
{
    map.loadMap(path, false);
}

Cylinder* Game::internalGetCylinder(Player* player, const Position& pos) const
{
    if (pos.x != 0xFFFF) {
        return map.getTile(pos);
    }

    //container
    if (pos.y & 0x40) {
        const uint8_t from_cid = pos.y & 0x0F;
        return player->getContainerByID(from_cid);
    }

    //inventory
    return player;
}

Thing* Game::internalGetThing(Player* player, const Position& pos, const int32_t index, const uint32_t spriteId, const stackPosType_t type) const
{
    if (pos.x != 0xFFFF) {
        Tile* tile = map.getTile(pos);
        if (!tile) {
            return nullptr;
        }

        Thing* thing;
        switch (type) {
            case STACKPOS_LOOK: {
                return tile->getTopVisibleThing(player);
            }

            case STACKPOS_MOVE: {
                Item* item = tile->getTopDownItem();
                if (item && item->isMoveable()) {
                    thing = item;
                } else {
                    thing = tile->getTopVisibleCreature(player);
                }
                break;
            }

            case STACKPOS_USEITEM: {
                thing = tile->getUseItem(index);
                break;
            }

            case STACKPOS_TOPDOWN_ITEM: {
                thing = tile->getTopDownItem();
                break;
            }

            case STACKPOS_USETARGET: {
                thing = tile->getTopVisibleCreature(player);
                if (!thing) {
                    thing = tile->getUseItem(index);
                }
                break;
            }

            default: {
                thing = nullptr;
                break;
            }
        }

        if (player && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
            //do extra checks here if the thing is accessable
            if (thing && thing->getItem()) {
                if (tile->hasProperty(CONST_PROP_ISVERTICAL)) {
                    if (player->getPosition().x + 1 == tile->getPosition().x) {
                        thing = nullptr;
                    }
                } else { // horizontal
                    if (player->getPosition().y + 1 == tile->getPosition().y) {
                        thing = nullptr;
                    }
                }
            }
        }
        return thing;
    }

    //container
    if (pos.y & 0x40) {
        const uint8_t fromCid = pos.y & 0x0F;

        Container* parentContainer = player->getContainerByID(fromCid);
        if (!parentContainer) {
            return nullptr;
        }

#if GAME_FEATURE_BROWSEFIELD > 0
        if (parentContainer->getID() == ITEM_BROWSEFIELD) {
            const Tile* tile = parentContainer->getTile();
            if (tile && tile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
                if (tile->hasProperty(CONST_PROP_ISVERTICAL)) {
                    if (player->getPosition().x + 1 == tile->getPosition().x) {
                        return nullptr;
                    }
                } else { // horizontal
                    if (player->getPosition().y + 1 == tile->getPosition().y) {
                        return nullptr;
                    }
                }
            }
        }
#endif

        uint8_t slot = pos.z;
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
        return parentContainer->getItemByIndex(player->getContainerIndex(fromCid) + slot);
#else
        return parentContainer->getItemByIndex(slot);
#endif
    }
    if (pos.y == 0 && pos.z == 0) {
        const ItemType& it = Item::items.getItemIdByClientId(spriteId);
        if (it.id == 0) {
            return nullptr;
        }

        int32_t subType;
        if (it.isFluidContainer()) {
            subType = clientFluidToServer(index);
        } else {
            subType = -1;
        }

        return findItemOfType(player, it.id, true, subType);
    }

    //inventory
    auto slot = static_cast<slots_t>(pos.y);
    return player->getInventoryItem(slot);
}

void Game::internalGetPosition(Item* item, Position& pos, uint8_t& stackpos)
{
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    stackpos = 0;

    Cylinder* topParent = item->getTopParent();
    if (topParent) {
        if (const auto player = dynamic_cast<Player*>(topParent)) {
            pos.x = 0xFFFF;

            const auto container = dynamic_cast<Container*>(item->getParent());
            if (container) {
                pos.y = static_cast<uint16_t>(0x40) | static_cast<uint16_t>(player->getContainerID(container));
                pos.z = container->getThingIndex(item);
                stackpos = pos.z;
            } else {
                pos.y = player->getThingIndex(item);
                stackpos = pos.y;
            }
        } else if (const Tile* tile = topParent->getTile()) {
            pos = tile->getPosition();
            stackpos = tile->getThingIndex(item);
        }
    }
}

Creature* Game::getCreatureByID(const uint32_t id)
{
    if (id <= Player::playerAutoID) {
        return getPlayerByID(id);
    }
    if (id <= Monster::monsterAutoID) {
        return getMonsterByID(id);
    }
    if (id <= Npc::npcAutoID) {
        return getNpcByID(id);
    }
    return nullptr;
}

Monster* Game::getMonsterByID(const uint32_t id)
{
    if (id == 0) {
        return nullptr;
    }

    const auto it = monsters.find(id);
    if (it == monsters.end()) {
        return nullptr;
    }
    return it->second;
}

Npc* Game::getNpcByID(const uint32_t id)
{
    if (id == 0) {
        return nullptr;
    }

    const auto it = npcs.find(id);
    if (it == npcs.end()) {
        return nullptr;
    }
    return it->second;
}

Player* Game::getPlayerByID(const uint32_t id)
{
    if (id == 0) {
        return nullptr;
    }

    const auto it = players.find(id);
    if (it == players.end()) {
        return nullptr;
    }
    return it->second;
}

Creature* Game::getCreatureByName(const std::string& s)
{
    if (s.empty()) {
        return nullptr;
    }

    const std::string& lowerCaseName = asLowerCaseString(s);

    const auto m_it = mappedPlayerNames.find(lowerCaseName);
    if (m_it != mappedPlayerNames.end()) {
        return m_it->second;
    }

    const size_t lowerCaseName_len = lowerCaseName.length();
    const char* lowerCaseName_cstr = lowerCaseName.c_str();

    for (const auto& it : npcs) {
        const std::string& npcName = it.second->getName();
        if (lowerCaseName_len == npcName.length() && !tfs_strncmp(lowerCaseName_cstr, asLowerCaseString(npcName).c_str(), lowerCaseName_len)) {
            return it.second;
        }
    }

    for (const auto& it : monsters) {
        const std::string& monsterName = it.second->getName();
        if (lowerCaseName_len == monsterName.length() && !tfs_strncmp(lowerCaseName_cstr, asLowerCaseString(monsterName).c_str(), lowerCaseName_len)) {
            return it.second;
        }
    }
    return nullptr;
}

Npc* Game::getNpcByName(const std::string& s) const
{
    if (s.empty()) {
        return nullptr;
    }

    const std::string& lowerCaseName = asLowerCaseString(s);
    const size_t lowerCaseName_len = lowerCaseName.length();
    const char* lowerCaseName_cstr = lowerCaseName.c_str();

    for (const auto& it : npcs) {
        const std::string& npcName = it.second->getName();
        if (lowerCaseName_len == npcName.length() && !tfs_strncmp(lowerCaseName_cstr, asLowerCaseString(npcName).c_str(), lowerCaseName_len)) {
            return it.second;
        }
    }
    return nullptr;
}

Player* Game::getPlayerByName(const std::string& s)
{
    if (s.empty()) {
        return nullptr;
    }

    const auto it = mappedPlayerNames.find(asLowerCaseString(s));
    if (it == mappedPlayerNames.end()) {
        return nullptr;
    }
    return it->second;
}

Player* Game::getPlayerByGUID(const uint32_t& guid) const
{
    if (guid == 0) {
        return nullptr;
    }

    for (const auto& it : players) {
        if (guid == it.second->getGUID()) {
            return it.second;
        }
    }
    return nullptr;
}

ReturnValue Game::getPlayerByNameWildcard(const std::string& s, Player*& player)
{
    const size_t strlen = s.length();
    if (strlen == 0 || strlen > NETWORKMESSAGE_PLAYERNAME_MAXLENGTH) {
        return RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE;
    }

    if (s.back() == '~') {
        const std::string& query = asLowerCaseString(s.substr(0, strlen - 1));
        std::string result;
        const ReturnValue ret = wildcardTree.findOne(query, result);
        if (ret != RETURNVALUE_NOERROR) {
            return ret;
        }

        player = getPlayerByName(result);
    } else {
        player = getPlayerByName(s);
    }

    if (!player) {
        return RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE;
    }

    return RETURNVALUE_NOERROR;
}

Player* Game::getPlayerByAccount(const uint32_t acc) const
{
    for (const auto& it : players) {
        if (it.second->getAccount() == acc) {
            return it.second;
        }
    }
    return nullptr;
}

bool Game::internalPlaceCreature(Creature* creature, const Position& pos, const bool extendedPos /*=false*/, const bool forced /*= false*/)
{
    if (creature->getParent() != nullptr) {
        return false;
    }

    if (!map.placeCreature(pos, creature, extendedPos, forced)) {
        return false;
    }

    creature->incrementReferenceCounter();
    creature->setID();
    creature->addList();
#if GAME_FEATURE_NEWSPEED_LAW > 0
    creature->cacheSpeed();
#endif
    return true;
}

bool Game::placeCreature(Creature* creature, const Position& pos, const bool extendedPos /*=false*/, const bool forced /*= false*/)
{
    if (!internalPlaceCreature(creature, pos, extendedPos, forced)) {
        return false;
    }

    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true);
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendCreatureAppear(creature, creature->getPosition(), true);
        }
    }

    for (Creature* spectator : spectators) {
        spectator->onCreatureAppear(creature, true);
    }

    creature->getParent()->postAddNotification(creature, nullptr, 0);

    addCreatureCheck(creature);
    creature->onPlacedCreature();
    return true;
}

bool Game::removeCreature(Creature* creature, const bool isLogout/* = true*/)
{
    if (creature->isRemoved()) {
        return false;
    }

    Tile* tile = creature->getTile();
    const Position& tilePosition = tile->getPosition();

    SpectatorVector spectators;
    map.getSpectators(spectators, tile->getPosition(), true);

    std::vector<int32_t> oldStackPosVector(spectators.size());
    auto i = static_cast<size_t>(-1); //Start index at -1 to avoid copying it
    for (Creature* spectator : spectators) {
        if (const Player* player = spectator->getPlayer()) {
            if (player->canSeeCreature(creature)) {
                oldStackPosVector[++i] = player->canSeeCreature(creature) ? tile->getStackposOfCreature(player, creature) : -1;
            }
        }
    }

    tile->removeCreature(creature);

    //send to client + event method
    i = static_cast<size_t>(-1); //Start index at -1 to avoid copying it
    for (Creature* spectator : spectators) {
        if (const Player* player = spectator->getPlayer()) {
            player->sendRemoveTileThing(tilePosition, oldStackPosVector[++i]);
        }

        spectator->onRemoveCreature(creature, isLogout);
    }

    if (creature->getMaster() && !creature->getMaster()->isRemoved()) {
        creature->setMaster(nullptr);
    }

    creature->getParent()->postRemoveNotification(creature, nullptr, 0);

    creature->removeList();
    creature->setRemoved();
    ReleaseCreature(creature);

    removeCreatureCheck(creature);

    for (Creature* summon : creature->summons) {
        summon->setSkillLoss(false);
        removeCreature(summon);
    }
    return true;
}

void Game::playerMoveThing(const uint32_t playerId, const Position& fromPos,
                           const uint16_t spriteId, const uint8_t fromStackPos, const Position& toPos, const uint8_t count)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    uint8_t fromIndex = 0;
    if (fromPos.x == 0xFFFF) {
        if (fromPos.y & 0x40) {
            fromIndex = fromPos.z;
        } else {
            fromIndex = static_cast<uint8_t>(fromPos.y);
        }
    } else {
        fromIndex = fromStackPos;
    }

    Thing* thing = internalGetThing(player, fromPos, fromIndex, 0, STACKPOS_MOVE);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (Creature* movingCreature = thing->getCreature()) {
        Tile* tile = map.getTile(toPos);
        if (!tile) {
            player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
            return;
        }

        if (Position::areInRange<1, 1, 0>(movingCreature->getPosition(), player->getPosition())) {
            player->setNextActionTask(1000, [this, capture0 = player->getID(), capture1 = movingCreature->getID(),
                                          capture2 = movingCreature->getPosition(), capture3 = tile->getPosition()] {
                playerMoveCreatureByID(capture0, capture1, capture2, capture3);
            });
        } else {
            playerMoveCreature(player, movingCreature, movingCreature->getPosition(), tile);
        }
    } else if (thing->getItem()) {
        Cylinder* toCylinder = internalGetCylinder(player, toPos);
        if (!toCylinder) {
            player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
            return;
        }

        playerMoveItem(player, fromPos, spriteId, fromStackPos, toPos, count, thing->getItem(), toCylinder);
    }
}

void Game::playerMoveCreatureByID(const uint32_t playerId, const uint32_t movingCreatureId, const Position& movingCreatureOrigPos, const Position& toPos)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    Creature* movingCreature = getCreatureByID(movingCreatureId);
    if (!movingCreature) {
        return;
    }

    Tile* toTile = map.getTile(toPos);
    if (!toTile) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    playerMoveCreature(player, movingCreature, movingCreatureOrigPos, toTile);
}

void Game::playerMoveCreature(Player* player, Creature* movingCreature, const Position& movingCreatureOrigPos, Tile* toTile)
{
    if (!player->canDoAction()) {
        const uint32_t delay = player->getNextActionTime();
        player->setNextActionTask(delay,
                                  [this, capture0 = player->getID(), capture1 = movingCreature->getID(),
                                      movingCreatureOrigPos, capture2 = toTile->getPosition()] {
            playerMoveCreatureByID(capture0, capture1, movingCreatureOrigPos, capture2);
        });
        return;
    }

    player->stopNextActionTask();

    if (!Position::areInRange<1, 1, 0>(movingCreatureOrigPos, player->getPosition())) {
        //need to walk to the creature first before moving it
        std::vector<Direction> listDir;
        if (player->getPathTo(movingCreatureOrigPos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(1500, [this, capture0 = player->getID(), capture1 = movingCreature->getID(),
                                              movingCreatureOrigPos, capture2 = toTile->getPosition()] {
                playerMoveCreatureByID(capture0, capture1, movingCreatureOrigPos,
                capture2);
            });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    if ((!movingCreature->isPushable() && !player->hasFlag(PlayerFlag_CanPushAllCreatures)) ||
            (movingCreature->isInGhostMode() && !player->isAccessPlayer())) {
        player->sendCancelMessage(RETURNVALUE_NOTMOVEABLE);
        return;
    }

    //check throw distance
    const Position& movingCreaturePos = movingCreature->getPosition();
    const Position& toPos = toTile->getPosition();

    const int32_t throwRange = movingCreature->getThrowRange();
    if (Position::getDistanceX(movingCreaturePos, toPos) > throwRange || Position::getDistanceY(movingCreaturePos, toPos) > throwRange ||
            Position::getDistanceZ(movingCreaturePos, toPos) > 0) {
        player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
        return;
    }

    if (player != movingCreature) {
        if (toTile->hasFlag(TILESTATE_BLOCKPATH)) {
            player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
            return;
        }
        if ((movingCreature->getZone() == ZONE_PROTECTION && !toTile->hasFlag(TILESTATE_PROTECTIONZONE)) || (movingCreature->getZone() == ZONE_NOPVP && !toTile->hasFlag(TILESTATE_NOPVPZONE))) {
            player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
            return;
        }
        if (const CreatureVector* tileCreatures = toTile->getCreatures()) {
            for (const Creature* tileCreature : *tileCreatures) {
                if (!tileCreature->isInGhostMode()) {
                    player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
                    return;
                }
            }
        }

        const Npc* movingNpc = movingCreature->getNpc();
        if (movingNpc && !Spawns::isInZone(movingNpc->getMasterPos(), movingNpc->getMasterRadius(), toPos)) {
            player->sendCancelMessage(RETURNVALUE_NOTENOUGHROOM);
            return;
        }
    }

    if (!g_events->eventPlayerOnMoveCreature(player, movingCreature, movingCreaturePos, toPos)) {
        return;
    }

    const ReturnValue ret = internalMoveCreature(*movingCreature, *toTile);
    if (ret != RETURNVALUE_NOERROR) {
        player->sendCancelMessage(ret);
    }
}

ReturnValue Game::internalMoveCreature(Creature* creature, const Direction direction, uint32_t flags /*= 0*/)
{
    creature->setLastPosition(creature->getPosition());
    const Position& currentPos = creature->getPosition();
    Position destPos = getNextPosition(direction, currentPos);
    Player* player = creature->getPlayer();

    const bool diagonalMovement = (direction & DIRECTION_DIAGONAL_MASK) != 0;
    if (player && !diagonalMovement) {
        //try go up
        if (currentPos.z != 8 && creature->getTile()->hasHeight(3)) {
            const Tile* tmpTile = map.getTile(currentPos.x, currentPos.y, currentPos.getZ() - 1);
            if (tmpTile == nullptr || (tmpTile->getGround() == nullptr && !tmpTile->hasFlag(TILESTATE_BLOCKSOLID))) {
                tmpTile = map.getTile(destPos.x, destPos.y, destPos.getZ() - 1);
                if (tmpTile && tmpTile->getGround() && !tmpTile->hasFlag(TILESTATE_BLOCKSOLID)) {
                    flags |= FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;

                    if (!tmpTile->hasFlag(TILESTATE_FLOORCHANGE)) {
                        player->setDirection(direction);
                        destPos.z--;
                    }
                }
            }
        }

        //try go down
        if (currentPos.z != 7 && currentPos.z == destPos.z) {
            const Tile* tmpTile = map.getTile(destPos.x, destPos.y, destPos.z);
            if (tmpTile == nullptr || (tmpTile->getGround() == nullptr && !tmpTile->hasFlag(TILESTATE_BLOCKSOLID))) {
                tmpTile = map.getTile(destPos.x, destPos.y, destPos.z + 1);
                if (tmpTile && tmpTile->hasHeight(3)) {
                    flags |= FLAG_IGNOREBLOCKITEM | FLAG_IGNOREBLOCKCREATURE;
                    player->setDirection(direction);
                    destPos.z++;
                }
            }
        }
    }

    Tile* toTile = map.getTile(destPos);
    if (!toTile) {
        return RETURNVALUE_NOTPOSSIBLE;
    }
    return internalMoveCreature(*creature, *toTile, flags);
}

ReturnValue Game::internalMoveCreature(Creature& creature, Tile& toTile, uint32_t flags /*= 0*/)
{
    //check if we can move the creature to the destination
    const ReturnValue ret = toTile.queryAdd(0, creature, 1, flags);
    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    map.moveCreature(creature, toTile);
    if (creature.getParent() != &toTile) {
        return RETURNVALUE_NOERROR;
    }

    int32_t index = 0;
    Item* toItem = nullptr;
    Tile* subCylinder = nullptr;
    Tile* toCylinder = &toTile;
    const Tile* fromCylinder = nullptr;
    uint32_t n = 0;

    while ((subCylinder = toCylinder->queryDestination(index, creature, &toItem, flags)) != toCylinder) {
        map.moveCreature(creature, *subCylinder);

        if (creature.getParent() != subCylinder) {
            //could happen if a script move the creature
            fromCylinder = nullptr;
            break;
        }

        fromCylinder = toCylinder;
        toCylinder = subCylinder;
        flags = 0;

        //to prevent infinite loop
        if (++n >= MAP_MAX_LAYERS) {
            break;
        }
    }

    if (fromCylinder) {
        const Position& fromPosition = fromCylinder->getPosition();
        const Position& toPosition = toCylinder->getPosition();
        if (fromPosition.z != toPosition.z && (fromPosition.x != toPosition.x || fromPosition.y != toPosition.y)) {
            const Direction dir = getDirectionTo(fromPosition, toPosition);
            if ((dir & DIRECTION_DIAGONAL_MASK) == 0) {
                internalCreatureTurn(&creature, dir);
            }
        }
    }

    return RETURNVALUE_NOERROR;
}

void Game::playerMoveItemByPlayerID(const uint32_t playerId, const Position& fromPos, const uint16_t spriteId, const uint8_t fromStackPos, const Position& toPos, const uint8_t count)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }
    playerMoveItem(player, fromPos, spriteId, fromStackPos, toPos, count, nullptr, nullptr);
}

void Game::playerMoveItem(Player* player, const Position& fromPos,
                          uint16_t spriteId, uint8_t fromStackPos, const Position& toPos, uint8_t count, Item* item, Cylinder* toCylinder)
{
    if (!player->canDoAction()) {
        const uint32_t delay = player->getNextActionTime();
        player->setNextActionTask(delay,
                                  [this, capture0 = player->getID(), fromPos, spriteId, fromStackPos, toPos, count] {
            playerMoveItemByPlayerID(capture0, fromPos, spriteId, fromStackPos, toPos, count);
        });
        return;
    }

    player->stopNextActionTask();

    if (item == nullptr) {
        uint8_t fromIndex = 0;
        if (fromPos.x == 0xFFFF) {
            if (fromPos.y & 0x40) {
                fromIndex = fromPos.z;
            } else {
                fromIndex = static_cast<uint8_t>(fromPos.y);
            }
        } else {
            fromIndex = fromStackPos;
        }

        Thing* thing = internalGetThing(player, fromPos, fromIndex, 0, STACKPOS_MOVE);
        if (!thing || !thing->getItem()) {
            player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
            return;
        }

        item = thing->getItem();
    }

    if (item->getClientID() != spriteId) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Cylinder* fromCylinder = internalGetCylinder(player, fromPos);
    if (fromCylinder == nullptr) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (toCylinder == nullptr) {
        toCylinder = internalGetCylinder(player, toPos);
        if (toCylinder == nullptr) {
            player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
            return;
        }
    }

#if GAME_FEATURE_STASH > 0
    if (Container* toCylinderContainer = toCylinder->getContainer()) {
        if (toCylinderContainer->getDepotLocker()) {
            Item* stashItem = toCylinderContainer->getItemByIndex(toPos.z);
            if (stashItem && stashItem->getID() == ITEM_SUPPLY_STASH) {
                if ((item->getContainer() || item->isPickupable()) && !item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
                    if (fromPos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(fromPos, player->getPosition())) {
                        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
                    } else if (item->getItemCount() >= count) {
                        playerStowItem(player, item, static_cast<uint32_t>(count));
                    }
                }
                return;
            }
        }
    }
#endif

    if (!item->isPushable() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTMOVEABLE);
        return;
    }

    const Position& playerPos = player->getPosition();
    const Position& mapFromPos = fromCylinder->getTile()->getPosition();
    if (playerPos.z != mapFromPos.z) {
        player->sendCancelMessage(playerPos.z > mapFromPos.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS);
        return;
    }

    if (!Position::areInRange<1, 1>(playerPos, mapFromPos)) {
        //need to walk to the item first before using it
        std::vector<Direction> listDir;
        if (player->getPathTo(item->getPosition(), listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(400, [this, capture0 = player->getID(), fromPos, spriteId, fromStackPos, toPos
                                              , count] {
                playerMoveItemByPlayerID(capture0, fromPos, spriteId, fromStackPos, toPos,
                count);
            });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    const Tile* toCylinderTile = toCylinder->getTile();
    const Position& mapToPos = toCylinderTile->getPosition();

    //hangable item specific code
    if (item->isHangable() && toCylinderTile->hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
        //destination supports hangable objects so need to move there first
        const bool vertical = toCylinderTile->hasProperty(CONST_PROP_ISVERTICAL);
        if (vertical) {
            if (playerPos.x + 1 == mapToPos.x) {
                player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
                return;
            }
        } else { // horizontal
            if (playerPos.y + 1 == mapToPos.y) {
                player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
                return;
            }
        }

        if (!Position::areInRange<1, 1, 0>(playerPos, mapToPos)) {
            Position walkPos = mapToPos;
            if (vertical) {
                walkPos.x++;
            } else {
                walkPos.y++;
            }

            Position itemPos = fromPos;
            uint8_t itemStackPos = fromStackPos;

            if (fromPos.x != 0xFFFF && Position::areInRange<1, 1>(mapFromPos, playerPos)
                    && !Position::areInRange<1, 1, 0>(mapFromPos, walkPos)) {
                //need to pickup the item first
                Item* moveItem = nullptr;

                const ReturnValue ret = internalMoveItem(fromCylinder, player, INDEX_WHEREEVER, item, count, &moveItem);
                if (ret != RETURNVALUE_NOERROR) {
                    player->sendCancelMessage(ret);
                    return;
                }

                //changing the position since its now in the inventory of the player
                internalGetPosition(moveItem, itemPos, itemStackPos);
            }

            std::vector<Direction> listDir;
            if (player->getPathTo(walkPos, listDir, 0, 0, true, false)) {
                g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                    playerAutoWalk(capture0, capture1);
                });
                player->setNextWalkActionTask(400, [this, capture0 = player->getID(), itemPos, spriteId, itemStackPos,
                                                  toPos, count] {
                    playerMoveItemByPlayerID(
                        capture0, itemPos, spriteId, itemStackPos, toPos, count);
                });
            } else {
                player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
            }
            return;
        }
    }

    const int32_t throwRange = item->getThrowRange();
    if (Position::getDistanceX(playerPos, mapToPos) > throwRange || Position::getDistanceY(playerPos, mapToPos) > throwRange ||
            (!item->isPickupable() && Position::getDistanceZ(mapFromPos, mapToPos) > 0)) {
        player->sendCancelMessage(RETURNVALUE_DESTINATIONOUTOFREACH);
        return;
    }

    if (!canThrowObjectTo(mapFromPos, mapToPos)) {
        player->sendCancelMessage(RETURNVALUE_CANNOTTHROW);
        return;
    }

    if (!g_events->eventPlayerOnMoveItem(player, item, count, fromPos, toPos, fromCylinder, toCylinder)) {
        return;
    }

    uint8_t toIndex = 0;
    if (toPos.x == 0xFFFF) {
        if (toPos.y & 0x40) {
            toIndex = toPos.z;
        } else {
            toIndex = static_cast<uint8_t>(toPos.y);
        }
    }

    const ReturnValue ret = internalMoveItem(fromCylinder, toCylinder, toIndex, item, count, nullptr, 0, player);
    if (ret != RETURNVALUE_NOERROR) {
        player->sendCancelMessage(ret);
    } else {
        g_events->eventPlayerOnItemMoved(player, item, count, fromPos, toPos, fromCylinder, toCylinder);
    }
}

ReturnValue Game::internalMoveItem(Cylinder* fromCylinder, Cylinder* toCylinder, int32_t index,
                                   Item* item, const uint32_t count, Item** _moveItem, uint32_t flags /*= 0*/, Creature* actor/* = nullptr*/, const Item* tradeItem/* = nullptr*/)
{
#if GAME_FEATURE_BROWSEFIELD > 0
    Tile* fromTile = fromCylinder->getTile();
    if (fromTile) {
        const auto it = browseFields.find(fromTile);
        if (it != browseFields.end() && it->second == fromCylinder) {
            fromCylinder = fromTile;
        }
    }
#endif

    Item* toItem = nullptr;

    Cylinder* subCylinder;
    int floorN = 0;

    while ((subCylinder = toCylinder->queryDestination(index, *item, &toItem, flags)) != toCylinder) {
        toCylinder = subCylinder;
        flags = 0;

        //to prevent infinite loop
        if (++floorN >= MAP_MAX_LAYERS) {
            break;
        }
    }

    //destination is the same as the source?
    if (item == toItem) {
        return RETURNVALUE_NOERROR;    //silently ignore move
    }

    //check if we can add this item
    ReturnValue ret = toCylinder->queryAdd(index, *item, count, flags, actor);
    if (ret == RETURNVALUE_NEEDEXCHANGE) {
        //check if we can add it to source cylinder
        ret = fromCylinder->queryAdd(fromCylinder->getThingIndex(item), *toItem, toItem->getItemCount(), 0);
        if (ret == RETURNVALUE_NOERROR) {
            //check how much we can move
            uint32_t maxExchangeQueryCount = 0;
            const ReturnValue retExchangeMaxCount = fromCylinder->queryMaxCount(INDEX_WHEREEVER, *toItem, toItem->getItemCount(), maxExchangeQueryCount, 0);

            if (retExchangeMaxCount != RETURNVALUE_NOERROR && maxExchangeQueryCount == 0) {
                return retExchangeMaxCount;
            }

            if (toCylinder->queryRemove(*toItem, toItem->getItemCount(), flags) == RETURNVALUE_NOERROR) {
                const int32_t oldToItemIndex = toCylinder->getThingIndex(toItem);
                toCylinder->removeThing(toItem, toItem->getItemCount());
                fromCylinder->addThing(toItem);

                if (oldToItemIndex != -1) {
                    toCylinder->postRemoveNotification(toItem, fromCylinder, oldToItemIndex);
                }

                const int32_t newToItemIndex = fromCylinder->getThingIndex(toItem);
                if (newToItemIndex != -1) {
                    fromCylinder->postAddNotification(toItem, toCylinder, newToItemIndex);
                }

                ret = toCylinder->queryAdd(index, *item, count, flags);
                toItem = nullptr;
            }
        }
    }

    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    //check how much we can move
    uint32_t maxQueryCount = 0;
    const ReturnValue retMaxCount = toCylinder->queryMaxCount(index, *item, count, maxQueryCount, flags);
    if (retMaxCount != RETURNVALUE_NOERROR && maxQueryCount == 0) {
        return retMaxCount;
    }

    uint32_t m;
    if (item->isStackable()) {
        m = std::min<uint32_t>(count, maxQueryCount);
    } else {
        m = maxQueryCount;
    }

    Item* moveItem = item;

    //check if we can remove this item
    ret = fromCylinder->queryRemove(*item, m, flags);
    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    if (tradeItem) {
        if (toCylinder->getItem() == tradeItem) {
            return RETURNVALUE_NOTENOUGHROOM;
        }

        Cylinder* tmpCylinder = toCylinder->getParent();
        while (tmpCylinder) {
            if (tmpCylinder->getItem() == tradeItem) {
                return RETURNVALUE_NOTENOUGHROOM;
            }

            tmpCylinder = tmpCylinder->getParent();
        }
    }

    //remove the item
    const int32_t itemIndex = fromCylinder->getThingIndex(item);
    Item* updateItem = nullptr;
    fromCylinder->removeThing(item, m);

    //update item(s)
    if (item->isStackable()) {
        uint32_t n;

        if (item->equals(toItem)) {
            n = std::min<uint32_t>(100 - toItem->getItemCount(), m);
            toCylinder->updateThing(toItem, toItem->getID(), toItem->getItemCount() + n);
            updateItem = toItem;
        } else {
            n = 0;
        }

        const int32_t newCount = m - n;
        if (newCount > 0) {
            moveItem = item->clone();
            moveItem->setItemCount(newCount);
        } else {
            moveItem = nullptr;
        }

        if (item->isRemoved()) {
            item->stopDecaying();
            ReleaseItem(item);
        }
    }

    //add item
    if (moveItem /*m - n > 0*/) {
        toCylinder->addThing(index, moveItem);
    }

    if (itemIndex != -1) {
        fromCylinder->postRemoveNotification(item, toCylinder, itemIndex);
    }

    if (moveItem) {
        const int32_t moveItemIndex = toCylinder->getThingIndex(moveItem);
        if (moveItemIndex != -1) {
            toCylinder->postAddNotification(moveItem, fromCylinder, moveItemIndex);
        }
        moveItem->startDecaying();
    }

    if (updateItem) {
        const int32_t updateItemIndex = toCylinder->getThingIndex(updateItem);
        if (updateItemIndex != -1) {
            toCylinder->postAddNotification(updateItem, fromCylinder, updateItemIndex);
        }
        updateItem->startDecaying();
    }

    if (_moveItem) {
        if (moveItem) {
            *_moveItem = moveItem;
        } else {
            *_moveItem = item;
        }
    }

    //we could not move all, inform the player
    if (item->isStackable() && maxQueryCount < count) {
        return retMaxCount;
    }

    return ret;
}

ReturnValue Game::internalAddItem(Cylinder* toCylinder, Item* item, const int32_t index /*= INDEX_WHEREEVER*/,
                                  const uint32_t flags/* = 0*/, const bool test/* = false*/)
{
    uint32_t remainderCount = 0;
    return internalAddItem(toCylinder, item, index, flags, test, remainderCount);
}

ReturnValue Game::internalAddItem(Cylinder* toCylinder, Item* item, int32_t index,
                                  uint32_t flags, const bool test, uint32_t& remainderCount)
{
    if (toCylinder == nullptr || item == nullptr) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    Cylinder* destCylinder = toCylinder;
    Item* toItem = nullptr;
    toCylinder = toCylinder->queryDestination(index, *item, &toItem, flags);

    //check if we can add this item
    ReturnValue ret = toCylinder->queryAdd(index, *item, item->getItemCount(), flags);
    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    /*
    Check if we can move add the whole amount, we do this by checking against the original cylinder,
    since the queryDestination can return a cylinder that might only hold a part of the full amount.
    */
    uint32_t maxQueryCount = 0;
    ret = destCylinder->queryMaxCount(INDEX_WHEREEVER, *item, item->getItemCount(), maxQueryCount, flags);

    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    if (test) {
        return RETURNVALUE_NOERROR;
    }

    if (item->isStackable() && item->equals(toItem)) {
        const uint32_t m = std::min<uint32_t>(item->getItemCount(), maxQueryCount);
        const uint32_t n = std::min<uint32_t>(100 - toItem->getItemCount(), m);

        toCylinder->updateThing(toItem, toItem->getID(), toItem->getItemCount() + n);

        const int32_t count = m - n;
        if (count > 0) {
            if (item->getItemCount() != count) {
                Item* remainderItem = item->clone();
                remainderItem->setItemCount(count);
                if (internalAddItem(destCylinder, remainderItem, INDEX_WHEREEVER, flags, false) != RETURNVALUE_NOERROR) {
                    ReleaseItem(remainderItem);
                    remainderCount = count;
                }
            } else {
                toCylinder->addThing(index, item);

                const int32_t itemIndex = toCylinder->getThingIndex(item);
                if (itemIndex != -1) {
                    toCylinder->postAddNotification(item, nullptr, itemIndex);
                }
            }
        } else {
            //fully merged with toItem, item will be destroyed
            item->onRemoved();
            ReleaseItem(item);

            const int32_t itemIndex = toCylinder->getThingIndex(toItem);
            if (itemIndex != -1) {
                toCylinder->postAddNotification(toItem, nullptr, itemIndex);
            }
        }
    } else {
        toCylinder->addThing(index, item);

        const int32_t itemIndex = toCylinder->getThingIndex(item);
        if (itemIndex != -1) {
            toCylinder->postAddNotification(item, nullptr, itemIndex);
        }
    }

    return RETURNVALUE_NOERROR;
}

ReturnValue Game::internalRemoveItem(Item* item, int32_t count /*= -1*/, const bool test /*= false*/, const uint32_t flags /*= 0*/)
{
    Cylinder* cylinder = item->getParent();
    if (cylinder == nullptr) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

#if GAME_FEATURE_BROWSEFIELD > 0
    Tile* fromTile = cylinder->getTile();
    if (fromTile) {
        const auto it = browseFields.find(fromTile);
        if (it != browseFields.end() && it->second == cylinder) {
            cylinder = fromTile;
        }
    }
#endif

    if (count == -1) {
        count = item->getItemCount();
    }

    //check if we can remove this item
    const ReturnValue ret = cylinder->queryRemove(*item, count, flags | FLAG_IGNORENOTMOVEABLE);
    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    if (!item->canRemove()) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    if (!test) {
        const int32_t index = cylinder->getThingIndex(item);

        //remove the item
        cylinder->removeThing(item, count);

        if (item->isRemoved()) {
            item->onRemoved();
            item->stopDecaying();
            ReleaseItem(item);
        }

        cylinder->postRemoveNotification(item, nullptr, index);
    }

    return RETURNVALUE_NOERROR;
}

#if GAME_FEATURE_FASTER_CLEAN > 0
ReturnValue Game::internalCleanItem(Item* item, int32_t count /*= -1*/)
{
    Cylinder* cylinder = item->getParent();
    if (cylinder == nullptr) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

#if GAME_FEATURE_BROWSEFIELD > 0
    Tile* fromTile = cylinder->getTile();
    if (fromTile) {
        const auto it = browseFields.find(fromTile);
        if (it != browseFields.end() && it->second == cylinder) {
            cylinder = fromTile;
        }
    }
#endif

    if (count == -1) {
        count = item->getItemCount();
    }

    //check if we can remove this item
    const ReturnValue ret = cylinder->queryRemove(*item, count, FLAG_IGNORENOTMOVEABLE);
    if (ret != RETURNVALUE_NOERROR) {
        return ret;
    }

    if (!item->canRemove()) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    const int32_t index = cylinder->getThingIndex(item);

    //remove the item
    Tile* tile = cylinder->getTile();
    if (tile) {
        tile->cleanItem(item, index, count);
    } else {
        cylinder->removeThing(item, count);
    }

    if (item->isRemoved()) {
        item->onRemoved();
        item->stopDecaying();
        ReleaseItem(item);
    }

    cylinder->postRemoveNotification(item, nullptr, index, LINK_CLEAN);
    return RETURNVALUE_NOERROR;
}
#endif

ReturnValue Game::internalPlayerAddItem(Player* player, Item* item, const bool dropOnMap /*= true*/, const slots_t slot /*= CONST_SLOT_WHEREEVER*/)
{
    ReturnValue ret = RETURNVALUE_NOTPOSSIBLE;
    if (item->isPickupable()) {
        uint32_t remainderCount = 0;
        ret = internalAddItem(player, item, slot, 0, false, remainderCount);
        if (remainderCount != 0) {
            Item* remainderItem = Item::CreateItem(item->getID(), remainderCount);
            const ReturnValue remaindRet = internalAddItem(player->getTile(), remainderItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
            if (remaindRet != RETURNVALUE_NOERROR) {
                ReleaseItem(remainderItem);
            }
        }
    }

    if (ret != RETURNVALUE_NOERROR && dropOnMap) {
        ret = internalAddItem(player->getTile(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);
    }

    return ret;
}

Item* Game::findItemOfType(const Cylinder* cylinder, const uint16_t itemId,
                           const bool depthSearch /*= true*/, const int32_t subType /*= -1*/)
{
    if (cylinder == nullptr) {
        return nullptr;
    }

    std::vector<Container*> containers;
    containers.reserve(32);

    for (size_t i = cylinder->getFirstIndex(), j = cylinder->getLastIndex(); i < j; ++i) {
        Thing* thing = cylinder->getThing(i);
        if (!thing) {
            continue;
        }

        Item* item = thing->getItem();
        if (!item) {
            continue;
        }

        if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
            return item;
        }

        if (depthSearch) {
            Container* container = item->getContainer();
            if (container) {
                containers.push_back(container);
            }
        }
    }

    auto i = static_cast<size_t>(-1);
    while (++i < containers.size()) {
        const Container* container = containers[i];
        for (Item* item : container->getItemList()) {
            if (item->getID() == itemId && (subType == -1 || subType == item->getSubType())) {
                return item;
            }

            Container* subContainer = item->getContainer();
            if (subContainer) {
                containers.push_back(subContainer);
            }
        }
    }
    return nullptr;
}

bool Game::removeMoney(Cylinder* cylinder, uint64_t money, const uint32_t flags /*= 0*/)
{
    if (cylinder == nullptr) {
        return false;
    }

    if (money == 0) {
        return true;
    }

    std::multimap<uint32_t, Item*> moneyMap;
    std::vector<Container*> containers;
    containers.reserve(32);

    uint64_t moneyCount = 0;
    for (size_t i = cylinder->getFirstIndex(), j = cylinder->getLastIndex(); i < j; ++i) {
        Thing* thing = cylinder->getThing(i);
        if (!thing) {
            continue;
        }

        Item* item = thing->getItem();
        if (!item) {
            continue;
        }

        Container* container = item->getContainer();
        if (container) {
            containers.push_back(container);
        } else {
            const uint32_t worth = item->getWorth();
            if (worth != 0) {
                moneyCount += worth;
                moneyMap.emplace(worth, item);
            }
        }
    }

    auto i = static_cast<size_t>(-1);
    while (++i < containers.size()) {
        const Container* container = containers[i];
        for (Item* item : container->getItemList()) {
            Container* tmpContainer = item->getContainer();
            if (tmpContainer) {
                containers.push_back(tmpContainer);
            } else {
                const uint32_t worth = item->getWorth();
                if (worth != 0) {
                    moneyCount += worth;
                    moneyMap.emplace(worth, item);
                }
            }
        }
    }

    if (moneyCount < money) {
        return false;
    }

    for (const auto& moneyEntry : moneyMap) {
        Item* item = moneyEntry.second;
        if (moneyEntry.first < money) {
            internalRemoveItem(item);
            money -= moneyEntry.first;
        } else if (moneyEntry.first > money) {
            const uint32_t worth = moneyEntry.first / item->getItemCount();
            const uint32_t removeCount = std::ceil(money / static_cast<double>(worth));

            addMoney(cylinder, worth * removeCount - money, flags);
            internalRemoveItem(item, removeCount);
            break;
        } else {
            internalRemoveItem(item);
            break;
        }
    }
    return true;
}

void Game::addMoney(Cylinder* cylinder, uint64_t money, const uint32_t flags /*= 0*/)
{
    if (money == 0) {
        return;
    }

    uint32_t crystalCoins = money / 10000;
    money -= crystalCoins * 10000;
    while (crystalCoins > 0) {
        const uint16_t count = std::min<uint32_t>(100, crystalCoins);

        Item* remaindItem = Item::CreateItem(ITEM_CRYSTAL_COIN, count);

        const ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
        if (ret != RETURNVALUE_NOERROR) {
            internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
        }

        crystalCoins -= count;
    }

    const uint16_t platinumCoins = money / 100;
    if (platinumCoins != 0) {
        Item* remaindItem = Item::CreateItem(ITEM_PLATINUM_COIN, platinumCoins);

        const ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
        if (ret != RETURNVALUE_NOERROR) {
            internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
        }

        money -= platinumCoins * 100;
    }

    if (money != 0) {
        Item* remaindItem = Item::CreateItem(ITEM_GOLD_COIN, money);

        const ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
        if (ret != RETURNVALUE_NOERROR) {
            internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
        }
    }
}

Item* Game::transformItem(Item* item, const uint16_t newId, const int32_t newCount /*= -1*/)
{
    if (item->getID() == newId && (newCount == -1 || (newCount == item->getSubType() && newCount != 0))) { //chargeless item placed on map = infinite
        return item;
    }

    Cylinder* cylinder = item->getParent();
    if (cylinder == nullptr) {
        return nullptr;
    }

#if GAME_FEATURE_BROWSEFIELD > 0
    Tile* fromTile = cylinder->getTile();
    if (fromTile) {
        const auto it = browseFields.find(fromTile);
        if (it != browseFields.end() && it->second == cylinder) {
            cylinder = fromTile;
        }
    }
#endif

    const int32_t itemIndex = cylinder->getThingIndex(item);
    if (itemIndex == -1) {
        return item;
    }

    if (!item->canTransform()) {
        return item;
    }

    const ItemType& newType = Item::items[newId];
    if (newType.id == 0) {
        return item;
    }

    const ItemType& curType = Item::items[item->getID()];
    if (curType.alwaysOnTop != newType.alwaysOnTop) {
        //This only occurs when you transform items on tiles from a downItem to a topItem (or vice versa)
        //Remove the old, and add the new
        cylinder->removeThing(item, item->getItemCount());
        cylinder->postRemoveNotification(item, cylinder, itemIndex);

        item->setID(newId);
        if (newCount != -1) {
            item->setSubType(newCount);
        }
        cylinder->addThing(item);

        Cylinder* newParent = item->getParent();
        if (newParent == nullptr) {
            item->stopDecaying();
            ReleaseItem(item);
            return nullptr;
        }

        newParent->postAddNotification(item, cylinder, newParent->getThingIndex(item));
        item->startDecaying();

        return item;
    }

    if (curType.type == newType.type) {
        //Both items has the same type so we can safely change id/subtype
        if (newCount == 0 && (item->isStackable() || item->hasAttribute(ITEM_ATTRIBUTE_CHARGES))) {
            if (item->isStackable()) {
                internalRemoveItem(item);
                return nullptr;
            }
            int32_t newItemId = newId;
            if (curType.id == newType.id) {
                newItemId = curType.decayTo;
            }

            if (newItemId < 0) {
                internalRemoveItem(item);
                return nullptr;
            }
            if (newItemId != newId) {
                //Replacing the the old item with the new while maintaining the old position
                Item* newItem = Item::CreateItem(newItemId, 1);
                if (newItem == nullptr) {
                    return nullptr;
                }

                cylinder->replaceThing(itemIndex, newItem);
                cylinder->postAddNotification(newItem, cylinder, itemIndex);

                item->setParent(nullptr);
                cylinder->postRemoveNotification(item, cylinder, itemIndex);
                item->stopDecaying();
                ReleaseItem(item);
                newItem->startDecaying();

                return newItem;
            }
            return transformItem(item, newItemId);
        }
        cylinder->postRemoveNotification(item, cylinder, itemIndex);
        uint16_t itemId = item->getID();
        int32_t count = item->getSubType();

        if (curType.id != newType.id) {
            if (newType.group != curType.group) {
                item->setDefaultSubtype();
            }

            itemId = newId;
        }

        if (newCount != -1 && newType.hasSubType()) {
            count = newCount;
        }

        cylinder->updateThing(item, itemId, count);
        cylinder->postAddNotification(item, cylinder, itemIndex);
        item->startDecaying();

        return item;
    }

    //Replacing the the old item with the new while maintaining the old position
    Item* newItem;
    if (newCount == -1) {
        newItem = Item::CreateItem(newId);
    } else {
        newItem = Item::CreateItem(newId, newCount);
    }

    if (newItem == nullptr) {
        return nullptr;
    }

    cylinder->replaceThing(itemIndex, newItem);
    cylinder->postAddNotification(newItem, cylinder, itemIndex);

    item->setParent(nullptr);
    cylinder->postRemoveNotification(item, cylinder, itemIndex);
    item->stopDecaying();
    ReleaseItem(item);
    newItem->startDecaying();

    return newItem;
}

ReturnValue Game::internalTeleport(Thing* thing, const Position& newPos, const bool pushMove/* = true*/, const uint32_t flags /*= 0*/)
{
    if (newPos == thing->getPosition()) {
        return RETURNVALUE_NOERROR;
    }
    if (thing->isRemoved()) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    Tile* toTile = map.getTile(newPos);
    if (!toTile) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    if (Creature* creature = thing->getCreature()) {
        const ReturnValue ret = toTile->queryAdd(0, *creature, 1, FLAG_NOLIMIT);
        if (ret != RETURNVALUE_NOERROR) {
            return ret;
        }

        map.moveCreature(*creature, *toTile, !pushMove);
        return RETURNVALUE_NOERROR;
    }
    if (Item* item = thing->getItem()) {
        return internalMoveItem(item->getParent(), toTile, INDEX_WHEREEVER, item, item->getItemCount(), nullptr, flags);
    }
    return RETURNVALUE_NOTPOSSIBLE;
}

Item* searchForItem(const Container* container, const uint16_t itemId)
{
    for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
        if ((*it)->getID() == itemId) {
            return *it;
        }
    }

    return nullptr;
}

slots_t getSlotType(const ItemType& it)
{
    slots_t slot = CONST_SLOT_RIGHT;
    if (it.weaponType != WEAPON_SHIELD) {
        const int32_t slotPosition = it.slotPosition;

        if (slotPosition & SLOTP_HEAD) {
            slot = CONST_SLOT_HEAD;
        } else if (slotPosition & SLOTP_NECKLACE) {
            slot = CONST_SLOT_NECKLACE;
        } else if (slotPosition & SLOTP_ARMOR) {
            slot = CONST_SLOT_ARMOR;
        } else if (slotPosition & SLOTP_LEGS) {
            slot = CONST_SLOT_LEGS;
        } else if (slotPosition & SLOTP_FEET) {
            slot = CONST_SLOT_FEET;
        } else if (slotPosition & SLOTP_RING) {
            slot = CONST_SLOT_RING;
        } else if (slotPosition & SLOTP_AMMO) {
            slot = CONST_SLOT_AMMO;
        } else if (slotPosition & SLOTP_TWO_HAND || slotPosition & SLOTP_LEFT) {
            slot = CONST_SLOT_LEFT;
        }
    }

    return slot;
}

//Implementation of player invoked events
void Game::playerEquipItem(Player* player, const uint16_t spriteId)
{
    Item* item = player->getInventoryItem(CONST_SLOT_BACKPACK);
    if (!item) {
        return;
    }

    const Container* backpack = item->getContainer();
    if (!backpack) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    const slots_t slot = getSlotType(it);

    Item* slotItem = player->getInventoryItem(slot);
    Item* equipItem = searchForItem(backpack, it.id);
    if (slotItem && slotItem->getID() == it.id && (!it.stackable || slotItem->getItemCount() == 100 || !equipItem)) {
        internalMoveItem(slotItem->getParent(), player, CONST_SLOT_WHEREEVER, slotItem, slotItem->getItemCount(), nullptr);
    } else if (equipItem) {
        internalMoveItem(equipItem->getParent(), player, slot, equipItem, equipItem->getItemCount(), nullptr);
    }
}

#if CLIENT_VERSION >= 1150
void Game::playerTeleport(Player* player, const Position& position)
{
    if (!player->isAccessPlayer()) {
        return;
    }

    ReturnValue ret = g_game.internalTeleport(player, position, false, FLAG_NOLIMIT);
    if (ret != RETURNVALUE_NOERROR) {
        player->sendCancelMessage(ret);
    }
}
#endif

void Game::playerMove(Player* player, const Direction direction)
{
    player->resetIdleTime();
    player->stopNextWalkActionTask();

    player->startAutoWalk(direction);
}

bool Game::playerBroadcastMessage(const Player* player, const std::string& text) const
{
    if (!player->hasFlag(PlayerFlag_CanBroadcast)) {
        return false;
    }

    std::cout << "> " << player->getName() << " broadcasted: \"" << text << "\"." << std::endl;

    for (const auto& it : players) {
        it.second->sendPrivateMessage(player, TALKTYPE_BROADCAST, text);
    }

    return true;
}

void Game::playerCreatePrivateChannel(Player* player)
{
    ChatChannel* channel = g_chat->createChannel(*player, CHANNEL_PRIVATE);
    if (!channel || !channel->addUser(*player)) {
        return;
    }

    player->sendCreatePrivateChannel(channel->getId(), channel->getName());
}

void Game::playerChannelInvite(const Player* player, const std::string& name)
{
    PrivateChatChannel* channel = g_chat->getPrivateChannel(*player);
    if (!channel) {
        return;
    }

    Player* invitePlayer = getPlayerByName(name);
    if (!invitePlayer) {
        return;
    }

    if (player == invitePlayer) {
        return;
    }

    channel->invitePlayer(*player, *invitePlayer);
}

void Game::playerChannelExclude(const Player* player, const std::string& name)
{
    PrivateChatChannel* channel = g_chat->getPrivateChannel(*player);
    if (!channel) {
        return;
    }

    Player* excludePlayer = getPlayerByName(name);
    if (!excludePlayer) {
        return;
    }

    if (player == excludePlayer) {
        return;
    }

    channel->excludePlayer(*player, *excludePlayer);
}

void Game::playerRequestChannels(Player* player)
{
    player->sendChannelsDialog();
}

void Game::playerOpenChannel(Player* player, const uint16_t channelId)
{
    ChatChannel* channel = g_chat->addUserToChannel(*player, channelId);
    if (!channel) {
        return;
    }

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
        return;
    }
#endif
    player->sendChannel(channel->getId(), channel->getName(), users, invitedUsers);
}

void Game::playerCloseChannel(const Player* player, const uint16_t channelId)
{
    g_chat->removeUserFromChannel(*player, channelId);
}

void Game::playerOpenPrivateChannel(Player* player, std::string& receiver)
{
    if (!IOLoginData::formatPlayerName(receiver)) {
        player->sendCancelMessage("A player with this name does not exist.");
        return;
    }

    if (player->getName() == receiver) {
        player->sendCancelMessage("You cannot set up a private message channel with yourself.");
        return;
    }

    player->sendOpenPrivateChannel(receiver);
}

#if GAME_FEATURE_RULEVIOLATION > 0
void Game::playerRuleViolation(Player* player, const std::string& target, const std::string& comment, uint8_t reason, uint8_t, uint32_t, bool ipBanishment)
{
    if (!player->isAccessPlayer()) {
        return;
    }

    std::string reasonStr;
#if CLIENT_VERSION >= 726 && CLIENT_VERSION <= 730
    switch (reason) {
        case 0: reasonStr = "Insulting name"; break;
        case 1: reasonStr = "Name containing parts of sentences"; break;
        case 2: reasonStr = "Name with nonsensical letter combination"; break;
        case 3: reasonStr = "Badly formatted name"; break;
        case 4: reasonStr = "Name not describing person"; break;
        case 5: reasonStr = "Name of celebrity"; break;
        case 6: reasonStr = "Name referring to country"; break;
        case 7: reasonStr = "Name to fake identity"; break;
        case 8: reasonStr = "Name to fake official position"; break;
        case 9: reasonStr = "Insulting statements"; break;
        case 10: reasonStr = "Spamming"; break;
        case 11: reasonStr = "Off-topic advertisment"; break;
        case 12: reasonStr = "Real money advertisment"; break;
        case 13: reasonStr = "Off-topic channel use"; break;
        case 14: reasonStr = "Inciting rule violation"; break;
        case 15: reasonStr = "Bug abuse"; break;
        case 16: reasonStr = "Game weakness abuse"; break;
        case 17: reasonStr = "Macro use"; break;
        case 18: reasonStr = "Using modified client"; break;
        case 19: reasonStr = "Hacking attempt"; break;
        case 20: reasonStr = "Multi-clienting"; break;
        case 21: reasonStr = "Account trading"; break;
        case 22: reasonStr = "Account sharing"; break;
        case 23: reasonStr = "Threatening gamemaster"; break;
        case 24: reasonStr = "Pretending official position"; break;
        case 25: reasonStr = "Pretending to have influence on staff"; break;
        case 26: reasonStr = "False reports"; break;
        case 27: reasonStr = "Excessive unjustified player killing"; break;
        case 28: reasonStr = "Destructive behaviour"; break;
        case 29: reasonStr = "Invalid payment"; break;
        default: reasonStr = "Unknown reason"; break;
    }
#elif CLIENT_VERSION >= 820
    switch (reason) {
        case 0: reasonStr = "Offensive Name"; break;
        case 1: reasonStr = "Invalid Name Format"; break;
        case 2: reasonStr = "Unsuitable Name"; break;
        case 3: reasonStr = "Name Inciting Rule Violation"; break;
        case 4: reasonStr = "Offensive Statement"; break;
        case 5: reasonStr = "Spamming"; break;
        case 6: reasonStr = "Illegal Advertising"; break;
        case 7: reasonStr = "Off-Topic Public Statement"; break;
        case 8: reasonStr = "Non-English Public Statement"; break;
        case 9: reasonStr = "Inciting Rule Violation"; break;
        case 10: reasonStr = "Bug Abuse"; break;
        case 11: reasonStr = "Game Weakness Abuse"; break;
        case 12: reasonStr = "Using Unofficial Software to Play"; break;
        case 13: reasonStr = "Hacking"; break;
        case 14: reasonStr = "Multi-Clienting"; break;
        case 15: reasonStr = "Account Trading or Sharing"; break;
        case 16: reasonStr = "Threatening Gamemaster"; break;
        case 17: reasonStr = "Pretending to Have Influence on Rule Enforcement"; break;
        case 18: reasonStr = "False Report to Gamemaster"; break;
        case 19: reasonStr = "Destructive Behaviour"; break;
        case 20: reasonStr = "Excessive Unjustified Player Killing"; break;
        case 21: reasonStr = "Invalid Payment"; break;
        case 22: reasonStr = "Spoiling Auction"; break;
        default: reasonStr = "Unknown reason"; break;
    }
#else
    switch (reason) {
        case 0: reasonStr = "Offensive name"; break;
        case 1: reasonStr = "Name containing part of sentence"; break;
        case 2: reasonStr = "Name with nonsensical letter combination"; break;
        case 3: reasonStr = "Invalid name format"; break;
        case 4: reasonStr = "Name not describing person"; break;
        case 5: reasonStr = "Name of celebrity"; break;
        case 6: reasonStr = "Name referring to country"; break;
        case 7: reasonStr = "Name to fake player identity"; break;
        case 8: reasonStr = "Name to fake official position"; break;
        case 9: reasonStr = "Offensive statement"; break;
        case 10: reasonStr = "Spamming"; break;
        case 11: reasonStr = "Advertisement not related to game"; break;
        case 12: reasonStr = "Real money advertisement"; break;
        case 13: reasonStr = "Non-English public statement"; break;
        case 14: reasonStr = "Off-topic public statement"; break;
        case 15: reasonStr = "Inciting rule violation"; break;
        case 16: reasonStr = "Bug abuse"; break;
        case 17: reasonStr = "Game weakness abuse"; break;
        case 18: reasonStr = "Macro use"; break;
        case 19: reasonStr = "Using unofficial software to play"; break;
        case 20: reasonStr = "Hacking"; break;
        case 21: reasonStr = "Multi-clienting"; break;
        case 22: reasonStr = "Account trading"; break;
        case 23: reasonStr = "Account sharing"; break;
        case 24: reasonStr = "Threatening gamemaster"; break;
        case 25: reasonStr = "Pretending to have official position"; break;
        case 26: reasonStr = "Pretending to have influence on gamemaster"; break;
        case 27: reasonStr = "False report to gamemaster"; break;
        case 28: reasonStr = "Excessive unjustified player killing"; break;
        case 29: reasonStr = "Destructive behaviour"; break;
        case 30: reasonStr = "Spoiling auction"; break;
        case 31: reasonStr = "Invalid payment"; break;
        default: reasonStr = "Unknown reason"; break;
    }
#endif

    time_t timeBan = 7 * 86400;
    time_t timeNow = time(nullptr);
    if (!comment.empty()) {
        std::string::size_type end = 0;
        if (comment.front() == '{' && (end = comment.find('}')) != std::string::npos) {
            timeBan = 0;

            StringVector timeVec = explodeString(comment.substr(1, end - 1), ";");
            for (const std::string& timeStr : timeVec) {
                StringVector timer = explodeString(timeStr, ",");
                uint32_t time = 1;
                if (timer.size() > 1) {
                    try {
                        time = std::stoul(timer[1]);
                    } catch (const std::invalid_argument&) {
                        time = 0;
                    } catch (const std::out_of_range&) {
                        time = 0;
                    }
                }

                if (!timer.empty() && !timer[0].empty()) {
                    if (timer[0].front() == 's') {
                        timeBan += (time);
                    } else if (timer[0].front() == 'm') {
                        timeBan += (time * 60);
                    } else if (timer[0].front() == 'h') {
                        timeBan += (time * 3600);
                    } else if (timer[0].front() == 'd') {
                        timeBan += (time * 86400);
                    } else if (timer[0].front() == 'w') {
                        timeBan += (time * 604800);
                    } else if (timer[0].front() == 'm') {
                        timeBan += (time * 2592000);
                    } else if (timer[0].front() == 'y') {
                        timeBan += (time * 31536000);
                    }
                }
            }

            ++end;
        }

        std::string commentStr = comment.substr(end);
        if (!commentStr.empty()) {
            reasonStr.push_back('(');
            reasonStr.append(commentStr);
            reasonStr.push_back(')');
        }
    }
    BanInfo banInfo;

    Player* targetPlayer = getPlayerByName(target);
    if (ipBanishment) {
        uint32_t targetIp;
        if (targetPlayer) {
            targetIp = targetPlayer->getIP();
        } else {
            targetIp = IOBan::getAccountLastIP(target);
        }

        if (targetIp == 0) {
            player->sendCancelMessage("A player with this name does not exist.");
            return;
        }

        if (IOBan::isIpBanned(targetIp, banInfo)) {
            player->sendCancelMessage((targetPlayer ? targetPlayer->getName() : target) + " is already IP banned.");
            if (targetPlayer) {
                targetPlayer->kickPlayer(true);
            }
            return;
        }

        std::stringExtended query(256);
        query << "INSERT INTO `ip_bans` (`ip`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (" << targetIp << ", " << g_database.escapeString(reasonStr) << ", " << timeNow << ", " << (timeNow + timeBan) << ", " << player->getGUID() << ")";
        g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)));

        player->sendTextMessage(MESSAGE_EVENT_ADVANCE, (targetPlayer ? targetPlayer->getName() : target) + " has been IP banned.");
        if (targetPlayer) {
            targetPlayer->kickPlayer(true);
        }
    } else {
        uint32_t targetAccId;
        if (targetPlayer) {
            targetAccId = targetPlayer->getAccount();
        } else {
            targetAccId = IOBan::getAccountID(target);
        }

        if (targetAccId == 0) {
            player->sendCancelMessage("A player with this name does not exist.");
            return;
        }

        if (IOBan::isAccountBanned(targetAccId, banInfo)) {
            player->sendCancelMessage((targetPlayer ? targetPlayer->getName() : target) + " is already banned.");
            if (targetPlayer) {
                targetPlayer->kickPlayer(true);
            }
            return;
        }

        std::stringExtended query(256);
        query << "INSERT INTO `account_bans` (`account_id`, `reason`, `banned_at`, `expires_at`, `banned_by`) VALUES (" << targetAccId << ", " << g_database.escapeString(reasonStr) << ", " << timeNow << ", " << (timeNow + timeBan) << ", " << player->getGUID() << ")";
        g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)));

        player->sendTextMessage(MESSAGE_EVENT_ADVANCE, (targetPlayer ? targetPlayer->getName() : target) + " has been banned.");
        if (targetPlayer) {
            targetPlayer->kickPlayer(true);
        }
    }
}

void Game::playerProcessRuleViolation(Player* player, const std::string& target)
{
    if (!player->hasFlag(PlayerFlag_CanAnswerRuleViolations)) {
        return;
    }

    if (Player* reporter = getPlayerByName(target)) {
        auto it = ruleViolations.find(reporter->getName());
        if (it != ruleViolations.end() && it->second.gamemaster == 0) {
            it->second.gamemaster = player->getID();
            if (ChatChannel* channel = g_chat->getChannelById(3)) {
                const UsersMap& users = channel->getUsers();
                for (const auto& uit : users) {
                    uit.second->sendRuleViolationRemove(reporter->getName());
                }
            }
        }
    }
}

void Game::playerCloseRuleViolation(Player* player, const std::string& target)
{
    if (!player->hasFlag(PlayerFlag_CanAnswerRuleViolations)) {
        return;
    }

    if (Player* reporter = getPlayerByName(target)) {
        auto it = ruleViolations.find(reporter->getName());
        if (it != ruleViolations.end()) {
            reporter->sendRuleViolationLock();
            if (ChatChannel* channel = g_chat->getChannelById(3)) {
                const UsersMap& users = channel->getUsers();
                for (const auto& uit : users) {
                    uit.second->sendRuleViolationRemove(reporter->getName());
                }
            }
            ruleViolations.erase(it);
        }
    }
}

void Game::playerCancelRuleViolation(Player* player)
{
    auto it = ruleViolations.find(player->getName());
    if (it != ruleViolations.end()) {
        if (Player* gamemaster = getPlayerByID(it->second.gamemaster)) {
            gamemaster->sendRuleViolationCancel(player->getName());
        } else if (ChatChannel* channel = g_chat->getChannelById(3)) {
            const UsersMap& users = channel->getUsers();
            for (const auto& uit : users) {
                uit.second->sendRuleViolationRemove(player->getName());
            }
        }
        ruleViolations.erase(it);
    }
}

void Game::playerReportRuleViolation(Player* player, const std::string& text)
{
    playerCancelRuleViolation(player);
    ruleViolations.emplace(std::piecewise_construct, std::forward_as_tuple(player->getName()), std::forward_as_tuple(player, text, OTSYS_TIME()));

    if (ChatChannel* channel = g_chat->getChannelById(3)) {
        const UsersMap& users = channel->getUsers();
        for (const auto& uit : users) {
            uit.second->sendChannelMessage(player, text, TALKTYPE_RVR_CHANNEL, 0);
        }
    }
}

void Game::playerContinueReport(Player* player, const std::string& text)
{
    auto it = ruleViolations.find(player->getName());
    if (it != ruleViolations.end()) {
        if (Player* gamemaster = getPlayerByID(it->second.gamemaster)) {
            gamemaster->sendPrivateMessage(player, TALKTYPE_RVR_CONTINUE, text);
            player->sendCancelMessage("Message sent to Gamemaster.");
        }
    }
}

void Game::playerCheckRuleViolation(Player* player)
{
    playerCancelRuleViolation(player);
    if (player->hasFlag(PlayerFlag_CanAnswerRuleViolations)) {
        for (auto it = ruleViolations.begin(); it != ruleViolations.end(); ++it) {
            RuleViolation& rvr = it->second;
            if (rvr.gamemaster == player->getID()) {
                if (Player* owner = g_game.getPlayerByID(rvr.owner)) {
                    owner->sendRuleViolationLock();
                }
                ruleViolations.erase(it);
                break;
            }
        }
    }
}
#endif

#if GAME_FEATURE_STASH > 0
void Game::playerStowItem(Player* player, Item* item, uint32_t count)
{
    Container* container = item->getContainer();
    if (!container) {
        const ItemType& it = Item::items[item->getID()];
        if (it.pickupable && it.stackable && it.wareId != 0) {
            if (player->getStashItemCount(item->getID()) == 0) {
                size_t stowedItems = player->getStashItemCount();
                if (stowedItems >= static_cast<size_t>(g_config.getNumber(ConfigManager::MAX_SUPPLY_STASH_STOWED_ITEMS))) {
                    player->sendCancelMessage("You don't have capacity in the Supply Stash to store this item.");
                    return;
                }
            }

            if (player->addStashItem(item->getID(), count)) {
                ReturnValue ret = internalRemoveItem(item, count);
                if (ret != RETURNVALUE_NOERROR) {
                    player->removeStashItem(item->getID(), count);
                    player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
                    return;
                }
            }
        }
        return;
    }

    if (container->empty()) {
        player->sendCancelMessage("There is nothing to stow in this container.");
        return;
    }

    std::vector<Item*> items;
    items.reserve(std::max<size_t>(32, container->size()));

    std::vector<Container*> containers{ container };
    containers.reserve(32);

    size_t i = 0;
    do {
        Container* tmpContainer = containers[i];
        for (Item* tmpContainerItem : tmpContainer->getItemList()) {
            const ItemType& it = Item::items[tmpContainerItem->getID()];
            if (Container* subContainer = tmpContainerItem->getContainer()) {
                containers.push_back(subContainer);
            } else if (it.pickupable && it.stackable && it.wareId != 0 && !tmpContainerItem->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
                items.push_back(tmpContainerItem);
            }
        }
    } while (++i < containers.size());

    if (items.empty()) {
        player->sendCancelMessage("There is nothing to stow in this container.");
        return;
    }

    bool insufficientRoom = false;
    for (Item* tmpItem : items) {
        if (player->getStashItemCount(tmpItem->getID()) == 0) {
            size_t stowedItems = player->getStashItemCount();
            if (stowedItems >= static_cast<size_t>(g_config.getNumber(ConfigManager::MAX_SUPPLY_STASH_STOWED_ITEMS))) {
                insufficientRoom = true;
                continue;
            }
        }

        count = static_cast<uint32_t>(tmpItem->getItemCount());
        if (player->addStashItem(tmpItem->getID(), count)) {
            ReturnValue ret = internalRemoveItem(tmpItem, count);
            if (ret != RETURNVALUE_NOERROR) {
                player->removeStashItem(tmpItem->getID(), count);
                insufficientRoom = true;
                continue;
            }
        }
    }

    if (insufficientRoom) {
        player->sendCancelMessage("You don't have capacity in the Supply Stash to store this container.");
    } else {
        player->sendCancelMessage("You have successfully stowed this container.");
    }
}

void Game::playerStowItem(Player* player, const Position& pos, uint16_t spriteId, uint8_t stackpos, uint32_t count)
{
    if (!player->isPremium()) {
        player->sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackpos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing || count == 0) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId || static_cast<uint32_t>(item->getItemCount()) < count) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if ((!item->getContainer() && !item->isPickupable()) || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        // moving towards stow items means we'll loose supply stash availability
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    playerStowItem(player, item, count);
}

void Game::playerStowContainer(Player* player, const Position& pos, uint16_t spriteId, uint8_t stackpos)
{
    if (!player->isPremium()) {
        player->sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackpos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if ((!item->getContainer() && !item->isPickupable()) || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        // moving towards stow items means we'll loose supply stash availability
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    playerStowItem(player, item, static_cast<uint32_t>(item->getItemCount()));
}

void Game::playerStowStack(Player* player, const Position& pos, uint16_t spriteId, uint8_t stackpos)
{
    if (!player->isPremium()) {
        player->sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackpos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if ((!item->getContainer() && !item->isPickupable()) || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        // moving towards stow items means we'll loose supply stash availability
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    playerStowItem(player, item, static_cast<uint32_t>(item->getItemCount()));
}

void Game::playerStashWithdraw(Player* player, uint16_t spriteId, uint32_t count, uint8_t)
{
    if (player->hasFlag(PlayerFlag_CannotPickupItem)) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    if (it.id == 0 || count == 0 || player->getStashItemCount(it.id) < count) {
        return;
    }

    if (!player->hasFlag(PlayerFlag_HasInfiniteCapacity)) {
        uint32_t itemWeight = it.weight * count;
        if (itemWeight > player->getFreeCapacity()) {
            player->sendCancelMessage(RETURNVALUE_NOTENOUGHCAPACITY);
            return;
        }
    }

    uint32_t remainingCount = count;
    while (remainingCount > 0) {
        uint32_t stackCount = std::min<uint32_t>(remainingCount, 100);

        uint32_t remainderCount = 0;
        Item* newItem = Item::CreateItem(it.id, stackCount);
        ReturnValue ret = internalAddItem(player, newItem, INDEX_WHEREEVER, 0, false, remainderCount);
        if (ret != RETURNVALUE_NOERROR) {
            delete newItem;
            break;
        }

        if (remainderCount != 0) {
            remainingCount -= (stackCount - remainderCount);
            break;
        }

        remainingCount -= stackCount;
    }

    uint32_t retrieved = (count - remainingCount);
    if (retrieved == 0) {
        player->sendCancelMessage("You do not have enough room to withdraw this item.");
        return;
    }

    std::stringExtended ss(it.name.length() + 128);
    if (retrieved != count) {
        ss << "Retrieved " << retrieved << "x " << it.name << ".\n";
        ss << remainingCount << "x are impossible to retrieve due to insufficient room in inventory.";
    } else {
        ss << "Retrieved " << retrieved << "x " << it.name << '.';
    }
    player->sendCancelMessage(ss);
    player->removeStashItem(it.id, retrieved);
    player->sendSupplyStash();
}
#endif

#if GAME_FEATURE_QUEST_TRACKER > 0
void Game::playerResetTrackedQuests(Player* player, std::vector<uint16_t>& quests)
{
    player->resetTrackedQuests(quests);
}
#endif

void Game::playerCloseNpcChannel(Player* player)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, player->getPosition());
    for (Creature* spectator : spectators) {
        if (const Npc* npc = spectator->getNpc()) {
            npc->onPlayerCloseChannel(player);
        }
    }
}
void Game::playerSetTyping(uint32_t playerId, uint8_t typing)
{
	Player* player = getPlayerByID(playerId);
	if (!player) {
		return;
	}

	SpectatorVec spectators;
	map.getSpectators(spectators, player->getPosition());
	for (Creature* spectator : spectators) {
		if (Player* p = spectator->getPlayer()) {
			p->sendPlayerTyping(player, typing);
		}
	}
}

void Game::playerReceivePing(Player* player)
{
    player->receivePing();
}

void Game::playerReceivePingBack(const Player* player)
{
    player->sendPingBack();
}

void Game::playerAutoWalk(const uint32_t playerId, const std::vector<Direction>& listDir)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    player->resetIdleTime();
    player->stopNextWalkTask();
    player->startAutoWalk(std::move(listDir));
}

void Game::playerStopAutoWalk(Player* player)
{
    player->stopWalk();
}

void Game::playerUseItemEx(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint16_t fromSpriteId,
                           const Position& toPos, uint8_t toStackPos, uint16_t toSpriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    const bool isHotkey = fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0;
    if (isHotkey && !g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
        return;
    }

    Thing* thing = internalGetThing(player, fromPos, fromStackPos, fromSpriteId, STACKPOS_USEITEM);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Item* item = thing->getItem();
    if (!item || !item->isUseable() || item->getClientID() != fromSpriteId) {
        player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
        return;
    }

    Position walkToPos = fromPos;
    ReturnValue ret = g_actions->canUse(player, fromPos);
    if (ret == RETURNVALUE_NOERROR) {
        ret = g_actions->canUse(player, toPos, item);
        if (ret == RETURNVALUE_TOOFARAWAY) {
            walkToPos = toPos;
        }
    }

    if (ret != RETURNVALUE_NOERROR) {
        if (ret == RETURNVALUE_TOOFARAWAY) {
            Position itemPos = fromPos;
            uint8_t itemStackPos = fromStackPos;

            if (fromPos.x != 0xFFFF && toPos.x != 0xFFFF && Position::areInRange<1, 1, 0>(fromPos, player->getPosition()) &&
                    !Position::areInRange<1, 1, 0>(fromPos, toPos)) {
                Item* moveItem = nullptr;

                ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(), &moveItem);
                if (ret != RETURNVALUE_NOERROR) {
                    player->sendCancelMessage(ret);
                    return;
                }

                //changing the position since its now in the inventory of the player
                internalGetPosition(moveItem, itemPos, itemStackPos);
            }

            std::vector<Direction> listDir;
            if (player->getPathTo(walkToPos, listDir, 0, 1, true, false)) {
                g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                    playerAutoWalk(capture0, capture1);
                });
                player->setNextWalkActionTask(400, [this, playerId, itemPos, itemStackPos, fromSpriteId, toPos,
                                                  toStackPos, toSpriteId] {
                    playerUseItemEx(playerId, itemPos, itemStackPos, fromSpriteId, toPos,
                    toStackPos, toSpriteId);
                });
            } else {
                player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
            }
            return;
        }

        player->sendCancelMessage(ret);
        return;
    }

    if (!player->canDoAction()) {
        const uint32_t delay = player->getNextActionTime();
        player->setNextActionTask(delay,
                                  [this, playerId, fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId] {
            playerUseItemEx(playerId, fromPos, fromStackPos, fromSpriteId, toPos, toStackPos,
            toSpriteId);
        });
        return;
    }

    player->resetIdleTime();
    player->stopNextActionTask();

    g_actions->useItemEx(player, fromPos, toPos, toStackPos, item, isHotkey);
}

void Game::playerUseItem(uint32_t playerId, const Position& pos, uint8_t stackPos,
                         uint8_t index, uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    const bool isHotkey = pos.x == 0xFFFF && pos.y == 0 && pos.z == 0;
    if (isHotkey && !g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackPos, spriteId, STACKPOS_USEITEM);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->isUseable() || item->getClientID() != spriteId) {
        player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
        return;
    }

    ReturnValue ret = g_actions->canUse(player, pos);
    if (ret != RETURNVALUE_NOERROR) {
        if (ret == RETURNVALUE_TOOFARAWAY) {
            std::vector<Direction> listDir;
            if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
                g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                    playerAutoWalk(capture0, capture1);
                });
                player->setNextWalkActionTask(400, [this, playerId, pos, stackPos, index, spriteId] {
                    playerUseItem(playerId, pos, stackPos, index, spriteId);
                });
                return;
            }

            ret = RETURNVALUE_THEREISNOWAY;
        }

        player->sendCancelMessage(ret);
        return;
    }

    if (!player->canDoAction()) {
        const uint32_t delay = player->getNextActionTime();
        player->setNextActionTask(delay, [this, playerId, pos, stackPos, index, spriteId] {
            playerUseItem(playerId, pos, stackPos, index, spriteId);
        });
        return;
    }

    player->resetIdleTime();
    player->stopNextActionTask();

    g_actions->useItem(player, pos, index, item, isHotkey);
}

void Game::playerUseWithCreature(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint32_t creatureId, uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    Creature* creature = getCreatureByID(creatureId);
    if (!creature) {
        return;
    }

    if (!Position::areInRange<7, 5, 0>(creature->getPosition(), player->getPosition())) {
        return;
    }

    const bool isHotkey = fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0;
    if (!g_config.getBoolean(ConfigManager::AIMBOT_HOTKEY_ENABLED)) {
        if (creature->getPlayer() || isHotkey) {
            player->sendCancelMessage(RETURNVALUE_DIRECTPLAYERSHOOT);
            return;
        }
    }

    Thing* thing = internalGetThing(player, fromPos, fromStackPos, spriteId, STACKPOS_USEITEM);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Item* item = thing->getItem();
    if (!item || !item->isUseable() || item->getClientID() != spriteId) {
        player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
        return;
    }

    const Position toPos = creature->getPosition();
    Position walkToPos = fromPos;
    ReturnValue ret = g_actions->canUse(player, fromPos);
    if (ret == RETURNVALUE_NOERROR) {
        ret = g_actions->canUse(player, toPos, item);
        if (ret == RETURNVALUE_TOOFARAWAY) {
            walkToPos = toPos;
        }
    }

    if (ret != RETURNVALUE_NOERROR) {
        if (ret == RETURNVALUE_TOOFARAWAY) {
            Position itemPos = fromPos;
            uint8_t itemStackPos = fromStackPos;

            if (fromPos.x != 0xFFFF && Position::areInRange<1, 1, 0>(fromPos, player->getPosition()) && !Position::areInRange<1, 1, 0>(fromPos, toPos)) {
                Item* moveItem = nullptr;
                ret = internalMoveItem(item->getParent(), player, INDEX_WHEREEVER, item, item->getItemCount(), &moveItem);
                if (ret != RETURNVALUE_NOERROR) {
                    player->sendCancelMessage(ret);
                    return;
                }

                //changing the position since its now in the inventory of the player
                internalGetPosition(moveItem, itemPos, itemStackPos);
            }

            std::vector<Direction> listDir;
            if (player->getPathTo(walkToPos, listDir, 0, 1, true, false)) {
                g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                    playerAutoWalk(capture0, capture1);
                });
                player->setNextWalkActionTask(400, [this, playerId, itemPos, itemStackPos, creatureId, spriteId] {
                    playerUseWithCreature(playerId, itemPos, itemStackPos, creatureId,
                    spriteId);
                });
            } else {
                player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
            }
            return;
        }

        player->sendCancelMessage(ret);
        return;
    }

    if (!player->canDoAction()) {
        const uint32_t delay = player->getNextActionTime();
        player->setNextActionTask(delay, [this, playerId, fromPos, fromStackPos, creatureId, spriteId] {
            playerUseWithCreature(playerId, fromPos, fromStackPos, creatureId, spriteId);
        });
        return;
    }

    player->resetIdleTime();
    player->stopNextActionTask();

    g_actions->useItemEx(player, fromPos, creature->getPosition(), creature->getParent()->getThingIndex(creature), item, isHotkey, creature);
}

void Game::playerCloseContainer(Player* player, const uint8_t cid)
{
    player->closeContainer(cid);
    player->sendCloseContainer(cid);
}

void Game::playerMoveUpContainer(Player* player, uint8_t cid)
{
    Container* container = player->getContainerByID(cid);
    if (!container) {
        return;
    }

    auto parentContainer = dynamic_cast<Container*>(container->getRealParent());
    if (!parentContainer) {
#if GAME_FEATURE_BROWSEFIELD > 0
        Tile* tile = container->getTile();
        if (!tile) {
            return;
        }

        const auto it = browseFields.find(tile);
        if (it == browseFields.end()) {
            parentContainer = new Container(tile);
            parentContainer->incrementReferenceCounter();
            browseFields[tile] = parentContainer;
            g_dispatcher.addEvent(30000, [this, capture0 = tile->getPosition()] { decreaseBrowseFieldRef(capture0); });
        } else {
            parentContainer = it->second;
        }
#else
        return;
#endif
    }

    const int8_t test_cid = player->getContainerID(parentContainer);
    if (test_cid != -1) {
        player->closeContainer(test_cid);
        player->sendCloseContainer(test_cid);
        return;
    }

    player->addContainer(cid, parentContainer);
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    player->sendContainer(cid, parentContainer, parentContainer->hasParent(), player->getContainerIndex(cid));
#else
    player->sendContainer(cid, parentContainer, parentContainer->hasParent());
#endif
}

void Game::playerUpdateContainer(Player* player, uint8_t cid)
{
    Container* container = player->getContainerByID(cid);
    if (!container) {
        return;
    }

#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    player->sendContainer(cid, container, container->hasParent(), player->getContainerIndex(cid));
#else
    player->sendContainer(cid, container, container->hasParent());
#endif
}

void Game::playerRotateItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId || !item->isRotatable() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(400, [this, playerId, pos, stackPos, spriteId] {
                playerRotateItem(playerId, pos, stackPos, spriteId);
            });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    const uint16_t newId = Item::items[item->getID()].rotateTo;
    if (newId != 0) {
        transformItem(item, newId);
    }
}

#if GAME_FEATURE_PODIUM > 0
void Game::playerConfigureShowOffSocket(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player || pos.x == 0xFFFF) {
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId || !item->isPodium() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (!Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask(std::bind(&Game::playerAutoWalk, this, player->getID(), std::move(listDir)));
            player->setNextWalkActionTask(400, std::bind(&Game::playerConfigureShowOffSocket, this, playerId, pos, stackPos, spriteId));
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    player->sendPodiumWindow(item, pos, spriteId, stackPos);
}

void Game::playerSetShowOffSocket(uint32_t playerId, Outfit_t& outfit, const Position& pos, uint8_t stackPos, const uint16_t spriteId, uint8_t podiumVisible, uint8_t direction)
{
    Player* player = getPlayerByID(playerId);
    if (!player || pos.x == 0xFFFF) {
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId || !item->isPodium() || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Tile* tile = dynamic_cast<Tile*>(item->getParent());
    if (!tile) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (!Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask(std::bind(&Game::playerAutoWalk, this, player->getID(), std::move(listDir)));
            player->setNextWalkActionTask(400, std::bind(&Game::playerSetShowOffSocket, this, playerId, outfit, pos, stackPos, spriteId, podiumVisible, direction));
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    if (!player->canWear(outfit.lookType, outfit.lookAddons)) {
        outfit.lookType = 0;
        outfit.lookAddons = 0;
    }

    Mount* mount = mounts.getMountByClientID(outfit.lookMount);
    if (!mount || !player->hasMount(mount)) {
        outfit.lookMount = 0;
    }

    std::string key; // Too lazy to fix it :) let's just use temporary key variable
    if (outfit.lookType != 0) {
        key = "LookType"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookType));
        key = "LookHead"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookHead));
        key = "LookBody"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookBody));
        key = "LookLegs"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookLegs));
        key = "LookFeet"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookFeet));
#if GAME_FEATURE_ADDONS > 0
        key = "LookAddons"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookAddons));
#endif
    } else {
        item->removeCustomAttribute("LookType");
    }

#if GAME_FEATURE_MOUNTS > 0
    if (outfit.lookMount != 0) {
        key = "LookMount"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookMount));
#if GAME_FEATURE_MOUNT_COLORS > 0
        key = "LookMountHead"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookMountHead));
        key = "LookMountBody"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookMountBody));
        key = "LookMountLegs"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookMountLegs));
        key = "LookMountFeet"; item->setCustomAttribute(key, static_cast<int64_t>(outfit.lookMountFeet));
#endif
    } else {
        item->removeCustomAttribute("LookMount");
    }
#endif

    key = "PodiumVisible"; item->setCustomAttribute(key, static_cast<int64_t>(podiumVisible));
    key = "LookDirection"; item->setCustomAttribute(key, static_cast<int64_t>(direction));

    SpectatorVector spectators;
    g_game.map.getSpectators(spectators, pos, true);

    //send to client
    for (Creature* spectator : spectators) {
        if (Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendUpdateTileItem(tile, pos, item);
        }
    }
}
#endif

#if CLIENT_VERSION >= 1092
void Game::playerWrapableItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    Thing* thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        return;
    }

    Item* item = thing->getItem();
    if (!item || item->getClientID() != spriteId || (!item->isWrapable() && item->getID() != ITEM_DECORATION_KIT) || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Tile* tile = map.getTile(pos);
    if (!tile) {
        player->sendCancelMessage("Put the construction kit on the floor first.");
        return;
    }

    const auto houseTile = dynamic_cast<HouseTile*>(tile);
    if (!houseTile || !houseTile->getHouse() || !houseTile->getHouse()->isInvited(player)) {
        player->sendCancelMessage("You may construct this only inside a house.");
        return;
    }

    if (pos.x != 0xFFFF && !Position::areInRange<1, 1, 0>(pos, player->getPosition())) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(400, [this, playerId, pos, stackPos, spriteId] {
                playerWrapableItem(playerId, pos, stackPos, spriteId);
            });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    const Container* container = item->getContainer();
    if (container && !container->empty()) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    const uint16_t itemId = item->getID();
    uint16_t newId = Item::items[itemId].wrapableTo;
    const std::string itemName = item->getName();
    if (newId != 0 && itemId != ITEM_DECORATION_KIT) {
        const uint16_t charges = item->getSubType();
        Item* newItem = transformItem(item, newId);
        if (newItem) {
            if (internalMoveItem(newItem->getParent(), player, INDEX_WHEREEVER, newItem, newItem->getItemCount(), nullptr) == RETURNVALUE_NOERROR) {
                newItem->setActionId(itemId);
                newItem->setDate(charges);
                newItem->setSpecialDescription("Unwrap it in your own house to create a " + itemName + ".");
                addMagicEffect(pos, CONST_ME_POFF);
                newItem->startDecaying();
            } else {
                player->sendCancelMessage("Make sure you have enough space in your backpack.");
                transformItem(newItem, itemId);
            }
        }
    } else if (newId == 0 && item->getActionId() != 0) {
        const auto charges = static_cast<uint16_t>(item->getDate());
        newId = item->getActionId();
        Item* newItem = transformItem(item, newId);
        if (newItem) {
            if (charges > 0) {
                newItem->setSubType(charges);
            }
            newItem->removeAttribute(ITEM_ATTRIBUTE_ACTIONID);
            newItem->removeAttribute(ITEM_ATTRIBUTE_DATE);
            newItem->removeAttribute(ITEM_ATTRIBUTE_DESCRIPTION);
            addMagicEffect(pos, CONST_ME_POFF);
            newItem->startDecaying();
        }
    }
}
#endif

void Game::playerWriteItem(Player* player, const uint32_t windowTextId, const std::string& text)
{
    uint16_t maxTextLength = 0;
    uint32_t internalWindowTextId = 0;

    Item* writeItem = player->getWriteItem(internalWindowTextId, maxTextLength);
    if (text.length() > maxTextLength || windowTextId != internalWindowTextId) {
        return;
    }

    if (!writeItem || writeItem->isRemoved()) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Cylinder* topParent = writeItem->getTopParent();

    const auto owner = dynamic_cast<Player*>(topParent);
    if (owner && owner != player) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    if (!Position::areInRange<1, 1, 0>(writeItem->getPosition(), player->getPosition())) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    for (const auto creatureEvent : player->getCreatureEvents(CREATURE_EVENT_TEXTEDIT)) {
        if (!creatureEvent->executeTextEdit(player, writeItem, text)) {
            player->setWriteItem(nullptr);
            return;
        }
    }

    if (!text.empty()) {
        if (writeItem->getText() != text) {
            writeItem->setText(text);
            writeItem->setWriter(player->getName());
            writeItem->setDate(time(nullptr));
        }
    } else {
        writeItem->resetText();
        writeItem->resetWriter();
        writeItem->resetDate();
    }

    const uint16_t newId = Item::items[writeItem->getID()].writeOnceItemId;
    if (newId != 0) {
        transformItem(writeItem, newId);
    }

    player->setWriteItem(nullptr);
}

#if GAME_FEATURE_BROWSEFIELD > 0
void Game::playerBrowseField(uint32_t playerId, const Position& pos)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    const Position& playerPos = player->getPosition();
    if (playerPos.z != pos.z) {
        player->sendCancelMessage(playerPos.z > pos.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS);
        return;
    }

    if (!Position::areInRange<1, 1>(playerPos, pos)) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(400, [this, playerId, pos] { playerBrowseField(playerId, pos); });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    Tile* tile = map.getTile(pos);
    if (!tile) {
        return;
    }

    if (!g_events->eventPlayerOnBrowseField(player, pos)) {
        return;
    }

    Container* container;

    const auto it = browseFields.find(tile);
    if (it == browseFields.end()) {
        container = new Container(tile);
        container->incrementReferenceCounter();
        browseFields[tile] = container;
        g_dispatcher.addEvent(30000, [this, capture0 = tile->getPosition()] { decreaseBrowseFieldRef(capture0); });
    } else {
        container = it->second;
    }

    const uint8_t dummyContainerId = 0xF - (pos.x % 3 * 3 + pos.y % 3);
    const Container* openContainer = player->getContainerByID(dummyContainerId);
    if (openContainer) {
        player->onCloseContainer(openContainer);
        player->closeContainer(dummyContainerId);
    } else {
        player->addContainer(dummyContainerId, container);
        player->sendContainer(dummyContainerId, container, false, 0);
    }
}
#endif

#if GAME_FEATURE_CONTAINER_PAGINATION > 0
void Game::playerSeekInContainer(Player* player, const uint8_t containerId, const uint16_t index)
{
    const Container* container = player->getContainerByID(containerId);
    if (!container || !container->hasPagination()) {
        return;
    }

    if (index % container->capacity() != 0 || index >= container->size()) {
        return;
    }

    player->setContainerIndex(containerId, index);
    player->sendContainer(containerId, container, container->hasParent(), index);
}
#endif

#if GAME_FEATURE_INSPECTION > 0
void Game::playerInspectItem(Player* player, const Position& pos)
{
    Thing* thing = internalGetThing(player, pos, 0, 0, STACKPOS_TOPDOWN_ITEM);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Item* item = thing->getItem();
    if (!item) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    player->sendItemInspection(item->getClientID(), item->getItemCount(), item, false);
}

void Game::playerInspectItem(Player* player, uint16_t itemId, uint8_t itemCount, bool cyclopedia)
{
    player->sendItemInspection(itemId, itemCount, nullptr, cyclopedia);
}
#endif

void Game::playerUpdateHouseWindow(Player* player, const uint8_t listId, const uint32_t windowTextId, const std::string& text)
{
    uint32_t internalWindowTextId;
    uint32_t internalListId;

    House* house = player->getEditHouse(internalWindowTextId, internalListId);
    if (house && house->canEditAccessList(internalListId, player) && internalWindowTextId == windowTextId && listId == 0) {
        house->setAccessList(internalListId, text);
    }

    player->setEditHouse(nullptr);
}

void Game::playerRequestTrade(uint32_t playerId, const Position& pos, uint8_t stackPos,
                              uint32_t tradePlayerId, uint16_t spriteId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    Player* tradePartner = getPlayerByID(tradePlayerId);
    if (!tradePartner || tradePartner == player) {
        player->sendTextMessage(MESSAGE_INFO_DESCR, "Sorry, not possible.");
        return;
    }

    if (!Position::areInRange<2, 2, 0>(tradePartner->getPosition(), player->getPosition())) {
        std::stringExtended ss(tradePartner->getName().length() + static_cast<size_t>(32));
        ss << tradePartner->getName() << " tells you to move closer.";
        player->sendTextMessage(MESSAGE_INFO_DESCR, ss);
        return;
    }

    if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition(), SightLine_CheckSightLineAndFloor)) {
        player->sendCancelMessage(RETURNVALUE_CREATUREISNOTREACHABLE);
        return;
    }

    Thing* tradeThing = internalGetThing(player, pos, stackPos, 0, STACKPOS_TOPDOWN_ITEM);
    if (!tradeThing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    Item* tradeItem = tradeThing->getItem();
    if (tradeItem->getClientID() != spriteId || !tradeItem->isPickupable() || tradeItem->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    const Position& playerPosition = player->getPosition();
    const Position& tradeItemPosition = tradeItem->getPosition();
    if (playerPosition.z != tradeItemPosition.z) {
        player->sendCancelMessage(playerPosition.z > tradeItemPosition.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS);
        return;
    }

    if (!Position::areInRange<1, 1>(tradeItemPosition, playerPosition)) {
        std::vector<Direction> listDir;
        if (player->getPathTo(pos, listDir, 0, 1, true, false)) {
            g_dispatcher.addTask([this, capture0 = player->getID(), capture1 = std::move(listDir)] {
                playerAutoWalk(capture0, capture1);
            });
            player->setNextWalkActionTask(400, [this, playerId, pos, stackPos, tradePlayerId, spriteId] {
                playerRequestTrade(playerId, pos, stackPos, tradePlayerId, spriteId);
            });
        } else {
            player->sendCancelMessage(RETURNVALUE_THEREISNOWAY);
        }
        return;
    }

    const Container* tradeItemContainer = tradeItem->getContainer();
    if (tradeItemContainer) {
        for (const auto& it : tradeItems) {
            Item* item = it.first;
            if (tradeItem == item) {
                player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
                return;
            }

            if (tradeItemContainer->isHoldingItem(item)) {
                player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
                return;
            }

            const Container* container = item->getContainer();
            if (container && container->isHoldingItem(tradeItem)) {
                player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
                return;
            }
        }
    } else {
        for (const auto& it : tradeItems) {
            Item* item = it.first;
            if (tradeItem == item) {
                player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
                return;
            }

            const Container* container = item->getContainer();
            if (container && container->isHoldingItem(tradeItem)) {
                player->sendTextMessage(MESSAGE_INFO_DESCR, "This item is already being traded.");
                return;
            }
        }
    }

    const Container* tradeContainer = tradeItem->getContainer();
    if (tradeContainer && tradeContainer->getItemHoldingCount() + 1 > 100) {
        player->sendTextMessage(MESSAGE_INFO_DESCR, "You can not trade more than 100 items.");
        return;
    }

    if (!g_events->eventPlayerOnTradeRequest(player, tradePartner, tradeItem)) {
        return;
    }

    internalStartTrade(player, tradePartner, tradeItem);
}

bool Game::internalStartTrade(Player* player, Player* tradePartner, Item* tradeItem)
{
    if (player->tradeState != TRADE_NONE && !(player->tradeState == TRADE_ACKNOWLEDGE && player->tradePartner == tradePartner)) {
        player->sendCancelMessage(RETURNVALUE_YOUAREALREADYTRADING);
        return false;
    }
    if (tradePartner->tradeState != TRADE_NONE && tradePartner->tradePartner != player) {
        player->sendCancelMessage(RETURNVALUE_THISPLAYERISALREADYTRADING);
        return false;
    }

    player->tradePartner = tradePartner;
    player->tradeItem = tradeItem;
    player->tradeState = TRADE_INITIATED;
    tradeItem->incrementReferenceCounter();
    tradeItems[tradeItem] = player->getID();

    player->sendTradeItemRequest(player->getName(), tradeItem, true);

    if (tradePartner->tradeState == TRADE_NONE) {
        std::stringExtended ss(player->getName().length() + static_cast<size_t>(32));
        ss << player->getName() << " wants to trade with you.";
        tradePartner->sendTextMessage(MESSAGE_EVENT_ADVANCE, ss);
        tradePartner->tradeState = TRADE_ACKNOWLEDGE;
        tradePartner->tradePartner = player;
    } else {
        const Item* counterOfferItem = tradePartner->tradeItem;
        player->sendTradeItemRequest(tradePartner->getName(), counterOfferItem, false);
        tradePartner->sendTradeItemRequest(player->getName(), tradeItem, false);
    }

    return true;
}

void Game::playerAcceptTrade(Player* player)
{
    if (!(player->getTradeState() == TRADE_ACKNOWLEDGE || player->getTradeState() == TRADE_INITIATED)) {
        return;
    }

    Player* tradePartner = player->tradePartner;
    if (!tradePartner) {
        return;
    }

    if (!canThrowObjectTo(tradePartner->getPosition(), player->getPosition(), SightLine_CheckSightLineAndFloor)) {
        player->sendCancelMessage(RETURNVALUE_CREATUREISNOTREACHABLE);
        return;
    }

    player->setTradeState(TRADE_ACCEPT);

    if (tradePartner->getTradeState() == TRADE_ACCEPT) {
        Item* playerTradeItem = player->tradeItem;
        Item* partnerTradeItem = tradePartner->tradeItem;

        if (!g_events->eventPlayerOnTradeAccept(player, tradePartner, playerTradeItem, partnerTradeItem)) {
            internalCloseTrade(player);
            return;
        }

        player->setTradeState(TRADE_TRANSFER);
        tradePartner->setTradeState(TRADE_TRANSFER);

        auto it = tradeItems.find(playerTradeItem);
        if (it != tradeItems.end()) {
            ReleaseItem(it->first);
            tradeItems.erase(it);
        }

        it = tradeItems.find(partnerTradeItem);
        if (it != tradeItems.end()) {
            ReleaseItem(it->first);
            tradeItems.erase(it);
        }

        bool isSuccess = false;

        ReturnValue tradePartnerRet = internalAddItem(tradePartner, playerTradeItem, INDEX_WHEREEVER, 0, true);
        ReturnValue playerRet = internalAddItem(player, partnerTradeItem, INDEX_WHEREEVER, 0, true);
        if (tradePartnerRet == RETURNVALUE_NOERROR && playerRet == RETURNVALUE_NOERROR) {
            playerRet = internalRemoveItem(playerTradeItem, playerTradeItem->getItemCount(), true);
            tradePartnerRet = internalRemoveItem(partnerTradeItem, partnerTradeItem->getItemCount(), true);
            if (tradePartnerRet == RETURNVALUE_NOERROR && playerRet == RETURNVALUE_NOERROR) {
                tradePartnerRet = internalMoveItem(playerTradeItem->getParent(), tradePartner, INDEX_WHEREEVER, playerTradeItem, playerTradeItem->getItemCount(), nullptr, FLAG_IGNOREAUTOSTACK, nullptr, partnerTradeItem);
                if (tradePartnerRet == RETURNVALUE_NOERROR) {
                    internalMoveItem(partnerTradeItem->getParent(), player, INDEX_WHEREEVER, partnerTradeItem, partnerTradeItem->getItemCount(), nullptr, FLAG_IGNOREAUTOSTACK);
                    playerTradeItem->onTradeEvent(ON_TRADE_TRANSFER, tradePartner);
                    partnerTradeItem->onTradeEvent(ON_TRADE_TRANSFER, player);
                    isSuccess = true;
                }
            }
        }

        if (!isSuccess) {
            std::string errorDescription;

            if (tradePartner->tradeItem) {
                errorDescription = getTradeErrorDescription(tradePartnerRet, playerTradeItem);
                tradePartner->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
                tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
            }

            if (player->tradeItem) {
                errorDescription = getTradeErrorDescription(playerRet, partnerTradeItem);
                player->sendTextMessage(MESSAGE_EVENT_ADVANCE, errorDescription);
                player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
            }
        }

        player->setTradeState(TRADE_NONE);
        player->tradeItem = nullptr;
        player->tradePartner = nullptr;
        player->sendTradeClose();

        tradePartner->setTradeState(TRADE_NONE);
        tradePartner->tradeItem = nullptr;
        tradePartner->tradePartner = nullptr;
        tradePartner->sendTradeClose();
    }
}

std::string Game::getTradeErrorDescription(const ReturnValue ret, const Item* item)
{
    if (item) {
        if (ret == RETURNVALUE_NOTENOUGHCAPACITY) {
            std::stringExtended ss(128);
            ss << "You do not have enough capacity to carry";

            if (item->isStackable() && item->getItemCount() > 1) {
                ss << " these objects.";
            } else {
                ss << " this object.";
            }

            ss << "\n " << item->getWeightDescription();
            return ss;
        }
        if (ret == RETURNVALUE_NOTENOUGHROOM || ret == RETURNVALUE_CONTAINERNOTENOUGHROOM) {
            std::stringExtended ss(128);
            ss << "You do not have enough room to carry";

            if (item->isStackable() && item->getItemCount() > 1) {
                ss << " these objects.";
            } else {
                ss << " this object.";
            }

            return ss;
        }
    }
    return "Trade could not be completed.";
}

void Game::playerLookInTrade(Player* player, const bool lookAtCounterOffer, uint8_t index)
{
    Player* tradePartner = player->tradePartner;
    if (!tradePartner) {
        return;
    }

    Item* tradeItem;
    if (lookAtCounterOffer) {
        tradeItem = tradePartner->getTradeItem();
    } else {
        tradeItem = player->getTradeItem();
    }

    if (!tradeItem) {
        return;
    }

    const Position& playerPosition = player->getPosition();
    const Position& tradeItemPosition = tradeItem->getPosition();

    const int32_t lookDistance = std::max<int32_t>(Position::getDistanceX(playerPosition, tradeItemPosition),
                                                   Position::getDistanceY(playerPosition, tradeItemPosition));
    if (index == 0) {
        g_events->eventPlayerOnLookInTrade(player, tradePartner, tradeItem, lookDistance);
        return;
    }

    Container* tradeContainer = tradeItem->getContainer();
    if (!tradeContainer) {
        return;
    }

    std::vector<const Container*> containers{ tradeContainer };
    containers.reserve(32);

    size_t i = 0;
    do {
        const Container* container = containers[i];
        for (Item* item : container->getItemList()) {
            const Container* tmpContainer = item->getContainer();
            if (tmpContainer) {
                containers.push_back(tmpContainer);
            }

            if (--index == 0) {
                g_events->eventPlayerOnLookInTrade(player, tradePartner, item, lookDistance);
                return;
            }
        }
    } while (++i < containers.size());
}

void Game::playerCloseTrade(Player* player)
{
    internalCloseTrade(player);
}

void Game::internalCloseTrade(Player* player)
{
    Player* tradePartner = player->tradePartner;
    if ((tradePartner && tradePartner->getTradeState() == TRADE_TRANSFER) || player->getTradeState() == TRADE_TRANSFER) {
        return;
    }

    if (player->getTradeItem()) {
        const auto it = tradeItems.find(player->getTradeItem());
        if (it != tradeItems.end()) {
            ReleaseItem(it->first);
            tradeItems.erase(it);
        }

        player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
        player->tradeItem = nullptr;
    }

    player->setTradeState(TRADE_NONE);
    player->tradePartner = nullptr;

    player->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
    player->sendTradeClose();

    if (tradePartner) {
        if (tradePartner->getTradeItem()) {
            const auto it = tradeItems.find(tradePartner->getTradeItem());
            if (it != tradeItems.end()) {
                ReleaseItem(it->first);
                tradeItems.erase(it);
            }

            tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
            tradePartner->tradeItem = nullptr;
        }

        tradePartner->setTradeState(TRADE_NONE);
        tradePartner->tradePartner = nullptr;

        tradePartner->sendTextMessage(MESSAGE_STATUS_SMALL, "Trade cancelled.");
        tradePartner->sendTradeClose();
    }
}

void Game::playerPurchaseItem(Player* player, const uint16_t spriteId, const uint8_t count, const uint8_t amount,
                              const bool ignoreCap/* = false*/, const bool inBackpacks/* = false*/)
{
    int32_t onBuy;
    int32_t onSell;

    const Npc* merchant = player->getShopOwner(onBuy, onSell);
    if (!merchant) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    if (it.id == 0) {
        return;
    }

    uint8_t subType;
    if (it.isSplash() || it.isFluidContainer()) {
        subType = clientFluidToServer(count);
    } else {
        subType = count;
    }

    if (!player->hasShopItemForSale(it.id, subType)) {
        return;
    }

    merchant->onPlayerTrade(player, onBuy, it.id, subType, amount, ignoreCap, inBackpacks);
}

void Game::playerSellItem(Player* player, const uint16_t spriteId, const uint8_t count, const uint8_t amount, const bool ignoreEquipped)
{
    int32_t onBuy;
    int32_t onSell;

    const Npc* merchant = player->getShopOwner(onBuy, onSell);
    if (!merchant) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    if (it.id == 0) {
        return;
    }

    uint8_t subType;
    if (it.isSplash() || it.isFluidContainer()) {
        subType = clientFluidToServer(count);
    } else {
        subType = count;
    }

    merchant->onPlayerTrade(player, onSell, it.id, subType, amount, ignoreEquipped);
}

void Game::playerCloseShop(Player* player)
{
    player->closeShopWindow();
}

void Game::playerLookInShop(Player* player, const uint16_t spriteId, const uint8_t count)
{
    int32_t onBuy;
    int32_t onSell;

    const Npc* merchant = player->getShopOwner(onBuy, onSell);
    if (!merchant) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    if (it.id == 0) {
        return;
    }

    int32_t subType;
    if (it.isFluidContainer() || it.isSplash()) {
        subType = clientFluidToServer(count);
    } else {
        subType = count;
    }

    if (!player->hasShopItemForSale(it.id, subType)) {
        return;
    }

    if (!g_events->eventPlayerOnLookInShop(player, &it, subType)) {
        return;
    }

    std::string str;
    const std::string description = Item::getDescription(it, 1, nullptr, subType);
    str.reserve(description.length() + static_cast<size_t>(10));
    str.append("You see ").append(description);

    player->sendTextMessage(MESSAGE_INFO_DESCR, str);
}

void Game::playerLookAt(Player* player, const Position& pos, const uint8_t stackPos) const
{
    Thing* thing = internalGetThing(player, pos, stackPos, 0, STACKPOS_LOOK);
    if (!thing) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    const Position thingPos = thing->getPosition();
    if (!player->canSee(thingPos)) {
        player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
        return;
    }

    const Position playerPos = player->getPosition();

    int32_t lookDistance;
    if (thing != player) {
        lookDistance = std::max<int32_t>(Position::getDistanceX(playerPos, thingPos), Position::getDistanceY(playerPos, thingPos));
        if (playerPos.z != thingPos.z) {
            lookDistance += 15;
        }
    } else {
        lookDistance = -1;
    }

    g_events->eventPlayerOnLook(player, pos, thing, stackPos, lookDistance);
}

void Game::playerLookInBattleList(Player* player, const uint32_t creatureId)
{
    Creature* creature = getCreatureByID(creatureId);
    if (!creature) {
        return;
    }

    if (!player->canSeeCreature(creature)) {
        return;
    }

    const Position& creaturePos = creature->getPosition();
    if (!player->canSee(creaturePos)) {
        return;
    }

    int32_t lookDistance;
    if (creature != player) {
        const Position& playerPos = player->getPosition();
        lookDistance = std::max<int32_t>(Position::getDistanceX(playerPos, creaturePos), Position::getDistanceY(playerPos, creaturePos));
        if (playerPos.z != creaturePos.z) {
            lookDistance += 15;
        }
    } else {
        lookDistance = -1;
    }

    g_events->eventPlayerOnLookInBattleList(player, creature, lookDistance);
}

void Game::playerCancelAttackAndFollow(const uint32_t playerId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    playerSetAttackedCreature(playerId, 0);
    playerFollowCreature(playerId, 0);
    player->stopWalk();
}

void Game::playerSetAttackedCreature(const uint32_t playerId, const uint32_t creatureId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    if (player->getAttackedCreature() && creatureId == 0) {
        player->setAttackedCreature(nullptr);
        player->sendCancelTarget();
        return;
    }

    Creature* attackCreature = getCreatureByID(creatureId);
    if (!attackCreature) {
        player->setAttackedCreature(nullptr);
        player->sendCancelTarget();
        return;
    }

    const ReturnValue ret = Combat::canTargetCreature(player, attackCreature);
    if (ret != RETURNVALUE_NOERROR) {
        player->sendCancelMessage(ret);
        player->sendCancelTarget();
        player->setAttackedCreature(nullptr);
        return;
    }

    player->setAttackedCreature(attackCreature);
}

void Game::playerFollowCreature(const uint32_t playerId, const uint32_t creatureId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    player->setAttackedCreature(nullptr);
    player->setFollowCreature(getCreatureByID(creatureId));
}

void Game::playerSetFightModes(Player* player, const fightMode_t fightMode, const bool chaseMode, const bool secureMode)
{
    player->setFightMode(fightMode);
    player->setChaseMode(chaseMode);
    player->setSecureMode(secureMode);
}

void Game::playerRequestAddVip(Player* player, const std::string& name)
{
    const Player* vipPlayer = getPlayerByName(name);
    if (!vipPlayer) {
        uint32_t guid;
        bool specialVip;
        std::string formattedName = name;
        if (!IOLoginData::getGuidByNameEx(guid, specialVip, formattedName)) {
            player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name does not exist.");
            return;
        }

        if (specialVip && !player->hasFlag(PlayerFlag_SpecialVIP)) {
            player->sendTextMessage(MESSAGE_STATUS_SMALL, "You can not add this player.");
            return;
        }

        player->addVIP(guid, formattedName, VIPSTATUS_OFFLINE);
    } else {
        if (vipPlayer->hasFlag(PlayerFlag_SpecialVIP) && !player->hasFlag(PlayerFlag_SpecialVIP)) {
            player->sendTextMessage(MESSAGE_STATUS_SMALL, "You can not add this player.");
            return;
        }

        if (!vipPlayer->isInGhostMode() || player->isAccessPlayer()) {
            player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_ONLINE);
        } else {
            player->addVIP(vipPlayer->getGUID(), vipPlayer->getName(), VIPSTATUS_OFFLINE);
        }
    }
}

void Game::playerRequestRemoveVip(Player* player, const uint32_t guid)
{
    player->removeVIP(guid);
}

void Game::playerRequestEditVip(Player* player, const uint32_t guid, const std::string& description, const uint32_t icon, const bool notify)
{
    player->editVIP(guid, description, icon, notify);
}

void Game::playerTurn(Player* player, const Direction dir)
{
    if (!g_events->eventPlayerOnTurn(player, dir)) {
        return;
    }

    player->resetIdleTime();
    internalCreatureTurn(player, dir);
}

void Game::playerRequestOutfit(Player* player)
{
    if (!g_config.getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
        return;
    }

    player->sendOutfitWindow();
}

#if GAME_FEATURE_MOUNTS > 0
void Game::playerToggleMount(Player* player, const bool mount)
{
    player->toggleMount(mount);
}
#endif

void Game::playerChangeOutfit(Player* player, Outfit_t outfit)
{
    if (!g_config.getBoolean(ConfigManager::ALLOW_CHANGEOUTFIT)) {
        return;
    }

#if GAME_FEATURE_MOUNTS > 0
    const Outfit* playerOutfit = Outfits::getInstance().getOutfitByLookType(player->getSex(), outfit.lookType);
    if (!playerOutfit) {
        outfit.lookMount = 0;
    }

    if (outfit.lookMount != 0) {
        const Mount* mount = mounts.getMountByClientID(outfit.lookMount);
        if (!mount) {
            return;
        }

        if (!player->hasMount(mount)) {
            return;
        }

        if (player->isMounted()) {
            const Mount* prevMount = mounts.getMountByID(player->getCurrentMount());
            if (prevMount) {
                changeSpeed(player, mount->speed - prevMount->speed);
            }

            player->setCurrentMount(mount->id);
        } else {
            player->setCurrentMount(mount->id);
            outfit.lookMount = 0;
        }
    } else if (player->isMounted()) {
        player->dismount();
    }
#endif

    if (player->canWear(outfit.lookType, outfit.lookAddons)) {
        player->defaultOutfit = outfit;

        if (player->hasCondition(CONDITION_OUTFIT)) {
            return;
        }

        internalCreatureChangeOutfit(player, outfit);
    }
}

void Game::playerShowQuestLog(Player* player)
{
    player->sendQuestLog();
}

void Game::playerShowQuestLine(Player* player, const uint16_t questId)
{
    const Quest* quest = quests.getQuestByID(questId);
    if (!quest) {
        return;
    }

    player->sendQuestLine(quest);
}

void Game::playerSay(Player* player, const uint16_t channelId, const SpeakClasses type,
                     const std::string& receiver, const std::string& text)
{
    player->resetIdleTime();
    if (playerSaySpell(player, type, text)) {
        return;
    }

    const uint32_t muteTime = player->isMuted();
    if (muteTime > 0) {
        std::stringExtended ss(64);
        ss << "You are still muted for " << muteTime << " seconds.";
        player->sendTextMessage(MESSAGE_STATUS_SMALL, ss);
        return;
    }

    if (text.front() == '/' && player->isAccessPlayer()) {
        return;
    }

    if (type != TALKTYPE_PRIVATE_PN) {
        player->removeMessageBuffer();
    }

    switch (type) {
        case TALKTYPE_SAY:
            internalCreatureSay(player, TALKTYPE_SAY, text, false);
            break;

        case TALKTYPE_WHISPER:
            playerWhisper(player, text);
            break;

        case TALKTYPE_YELL:
            playerYell(player, text);
            break;

        case TALKTYPE_PRIVATE_TO:
        case TALKTYPE_PRIVATE_RED_TO:
#if GAME_FEATURE_RULEVIOLATION > 0
        case TALKTYPE_RVR_ANSWER:
#endif
            playerSpeakTo(player, type, receiver, text);
            break;

        case TALKTYPE_CHANNEL_O:
        case TALKTYPE_CHANNEL_Y:
        case TALKTYPE_CHANNEL_R1:
            g_chat->talkToChannel(*player, type, text, channelId);
            break;

        case TALKTYPE_PRIVATE_PN:
            playerSpeakToNpc(player, text);
            break;

        case TALKTYPE_BROADCAST:
            playerBroadcastMessage(player, text);
            break;

#if GAME_FEATURE_RULEVIOLATION > 0
        case TALKTYPE_RVR_CHANNEL:
            playerReportRuleViolation(player, text);
            break;

        case TALKTYPE_RVR_CONTINUE:
            playerContinueReport(player, text);
            break;
#endif

        default:
            break;
    }
}

bool Game::playerSaySpell(Player* player, const SpeakClasses type, const std::string& text) const
{
    std::string words = text;
    const std::string& lowerWords = asLowerCaseString(words);

    TalkActionResult_t result;
    if (text.front() == '/' || text.front() == '!') {
        result = g_talkActions->playerSaySpell(player, type, words);
        if (result == TALKACTION_BREAK) {
            return true;
        }
    }

    result = g_spells->playerSaySpell(player, words, lowerWords);
    if (result == TALKACTION_BREAK) {
        if (!g_config.getBoolean(ConfigManager::EMOTE_SPELLS)) {
            return internalCreatureSay(player, TALKTYPE_SPELL, words, false);
        }
        return internalCreatureSay(player, TALKTYPE_MONSTER_SAY, words, false);
    }
    if (result == TALKACTION_FAILED) {
        return true;
    }
    return false;
}

void Game::playerWhisper(Player* player, const std::string& text) const
{
    // Use getSpectatorsInternal here because we won't get any cached spectators anyway
    SpectatorVector spectators;
    const Position& pos = player->getPosition();
    map.getSpectatorsInternal(spectators, pos, Map::maxClientViewportX, Map::maxClientViewportX,
            Map::maxClientViewportY, Map::maxClientViewportY, pos.z, pos.z, false);

    //send to client + event method
    for (Creature* spectator : spectators) {
        if (const Player* spectatorPlayer = spectator->getPlayer()) {
            if (!Position::areInRange<1, 1>(player->getPosition(), spectatorPlayer->getPosition())) {
                spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, "pspsps");
            } else {
                spectatorPlayer->sendCreatureSay(player, TALKTYPE_WHISPER, text);
            }
        }

        spectator->onCreatureSay(player, TALKTYPE_WHISPER, text);
    }
}

bool Game::playerYell(Player* player, const std::string& text) const
{
    if (player->getLevel() == 1) {
        player->sendTextMessage(MESSAGE_STATUS_SMALL, "You may not yell as long as you are on level 1.");
        return false;
    }

    if (player->hasCondition(CONDITION_YELLTICKS)) {
        player->sendCancelMessage(RETURNVALUE_YOUAREEXHAUSTED);
        return false;
    }

    if (player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER) {
        Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_YELLTICKS, 30000, 0);
        player->addCondition(condition);
    }

    internalCreatureSay(player, TALKTYPE_YELL, asUpperCaseString(text), false);
    return true;
}

bool Game::playerSpeakTo(Player* player, SpeakClasses type, const std::string& receiver,
                         const std::string& text)
{
    Player* toPlayer = getPlayerByName(receiver);
    if (!toPlayer) {
        player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
        return false;
    }

    if (type == TALKTYPE_PRIVATE_RED_TO && (player->hasFlag(PlayerFlag_CanTalkRedPrivate) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER)) {
        type = TALKTYPE_PRIVATE_RED_FROM;
    } else {
#if GAME_FEATURE_RULEVIOLATION > 0
        if (type != TALKTYPE_RVR_ANSWER) {
            type = TALKTYPE_PRIVATE_FROM;
        }
#else
        type = TALKTYPE_PRIVATE_FROM;
#endif
    }

    toPlayer->sendPrivateMessage(player, type, text);
    toPlayer->onCreatureSay(player, type, text);

    if (toPlayer->isInGhostMode() && !player->isAccessPlayer()) {
        player->sendTextMessage(MESSAGE_STATUS_SMALL, "A player with this name is not online.");
    } else {
        std::stringExtended ss(toPlayer->getName().length() + static_cast<size_t>(32));
        ss << "Message sent to " << toPlayer->getName() << '.';
        player->sendTextMessage(MESSAGE_STATUS_SMALL, ss);
    }
    return true;
}

void Game::playerSpeakToNpc(Player* player, const std::string& text)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, player->getPosition());
    for (Creature* spectator : spectators) {
        if (spectator->getNpc()) {
            spectator->onCreatureSay(player, TALKTYPE_PRIVATE_PN, text);
        }
    }
}

//--
bool Game::canThrowObjectTo(const Position& fromPos, const Position& toPos, const SightLines_t lineOfSight /*= SightLine_CheckSightLine*/,
                            const int32_t rangex /*= Map::maxClientViewportX*/, const int32_t rangey /*= Map::maxClientViewportY*/) const
{
    return map.canThrowObjectTo(fromPos, toPos, lineOfSight, rangex, rangey);
}

bool Game::isSightClear(const Position& fromPos, const Position& toPos, const bool floorCheck) const
{
    return map.isSightClear(fromPos, toPos, floorCheck);
}

bool Game::internalCreatureTurn(Creature* creature, const Direction dir)
{
    if (creature->getDirection() == dir) {
        return false;
    }

    creature->setDirection(dir);

    //send to client
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureTurn(creature);
    }
    return true;
}

bool Game::internalCreatureSay(Creature* creature, const SpeakClasses type, const std::string& text,
                               const bool ghostMode, SpectatorVector* spectatorsPtr/* = nullptr*/, const Position* pos/* = nullptr*/) const
{
    if (text.empty()) {
        return false;
    }

    if (!pos) {
        pos = &creature->getPosition();
    }

    SpectatorVector spectators;
    if (!spectatorsPtr || spectatorsPtr->empty()) {
        // This somewhat complex construct ensures that the cached SpectatorVector
        // is used if available and if it can be used, else a local vector is
        // used (hopefully the compiler will optimize away the construction of
        // the temporary when it's not used).

        // Use getSpectatorsInternal here because we won't get any cached spectators anyway
        if (type != TALKTYPE_YELL && type != TALKTYPE_MONSTER_YELL) {
            map.getSpectatorsInternal(spectators, *pos, Map::maxClientViewportX, Map::maxClientViewportX,
                    Map::maxClientViewportY, Map::maxClientViewportY, pos->z, pos->z, false);
        } else {
            if (pos->z < 8) {
                map.getSpectatorsInternal(spectators, *pos, 18, 18, 14, 14, 0, 7, false);
            } else {
                map.getSpectatorsInternal(spectators, *pos, 18, 18, 14, 14, pos->z, pos->z, false);
            }
        }
    } else {
        spectators = std::move(*spectatorsPtr);
    }

    //send to client + event method
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            if (!ghostMode || tmpPlayer->canSeeCreature(creature)) {
                tmpPlayer->sendCreatureSay(creature, type, text, pos);
            }
        }

        spectator->onCreatureSay(creature, type, text);
    }
    return true;
}

void Game::checkCreatureWalk(const uint32_t creatureId)
{
    Creature* creature = getCreatureByID(creatureId);
    if (creature && creature->getHealth() > 0) {
        creature->onWalk();
        cleanup();
    }
}

void Game::updateCreatureWalk(const uint32_t creatureId)
{
    Creature* creature = getCreatureByID(creatureId);
    if (creature && creature->getHealth() > 0) {
        creature->goToFollowCreature();
    }
}

void Game::checkCreatureAttack(const uint32_t creatureId)
{
    Creature* creature = getCreatureByID(creatureId);
    if (creature && creature->getHealth() > 0) {
        creature->onAttacking(0);
    }
}

void Game::addCreatureCheck(Creature* creature)
{
    creature->creatureCheck = true;
    if (creature->inCheckCreaturesVector) {
        // already in a vector
        return;
    }

    creature->inCheckCreaturesVector = true;
    checkCreatureLists[uniform_random(0, EVENT_CREATURECOUNT - 1)].push_back(creature);
    creature->incrementReferenceCounter();
}

void Game::removeCreatureCheck(Creature* creature)
{
    if (creature->inCheckCreaturesVector) {
        creature->creatureCheck = false;
    }
}

void Game::checkCreatures(const size_t index)
{
    g_dispatcher.addEvent(EVENT_CHECK_CREATURE_INTERVAL, [this, capture0 = (index + 1) % EVENT_CREATURECOUNT] {
        checkCreatures(capture0);
    });

    auto& checkCreatureList = checkCreatureLists[index];
    size_t it = 0;
    size_t end = checkCreatureList.size();
    while (it < end) {
        Creature* creature = checkCreatureList[it];
        if (creature->creatureCheck) {
            if (creature->getHealth() > 0) {
                creature->onThink(EVENT_CREATURE_THINK_INTERVAL);
                creature->onAttacking(EVENT_CREATURE_THINK_INTERVAL);
                creature->executeConditions(EVENT_CREATURE_THINK_INTERVAL);
            } else {
                creature->onDeath();
            }
            ++it;
        } else {
            creature->inCheckCreaturesVector = false;
            ReleaseCreature(creature);

            checkCreatureList[it] = checkCreatureList.back();
            checkCreatureList.pop_back();
            --end;
        }
    }
    cleanup();
}

void Game::changeSpeed(Creature* creature, const int32_t varSpeedDelta)
{
    int32_t varSpeed = creature->getSpeed() - creature->getBaseSpeed();
    varSpeed += varSpeedDelta;

    creature->setSpeed(varSpeed);

    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), false, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendChangeSpeed(creature, creature->getStepSpeed());
    }
}

void Game::internalCreatureChangeOutfit(Creature* creature, const Outfit_t& outfit)
{
    if (!g_events->eventCreatureOnChangeOutfit(creature, outfit)) {
        return;
    }

    creature->setCurrentOutfit(outfit);

    if (creature->isInvisible()) {
        return;
    }

    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureChangeOutfit(creature, outfit);
    }
}

void Game::internalCreatureChangeVisible(const Creature* creature, const bool visible)
{
    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureChangeVisible(creature, visible);
    }
}

void Game::changeLight(const Creature* creature)
{
    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureLight(creature);
    }
}

bool Game::combatBlockHit(CombatDamage& damage, Creature* attacker, Creature* target, const bool checkDefense, const bool checkArmor, const bool field)
{
    if (damage.primary.type == COMBAT_NONE && damage.secondary.type == COMBAT_NONE) {
        return true;
    }

    if (target->getPlayer() && target->isInGhostMode()) {
        return true;
    }

    if (damage.primary.value > 0) {
        return false;
    }

    static const auto sendBlockEffect = [this](const BlockType_t blockType, const CombatType_t combatType, const Position& targetPos) {
        if (blockType == BLOCK_DEFENSE) {
            addMagicEffect(targetPos, CONST_ME_POFF);
        } else if (blockType == BLOCK_ARMOR) {
            addMagicEffect(targetPos, CONST_ME_BLOCKHIT);
        } else if (blockType == BLOCK_IMMUNITY) {
            uint8_t hitEffect = 0;
            switch (combatType) {
                case COMBAT_UNDEFINEDDAMAGE: {
                    return;
                }
                case COMBAT_ENERGYDAMAGE:
                case COMBAT_FIREDAMAGE:
                case COMBAT_PHYSICALDAMAGE:
                case COMBAT_ICEDAMAGE:
                case COMBAT_DEATHDAMAGE: {
                    hitEffect = CONST_ME_BLOCKHIT;
                    break;
                }
                case COMBAT_EARTHDAMAGE: {
                    hitEffect = CONST_ME_GREEN_RINGS;
                    break;
                }
                case COMBAT_HOLYDAMAGE: {
                    hitEffect = CONST_ME_HOLYDAMAGE;
                    break;
                }
                default: {
                    hitEffect = CONST_ME_POFF;
                    break;
                }
            }
            addMagicEffect(targetPos, hitEffect);
        }
    };

    BlockType_t primaryBlockType;
    BlockType_t secondaryBlockType;
    if (damage.primary.type != COMBAT_NONE) {
        damage.primary.value = -damage.primary.value;
        primaryBlockType = target->blockHit(attacker, damage.primary.type, damage.primary.value, checkDefense, checkArmor, field);

        damage.primary.value = -damage.primary.value;
        sendBlockEffect(primaryBlockType, damage.primary.type, target->getPosition());
    } else {
        primaryBlockType = BLOCK_NONE;
    }

    if (damage.secondary.type != COMBAT_NONE) {
        damage.secondary.value = -damage.secondary.value;
        secondaryBlockType = target->blockHit(attacker, damage.secondary.type, damage.secondary.value, false, false, field);

        damage.secondary.value = -damage.secondary.value;
        sendBlockEffect(secondaryBlockType, damage.secondary.type, target->getPosition());
    } else {
        secondaryBlockType = BLOCK_NONE;
    }
    damage.blockType = primaryBlockType;
    return primaryBlockType != BLOCK_NONE && secondaryBlockType != BLOCK_NONE;
}

void Game::combatGetTypeInfo(const CombatType_t combatType, Creature* target, TextColor_t& color, uint8_t& effect)
{
    switch (combatType) {
        case COMBAT_PHYSICALDAMAGE: {
            Item* splash = nullptr;
            switch (target->getRace()) {
                case RACE_VENOM:
                    color = TEXTCOLOR_LIGHTGREEN;
                    effect = CONST_ME_HITBYPOISON;
                    splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_SLIME);
                    break;
                case RACE_BLOOD:
                    color = TEXTCOLOR_RED;
                    effect = CONST_ME_DRAWBLOOD;
                    if (const Tile* tile = target->getTile()) {
                        if (!tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
                            splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_BLOOD);
                        }
                    }
                    break;
                case RACE_UNDEAD:
                    color = TEXTCOLOR_LIGHTGREY;
                    effect = CONST_ME_HITAREA;
                    break;
                case RACE_FIRE:
                    color = TEXTCOLOR_ORANGE;
                    effect = CONST_ME_DRAWBLOOD;
                    break;
                case RACE_ENERGY:
                    color = TEXTCOLOR_ELECTRICPURPLE;
                    effect = CONST_ME_ENERGYHIT;
                    break;
                default:
                    color = TEXTCOLOR_NONE;
                    effect = CONST_ME_NONE;
                    break;
            }

            if (splash) {
                internalAddItem(target->getTile(), splash, INDEX_WHEREEVER, FLAG_NOLIMIT);
                splash->startDecaying();
            }

            break;
        }

        case COMBAT_ENERGYDAMAGE: {
            color = TEXTCOLOR_ELECTRICPURPLE;
            effect = CONST_ME_ENERGYHIT;
            break;
        }

        case COMBAT_EARTHDAMAGE: {
            color = TEXTCOLOR_LIGHTGREEN;
            effect = CONST_ME_GREEN_RINGS;
            break;
        }

        case COMBAT_DROWNDAMAGE: {
            color = TEXTCOLOR_LIGHTBLUE;
            effect = CONST_ME_LOSEENERGY;
            break;
        }
        case COMBAT_FIREDAMAGE: {
            color = TEXTCOLOR_ORANGE;
            effect = CONST_ME_HITBYFIRE;
            break;
        }
        case COMBAT_ICEDAMAGE: {
            color = TEXTCOLOR_SKYBLUE;
            effect = CONST_ME_ICEATTACK;
            break;
        }
        case COMBAT_HOLYDAMAGE: {
            color = TEXTCOLOR_YELLOW;
            effect = CONST_ME_HOLYDAMAGE;
            break;
        }
        case COMBAT_DEATHDAMAGE: {
            color = TEXTCOLOR_DARKRED;
            effect = CONST_ME_SMALLCLOUDS;
            break;
        }
        case COMBAT_LIFEDRAIN: {
            color = TEXTCOLOR_RED;
            effect = CONST_ME_MAGIC_RED;
            break;
        }
        default: {
            color = TEXTCOLOR_NONE;
            effect = CONST_ME_NONE;
            break;
        }
    }
}

bool Game::combatChangeHealth(Creature* attacker, Creature* target, CombatDamage& damage)
{
    const Position& targetPos = target->getPosition();
    if (damage.primary.value > 0) {
        if (target->getHealth() <= 0) {
            return false;
        }

        Player* attackerPlayer;
        if (attacker) {
            attackerPlayer = attacker->getPlayer();
        } else {
            attackerPlayer = nullptr;
        }

        Player* targetPlayer = target->getPlayer();
        if (attackerPlayer && targetPlayer && attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
            return false;
        }

        if (damage.origin != ORIGIN_NONE) {
            const auto& events = target->getCreatureEvents(CREATURE_EVENT_HEALTHCHANGE);
            if (!events.empty()) {
                for (CreatureEvent* creatureEvent : events) {
                    creatureEvent->executeHealthChange(target, attacker, damage);
                }
                damage.origin = ORIGIN_NONE;
                return combatChangeHealth(attacker, target, damage);
            }
        }

        int32_t realHealthChange = target->getHealth();
        target->gainHealth(attacker, damage.primary.value);
        realHealthChange = target->getHealth() - realHealthChange;

        if (realHealthChange > 0 && !target->isInGhostMode()) {
#if GAME_FEATURE_ANALYTICS > 0
            if (targetPlayer) {
                targetPlayer->sendImpactTracking(true, realHealthChange);
            }
#endif

#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
            std::stringExtended damageString(32);
            damageString << realHealthChange << (realHealthChange != 1 ? " hitpoints." : " hitpoint.");
#endif

            TextMessage message;
            message.type = MESSAGE_HEALED;
            message.position = targetPos;
            message.primary.value = realHealthChange;
            message.primary.color = TEXTCOLOR_PASTELRED;

            SpectatorVector spectators;
            map.getSpectators(spectators, targetPos, false, true);
            for (Creature* spectator : spectators) {
                Player* tmpPlayer = spectator->getPlayer();
#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
                if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
                    std::stringExtended sink(target->getNameDescription().length() + damageString.length() + 16);
                    sink << "You heal " << target->getNameDescription() << " for " << damageString;
                    message.type = MESSAGE_HEALED;
                    message.text = std::move(static_cast<std::string&>(sink));
                } else if (tmpPlayer == targetPlayer) {
                    std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + damageString.length() + 32);
                    if (!attacker) {
                        sink << "You were healed";
                    } else if (targetPlayer == attackerPlayer) {
                        sink << "You healed yourself";
                    } else {
                        sink << "You were healed by " << attacker->getNameDescription();
                    }
                    sink << " for " << damageString;
                    message.type = MESSAGE_HEALED;
                    message.text = std::move(static_cast<std::string&>(sink));
                } else {
                    if (message.type != MESSAGE_HEALED_OTHERS) {
                        std::stringExtended sink;
                        if (!attacker) {
                            sink.reserve(target->getNameDescription().length() + damageString.length() + 32);
                            sink << target->getNameDescription() << " was healed";
                        } else {
                            sink.reserve(attacker->getNameDescription().length() + target->getNameDescription().length() + damageString.length() + 32);
                            sink << attacker->getNameDescription() << " healed ";
                            if (attacker == target) {
                                sink << (targetPlayer ? (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "herself" : "himself") : "itself");
                            } else {
                                sink << target->getNameDescription();
                            }
                        }
                        sink << " for " << damageString;
                        message.type = MESSAGE_HEALED_OTHERS;
                        message.text = std::move(static_cast<std::string&>(sink));
                    }
                }
#endif
                tmpPlayer->sendTextMessage(message);
            }
        }
    } else {
        if (!target->isAttackable()) {
            if (!target->isInGhostMode()) {
                addMagicEffect(targetPos, CONST_ME_POFF);
            }
            return true;
        }

        Player* attackerPlayer;
        if (attacker) {
            attackerPlayer = attacker->getPlayer();
        } else {
            attackerPlayer = nullptr;
        }

        Player* targetPlayer = target->getPlayer();
        if (attackerPlayer && targetPlayer && attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
            return false;
        }

        damage.primary.value = std::abs(damage.primary.value);
        damage.secondary.value = std::abs(damage.secondary.value);

        int32_t healthChange = damage.primary.value + damage.secondary.value;
        if (healthChange == 0) {
            return true;
        }

        TextMessage message;
        message.position = targetPos;

        SpectatorVector spectators;
        if (targetPlayer && target->hasCondition(CONDITION_MANASHIELD) && damage.primary.type != COMBAT_UNDEFINEDDAMAGE) {
            int32_t manaDamage = std::min<int32_t>(targetPlayer->getMana(), healthChange);
            if (manaDamage != 0) {
                if (damage.origin != ORIGIN_NONE) {
                    const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
                    if (!events.empty()) {
                        for (CreatureEvent* creatureEvent : events) {
                            creatureEvent->executeManaChange(target, attacker, damage);
                        }
                        healthChange = damage.primary.value + damage.secondary.value;
                        if (healthChange == 0) {
                            return true;
                        }
                        manaDamage = std::min<int32_t>(targetPlayer->getMana(), healthChange);
                    }
                }

                targetPlayer->drainMana(attacker, manaDamage);
                map.getSpectators(spectators, targetPos, true, true);
                addMagicEffect(spectators, targetPos, CONST_ME_LOSEENERGY);

                message.type = MESSAGE_DAMAGE_DEALT;
                message.primary.value = manaDamage;
                message.primary.color = TEXTCOLOR_BLUE;

                for (Creature* spectator : spectators) {
                    Player* tmpPlayer = spectator->getPlayer();
                    if (tmpPlayer->getPosition().z != targetPos.z) {
                        continue;
                    }

#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
                    if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
                        std::stringExtended sink(target->getNameDescription().length() + 64);
                        sink << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana due to your attack.";
                        message.type = MESSAGE_DAMAGE_DEALT;
                        message.text = std::move(static_cast<std::string&>(sink));
                    } else if (tmpPlayer == targetPlayer) {
                        std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + 64);
                        sink << "You lose " << manaDamage << " mana";
                        if (!attacker) {
                            sink << '.';
                        } else if (targetPlayer == attackerPlayer) {
                            sink << " due to your own attack.";
                        } else {
                            sink << " due to an attack by " << attacker->getNameDescription() << '.';
                        }
                        message.type = MESSAGE_DAMAGE_RECEIVED;
                        message.text = std::move(static_cast<std::string&>(sink));
                    } else {
                        if (message.type != MESSAGE_DAMAGE_OTHERS) {
                            std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + target->getNameDescription().length() + 64);
                            sink << ucfirst(target->getNameDescription()) << " loses " << manaDamage << " mana";
                            if (attacker) {
                                sink << " due to ";
                                if (attacker == target) {
                                    sink << (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her own attack" : "his own attack");
                                } else {
                                    sink << "an attack by " << attacker->getNameDescription();
                                }
                            }
                            sink << '.';
                            message.type = MESSAGE_DAMAGE_OTHERS;
                            message.text = std::move(static_cast<std::string&>(sink));
                        }
                    }
#endif
                    tmpPlayer->sendTextMessage(message);
                }

                damage.primary.value -= manaDamage;
                if (damage.primary.value < 0) {
                    damage.secondary.value = std::max<int32_t>(0, damage.secondary.value + damage.primary.value);
                    damage.primary.value = 0;
                }
            }
        }

        int32_t realDamage = damage.primary.value + damage.secondary.value;
        if (realDamage == 0) {
            return true;
        }

        if (damage.origin != ORIGIN_NONE) {
            const auto& events = target->getCreatureEvents(CREATURE_EVENT_HEALTHCHANGE);
            if (!events.empty()) {
                for (CreatureEvent* creatureEvent : events) {
                    creatureEvent->executeHealthChange(target, attacker, damage);
                }
                damage.origin = ORIGIN_NONE;
                return combatChangeHealth(attacker, target, damage);
            }
        }

        int32_t targetHealth = target->getHealth();
        if (damage.primary.value >= targetHealth) {
            damage.primary.value = targetHealth;
            damage.secondary.value = 0;
        } else if (damage.secondary.value) {
            damage.secondary.value = std::min<int32_t>(damage.secondary.value, targetHealth - damage.primary.value);
        }

        realDamage = damage.primary.value + damage.secondary.value;
        if (realDamage == 0) {
            return true;
        }

        if (spectators.empty()) {
            map.getSpectators(spectators, targetPos, true, true);
        }

        message.primary.value = damage.primary.value;
        message.secondary.value = damage.secondary.value;

        uint8_t hitEffect;
        if (message.primary.value) {
            combatGetTypeInfo(damage.primary.type, target, message.primary.color, hitEffect);
            if (hitEffect != CONST_ME_NONE) {
                addMagicEffect(spectators, targetPos, hitEffect);
            }
        }

        if (message.secondary.value) {
            combatGetTypeInfo(damage.secondary.type, target, message.secondary.color, hitEffect);
            if (hitEffect != CONST_ME_NONE) {
                addMagicEffect(spectators, targetPos, hitEffect);
            }
        }

        message.type = MESSAGE_DAMAGE_DEALT;
        if (message.primary.color != TEXTCOLOR_NONE || message.secondary.color != TEXTCOLOR_NONE) {
#if GAME_FEATURE_ANALYTICS > 0
            if (attackerPlayer) {
#if GAME_FEATURE_ANALYTICS_IMPACT_TRACKING_EXTENDED > 0
                attackerPlayer->sendImpactTracking(damage.primary.type, damage.primary.value, "");
                if (damage.secondary.type != COMBAT_NONE) {
                    attackerPlayer->sendImpactTracking(damage.secondary.type, damage.secondary.value, "");
                }
#else
                attackerPlayer->sendImpactTracking(false, realDamage);
#endif
            }
#endif
#if GAME_FEATURE_ANALYTICS_IMPACT_TRACKING_EXTENDED > 0
            if (targetPlayer) {
                std::string cause = "field item"; //I don't have access to test server so it might be called something else
                if (attacker) {
                    cause = attacker->getName();
                }
                targetPlayer->sendImpactTracking(damage.primary.type, damage.primary.value, cause);
                if (damage.secondary.type != COMBAT_NONE) {
                    attackerPlayer->sendImpactTracking(damage.secondary.type, damage.secondary.value, cause);
                }
            }
#endif

#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
            std::stringExtended damageString(32);
            damageString << realDamage << (realDamage != 1 ? " hitpoints" : " hitpoint");
#endif

            for (Creature* spectator : spectators) {
                Player* tmpPlayer = spectator->getPlayer();
                if (tmpPlayer->getPosition().z != targetPos.z) {
                    continue;
                }

#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
                if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
                    std::stringExtended sink(target->getNameDescription().length() + damageString.length() + 32);
                    sink << ucfirst(target->getNameDescription()) << " loses " << damageString << " due to your attack.";
                    message.type = MESSAGE_DAMAGE_DEALT;
                    message.text = std::move(static_cast<std::string&>(sink));
                } else if (tmpPlayer == targetPlayer) {
                    std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + damageString.length() + 32);
                    sink << "You lose " << damageString;
                    if (!attacker) {
                        sink << '.';
                    } else if (targetPlayer == attackerPlayer) {
                        sink << " due to your own attack.";
                    } else {
                        sink << " due to an attack by " << attacker->getNameDescription() << '.';
                    }
                    message.type = MESSAGE_DAMAGE_RECEIVED;
                    message.text = std::move(static_cast<std::string&>(sink));
                } else {
                    if (message.type != MESSAGE_DAMAGE_OTHERS) {
                        std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + target->getNameDescription().length() + damageString.length() + 32);
                        sink << ucfirst(target->getNameDescription()) << " loses " << damageString;
                        if (attacker) {
                            sink << " due to ";
                            if (attacker == target) {
                                if (targetPlayer) {
                                    sink << (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her own attack" : "his own attack");
                                } else {
                                    sink << "its own attack";
                                }
                            } else {
                                sink << "an attack by " << attacker->getNameDescription();
                            }
                        }
                        sink << '.';
                        message.type = MESSAGE_DAMAGE_OTHERS;
                        message.text = std::move(static_cast<std::string&>(sink));
                    }
                }
#endif
                tmpPlayer->sendTextMessage(message);
            }
        }

        if (realDamage >= targetHealth) {
            for (CreatureEvent* creatureEvent : target->getCreatureEvents(CREATURE_EVENT_PREPAREDEATH)) {
                if (!creatureEvent->executeOnPrepareDeath(target, attacker)) {
                    return false;
                }
            }

            //Dispatch creature death event to the first safe cpu cycle
            g_dispatcher.addTask([this, capture0 = target->getID()] { checkCreatureDeath(capture0); });
        }

        target->drainHealth(attacker, realDamage);
        addCreatureHealth(spectators, target);
    }

    return true;
}

bool Game::combatChangeMana(Creature* attacker, Creature* target, CombatDamage& damage)
{
    Player* targetPlayer = target->getPlayer();
    if (!targetPlayer) {
        return true;
    }

    int32_t manaChange = damage.primary.value + damage.secondary.value;
    if (manaChange > 0) {
        if (attacker) {
            const Player* attackerPlayer = attacker->getPlayer();
            if (attackerPlayer && attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(target) == SKULL_NONE) {
                return false;
            }
        }

        if (damage.origin != ORIGIN_NONE) {
            const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
            if (!events.empty()) {
                for (CreatureEvent* creatureEvent : events) {
                    creatureEvent->executeManaChange(target, attacker, damage);
                }
                damage.origin = ORIGIN_NONE;
                return combatChangeMana(attacker, target, damage);
            }
        }

        int32_t realManaChange = targetPlayer->getMana();
        targetPlayer->changeMana(manaChange);
        realManaChange = targetPlayer->getMana() - realManaChange;

        if (realManaChange > 0 && !targetPlayer->isInGhostMode()) {
            TextMessage message(MESSAGE_HEALED, "You gained " + std::to_string(realManaChange) + " mana.");
            message.position = target->getPosition();
            message.primary.value = realManaChange;
            message.primary.color = TEXTCOLOR_MAYABLUE;
            targetPlayer->sendTextMessage(message);
        }
    } else {
        const Position& targetPos = target->getPosition();
        if (!target->isAttackable()) {
            if (!target->isInGhostMode()) {
                addMagicEffect(targetPos, CONST_ME_POFF);
            }
            return false;
        }

        Player* attackerPlayer;
        if (attacker) {
            attackerPlayer = attacker->getPlayer();
        } else {
            attackerPlayer = nullptr;
        }

        if (attackerPlayer && attackerPlayer->getSkull() == SKULL_BLACK && attackerPlayer->getSkullClient(targetPlayer) == SKULL_NONE) {
            return false;
        }

        int32_t manaLoss = std::min<int32_t>(targetPlayer->getMana(), -manaChange);
        BlockType_t blockType = target->blockHit(attacker, COMBAT_MANADRAIN, manaLoss);
        if (blockType != BLOCK_NONE) {
            addMagicEffect(targetPos, CONST_ME_POFF);
            return false;
        }

        if (manaLoss <= 0) {
            return true;
        }

        if (damage.origin != ORIGIN_NONE) {
            const auto& events = target->getCreatureEvents(CREATURE_EVENT_MANACHANGE);
            if (!events.empty()) {
                for (CreatureEvent* creatureEvent : events) {
                    creatureEvent->executeManaChange(target, attacker, damage);
                }
                damage.origin = ORIGIN_NONE;
                return combatChangeMana(attacker, target, damage);
            }
        }

        targetPlayer->drainMana(attacker, manaLoss);

        TextMessage message;
        message.type = MESSAGE_DAMAGE_DEALT;
        message.position = targetPos;
        message.primary.value = manaLoss;
        message.primary.color = TEXTCOLOR_BLUE;

        SpectatorVector spectators;
        map.getSpectators(spectators, targetPos, false, true);
        for (Creature* spectator : spectators) {
            Player* tmpPlayer = spectator->getPlayer();
#if GAME_FEATURE_SERVER_LOG_DETAILS > 0
            if (tmpPlayer == attackerPlayer && attackerPlayer != targetPlayer) {
                std::stringExtended sink(target->getNameDescription().length() + 64);
                sink << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana due to your attack.";
                message.type = MESSAGE_DAMAGE_DEALT;
                message.text = std::move(static_cast<std::string&>(sink));
            } else if (tmpPlayer == targetPlayer) {
                std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + 64);
                sink << "You lose " << manaLoss << " mana";
                if (!attacker) {
                    sink << '.';
                } else if (targetPlayer == attackerPlayer) {
                    sink << " due to your own attack.";
                } else {
                    sink << " mana due to an attack by " << attacker->getNameDescription() << '.';
                }
                message.type = MESSAGE_DAMAGE_RECEIVED;
                message.text = std::move(static_cast<std::string&>(sink));
            } else {
                if (message.type != MESSAGE_DAMAGE_OTHERS) {
                    std::stringExtended sink(NETWORKMESSAGE_PLAYERNAME_MAXLENGTH + target->getNameDescription().length() + 64);
                    sink << ucfirst(target->getNameDescription()) << " loses " << manaLoss << " mana";
                    if (attacker) {
                        sink << " due to ";
                        if (attacker == target) {
                            sink << (targetPlayer->getSex() == PLAYERSEX_FEMALE ? "her own attack" : "his own attack");
                        } else {
                            sink << "an attack by " << attacker->getNameDescription();
                        }
                    }
                    sink << '.';
                    message.type = MESSAGE_DAMAGE_OTHERS;
                    message.text = std::move(static_cast<std::string&>(sink));
                }
            }
#endif
            tmpPlayer->sendTextMessage(message);
        }
    }

    return true;
}

void Game::addCreatureHealth(const Creature* target)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, target->getPosition(), true, true);
    addCreatureHealth(spectators, target);
}

void Game::addCreatureHealth(const SpectatorVector& spectators, const Creature* target)
{
    uint8_t healthPercent = std::ceil(static_cast<double>(target->getHealth()) / std::max<int32_t>(target->getMaxHealth(), 1) * 100);
#if GAME_FEATURE_PARTY_LIST > 0
    if (const Player* targetPlayer = target->getPlayer()) {
        if (Party* party = targetPlayer->getParty()) {
            party->updatePlayerHealth(targetPlayer, target, healthPercent);
        }
    } else if (const Creature* master = target->getMaster()) {
        if (const Player* masterPlayer = master->getPlayer()) {
            if (Party* party = masterPlayer->getParty()) {
                party->updatePlayerHealth(masterPlayer, target, healthPercent);
            }
        }
    }
#endif

    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendCreatureHealth(target, healthPercent);
        }
    }
}

#if GAME_FEATURE_PARTY_LIST > 0
void Game::addPlayerMana(const Player* target)
{
    if (Party* party = target->getParty()) {
        uint8_t manaPercent = std::ceil((static_cast<double>(target->getMana()) / std::max<int32_t>(target->getMaxMana(), 1)) * 100);
        party->updatePlayerMana(target, manaPercent);
    }
}
#endif

#if GAME_FEATURE_PLAYER_VOCATIONS > 0
void Game::addPlayerVocation(const Player* target)
{
#if GAME_FEATURE_PARTY_LIST > 0
    if (Party* party = target->getParty()) {
        party->updatePlayerVocation(target);
    }
#endif

    SpectatorVector spectators;
    map.getSpectators(spectators, target->getPosition(), true, true);

    for (Creature* spectator : spectators) {
        if (Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendPlayerVocation(target);
        }
    }
}
#endif

void Game::addMagicEffect(const Position& pos, const uint8_t effect)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, pos, true, true);
    addMagicEffect(spectators, pos, effect);
}

void Game::addMagicEffect(const SpectatorVector& spectators, const Position& pos, const uint8_t effect)
{
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendMagicEffect(pos, effect);
        }
    }
}

void Game::addDistanceEffect(const Position& fromPos, const Position& toPos, const uint8_t effect)
{
    SpectatorVector spectators;
    if (std::abs(fromPos.x - toPos.x) == 1 || std::abs(fromPos.y - toPos.y) == 1) {
        int32_t minRangeX = Map::maxViewportX;
        int32_t maxRangeX = Map::maxViewportX;
        int32_t minRangeY = Map::maxViewportY;
        int32_t maxRangeY = Map::maxViewportY;
        if (fromPos.y > toPos.y) {
            ++minRangeY;
        } else if (fromPos.y < toPos.y) {
            ++maxRangeY;
        }

        if (fromPos.x < toPos.x) {
            ++maxRangeX;
        } else if (fromPos.x > toPos.x) {
            ++minRangeX;
        }
        map.getSpectators(spectators, fromPos, false, true, minRangeX, maxRangeX, minRangeY, maxRangeY);
    } else {
        SpectatorVector tospectators;
        map.getSpectators(spectators, fromPos, false, true);
        map.getSpectators(tospectators, toPos, false, true);
        spectators.mergeSpectators(tospectators);
    }
    addDistanceEffect(spectators, fromPos, toPos, effect);
}

void Game::addDistanceEffect(const SpectatorVector& spectators, const Position& fromPos, const Position& toPos, const uint8_t effect)
{
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendDistanceShoot(fromPos, toPos, effect);
        }
    }
}

void Game::updateCreatureData(const Creature* creature) const
{
    for (const auto& it : players) {
        it.second->updateCreatureData(creature);
    }
}

void Game::startDecay(Item* item)
{
    if (!item) {
        return;
    }

    const ItemDecayState_t decayState = item->getDecaying();
    if (decayState == DECAYING_STOPPING || (!item->canDecay() && decayState == DECAYING_TRUE)) {
        stopDecay(item);
        return;
    }

    if (!item->canDecay() || decayState == DECAYING_TRUE) {
        return;
    }

    const int32_t duration = item->getIntAttr(ITEM_ATTRIBUTE_DURATION);
    if (duration > 0) {
        g_decay.startDecay(item, duration);
    } else {
        internalDecayItem(item);
    }
}

void Game::stopDecay(Item* item)
{
    if (item->hasAttribute(ITEM_ATTRIBUTE_DECAYSTATE)) {
        if (item->hasAttribute(ITEM_ATTRIBUTE_DURATION_TIMESTAMP)) {
            g_decay.stopDecay(item, item->getIntAttr(ITEM_ATTRIBUTE_DURATION_TIMESTAMP));
            item->removeAttribute(ITEM_ATTRIBUTE_DURATION_TIMESTAMP);
        } else {
            item->removeAttribute(ITEM_ATTRIBUTE_DECAYSTATE);
        }
    }
}

void Game::internalDecayItem(Item* item)
{
    const ItemType& it = Item::items[item->getID()];
    if (it.decayTo != 0) {
        transformItem(item, it.decayTo);
    } else {
        const ReturnValue ret = internalRemoveItem(item);
        if (ret != RETURNVALUE_NOERROR) {
            std::cout << "[Debug - Game::internalDecayItem] internalDecayItem failed, error code: " << static_cast<uint32_t>(ret) << ", item id: " << item->getID() << std::endl;
        }
    }
}

void Game::checkLight()
{
    g_dispatcher.addEvent(EVENT_LIGHTINTERVAL, [this] { checkLight(); });

    lightHour += lightHourDelta;

    if (lightHour > 1440) {
        lightHour -= 1440;
    }

    if (std::abs(lightHour - SUNRISE) < 2 * lightHourDelta) {
        lightState = LIGHT_STATE_SUNRISE;
    } else if (std::abs(lightHour - SUNSET) < 2 * lightHourDelta) {
        lightState = LIGHT_STATE_SUNSET;
    }

    int32_t newLightLevel = lightLevel;
    bool lightChange = false;

    switch (lightState) {
        case LIGHT_STATE_SUNRISE: {
            newLightLevel += (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT) / 30;
            lightChange = true;
            break;
        }
        case LIGHT_STATE_SUNSET: {
            newLightLevel -= (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT) / 30;
            lightChange = true;
            break;
        }
        default:
            break;
    }

    if (newLightLevel <= LIGHT_LEVEL_NIGHT) {
        lightLevel = LIGHT_LEVEL_NIGHT;
        lightState = LIGHT_STATE_NIGHT;
    } else if (newLightLevel >= LIGHT_LEVEL_DAY) {
        lightLevel = LIGHT_LEVEL_DAY;
        lightState = LIGHT_STATE_DAY;
    } else {
        lightLevel = newLightLevel;
    }

    if (lightChange) {
        const LightInfo lightInfo = getWorldLightInfo();

        for (const auto& it : players) {
            it.second->sendWorldLight(lightInfo);
#if CLIENT_VERSION >= 1121
            it.second->sendTibiaTime(lightHour);
#endif
        }
    }
#if CLIENT_VERSION >= 1121
    else {
        for (const auto& it : players) {
            it.second->sendTibiaTime(lightHour);
        }
    }
#endif
}

LightInfo Game::getWorldLightInfo() const
{
    return { lightLevel, 0xD7 };
}

void Game::shutdown()
{
    std::cout << "Shutting down..." << std::flush;

    g_databaseTasks.shutdown();
    g_dispatcher.shutdown();
    map.spawns.clear();
    raids.clear();

    cleanup();

    if (serviceManager) {
        serviceManager->stop();
    }

    ConnectionManager::getInstance().closeAll();

    std::cout << " done!" << std::endl;
}

void Game::cleanup()
{
    //free memory
    for (const auto creature : ToReleaseCreatures) {
        creature->decrementReferenceCounter();
    }
    ToReleaseCreatures.clear();

    for (const auto item : ToReleaseItems) {
        item->decrementReferenceCounter();
    }
    ToReleaseItems.clear();
}

void Game::ReleaseCreature(Creature* creature)
{
    ToReleaseCreatures.push_back(creature);
}

void Game::ReleaseItem(Item* item)
{
    ToReleaseItems.push_back(item);
}

void Game::broadcastMessage(const std::string& text, const MessageClasses type) const
{
    std::cout << "> Broadcasted message: \"" << text << "\"." << std::endl;
    for (const auto& it : players) {
        it.second->sendTextMessage(type, text);
    }
}

#if CLIENT_VERSION >= 854
void Game::updateCreatureWalkthrough(const Creature* creature)
{
    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        const Player* tmpPlayer = spectator->getPlayer();
        tmpPlayer->sendCreatureWalkthrough(creature, tmpPlayer->canWalkthroughEx(creature));
    }
}
#endif

void Game::updateCreatureSkull(const Creature* creature)
{
    if (getWorldType() != WORLD_TYPE_PVP) {
        return;
    }

    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureSkull(creature);
    }
}

#if CLIENT_VERSION >= 1000 && CLIENT_VERSION < 1185
void Game::updatePlayerHelpers(const Player& player)
{
    const uint32_t creatureId = player.getID();
    const uint16_t helpers = player.getHelpers();

    SpectatorVector spectators;
    map.getSpectators(spectators, player.getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendCreatureHelpers(creatureId, helpers);
    }
}
#endif

#if CLIENT_VERSION >= 910
void Game::updateCreatureType(const Creature* creature)
{
    const Player* masterPlayer = nullptr;

    CreatureType_t creatureType = creature->getType();
    if (creatureType == CREATURETYPE_MONSTER) {
        const Creature* master = creature->getMaster();
        if (master) {
            masterPlayer = master->getPlayer();
            if (masterPlayer) {
                creatureType = CREATURETYPE_SUMMON_OTHERS;
            }
        }
    }
    if (creature->isHealthHidden()) {
        creatureType = CREATURETYPE_HIDDEN;
    }

    //send to clients
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), true, true);
    if (creatureType == CREATURETYPE_SUMMON_OTHERS) {
        for (Creature* spectator : spectators) {
            const Player* player = spectator->getPlayer();
            if (masterPlayer == player) {
                player->sendCreatureType(creature, CREATURETYPE_SUMMON_OWN);
            } else {
                player->sendCreatureType(creature, creatureType);
            }
        }
    } else {
        for (Creature* spectator : spectators) {
            spectator->getPlayer()->sendCreatureType(creature, creatureType);
        }
    }
}
#endif

void Game::updatePremium(Account& account)
{
    bool save = false;
    const time_t timeNow = time(nullptr);

    if (account.premiumDays != 0 && account.premiumDays != std::numeric_limits<uint16_t>::max()) {
        if (account.lastDay == 0) {
            account.lastDay = timeNow;
            save = true;
        } else {
            const uint32_t days = (timeNow - account.lastDay) / 86400;
            if (days > 0) {
                if (days >= account.premiumDays) {
                    account.premiumDays = 0;
                    account.lastDay = 0;
                } else {
                    account.premiumDays -= days;
                    const time_t remainder = (timeNow - account.lastDay) % 86400;
                    account.lastDay = timeNow - remainder;
                }

                save = true;
            }
        }
    } else if (account.lastDay != 0) {
        account.lastDay = 0;
        save = true;
    }

    if (save && !IOLoginData::saveAccount(account)) {
        std::cout << "> ERROR: Failed to save account: " << account.name << "!" << std::endl;
    }
}

void Game::loadMotdNum()
{
    DBResult_ptr result = g_database.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'motd_num' LIMIT 1");
    if (result) {
        motdNum = result->getNumber<uint32_t>("value");
    } else {
        g_database.executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('motd_num', '0')");
    }

    result = g_database.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'motd_hash' LIMIT 1");
    if (result) {
        motdHash = std::move(result->getString("value"));
        if (motdHash != transformToSHA1(g_config.getString(ConfigManager::MOTD))) {
            ++motdNum;
        }
    } else {
        g_database.executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('motd_hash', '')");
    }
}

void Game::saveMotdNum() const
{
    std::stringExtended query(128);
    query << "UPDATE `server_config` SET `value` = '" << motdNum << "' WHERE `config` = 'motd_num'";
    g_database.executeQuery(query);

    query.clear();
    query << "UPDATE `server_config` SET `value` = '" << transformToSHA1(g_config.getString(ConfigManager::MOTD)) << "' WHERE `config` = 'motd_hash'";
    g_database.executeQuery(query);
}

void Game::checkPlayersRecord()
{
    const size_t playersOnline = getPlayersOnline();
    if (playersOnline > playersRecord) {
        const uint32_t previousRecord = playersRecord;
        playersRecord = playersOnline;

        for (auto& it : g_globalEvents->getEventMap(GLOBALEVENT_RECORD)) {
            it.second.executeRecord(playersRecord, previousRecord);
        }
        updatePlayersRecord();
    }
}

void Game::updatePlayersRecord() const
{
    std::stringExtended query(128);
    query << "UPDATE `server_config` SET `value` = '" << playersRecord << "' WHERE `config` = 'players_record'";
    g_database.executeQuery(query);
}

void Game::loadPlayersRecord()
{
    const DBResult_ptr result = g_database.storeQuery("SELECT `value` FROM `server_config` WHERE `config` = 'players_record'");
    if (result) {
        playersRecord = result->getNumber<uint32_t>("value");
    } else {
        g_database.executeQuery("INSERT INTO `server_config` (`config`, `value`) VALUES ('players_record', '0')");
    }
}

uint64_t Game::getExperienceStage(const uint32_t level)
{
    if (!stagesEnabled) {
        return g_config.getNumber(ConfigManager::RATE_EXPERIENCE);
    }

    if (useLastStageLevel && level >= lastStageLevel) {
        return stages[lastStageLevel];
    }

    return stages[level];
}

bool Game::loadExperienceStages()
{
    pugi::xml_document doc;
    const pugi::xml_parse_result result = doc.load_file("data/XML/stages.xml");
    if (!result) {
        printXMLError("Error - Game::loadExperienceStages", "data/XML/stages.xml", result);
        return false;
    }

    for (auto stageNode : doc.child("stages").children()) {
        if (strcasecmp(stageNode.name(), "config") == 0) {
            stagesEnabled = stageNode.attribute("enabled").as_bool();
        } else {
            uint32_t minLevel;
            uint32_t maxLevel;
            uint32_t multiplier;

            pugi::xml_attribute minLevelAttribute = stageNode.attribute("minlevel");
            if (minLevelAttribute) {
                minLevel = pugi::cast<uint32_t>(minLevelAttribute.value());
            } else {
                minLevel = 1;
            }

            pugi::xml_attribute maxLevelAttribute = stageNode.attribute("maxlevel");
            if (maxLevelAttribute) {
                maxLevel = pugi::cast<uint32_t>(maxLevelAttribute.value());
            } else {
                maxLevel = 0;
                lastStageLevel = minLevel;
                useLastStageLevel = true;
            }

            pugi::xml_attribute multiplierAttribute = stageNode.attribute("multiplier");
            if (multiplierAttribute) {
                multiplier = pugi::cast<uint32_t>(multiplierAttribute.value());
            } else {
                multiplier = 1;
            }

            if (useLastStageLevel) {
                stages[lastStageLevel] = multiplier;
            } else {
                for (uint32_t i = minLevel; i <= maxLevel; ++i) {
                    stages[i] = multiplier;
                }
            }
        }
    }
    return true;
}

void Game::playerInviteToParty(Player* player, const uint32_t invitedId)
{
    if (player->getID() == invitedId) {
        return;
    }

    Player* invitedPlayer = getPlayerByID(invitedId);
    if (!invitedPlayer || invitedPlayer->isInviting(player)) {
        return;
    }

    if (invitedPlayer->getParty()) {
        std::stringExtended ss(invitedPlayer->getName().length() + static_cast<size_t>(32));
        ss << invitedPlayer->getName() << " is already in a party.";
        player->sendTextMessage(MESSAGE_INFO_DESCR, ss);
        return;
    }

    Party* party = player->getParty();
    if (!party) {
        party = new Party(player);
    } else if (party->getLeader() != player) {
        return;
    }

    party->invitePlayer(*invitedPlayer);
}

void Game::playerJoinParty(Player* player, const uint32_t leaderId)
{
    const Player* leader = getPlayerByID(leaderId);
    if (!leader || !leader->isInviting(player)) {
        return;
    }

    Party* party = leader->getParty();
    if (!party || party->getLeader() != leader) {
        return;
    }

    if (player->getParty()) {
        player->sendTextMessage(MESSAGE_INFO_DESCR, "You are already in a party.");
        return;
    }

    party->joinParty(*player);
}

void Game::playerRevokePartyInvitation(const Player* player, const uint32_t invitedId)
{
    Party* party = player->getParty();
    if (!party || party->getLeader() != player) {
        return;
    }

    Player* invitedPlayer = getPlayerByID(invitedId);
    if (!invitedPlayer || !player->isInviting(invitedPlayer)) {
        return;
    }

    party->revokeInvitation(*invitedPlayer);
}

void Game::playerPassPartyLeadership(const Player* player, const uint32_t newLeaderId)
{
    Party* party = player->getParty();
    if (!party || party->getLeader() != player) {
        return;
    }

    Player* newLeader = getPlayerByID(newLeaderId);
    if (!newLeader || !player->isPartner(newLeader)) {
        return;
    }

    party->passPartyLeadership(newLeader);
}

void Game::playerLeaveParty(Player* player)
{
    Party* party = player->getParty();
    if (!party || player->hasCondition(CONDITION_INFIGHT)) {
        return;
    }

    party->leaveParty(player);
}

void Game::playerEnableSharedPartyExperience(Player* player, const bool sharedExpActive)
{
    Party* party = player->getParty();
    if (!party || (player->hasCondition(CONDITION_INFIGHT) && player->getZone() != ZONE_PROTECTION)) {
        return;
    }

    party->setSharedExperience(player, sharedExpActive);
}

void Game::sendGuildMotd(const uint32_t playerId)
{
    const Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    const Guild* guild = player->getGuild();
    if (guild) {
        player->sendChannelMessage("Message of the Day", guild->getMotd(), TALKTYPE_CHANNEL_R1, CHANNEL_GUILD);
    }
}

void Game::kickPlayer(const uint32_t playerId, const bool displayEffect)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    player->kickPlayer(displayEffect);
}

void Game::playerReportRuleViolation(Player* player, const std::string& targetName, const uint8_t reportType, const uint8_t reportReason, const std::string& comment, const std::string& translation)
{
    g_events->eventPlayerOnReportRuleViolation(player, targetName, reportType, reportReason, comment, translation);
}

void Game::playerMonsterCyclopedia(Player* player)
{
    player->sendMonsterCyclopedia();
    player->sendCyclopediaBonusEffects();
}

void Game::playerCyclopediaMonsters(Player* player, const std::string& race)
{
    player->sendCyclopediaMonsters(race);
}

void Game::playerCyclopediaRace(Player* player, const uint16_t monsterId)
{
    player->sendCyclopediaRace(monsterId);
}

#if GAME_FEATURE_CYCLOPEDIA_CHARACTERINFO > 0
void Game::playerCyclopediaCharacterInfo(Player* player, uint32_t characterID, CyclopediaCharacterInfoType_t characterInfoType, uint16_t entriesPerPage, uint16_t page)
{
    uint32_t playerGUID = player->getGUID();
    if (characterID != playerGUID) {
        //For now allow viewing only our character since we don't have tournaments supported
        player->sendCyclopediaCharacterNoData(characterInfoType, 2);
        return;
    }

    switch (characterInfoType) {
        case CYCLOPEDIA_CHARACTERINFO_BASEINFORMATION: player->sendCyclopediaCharacterBaseInformation(); break;
        case CYCLOPEDIA_CHARACTERINFO_GENERALSTATS: player->sendCyclopediaCharacterGeneralStats(); break;
        case CYCLOPEDIA_CHARACTERINFO_COMBATSTATS: player->sendCyclopediaCharacterCombatStats(); break;
        case CYCLOPEDIA_CHARACTERINFO_RECENTDEATHS: {
            std::stringExtended query(1024);
            uint32_t offset = static_cast<uint32_t>(page - 1) * entriesPerPage;
            query << "SELECT `time`, `level`, `killed_by`, `mostdamage_by`, (select count(*) FROM `player_deaths` WHERE `player_id` = " << playerGUID << ") as `entries` FROM `player_deaths` WHERE `player_id` = " << playerGUID << " ORDER BY `time` DESC LIMIT " << offset << ", " << entriesPerPage;

            uint32_t playerID = player->getID();
            std::function<void(DBResult_ptr, bool)> callback = [playerID, page, entriesPerPage](DBResult_ptr result, bool) {
                Player* player = g_game.getPlayerByID(playerID);
                if (!player) {
                    return;
                }

                player->resetAsyncOngoingTask(PlayerAsyncTask_RecentDeaths);
                if (!result) {
                    player->sendCyclopediaCharacterRecentDeaths(0, 0, {});
                    return;
                }

                uint32_t pages = result->getNumber<uint32_t>("entries");
                pages += entriesPerPage - 1;
                pages /= entriesPerPage;

                std::vector<RecentDeathEntry> entries;
                entries.reserve(result->countResults());
                do {
                    std::string cause1 = result->getString("killed_by");
                    std::string cause2 = result->getString("mostdamage_by");

                    std::stringExtended cause(1024);
                    cause << "Died at Level " << result->getNumber<uint32_t>("level") << " by";
                    if (!cause1.empty()) {
                        const char& character = cause1.front();
                        if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u') {
                            cause << " an ";
                        } else {
                            cause << " a ";
                        }
                        cause << cause1;
                    }

                    if (!cause2.empty()) {
                        if (!cause1.empty()) {
                            cause << " and ";
                        }

                        const char& character = cause2.front();
                        if (character == 'a' || character == 'e' || character == 'i' || character == 'o' || character == 'u') {
                            cause << " an ";
                        } else {
                            cause << " a ";
                        }
                        cause << cause2;
                    }
                    cause << '.';
                    entries.emplace_back(std::move(static_cast<std::string&>(cause)), result->getNumber<uint32_t>("time"));
                } while (result->next());
                player->sendCyclopediaCharacterRecentDeaths(page, static_cast<uint16_t>(pages), entries);
            };
            g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)), callback, true);
            player->addAsyncOngoingTask(PlayerAsyncTask_RecentDeaths);
            break;
        }
        case CYCLOPEDIA_CHARACTERINFO_RECENTPVPKILLS: {
            //TODO: add guildwar, assists and arena kills
            const std::string& escapedName = g_database.escapeString(player->getName());
            std::stringExtended query(1024);
            uint32_t offset = static_cast<uint32_t>(page - 1) * entriesPerPage;
            query << "SELECT `d`.`time`, `d`.`killed_by`, `d`.`mostdamage_by`, `d`.`unjustified`, `d`.`mostdamage_unjustified`, `p`.`name`, (select count(*) FROM `player_deaths` WHERE ((`killed_by` = " << escapedName << " AND `is_player` = 1) OR (`mostdamage_by` = " << escapedName << " AND `mostdamage_is_player` = 1))) as `entries` FROM `player_deaths` AS `d` INNER JOIN `players` AS `p` ON `d`.`player_id` = `p`.`id` WHERE ((`d`.`killed_by` = " << escapedName << " AND `d`.`is_player` = 1) OR (`d`.`mostdamage_by` = " << escapedName << " AND `d`.`mostdamage_is_player` = 1)) ORDER BY `time` DESC LIMIT " << offset << ", " << entriesPerPage;

            uint32_t playerID = player->getID();
            std::function<void(DBResult_ptr, bool)> callback = [playerID, page, entriesPerPage](DBResult_ptr result, bool) {
                Player* player = g_game.getPlayerByID(playerID);
                if (!player) {
                    return;
                }

                player->resetAsyncOngoingTask(PlayerAsyncTask_RecentPvPKills);
                if (!result) {
                    player->sendCyclopediaCharacterRecentPvPKills(0, 0, {});
                    return;
                }

                uint32_t pages = result->getNumber<uint32_t>("entries");
                pages += entriesPerPage - 1;
                pages /= entriesPerPage;

                std::vector<RecentPvPKillEntry> entries;
                entries.reserve(result->countResults());
                do {
                    std::string cause1 = result->getString("killed_by");
                    std::string cause2 = result->getString("mostdamage_by");
                    std::string name = result->getString("name");

                    uint8_t status = CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_JUSTIFIED;
                    if (player->getName() == cause1) {
                        if (result->getNumber<uint32_t>("unjustified") == 1) {
                            status = CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_UNJUSTIFIED;
                        }
                    } else if (player->getName() == cause2) {
                        if (result->getNumber<uint32_t>("mostdamage_unjustified") == 1) {
                            status = CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_UNJUSTIFIED;
                        }
                    }

                    std::stringExtended description(1024);
                    description << "Killed " << name << '.';
                    entries.emplace_back(std::move(static_cast<std::string&>(description)), result->getNumber<uint32_t>("time"), status);
                } while (result->next());
                player->sendCyclopediaCharacterRecentPvPKills(page, static_cast<uint16_t>(pages), entries);
            };
            g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)), callback, true);
            player->addAsyncOngoingTask(PlayerAsyncTask_RecentPvPKills);
            break;
        }
        case CYCLOPEDIA_CHARACTERINFO_ACHIEVEMENTS: player->sendCyclopediaCharacterAchievements(); break;
        case CYCLOPEDIA_CHARACTERINFO_ITEMSUMMARY: player->sendCyclopediaCharacterItemSummary(); break;
        case CYCLOPEDIA_CHARACTERINFO_OUTFITSMOUNTS: player->sendCyclopediaCharacterOutfitsMounts(); break;
        case CYCLOPEDIA_CHARACTERINFO_STORESUMMARY: player->sendCyclopediaCharacterStoreSummary(); break;
        case CYCLOPEDIA_CHARACTERINFO_INSPECTION: player->sendCyclopediaCharacterInspection(); break;
        case CYCLOPEDIA_CHARACTERINFO_BADGES: player->sendCyclopediaCharacterBadges(); break;
        case CYCLOPEDIA_CHARACTERINFO_TITLES: player->sendCyclopediaCharacterTitles(); break;
        default: player->sendCyclopediaCharacterNoData(characterInfoType, 1); break;
    }
}
#endif

#if GAME_FEATURE_HIGHSCORES > 0
void Game::playerHighscores(Player* player, HighscoreType_t type, uint8_t category, uint32_t vocation, const std::string&, uint16_t page, uint8_t entriesPerPage)
{
    if (player->hasAsyncOngoingTask(PlayerAsyncTask_Highscore)) {
        return;
    }

    std::string categoryName;
    switch (category) {
        case HIGHSCORE_CATEGORY_FIST_FIGHTING: categoryName = "skill_fist"; break;
        case HIGHSCORE_CATEGORY_CLUB_FIGHTING: categoryName = "skill_club"; break;
        case HIGHSCORE_CATEGORY_SWORD_FIGHTING: categoryName = "skill_sword"; break;
        case HIGHSCORE_CATEGORY_AXE_FIGHTING: categoryName = "skill_axe"; break;
        case HIGHSCORE_CATEGORY_DISTANCE_FIGHTING: categoryName = "skill_dist"; break;
        case HIGHSCORE_CATEGORY_SHIELDING: categoryName = "skill_shielding"; break;
        case HIGHSCORE_CATEGORY_FISHING: categoryName = "skill_fishing"; break;
        case HIGHSCORE_CATEGORY_MAGIC_LEVEL: categoryName = "maglevel"; break;
        default: {
            category = HIGHSCORE_CATEGORY_EXPERIENCE;
            categoryName = "experience";
            break;
        }
    }

    std::stringExtended query(1024);
    if (type == HIGHSCORE_GETENTRIES) {
        uint32_t startPage = (static_cast<uint32_t>(page - 1) * static_cast<uint32_t>(entriesPerPage));
        uint32_t endPage = startPage + static_cast<uint32_t>(entriesPerPage);
        query << "SELECT *, @row AS `entries`, " << page << " AS `page` FROM (SELECT *, (@row := @row + 1) AS `rn` FROM (SELECT `id`, `name`, `level`, `vocation`, `" << categoryName << "` AS `points`, @curRank := IF(@prevRank = `" << categoryName << "`, @curRank, IF(@prevRank := `" << categoryName << "`, @curRank + 1, @curRank + 1)) AS `rank` FROM `players` `p`, (SELECT @curRank := 0, @prevRank := NULL, @row := 0) `r` ORDER BY `" << categoryName << "` DESC) `t`";
        if (vocation != 0xFFFFFFFF) {
            bool firstVocation = true;

            const auto& vocationsMap = g_vocations.getVocations();
            for (const auto& it : vocationsMap) {
                const Vocation& voc = it.second;
                if (voc.getFromVocation() == vocation) {
                    if (firstVocation) {
                        query << " WHERE `vocation` = " << voc.getId();
                        firstVocation = false;
                    } else {
                        query << " OR `vocation` = " << voc.getId();
                    }
                }
            }
        }
        query << ") `T` WHERE `rn` > " << startPage << " AND `rn` <= " << endPage;
    } else if (type == HIGHSCORE_OURRANK) {
        std::string entriesStr = std::to_string(entriesPerPage);
        query << "SELECT *, @row AS `entries`, (@ourRow DIV " << entriesStr << ") + 1 AS `page` FROM (SELECT *, (@row := @row + 1) AS `rn`, @ourRow := IF(`id` = " << player->getGUID() << ", @row - 1, @ourRow) AS `rw` FROM (SELECT `id`, `name`, `level`, `vocation`, `" << categoryName << "` AS `points`, @curRank := IF(@prevRank = `" << categoryName << "`, @curRank, IF(@prevRank := `" << categoryName << "`, @curRank + 1, @curRank + 1)) AS `rank` FROM `players` `p`, (SELECT @curRank := 0, @prevRank := NULL, @row := 0, @ourRow := 0) `r` ORDER BY `" << categoryName << "` DESC) `t`";
        if (vocation != 0xFFFFFFFF) {
            bool firstVocation = true;

            const auto& vocationsMap = g_vocations.getVocations();
            for (const auto& it : vocationsMap) {
                const Vocation& voc = it.second;
                if (voc.getFromVocation() == vocation) {
                    if (firstVocation) {
                        query << " WHERE `vocation` = " << voc.getId();
                        firstVocation = false;
                    } else {
                        query << " OR `vocation` = " << voc.getId();
                    }
                }
            }
        }
        query << ") `T` WHERE `rn` > ((@ourRow DIV " << entriesStr << ") * " << entriesStr << ") AND `rn` <= (((@ourRow DIV " << entriesStr << ") * " << entriesStr << ") + " << entriesStr << ")";
    }

    uint32_t playerID = player->getID();
    std::function<void(DBResult_ptr, bool)> callback = [playerID, category, vocation, entriesPerPage](DBResult_ptr result, bool) {
        Player* player = g_game.getPlayerByID(playerID);
        if (!player) {
            return;
        }

        player->resetAsyncOngoingTask(PlayerAsyncTask_Highscore);
        if (!result) {
            player->sendHighscoresNoData();
            return;
        }

        uint16_t page = result->getNumber<uint16_t>("page");
        uint32_t pages = result->getNumber<uint32_t>("entries");
        pages += entriesPerPage - 1;
        pages /= entriesPerPage;

        std::vector<HighscoreCharacter> characters;
        characters.reserve(result->countResults());
        do {
            uint8_t characterVocation;
            Vocation* voc = g_vocations.getVocation(result->getNumber<uint16_t>("vocation"));
            if (voc) {
                characterVocation = voc->getClientId();
            } else {
                characterVocation = 0;
            }
            characters.emplace_back(std::move(result->getString("name")), result->getNumber<uint64_t>("points"), result->getNumber<uint32_t>("id"), result->getNumber<uint32_t>("rank"), result->getNumber<uint16_t>("level"), characterVocation);
        } while (result->next());
        player->sendHighscores(characters, category, vocation, page, static_cast<uint16_t>(pages));
    };
    g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)), callback, true);
    player->addAsyncOngoingTask(PlayerAsyncTask_Highscore);
}
#endif

void Game::playerTournamentLeaderboard(Player* player, const uint8_t leaderboardType)
{
    if (leaderboardType > 1) {
        return;
    }

    player->sendTournamentLeaderboard();
}

void Game::playerReportBug(Player* player, const std::string& message, const Position& position, const uint8_t category)
{
    g_events->eventPlayerOnReportBug(player, message, position, category);
}

void Game::playerDebugAssert(const Player* player, const std::string& assertLine, const std::string& date, const std::string& description, const std::string& comment)
{
    // TODO: move debug assertions to database
    FILE* file = fopen("client_assertions.txt", "a");
    if (file) {
        fprintf(file, "----- %s - %s (%s) -----\n", formatDate(time(nullptr)).c_str(), player->getName().c_str(), convertIPToString(player->getIP()).c_str());
        fprintf(file, "%s\n%s\n%s\n%s\n", assertLine.c_str(), date.c_str(), description.c_str(), comment.c_str());
        fclose(file);
    }
}

#if GAME_FEATURE_MARKET > 0
void Game::playerLeaveMarket(Player* player)
{
    player->setInMarket(false);
}

void Game::playerBrowseMarket(Player* player, const uint16_t spriteId)
{
#if GAME_FEATURE_STASH > 0
    if (!player->isInMarket() && !player->isMarketAvailable()) {
#else
    if (!player->isInMarket()) {
#endif
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(spriteId);
    if (it.id == 0) {
        return;
    }

    if (it.wareId == 0) {
        return;
    }

#if GAME_FEATURE_STASH > 0
    if (!player->isInMarket()) {
        // we should have depot id from the depot we are standing in front of
        if (player->getLastDepotId() == -1) {
            return;
        }

        player->sendMarketEnter(player->getLastDepotId());
    }
#endif

    const MarketOfferList& buyOffers = IOMarket::getActiveOffers(MARKETACTION_BUY, it.id);
    const MarketOfferList& sellOffers = IOMarket::getActiveOffers(MARKETACTION_SELL, it.id);
    player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
    player->sendMarketDetail(it.id);
}

void Game::playerBrowseMarketOwnOffers(const Player * player)
{
    if (!player->isInMarket()) {
        return;
    }

    const MarketOfferList& buyOffers = IOMarket::getOwnOffers(MARKETACTION_BUY, player->getGUID());
    const MarketOfferList& sellOffers = IOMarket::getOwnOffers(MARKETACTION_SELL, player->getGUID());
    player->sendMarketBrowseOwnOffers(buyOffers, sellOffers);
}

void Game::playerBrowseMarketOwnHistory(const Player * player)
{
    if (!player->isInMarket()) {
        return;
    }

    const HistoryMarketOfferList& buyOffers = IOMarket::getOwnHistory(MARKETACTION_BUY, player->getGUID());
    const HistoryMarketOfferList& sellOffers = IOMarket::getOwnHistory(MARKETACTION_SELL, player->getGUID());
    player->sendMarketBrowseOwnHistory(buyOffers, sellOffers);
}

void Game::playerCreateMarketOffer(Player * player, uint8_t type, const uint16_t spriteId, const uint16_t amount, const uint32_t price, const bool anonymous)
{
    if (!player->isInMarket()) {
        return;
    }

    if (g_config.getBoolean(ConfigManager::MARKET_PREMIUM) && !player->isPremium()) {
        player->sendMarketLeave();
        return;
    }

    const ItemType& itt = Item::items.getItemIdByClientId(spriteId);
    if (itt.id == 0 || itt.wareId == 0) {
        return;
    }

    const ItemType& it = Item::items.getItemIdByClientId(itt.wareId);
    if (it.id == 0 || it.wareId == 0) {
        return;
    }

    if (!it.stackable && amount > 2000) {
        return;
    }

    const uint32_t maxOfferCount = g_config.getNumber(ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER);
    if (maxOfferCount != 0 && IOMarket::getPlayerOfferCount(player->getGUID()) >= maxOfferCount) {
        return;
    }

    uint64_t fee = price / 100. * amount;
    if (fee < 20) {
        fee = 20;
#if CLIENT_VERSION >= 1220
    } else if (fee > 100000) {
        fee = 100000;
    }
#else
    } else if (fee > 1000) {
        fee = 1000;
    }
#endif

    if (type == MARKETACTION_SELL) {
        if (fee > player->bankBalance) {
            return;
        }

        DepotChest* depotChest = player->getDepotChest(player->getLastDepotId(), false);
        if (!depotChest) {
            return;
        }

        const std::vector<Item*> itemList = getMarketItemList(it.wareId, amount, depotChest, player->getInbox());
        if (itemList.empty()) {
            return;
        }

        if (it.stackable) {
            uint16_t tmpAmount = amount;
            for (Item* item : itemList) {
                const uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
                tmpAmount -= removeCount;
                internalRemoveItem(item, removeCount);
                if (tmpAmount == 0) {
                    break;
                }
            }
        } else {
            for (Item* item : itemList) {
                internalRemoveItem(item);
            }
        }

        player->bankBalance -= fee;
    } else {
        uint64_t totalPrice = static_cast<uint64_t>(price) * amount;
        totalPrice += fee;
        if (totalPrice > player->bankBalance) {
            return;
        }

        player->bankBalance -= totalPrice;
    }

    IOMarket::createOffer(player->getGUID(), static_cast<MarketAction_t>(type), it.id, amount, price, anonymous);

    player->sendMarketEnter(player->getLastDepotId());
    const MarketOfferList& buyOffers = IOMarket::getActiveOffers(MARKETACTION_BUY, it.id);
    const MarketOfferList& sellOffers = IOMarket::getActiveOffers(MARKETACTION_SELL, it.id);
    player->sendMarketBrowseItem(it.id, buyOffers, sellOffers);
}

void Game::playerCancelMarketOffer(Player* player, const uint32_t timestamp, const uint16_t counter)
{
    if (!player->isInMarket()) {
        return;
    }

    MarketOfferEx offer = IOMarket::getOfferByCounter(timestamp, counter);
    if (offer.id == 0 || offer.playerId != player->getGUID()) {
        return;
    }

    if (offer.type == MARKETACTION_BUY) {
        player->bankBalance += static_cast<uint64_t>(offer.price) * offer.amount;
        player->sendMarketEnter(player->getLastDepotId());
    } else {
        const ItemType& it = Item::items[offer.itemId];
        if (it.id == 0) {
            return;
        }

        if (it.stackable) {
            uint16_t tmpAmount = offer.amount;
            while (tmpAmount > 0) {
                const int32_t stackCount = std::min<int32_t>(100, tmpAmount);
                Item* item = Item::CreateItem(it.id, stackCount);
                if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }

                tmpAmount -= stackCount;
            }
        } else {
            int32_t subType;
            if (it.charges != 0) {
                subType = it.charges;
            } else {
                subType = -1;
            }

            for (uint16_t i = 0; i < offer.amount; ++i) {
                Item* item = Item::CreateItem(it.id, subType);
                if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }
            }
        }
    }

    IOMarket::moveOfferToHistory(offer.id, OFFERSTATE_CANCELLED);
    offer.amount = 0;
    offer.timestamp += g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);
    player->sendMarketCancelOffer(offer);
    player->sendMarketEnter(player->getLastDepotId());
}

void Game::playerAcceptMarketOffer(Player* player, const uint32_t timestamp, const uint16_t counter, const uint16_t amount)
{
    if (!player->isInMarket()) {
        return;
    }

    MarketOfferEx offer = IOMarket::getOfferByCounter(timestamp, counter);
    if (offer.id == 0) {
        return;
    }

    if (amount > offer.amount) {
        return;
    }

    const ItemType& it = Item::items[offer.itemId];
    if (it.id == 0) {
        return;
    }

    const uint64_t totalPrice = static_cast<uint64_t>(offer.price) * amount;

    if (offer.type == MARKETACTION_BUY) {
        DepotChest* depotChest = player->getDepotChest(player->getLastDepotId(), false);
        if (!depotChest) {
            return;
        }

        const std::vector<Item*> itemList = getMarketItemList(it.wareId, amount, depotChest, player->getInbox());
        if (itemList.empty()) {
            return;
        }

        Player* buyerPlayer = getPlayerByGUID(offer.playerId);
        if (!buyerPlayer) {
            buyerPlayer = new Player(nullptr);
            if (!IOLoginData::loadPlayerById(buyerPlayer, offer.playerId)) {
                delete buyerPlayer;
                return;
            }
        }

        if (it.stackable) {
            uint16_t tmpAmount = amount;
            for (Item* item : itemList) {
                const uint16_t removeCount = std::min<uint16_t>(tmpAmount, item->getItemCount());
                tmpAmount -= removeCount;
                internalRemoveItem(item, removeCount);

                if (tmpAmount == 0) {
                    break;
                }
            }
        } else {
            for (Item* item : itemList) {
                internalRemoveItem(item);
            }
        }

        player->bankBalance += totalPrice;
        if (it.stackable) {
            uint16_t tmpAmount = amount;
            while (tmpAmount > 0) {
                const uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
                Item* item = Item::CreateItem(it.id, stackCount);
                if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }

                tmpAmount -= stackCount;
            }
        } else {
            int32_t subType;
            if (it.charges != 0) {
                subType = it.charges;
            } else {
                subType = -1;
            }

            for (uint16_t i = 0; i < amount; ++i) {
                Item* item = Item::CreateItem(it.id, subType);
                if (internalAddItem(buyerPlayer->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }
            }
        }

        if (buyerPlayer->isOffline()) {
            IOLoginData::savePlayer(buyerPlayer);
            delete buyerPlayer;
        } else {
            buyerPlayer->onReceiveMail();
        }
    } else {
        if (totalPrice > player->bankBalance) {
            return;
        }

        player->bankBalance -= totalPrice;
        if (it.stackable) {
            uint16_t tmpAmount = amount;
            while (tmpAmount > 0) {
                const uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
                Item* item = Item::CreateItem(it.id, stackCount);
                if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }

                tmpAmount -= stackCount;
            }
        } else {
            int32_t subType;
            if (it.charges != 0) {
                subType = it.charges;
            } else {
                subType = -1;
            }

            for (uint16_t i = 0; i < amount; ++i) {
                Item* item = Item::CreateItem(it.id, subType);
                if (internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                    delete item;
                    break;
                }
            }
        }

        Player* sellerPlayer = getPlayerByGUID(offer.playerId);
        if (sellerPlayer) {
            sellerPlayer->bankBalance += totalPrice;
        } else {
            IOLoginData::increaseBankBalance(offer.playerId, totalPrice);
        }

        player->onReceiveMail();
    }

    const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    IOMarket::appendHistory(player->getGUID(), offer.type == MARKETACTION_BUY ? MARKETACTION_SELL : MARKETACTION_BUY, offer.itemId, amount, offer.price, offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTEDEX);

    IOMarket::appendHistory(offer.playerId, offer.type, offer.itemId, amount, offer.price, offer.timestamp + marketOfferDuration, OFFERSTATE_ACCEPTED);

    offer.amount -= amount;
    if (offer.amount == 0) {
        IOMarket::deleteOffer(offer.id);
    } else {
        IOMarket::acceptOffer(offer.id, amount);
    }

    player->sendMarketEnter(player->getLastDepotId());
    offer.timestamp += marketOfferDuration;
    player->sendMarketAcceptOffer(offer);
}

std::vector<Item*> Game::getMarketItemList(const uint16_t wareId, const uint16_t sufficientCount, DepotChest* depotChest, Inbox* inbox)
{
    std::vector<Item*> itemList;
    itemList.reserve(std::max<size_t>(32, depotChest->size() + inbox->size()));

    std::vector<Container*> containers{ depotChest, inbox };
    containers.reserve(32);

    uint16_t count = 0;
    size_t i = 0;
    do {
        const Container* container = containers[i];
        for (Item* item : container->getItemList()) {
            Container* c = item->getContainer();
            if (c && !c->empty()) {
                containers.push_back(c);
                continue;
            }

            const ItemType& itemType = Item::items[item->getID()];
            if (itemType.wareId != wareId) {
                continue;
            }

            if (c && (!itemType.isContainer() || c->capacity() != itemType.maxItems)) {
                continue;
            }

            if (!item->hasMarketAttributes()) {
                continue;
            }

            itemList.push_back(item);

            count += Item::countByType(item, -1);
            if (count >= sufficientCount) {
                return itemList;
            }
        }
    } while (++i < containers.size());

    itemList.clear();
    return itemList;
}
#endif

void Game::playerExtendedOpcode(Player* player, const uint8_t opcode, const std::string& buffer)
{
    for (const CreatureEvent* creatureEvent : player->getCreatureEvents(CREATURE_EVENT_EXTENDED_OPCODE)) {
        creatureEvent->executeExtendedOpcode(player, opcode, buffer);
    }
}

void Game::forceAddCondition(const uint32_t creatureId, Condition* condition)
{
    Creature* creature = getCreatureByID(creatureId);
    if (!creature) {
        delete condition;
        return;
    }

    creature->addCondition(condition, true);
}

void Game::forceRemoveCondition(const uint32_t creatureId, const ConditionType_t type)
{
    Creature* creature = getCreatureByID(creatureId);
    if (!creature) {
        return;
    }

    creature->removeCondition(type, true);
}

void Game::sendOfflineTrainingDialog(Player* player) const
{
    if (!player) {
        return;
    }

    if (!player->hasModalWindowOpen(offlineTrainingWindow.id)) {
        player->sendModalWindow(offlineTrainingWindow);
    }
}

void Game::playerAnswerModalWindow(Player* player, const uint32_t modalWindowId, const uint8_t button, const uint8_t choice) const
{
    if (!player->hasModalWindowOpen(modalWindowId)) {
        return;
    }

    player->onModalWindowHandled(modalWindowId);

    // offline training, hardcoded
    if (modalWindowId == std::numeric_limits<uint32_t>::max()) {
        if (button == offlineTrainingWindow.defaultEnterButton) {
            if (choice == SKILL_SWORD || choice == SKILL_AXE || choice == SKILL_CLUB || choice == SKILL_DISTANCE || choice == SKILL_MAGLEVEL) {
                BedItem* bedItem = player->getBedItem();
                if (bedItem && bedItem->sleep(player)) {
                    player->setOfflineTrainingSkill(choice);
                    return;
                }
            }
        } else {
            player->sendTextMessage(MESSAGE_EVENT_ADVANCE, "Offline training aborted.");
        }

        player->setBedItem(nullptr);
    } else {
        for (const auto creatureEvent : player->getCreatureEvents(CREATURE_EVENT_MODALWINDOW)) {
            creatureEvent->executeModalWindow(player, modalWindowId, button, choice);
        }
    }
}

void Game::updatePlayerEvent(const uint32_t playerId)
{
    Player* player = getPlayerByID(playerId);
    if (!player) {
        return;
    }

    if (player->hasScheduledUpdates(PlayerUpdate_Weight)) {
        player->updateInventoryWeight();
    }
    if (player->hasScheduledUpdates(PlayerUpdate_Light)) {
        player->updateItemsLight();
    }
    if (player->hasScheduledUpdates(PlayerUpdate_Stats)) {
        player->sendStats();
    }
    if (player->hasScheduledUpdates(PlayerUpdate_Skills)) {
        player->sendSkills();
    }
#if GAME_FEATURE_NPC_INTERFACE > 0 || GAME_FEATURE_INVENTORY_LIST > 0
    if (player->hasScheduledUpdates(PlayerUpdate_Inventory | PlayerUpdate_Sale)) {
        std::map<uint32_t, uint32_t> tempInventoryMap;
        player->getAllItemTypeCountAndSubtype(tempInventoryMap);
#if GAME_FEATURE_INVENTORY_LIST > 0
        if (player->hasScheduledUpdates(PlayerUpdate_Inventory)) {
            player->sendItems(tempInventoryMap);
        }
#endif
        if (player->hasScheduledUpdates(PlayerUpdate_Sale)) {
            player->sendSaleItemList(tempInventoryMap);
        }
    }
#endif
    player->resetScheduledUpdates();
}

void Game::checkCreatureDeath(const uint32_t creatureId)
{
    Creature* creature = getCreatureByID(creatureId);
    if (!creature) {
        return;
    }

    if (creature->getHealth() <= 0) {
        creature->onDeath();
    }
}

void Game::addPlayer(Player* player)
{
    const std::string& lowercase_name = asLowerCaseString(player->getName());
    mappedPlayerNames[lowercase_name] = player;
    wildcardTree.insert(lowercase_name);
    players[player->getID()] = player;
}

void Game::removePlayer(const Player* player)
{
    const std::string& lowercase_name = asLowerCaseString(player->getName());
    mappedPlayerNames.erase(lowercase_name);
    wildcardTree.remove(lowercase_name);
    players.erase(player->getID());
}

void Game::addNpc(Npc* npc)
{
    npcs[npc->getID()] = npc;
}

void Game::removeNpc(const Npc* npc)
{
    npcs.erase(npc->getID());
}

void Game::addMonster(Monster* monster)
{
    monsters[monster->getID()] = monster;
}

void Game::removeMonster(const Monster* monster)
{
    monsters.erase(monster->getID());
}

Guild* Game::getGuild(const uint32_t id) const
{
    const auto it = guilds.find(id);
    if (it == guilds.end()) {
        return nullptr;
    }
    return it->second;
}

void Game::addGuild(Guild* guild)
{
    guilds[guild->getId()] = guild;
}

void Game::removeGuild(const uint32_t guildId)
{
    guilds.erase(guildId);
}

#if GAME_FEATURE_BROWSEFIELD > 0
void Game::decreaseBrowseFieldRef(const Position& pos)
{
    Tile* tile = map.getTile(pos.x, pos.y, pos.z);
    if (!tile) {
        return;
    }

    const auto it = browseFields.find(tile);
    if (it != browseFields.end()) {
        it->second->decrementReferenceCounter();
    }
}
#endif

void Game::internalRemoveItems(const std::vector<Item*>& itemList, uint32_t amount, const bool stackable)
{
    if (stackable) {
        for (Item* item : itemList) {
            if (item->getItemCount() > amount) {
                internalRemoveItem(item, amount);
                break;
            }
            amount -= item->getItemCount();
            internalRemoveItem(item);
        }
    } else {
        for (Item* item : itemList) {
            internalRemoveItem(item);
        }
    }
}

BedItem* Game::getBedBySleeper(const uint32_t guid) const
{
    const auto it = bedSleepersMap.find(guid);
    if (it == bedSleepersMap.end()) {
        return nullptr;
    }
    return it->second;
}

void Game::setBedSleeper(BedItem* bed, const uint32_t guid)
{
    bedSleepersMap[guid] = bed;
}

void Game::removeBedSleeper(const uint32_t guid)
{
    const auto it = bedSleepersMap.find(guid);
    if (it != bedSleepersMap.end()) {
        bedSleepersMap.erase(it);
    }
}

Item* Game::getUniqueItem(const uint16_t uniqueId)
{
    const auto it = uniqueItems.find(uniqueId);
    if (it == uniqueItems.end()) {
        return nullptr;
    }
    return it->second;
}

bool Game::addUniqueItem(uint16_t uniqueId, Item* item)
{
    const auto result = uniqueItems.emplace(uniqueId, item);
    if (!result.second) {
        std::cout << "Duplicate unique id: " << uniqueId << std::endl;
    }
    return result.second;
}

void Game::removeUniqueItem(const uint16_t uniqueId)
{
    const auto it = uniqueItems.find(uniqueId);
    if (it != uniqueItems.end()) {
        uniqueItems.erase(it);
    }
}

bool Game::reloadCreatureScripts(const bool fromLua, const bool reload)
{
    std::map<uint32_t, std::vector<std::string>> cacheCreaturesEvents;
#define cacheCreatures(container)																	\
		do {																							\
			for (const auto& it : container) {															\
				CreatureEventList& creatureEvents = it.second->getCreatureEvents();						\
				for (auto creatureEvent : creatureEvents) {												\
					cacheCreaturesEvents[it.second->getID()].emplace_back(creatureEvent->getName());	\
				}																						\
				it.second->resetEventsRegistered();														\
				creatureEvents.clear();																	\
			}																							\
		} while(0)

    cacheCreatures(players);
    cacheCreatures(npcs);
    cacheCreatures(monsters);
#undef cacheCreatures

    bool result = true;
    if (fromLua) {
        if (reload) {
            result = g_creatureEvents->reload();
        }
        g_creatureEvents->clear(true);
        g_scripts->loadScripts("scripts", false, true);
    } else {
        result = g_creatureEvents->reload();
    }

    for (const auto& it : cacheCreaturesEvents) {
        Creature* creature = getCreatureByID(it.first);
        if (creature) {
            for (const std::string& creatureEvent : it.second) {
                creature->registerCreatureEvent(creatureEvent);
            }
        }
    }
    return result;
}

bool Game::reload(const ReloadTypes_t reloadType)
{
    switch (reloadType) {
        case RELOAD_TYPE_ACTIONS: return g_actions->reload();
        case RELOAD_TYPE_CHAT: return g_chat->load();
        case RELOAD_TYPE_CONFIG: return g_config.reload();
        case RELOAD_TYPE_CREATURESCRIPTS: return reloadCreatureScripts();
        case RELOAD_TYPE_EVENTS: return g_events->load();
        case RELOAD_TYPE_GLOBALEVENTS: return g_globalEvents->reload();
        case RELOAD_TYPE_ITEMS: return Item::items.reload();
        case RELOAD_TYPE_MONSTERS: return g_monsters.reload();
        case RELOAD_TYPE_MODULES: return g_modules.load();
        case RELOAD_TYPE_MOUNTS:
#if GAME_FEATURE_MOUNTS > 0
            return mounts.reload();
#else
            return false;
#endif
        case RELOAD_TYPE_MOVEMENTS: return g_moveEvents->reload();
        case RELOAD_TYPE_NPCS: {
            Npcs::reload();
            return true;
        }

        case RELOAD_TYPE_QUESTS: return quests.reload();
        case RELOAD_TYPE_RAIDS: return raids.reload() && raids.startup();

        case RELOAD_TYPE_SPELLS: {
            if (!g_spells->reload()) {
                std::cout << "[Error - Game::reload] Failed to reload spells." << std::endl;
                std::terminate();
            }
            if (!g_monsters.reload()) {
                std::cout << "[Error - Game::reload] Failed to reload monsters." << std::endl;
                std::terminate();
            }
            return true;
        }

        case RELOAD_TYPE_TALKACTIONS: return g_talkActions->reload();

        case RELOAD_TYPE_WEAPONS: {
            const bool results = g_weapons->reload();
            g_weapons->loadDefaults();
            return results;
        }

        case RELOAD_TYPE_SCRIPTS: {
            // commented out stuff is TODO, once we approach further in revscriptsys
            g_actions->clear(true);
            g_moveEvents->clear(true);
            g_talkActions->clear(true);
            g_globalEvents->clear(true);
            g_weapons->clear(true);
            g_weapons->loadDefaults();
            g_spells->clear(true);
            reloadCreatureScripts(true, false); //Keep it as the last because it'll call loadScripts
            /*
            Npcs::reload();
            raids.reload() && raids.startup();
            Item::items.reload();
            quests.reload();
            mounts.reload();
            g_config.reload();
            g_events->load();
            g_chat->load();
            */
            return true;
        }

        default: {
            if (!g_spells->reload()) {
                std::cout << "[Error - Game::reload] Failed to reload spells." << std::endl;
                std::terminate();
            }
            if (!g_monsters.reload()) {
                std::cout << "[Error - Game::reload] Failed to reload monsters." << std::endl;
                std::terminate();
            }

            g_actions->reload();
            g_config.reload();
            g_monsters.reload();
            g_moveEvents->reload();
            Npcs::reload();
            raids.reload() && raids.startup();
            g_talkActions->reload();
            Item::items.reload();
            g_weapons->reload();
            g_weapons->clear(true);
            g_weapons->loadDefaults();
            quests.reload();
#if GAME_FEATURE_MOUNTS > 0
            mounts.reload();
#endif
            g_globalEvents->reload();
            g_events->load();
            g_chat->load();
            g_actions->clear(true);
            g_moveEvents->clear(true);
            g_talkActions->clear(true);
            g_globalEvents->clear(true);
            g_spells->clear(true);
            reloadCreatureScripts(true); //Keep it as the last because it'll call loadScripts
            return true;
        }
    }
}

void Game::sendAttachedEffect(const Creature* creature, uint16_t effectId)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), false, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendAttachedEffect(creature, effectId);
    }
}

void Game::sendDetachEffect(const Creature* creature, uint16_t effectId)
{
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), false, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendDetachEffect(creature, effectId);
    }
}

void Game::updateCreatureShader(const Creature* creature) {
    SpectatorVector spectators;
    map.getSpectators(spectators, creature->getPosition(), false, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendShader(creature, creature->getShader());
    }
}

void Game::refreshItem(const Item* item) {
    if (!item || !item->getParent())
        return;

    const auto parent = item->getParent();

    if (const auto creature = parent->getCreature()) {
        if (const auto player = creature->getPlayer()) {
            int32_t index = creature->getPlayer()->getThingIndex(item);
            if (index > -1)
                player->sendInventoryItem(static_cast<slots_t>(index), item);
        }

        return;
    }

    if (const auto container = parent->getContainer()) {
        int32_t index = container->getThingIndex(item);
        if (index > -1) {
            SpectatorVector spectators;
            g_game.map.getSpectators(spectators, container->getPosition(), false, true, 2, 2, 2, 2);

            //send to client
            for (auto spectator : spectators) {
                spectator->getPlayer()->sendUpdateContainerItem(container, index, item);
            }
        }

        return;
    }

    if (const auto tile = parent->getTile()) {

        SpectatorVector spectators;
        g_game.map.getSpectators(spectators, tile->getPosition(), true, true);

        //send to client
        for (auto spectator : spectators) {
            if (const Player* tmpPlayer = spectator->getPlayer()) {
                tmpPlayer->sendUpdateTileItem(tile, tile->getPosition(), item);
            }
        }
        return;
    }
}
