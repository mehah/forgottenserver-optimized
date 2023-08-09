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

#ifndef FS_PLAYER_H_4083D3D3A05B4EDE891B31BB720CD06F
#define FS_PLAYER_H_4083D3D3A05B4EDE891B31BB720CD06F

#include "creature.h"
#include "container.h"
#include "cylinder.h"
#include "outfit.h"
#include "enums.h"
#include "vocation.h"
#include "protocolgame.h"
#include "ioguild.h"
#include "party.h"
#include "inbox.h"
#include "depotchest.h"
#include "depotlocker.h"
#include "guild.h"
#include "groups.h"
#include "town.h"
#include "mounts.h"

class House;
class NetworkMessage;
class Weapon;
class ProtocolGame;
class Npc;
class Party;
class Bed;
class Guild;

enum skillsid_t
{
    SKILLVALUE_LEVEL = 0,
    SKILLVALUE_TRIES = 1,
    SKILLVALUE_PERCENT = 2,
};

enum fightMode_t : uint8_t
{
    FIGHTMODE_ATTACK = 1,
    FIGHTMODE_BALANCED = 2,
    FIGHTMODE_DEFENSE = 3,
};

enum pvpMode_t : uint8_t
{
    PVP_MODE_DOVE = 0,
    PVP_MODE_WHITE_HAND = 1,
    PVP_MODE_YELLOW_HAND = 2,
    PVP_MODE_RED_FIST = 3,
};

enum tradestate_t : uint8_t
{
    TRADE_NONE,
    TRADE_INITIATED,
    TRADE_ACCEPT,
    TRADE_ACKNOWLEDGE,
    TRADE_TRANSFER,
};

#if GAME_FEATURE_ADDITIONAL_VIPINFO > 0
struct VIPEntry
{
    VIPEntry(const uint32_t guid, std::string name, std::string description, const uint32_t icon, const bool notify) :
        guid(guid), name(std::move(name)), description(std::move(description)), icon(icon), notify(notify) {}

    uint32_t guid;
    std::string name;
    std::string description;
    uint32_t icon;
    bool notify;
};
#else
struct VIPEntry
{
    VIPEntry(uint32_t guid, std::string name) :
        guid(guid), name(std::move(name)) {}

    uint32_t guid;
    std::string name;
};
#endif

struct OpenContainer
{
    Container* container;
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    uint16_t index;
#endif
};

struct OutfitEntry
{
    constexpr OutfitEntry(const uint16_t lookType, const uint8_t addons) : lookType(lookType), addons(addons) {}

    uint16_t lookType;
    uint8_t addons;
};

struct Skill
{
    uint64_t tries = 0;
    uint16_t level = 10;
#if GAME_FEATURE_DOUBLE_PERCENT_SKILLS > 0
    uint16_t percent = 0;
#else
    uint8_t percent = 0;
#endif
};

enum PlayerUpdateFlags : uint32_t
{
    PlayerUpdate_Weight = 1 << 0,
    PlayerUpdate_Light = 1 << 1,
    PlayerUpdate_Stats = 1 << 2,
    PlayerUpdate_Skills = 1 << 3,
    PlayerUpdate_Inventory = 1 << 4,
    PlayerUpdate_Sale = 1 << 5
};

enum PlayerAsyncOngoingTaskFlags : uint64_t
{
    PlayerAsyncTask_Highscore = 1 << 0,
    PlayerAsyncTask_RecentDeaths = 1 << 1,
    PlayerAsyncTask_RecentPvPKills = 1 << 2
};

using MuteCountMap = std::map<uint32_t, uint32_t>;

static constexpr int32_t PLAYER_MAX_SPEED = 1500;
static constexpr int32_t PLAYER_MIN_SPEED = 10;

#if GAME_FEATURE_QUEST_TRACKER > 0
class Mission;
#endif

class Player final : public Creature, public Cylinder
{
public:
    explicit Player(ProtocolGame_ptr p);
    ~Player() override;

    // non-copyable
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player* getPlayer() override {
        return this;
    }
    const Player* getPlayer() const override {
        return this;
    }

    void setID() override {
        if (id == 0) {
            // keep id the same as guid because client save data using this id
            id = 0x10000000 + guid;
            playerAutoID = std::max<uint32_t>(playerAutoID, id);
        }
    }

    static MuteCountMap muteCountMap;

    const std::string& getName() const override {
        return name;
    }
    void setName(std::string name) {
        this->name = std::move(name);
    }
    const std::string& getNameDescription() const override {
        return name;
    }
    std::string getDescription(int32_t lookDistance) const override;

    CreatureType_t getType() const override {
        return CREATURETYPE_PLAYER;
    }

#if GAME_FEATURE_MOUNTS > 0
    uint8_t getCurrentMount() const;
    void setCurrentMount(uint8_t mountId);
    bool isMounted() const {
        return defaultOutfit.lookMount != 0;
    }
    bool toggleMount(bool mount);
    bool tameMount(uint8_t mountId);
    bool untameMount(uint8_t mountId);
    bool hasMount(const Mount* mount) const;
    void dismount();
#endif

    void sendFYIBox(const std::string& message) const
    {
        if (client) {
            client->sendFYIBox(message);
        }
    }

    void setGUID(const uint32_t guid) {
        this->guid = guid;
    }
    uint32_t getGUID() const {
        return guid;
    }
    bool canSeeInvisibility() const override {
        return hasFlag(PlayerFlag_CanSenseInvisibility) || group->access;
    }

    void removeList() override;
    void addList() override;
    void kickPlayer(bool displayEffect);

    static uint64_t getExpForLevel(const uint64_t lv) {
        return (((lv - 6ULL) * lv + 17ULL) * lv - 12ULL) / 6ULL * 100ULL;
    }

    uint16_t getStaminaMinutes() const {
        return staminaMinutes;
    }

    bool addOfflineTrainingTries(skills_t skill, uint64_t tries);

    void addOfflineTrainingTime(const int32_t addTime) {
        offlineTrainingTime = std::min<int32_t>(12 * 3600 * 1000, offlineTrainingTime + addTime);
    }
    void removeOfflineTrainingTime(const int32_t removeTime) {
        offlineTrainingTime = std::max<int32_t>(0, offlineTrainingTime - removeTime);
    }
    int32_t getOfflineTrainingTime() const {
        return offlineTrainingTime;
    }

    int32_t getOfflineTrainingSkill() const {
        return offlineTrainingSkill;
    }
    void setOfflineTrainingSkill(const int32_t skill) {
        offlineTrainingSkill = skill;
    }

    uint64_t getBankBalance() const {
        return bankBalance;
    }
    void setBankBalance(const uint64_t balance) {
        bankBalance = balance;
    }

    Guild* getGuild() const {
        return guild;
    }
    void setGuild(Guild* guild);

    const GuildRank* getGuildRank() const {
        return guildRank;
    }
    void setGuildRank(const GuildRank* newGuildRank) {
        guildRank = newGuildRank;
    }

    bool isGuildMate(const Player* player) const;

    const std::string& getGuildNick() const {
        return guildNick;
    }
    void setGuildNick(std::string nick) {
        guildNick = std::move(nick);
    }

    bool isInWar(const Player* player) const;
    bool isInWarList(uint32_t guildId) const;

    void setLastWalkthroughAttempt(const int64_t walkthroughAttempt) {
        lastWalkthroughAttempt = walkthroughAttempt;
    }
    void setLastWalkthroughPosition(const Position walkthroughPosition) {
        lastWalkthroughPosition = walkthroughPosition;
    }

#if GAME_FEATURE_MARKET > 0
    Inbox* getInbox() const {
        return inbox;
    }
#endif

    uint32_t getClientIcons() const;

    const GuildWarVector& getGuildWarVector() const {
        return guildWarVector;
    }

    Vocation* getVocation() const {
        return vocation;
    }

    OperatingSystem_t getOperatingSystem() const {
        return operatingSystem;
    }
    void setOperatingSystem(const OperatingSystem_t clientos) {
        operatingSystem = clientos;
    }
    OperatingSystem_t getTfcOperatingSystem() const {
        return tfcOperatingSystem;
    }
    void setTfcOperatingSystem(const OperatingSystem_t clientos) {
        tfcOperatingSystem = clientos;
    }

    uint16_t getProtocolVersion() const {
        if (!client) {
            return 0;
        }

        return client->getVersion();
    }

    bool hasSecureMode() const {
        return secureMode;
    }

    void setParty(Party* party) {
        this->party = party;
    }
    Party* getParty() const {
        return party;
    }
    PartyShields_t getPartyShield(const Player* player) const;
    bool isInviting(const Player* player) const;
    bool isPartner(const Player* player) const;
    void sendPlayerPartyIcons(Player* player) const;
    bool addPartyInvitation(Party* party);
    void removePartyInvitation(const Party* party);
    void clearPartyInvitations();

    GuildEmblems_t getGuildEmblem(const Player* player) const;

    uint64_t getSpentMana() const {
        return manaSpent;
    }

    bool hasFlag(const PlayerFlags value) const {
        return (group->flags & value) != 0;
    }

    BedItem* getBedItem() const
    {
        return bedItem;
    }
    void setBedItem(BedItem* b) {
        bedItem = b;
    }

    void addBlessing(const uint8_t blessing) {
        blessings |= blessing;
    }
    void removeBlessing(const uint8_t blessing) {
        blessings &= ~blessing;
    }
    bool hasBlessing(const uint8_t value) const {
        return (blessings & static_cast<uint8_t>(1) << value) != 0;
    }

    bool isOffline() const {
        return getID() == 0;
    }
    void disconnect() const
    {
        if (client) {
            client->disconnect();
        }
    }
    uint32_t getIP() const;

    void addContainer(uint8_t cid, Container* container);
    void closeContainer(uint8_t cid);
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    void setContainerIndex(uint8_t cid, uint16_t index);
#endif

    Container* getContainerByID(uint8_t cid);
    int8_t getContainerID(const Container* container) const;
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    uint16_t getContainerIndex(uint8_t cid) const;
#endif

    bool canOpenCorpse(uint32_t ownerId) const;

    void addStorageValue(uint32_t key, int32_t value, bool isLogin = false);
    bool getStorageValue(uint32_t key, int32_t& value) const;
    void genReservedStorageRange();

#if GAME_FEATURE_QUEST_TRACKER > 0
    size_t getAllowedTrackedQuestCount() const;
    bool hasTrackingQuest(uint16_t missionId) const;
    void resetTrackedQuests(std::vector<uint16_t>& quests);
#endif

    void setGroup(Group* newGroup) {
        group = newGroup;
    }
    Group* getGroup() const {
        return group;
    }

#if GAME_FEATURE_MARKET > 0
    void setInMarket(const bool value) {
        inMarket = value;
    }
    bool isInMarket() const {
        return inMarket;
    }
#endif
#if GAME_FEATURE_STASH > 0
    void setSupplyStashAvailable(bool value) {
        supplyStashAvailable = value;
    }
    bool isSupplyStashAvailable() const {
        return supplyStashAvailable;
    }
    void setMarketAvailable(bool value) {
        marketAvailable = value;
    }
    bool isMarketAvailable() const {
        return marketAvailable;
    }

    size_t getStashItemCount() const { return stashItems.size(); }
    uint32_t getStashItemCount(uint16_t itemId) const;
    bool addStashItem(uint16_t itemId, uint32_t itemCount);
    bool removeStashItem(uint16_t itemId, uint32_t itemCount);
#endif

    void setLastDepotId(const int16_t newId) {
        lastDepotId = newId;
    }
    int16_t getLastDepotId() const {
        return lastDepotId;
    }

    void resetIdleTime() {
        idleTime = 0;
    }

    bool isInGhostMode() const override {
        return ghostMode;
    }
    void switchGhostMode() {
        ghostMode = !ghostMode;
    }

    uint32_t getAccount() const {
        return accountNumber;
    }
    AccountType_t getAccountType() const {
        return accountType;
    }
    uint32_t getLevel() const {
        return level;
    }
    uint8_t getLevelPercent() const {
        return levelPercent;
    }
    uint32_t getMagicLevel() const {
        return std::max<int32_t>(0, magLevel + varStats[STAT_MAGICPOINTS]);
    }
    uint32_t getBaseMagicLevel() const {
        return magLevel;
    }
#if GAME_FEATURE_DOUBLE_PERCENT_SKILLS > 0
    uint16_t getMagicLevelPercent() const {
#else
    uint8_t getMagicLevelPercent() const {
#endif
        return magLevelPercent;
    }
    uint8_t getSoul() const {
        return soul;
    }
    bool isAccessPlayer() const {
        return group->access;
    }
    bool isPremium() const;
    void setPremiumDays(int32_t v);

    uint16_t getHelpers() const;

    bool setVocation(uint16_t vocId, bool internal = false);
    uint16_t getVocationId() const {
        return vocation->getId();
    }

    PlayerSex_t getSex() const {
        return sex;
    }
    void setSex(PlayerSex_t);
    uint64_t getExperience() const {
        return experience;
    }

    time_t getLastLoginSaved() const {
        return lastLoginSaved;
    }

    time_t getLastLogout() const {
        return lastLogout;
    }

    const Position& getLoginPosition() const {
        return loginPosition;
    }
    const Position& getTemplePosition() const {
        return town->getTemplePosition();
    }
    Town* getTown() const {
        return town;
    }
    void setTown(Town * town) {
        this->town = town;
    }

    void clearModalWindows();
    bool hasModalWindowOpen(uint32_t modalWindowId) const;
    void onModalWindowHandled(uint32_t modalWindowId);

    bool isPushable() const override;
    uint32_t isMuted() const;
    void addMessageBuffer();
    void removeMessageBuffer();

    bool removeItemOfType(uint16_t itemId, uint32_t amount, int32_t subType, bool ignoreEquipped = false) const;

    uint32_t getCapacity() const {
        if (hasFlag(PlayerFlag_CannotPickupItem)) {
            return 0;
        }
        if (hasFlag(PlayerFlag_HasInfiniteCapacity)) {
            return std::numeric_limits<uint32_t>::max();
        }
        return capacity;
    }

    uint32_t getFreeCapacity() const {
        if (hasFlag(PlayerFlag_CannotPickupItem)) {
            return 0;
        }
        if (hasFlag(PlayerFlag_HasInfiniteCapacity)) {
            return std::numeric_limits<uint32_t>::max();
        }
        return std::max<int32_t>(0, capacity - inventoryWeight);
    }

    int32_t getMaxHealth() const override {
        return std::max<int32_t>(1, healthMax + varStats[STAT_MAXHITPOINTS]);
    }
    uint32_t getMana() const {
        return mana;
    }
    uint32_t getMaxMana() const {
        return std::max<int32_t>(0, manaMax + varStats[STAT_MAXMANAPOINTS]);
    }

    Item* getInventoryItem(slots_t slot) const;

    bool isItemAbilityEnabled(const slots_t slot) const {
        return inventoryAbilities[slot];
    }
    void setItemAbility(const slots_t slot, const bool enabled) {
        inventoryAbilities[slot] = enabled;
    }

    void setVarSkill(const skills_t skill, const int32_t modifier) {
        varSkills[skill] += modifier;
    }

    void setVarSpecialSkill(const SpecialSkills_t skill, const int32_t modifier) {
        varSpecialSkills[skill] += modifier;
    }

    void setVarStats(stats_t stat, int32_t modifier);
    int32_t getDefaultStats(stats_t stat) const;

    void addConditionSuppressions(uint32_t conditions);
    void removeConditionSuppressions(uint32_t conditions);

    DepotChest* getDepotChest(uint32_t depotId, bool autoCreate);
    DepotLocker* getDepotLocker(uint32_t depotId);
    void onReceiveMail() const;
    bool isNearDepotBox() const;

    bool canSee(const Position & pos) const override;
    bool canSeeCreature(const Creature * creature) const override;

    bool canWalkthrough(const Creature * creature) const;
    bool canWalkthroughEx(const Creature * creature) const;

    RaceType_t getRace() const override {
        return RACE_BLOOD;
    }

    uint64_t getMoney() const;

    //safe-trade functions
    void setTradeState(const tradestate_t state) {
        tradeState = state;
    }
    tradestate_t getTradeState() const {
        return tradeState;
    }
    Item* getTradeItem() const
    {
        return tradeItem;
    }

    //shop functions
    void setShopOwner(Npc * owner, const int32_t onBuy, const int32_t onSell) {
        shopOwner = owner;
        purchaseCallback = onBuy;
        saleCallback = onSell;
    }

    Npc* getShopOwner(int32_t & onBuy, int32_t & onSell) {
        onBuy = purchaseCallback;
        onSell = saleCallback;
        return shopOwner;
    }

    const Npc* getShopOwner(int32_t & onBuy, int32_t & onSell) const {
        onBuy = purchaseCallback;
        onSell = saleCallback;
        return shopOwner;
    }

    //V.I.P. functions
    void notifyStatusChange(const Player * loginPlayer, VipStatus_t status);
    bool removeVIP(uint32_t vipGuid);
    bool addVIP(uint32_t vipGuid, const std::string & vipName, VipStatus_t status);
    bool addVIPInternal(uint32_t vipGuid);
    bool editVIP(uint32_t vipGuid, const std::string & description, uint32_t icon, bool notify);

    //follow functions
    bool setFollowCreature(Creature * creature) override;
    void goToFollowCreature() override;

    //follow events
    void onFollowCreature(const Creature * creature) override;

    //walk events
    void onWalk(Direction & dir) override;
    void onWalkAborted() override;
    void onWalkComplete() override;

    void stopWalk();
    void openShopWindow(Npc * npc, std::vector<ShopInfo>&shop);
    bool closeShopWindow(bool sendCloseShopWindow = true);
    bool updateSaleShopList(const Item * item);
    bool hasShopItemForSale(uint32_t itemId, uint8_t subType) const;

    void setChaseMode(bool mode);
    void setFightMode(const fightMode_t mode) {
        fightMode = mode;
    }
    void setSecureMode(const bool mode) {
        secureMode = mode;
    }

    //combat functions
    bool setAttackedCreature(Creature * creature) override;
    bool isImmune(CombatType_t type) const override;
    bool isImmune(ConditionType_t type) const override;
    bool hasShield() const;
    bool isAttackable() const override;
    static bool lastHitIsPlayer(Creature * lastHitCreature);

    void changeHealth(int32_t healthChange, bool sendHealthChange = true) override;
    void changeMana(int32_t manaChange);
    void changeSoul(int32_t soulChange);

    bool isPzLocked() const {
        return pzLocked;
    }
    BlockType_t blockHit(Creature * attacker, CombatType_t combatType, int32_t & damage,
                                 bool checkDefense = false, bool checkArmor = false, bool field = false) override;
    void doAttacking(uint32_t interval) override;
    bool hasExtraSwing() override {
        return lastAttack > 0 && OTSYS_TIME() - lastAttack >= getAttackSpeed();
    }

    uint16_t getSpecialSkill(const uint8_t skill) const {
        return std::max<int32_t>(0, varSpecialSkills[skill]);
    }
    uint16_t getSkillLevel(const uint8_t skill) const {
        return std::max<int32_t>(0, skills[skill].level + varSkills[skill]);
    }
    uint16_t getBaseSkill(const uint8_t skill) const {
        return skills[skill].level;
    }
#if GAME_FEATURE_DOUBLE_PERCENT_SKILLS > 0
    uint16_t getSkillPercent(uint8_t skill) const {
#else
    uint8_t getSkillPercent(const uint8_t skill) const {
#endif
        return skills[skill].percent;
    }

    bool getAddAttackSkill() const {
        return addAttackSkillPoint;
    }
    BlockType_t getLastAttackBlockType() const {
        return lastAttackBlockType;
    }

    Item* getWeapon(slots_t slot, bool ignoreAmmo) const;
    Item* getWeapon(bool ignoreAmmo = false) const;
    WeaponType_t getWeaponType() const;
    int32_t getWeaponSkill(const Item * item) const;
    void getShieldAndWeapon(const Item * &shield, const Item * &weapon) const;

    void drainHealth(Creature * attacker, int32_t damage) override;
    void drainMana(const Creature * attacker, int32_t manaLoss);
    void addManaSpent(uint64_t amount);
    void addSkillAdvance(skills_t skill, uint64_t count);

    int32_t getArmor() const override;
    int32_t getDefense() const override;
    float getAttackFactor() const override;
    float getDefenseFactor() const override;

    void addInFightTicks(bool pzlock = false);

    uint64_t getGainedExperience(Creature * attacker) const override;

    //combat event functions
    void onAddCondition(ConditionType_t type) override;
    void onAddCombatCondition(ConditionType_t type) override;
    void onEndCondition(ConditionType_t type) override;
    void onCombatRemoveCondition(Condition * condition) override;
    void onAttackedCreature(Creature * target) override;
    void onAttacked() override;
    void onAttackedCreatureDrainHealth(Creature * target, int32_t points) override;
    void onTargetCreatureGainHealth(Creature * target, int32_t points) override;
    bool onKilledCreature(Creature * target, bool lastHit = true) override;
    void onGainExperience(uint64_t gainExp, Creature * target) override;
    void onGainSharedExperience(uint64_t gainExp, Creature * source);
    void onAttackedCreatureBlockHit(BlockType_t blockType) override;
    void onBlockHit() override;
    void onChangeZone(ZoneType_t zone) override;
    void onAttackedCreatureChangeZone(ZoneType_t zone) override;
    void onIdleStatus() override;
    void onPlacedCreature() override;

    LightInfo getCreatureLight() const override;

    Skulls_t getSkull() const override;
    Skulls_t getSkullClient(const Creature * creature) const override;
    int64_t getSkullTicks() const { return skullTicks; }
    void setSkullTicks(const int64_t ticks) { skullTicks = ticks; }

    bool hasAttacked(const Player * attacked) const;
    void addAttacked(const Player * attacked);
    void removeAttacked(const Player * attacked);
    void clearAttacked();
    void addUnjustifiedDead(const Player * attacked);
    void sendCreatureSkull(const Creature * creature) const {
        if (client) {
            client->sendCreatureSkull(creature);
        }
    }
    void checkSkullTicks(int64_t ticks);

    bool canWear(uint32_t lookType, uint8_t addons) const;
    void addOutfit(uint16_t lookType, uint8_t addons);
    bool removeOutfit(uint16_t lookType);
    bool removeOutfitAddon(uint16_t lookType, uint8_t addons);
    bool getOutfitAddons(const Outfit & outfit, uint8_t & addons) const;

    bool canLogout();

    size_t getMaxVIPEntries() const;
    size_t getMaxDepotItems() const;

    //tile
    //send methods
    void sendAddTileItem(const Tile * tile, const Position & pos, const Item * item) const
    {
        if (client) {
            const int32_t stackpos = tile->getStackposOfItem(this, item);
            if (stackpos != -1) {
#if GAME_FEATURE_TILE_ADDTHING_STACKPOS > 0
                client->sendAddTileItem(pos, stackpos, item);
#else
                client->sendAddTileItem(pos, item);
#endif
            }
        }
    }
    void sendUpdateTileItem(const Tile * tile, const Position & pos, const Item * item) const
    {
        if (client) {
            const int32_t stackpos = tile->getStackposOfItem(this, item);
            if (stackpos != -1) {
                client->sendUpdateTileItem(pos, stackpos, item);
            }
        }
    }
    void sendRemoveTileThing(const Position & pos, const int32_t stackpos) const
    {
        if (stackpos != -1 && client) {
            client->sendRemoveTileThing(pos, stackpos);
        }
    }
    void sendUpdateTile(const Tile * tile, const Position & pos) const
    {
        if (client) {
            client->sendUpdateTile(tile, pos);
        }
    }
    void sendMapDescription() const
    {
        if (client) {
            client->sendMapDescription(getPosition());
        }
    }

    void sendChannelMessage(const std::string & author, const std::string & text, const SpeakClasses type, const uint16_t channel) const
    {
        if (client) {
            client->sendChannelMessage(author, text, type, channel);
        }
    }
#if GAME_FEATURE_CHAT_PLAYERLIST > 0
    void sendChannelEvent(const uint16_t channelId, const std::string & playerName, const ChannelEvent_t channelEvent) const
    {
        if (client) {
            client->sendChannelEvent(channelId, playerName, channelEvent);
        }
    }
#endif
    void sendCreatureAppear(const Creature * creature, const Position & pos, const bool isLogin) const
    {
        if (client) {
            client->sendAddCreature(creature, pos, creature->getTile()->getStackposOfCreature(this, creature), isLogin);
        }
    }
    void sendCreatureMove(const Creature * creature, const Position & newPos, const int32_t newStackPos, const Position & oldPos, const int32_t oldStackPos, const bool teleport) const
    {
        if (client) {
            client->sendMoveCreature(creature, newPos, newStackPos, oldPos, oldStackPos, teleport);
        }
    }
	void sendPlayerTyping(const Creature* creature, uint8_t typing)
	{
		if (client) {
			client->sendPlayerTyping(creature, typing);
		}
	}
    void sendCreatureTurn(const Creature * creature) const
    {
        if (client && canSeeCreature(creature)) {
            const int32_t stackpos = creature->getTile()->getStackposOfCreature(this, creature);
            if (stackpos != -1) {
                client->sendCreatureTurn(creature, stackpos);
            }
        }
    }
    void sendCreatureSay(const Creature * creature, const SpeakClasses type, const std::string & text, const Position * pos = nullptr) const
    {
        if (client) {
            client->sendCreatureSay(creature, type, text, pos);
        }
    }
    void sendPrivateMessage(const Player * speaker, const SpeakClasses type, const std::string & text) const
    {
        if (client) {
            client->sendPrivateMessage(speaker, type, text);
        }
    }
    void sendCreatureSquare(const Creature * creature, const SquareColor_t color) const
    {
        if (client) {
            client->sendCreatureSquare(creature, color);
        }
    }
    void sendCreatureChangeOutfit(const Creature * creature, const Outfit_t & outfit) const
    {
        if (client) {
            client->sendCreatureOutfit(creature, outfit);
        }
    }
    void sendCreatureChangeVisible(const Creature * creature, const bool visible) const
    {
        if (!client) {
            return;
        }

        if (creature->getPlayer()) {
            if (visible) {
                client->sendCreatureOutfit(creature, creature->getCurrentOutfit());
            } else {
                static Outfit_t outfit;
                client->sendCreatureOutfit(creature, outfit);
            }
        } else if (canSeeInvisibility()) {
            client->sendCreatureOutfit(creature, creature->getCurrentOutfit());
        } else {
            const int32_t stackpos = creature->getTile()->getStackposOfCreature(this, creature);
            if (stackpos == -1) {
                return;
            }

            if (visible) {
                client->sendAddCreature(creature, creature->getPosition(), stackpos, false);
            } else {
                client->sendRemoveTileThing(creature->getPosition(), stackpos);
            }
        }
    }
    void sendCreatureLight(const Creature * creature) const
    {
        if (client) {
            client->sendCreatureLight(creature);
        }
    }
#if CLIENT_VERSION >= 854
    void sendCreatureWalkthrough(const Creature * creature, const bool walkthrough) const
    {
        if (client) {
            client->sendCreatureWalkthrough(creature, walkthrough);
        }
    }
#endif
    void sendCreatureShield(const Creature * creature) const
    {
        if (client) {
            client->sendCreatureShield(creature);
        }
    }
#if CLIENT_VERSION >= 910
    void sendCreatureType(const Creature * creature, const uint8_t creatureType) const
    {
        if (client) {
            client->sendCreatureType(creature, creatureType);
        }
    }
#endif
#if CLIENT_VERSION >= 1000 && CLIENT_VERSION < 1185
    void sendCreatureHelpers(const uint32_t creatureId, const uint16_t helpers) const
    {
        if (client) {
            client->sendCreatureHelpers(creatureId, helpers);
        }
    }
#endif
#if CLIENT_VERSION >= 870
    void sendSpellCooldown(const uint8_t spellId, const uint32_t time) const
    {
        if (client) {
            client->sendSpellCooldown(spellId, time);
        }
    }
    void sendSpellGroupCooldown(const SpellGroup_t groupId, const uint32_t time) const
    {
        if (client) {
            client->sendSpellGroupCooldown(groupId, time);
        }
    }
#endif
    void sendModalWindow(const ModalWindow & modalWindow);

    //container
    void sendAddContainerItem(const Container * container, const Item * item) const;
#if GAME_FEATURE_CONTAINER_PAGINATION > 0
    void sendUpdateContainerItem(const Container * container, uint16_t slot, const Item * newItem);
    void sendRemoveContainerItem(const Container * container, uint16_t slot);
    void sendContainer(const uint8_t cid, const Container * container, const bool hasParent, const uint16_t firstIndex) const
    {
        if (client) {
            client->sendContainer(cid, container, hasParent, firstIndex);
        }
    }
#else
    void sendUpdateContainerItem(const Container * container, uint8_t slot, const Item * newItem);
    void sendRemoveContainerItem(const Container * container, uint8_t slot);
    void sendContainer(uint8_t cid, const Container * container, bool hasParent) {
        if (client) {
            client->sendContainer(cid, container, hasParent);
        }
    }
#endif

    //inventory
    void sendInventoryItem(const slots_t slot, const Item * item) const
    {
        if (client) {
            client->sendInventoryItem(slot, item);
        }
    }
#if GAME_FEATURE_INVENTORY_LIST > 0
    void sendItems(const std::map<uint32_t, uint32_t>&inventoryMap) const
    {
        if (client) {
            client->sendItems(inventoryMap);
        }
    }
#endif

    void sendAttachedEffect(const Creature * creature, uint16_t effectId) {
        if (client) {
            client->sendAttachedEffect(creature, effectId);
        }
    }

    void sendDetachEffect(const Creature * creature, uint16_t effectId) {
        if (client) {
            client->sendDetachEffect(creature, effectId);
        }
    }

    void sendShader(const Creature * creature, const std::string & shaderName) {
        if (client) {
            client->sendShader(creature, shaderName);
        }
    }

    void sendMapShader(const std::string & shaderName) {
        if (client) {
            client->sendMapShader(shaderName);
        }
    }

    //event methods
    void onUpdateTileItem(const Tile * tile, const Position & pos, const Item * oldItem,
                                  const ItemType & oldType, const Item * newItem, const ItemType & newType) override;
    void onRemoveTileItem(const Tile * tile, const Position & pos, const ItemType & iType,
                                  const Item * item) override;

    void onCreatureAppear(Creature * creature, bool isLogin) override;
    void onRemoveCreature(Creature * creature, bool isLogout) override;
    void onCreatureMove(Creature * creature, const Tile * newTile, const Position & newPos,
                                const Tile * oldTile, const Position & oldPos, bool teleport) override;

    void onAttackedCreatureDisappear(bool isLogout) override;
    void onFollowCreatureDisappear(bool isLogout) override;

    //container
    void onAddContainerItem(const Item * item);
    void onUpdateContainerItem(const Container * container, const Item * oldItem, const Item * newItem);
    void onRemoveContainerItem(const Container * container, const Item * item);

    void onCloseContainer(const Container * container) const;
    void onSendContainer(const Container * container) const;
    void autoCloseContainers(const Container * container);

    //inventory
    void onUpdateInventoryItem(Item * oldItem, const Item * newItem);
    void onRemoveInventoryItem(Item * item);

    void updateCreatureData(const Creature * creature) const {
        if (client) {
            client->updateCreatureData(creature);
        }
    }
    void sendCancelMessage(const std::string & msg) const {
        if (client) {
            client->sendTextMessage(TextMessage(MESSAGE_STATUS_SMALL, msg));
        }
    }
    void sendCancelMessage(ReturnValue message) const;
    void sendCancelTarget() const {
        if (client) {
            client->sendCancelTarget();
        }
    }
    void sendCancelWalk() const {
        if (client) {
            client->sendCancelWalk();
        }
    }
    void sendChangeSpeed(const Creature * creature, const uint32_t newSpeed) const {
        if (client) {
            client->sendChangeSpeed(creature, newSpeed);
        }
    }
    void sendCreatureHealth(const Creature * creature, const uint8_t healthPercent) const {
        if (client) {
            client->sendCreatureHealth(creature, healthPercent);
        }
    }
#if GAME_FEATURE_PARTY_LIST > 0
    void sendPartyCreatureUpdate(const Creature * creature) const {
        if (client) {
            client->sendPartyCreatureUpdate(creature);
        }
    }
    void sendPartyCreatureShield(const Creature * creature) const {
        if (client) {
            client->sendPartyCreatureShield(creature);
        }
    }
    void sendPartyCreatureSkull(const Creature * creature) const {
        if (client) {
            client->sendPartyCreatureSkull(creature);
        }
    }
    void sendPartyCreatureHealth(const Creature * creature, uint8_t healthPercent) const {
        if (client) {
            client->sendPartyCreatureHealth(creature, healthPercent);
        }
    }
    void sendPartyPlayerMana(const Player * player, uint8_t manaPercent) const {
        if (client) {
            client->sendPartyPlayerMana(player, manaPercent);
        }
    }
    void sendPartyCreatureShowStatus(const Creature * creature, bool showStatus) const {
        if (client) {
            client->sendPartyCreatureShowStatus(creature, showStatus);
        }
    }
#endif
#if GAME_FEATURE_PLAYER_VOCATIONS > 0
#if GAME_FEATURE_PARTY_LIST > 0
    void sendPartyPlayerVocation(const Player * player) const {
        if (client) {
            client->sendPartyPlayerVocation(player);
        }
    }
#endif
    void sendPlayerVocation(const Player * player) const {
        if (client) {
            client->sendPlayerVocation(player);
        }
    }
#endif
    void sendDistanceShoot(const Position & from, const Position & to, const unsigned char type) const {
        if (client) {
            client->sendDistanceShoot(from, to, type);
        }
    }
    void sendHouseWindow(const House * house, uint32_t listId) const;
    void sendCreatePrivateChannel(const uint16_t channelId, const std::string & channelName) const
    {
        if (client) {
            client->sendCreatePrivateChannel(channelId, channelName);
        }
    }
    void sendClosePrivate(uint16_t channelId) const;
    void sendIcons() const {
        if (client) {
            client->sendIcons(getClientIcons());
        }
    }
    void sendMagicEffect(const Position & pos, const uint8_t type) const {
        if (client) {
            client->sendMagicEffect(pos, type);
        }
    }
    void sendPing();
    void sendPingBack() const {
        if (client) {
            client->sendPingBack();
        }
    }
    void sendStats();
#if CLIENT_VERSION >= 950
    void sendBasicData() const {
        if (client) {
            client->sendBasicData();
        }
    }
#endif
    void sendSkills() const {
        if (client) {
            client->sendSkills();
        }
    }
    void sendTextMessage(const MessageClasses mclass, const std::string & message) const {
        if (client) {
            client->sendTextMessage(TextMessage(mclass, message));
        }
    }
    void sendTextMessage(const TextMessage & message) const {
        if (client) {
            client->sendTextMessage(message);
        }
    }
    void sendReLoginWindow(const uint8_t unfairFightReduction) const {
        if (client) {
            client->sendReLoginWindow(unfairFightReduction);
        }
    }
    void sendTextWindow(const Item * item, const uint16_t maxlen, const bool canWrite) const {
        if (client) {
            client->sendTextWindow(windowTextId, item, maxlen, canWrite);
        }
    }
    void sendTextWindow(const uint32_t itemId, const std::string & text) const {
        if (client) {
            client->sendTextWindow(windowTextId, itemId, text);
        }
    }
    void sendToChannel(const Creature * creature, const SpeakClasses type, const std::string & text, const uint16_t channelId) const {
        if (client) {
            client->sendToChannel(creature, type, text, channelId);
        }
    }
    void sendShop(Npc * npc) const {
        if (client) {
            client->sendShop(npc, shopItemList);
        }
    }
    void sendSaleItemList(const std::map<uint32_t, uint32_t>&inventoryMap) const {
        if (client) {
            client->sendSaleItemList(shopItemList, inventoryMap);
        }
    }
    void sendCloseShop() const {
        if (client) {
            client->sendCloseShop();
        }
    }
#if GAME_FEATURE_MARKET > 0
    void sendMarketEnter(const uint32_t depotId) const {
        if (client) {
            client->sendMarketEnter(depotId);
        }
    }
    void sendMarketLeave() {
        inMarket = false;
        if (client) {
            client->sendMarketLeave();
        }
    }
    void sendMarketBrowseItem(const uint16_t itemId, const MarketOfferList & buyOffers, const MarketOfferList & sellOffers) const {
        if (client) {
            client->sendMarketBrowseItem(itemId, buyOffers, sellOffers);
        }
    }
    void sendMarketBrowseOwnOffers(const MarketOfferList & buyOffers, const MarketOfferList & sellOffers) const {
        if (client) {
            client->sendMarketBrowseOwnOffers(buyOffers, sellOffers);
        }
    }
    void sendMarketBrowseOwnHistory(const HistoryMarketOfferList & buyOffers, const HistoryMarketOfferList & sellOffers) const {
        if (client) {
            client->sendMarketBrowseOwnHistory(buyOffers, sellOffers);
        }
    }
    void sendMarketDetail(const uint16_t itemId) const {
        if (client) {
            client->sendMarketDetail(itemId);
        }
    }
    void sendMarketAcceptOffer(const MarketOfferEx & offer) const {
        if (client) {
            client->sendMarketAcceptOffer(offer);
        }
    }
    void sendMarketCancelOffer(const MarketOfferEx & offer) const {
        if (client) {
            client->sendMarketCancelOffer(offer);
        }
    }
#endif
    void sendTradeItemRequest(const std::string & traderName, const Item * item, const bool ack) const {
        if (client) {
            client->sendTradeItemRequest(traderName, item, ack);
        }
    }
    void sendTradeClose() const {
        if (client) {
            client->sendCloseTrade();
        }
    }
    void sendWorldLight(const LightInfo lightInfo) const
    {
        if (client) {
            client->sendWorldLight(lightInfo);
        }
    }
#if CLIENT_VERSION >= 1121
    void sendTibiaTime(int32_t time) {
        if (client) {
            client->sendTibiaTime(time);
        }
    }
#endif
#if GAME_FEATURE_STASH > 0
    void sendSupplyStash() {
        if (client) {
            client->sendSupplyStash(stashItems);
        }
    }
    void sendSpecialContainersAvailable(bool supplyStashAvailable, bool marketAvailable) {
        if (client) {
            client->sendSpecialContainersAvailable(supplyStashAvailable, marketAvailable);
        }
    }
#endif
#if GAME_FEATURE_INSPECTION > 0
    void sendItemInspection(uint16_t itemId, uint8_t itemCount, const Item * item, bool cyclopedia) {
        if (client) {
            client->sendItemInspection(itemId, itemCount, item, cyclopedia);
        }
    }
#endif
    void sendChannelsDialog() const
    {
        if (client) {
            client->sendChannelsDialog();
        }
    }
    void sendOpenPrivateChannel(const std::string & receiver) const
    {
        if (client) {
            client->sendOpenPrivateChannel(receiver);
        }
    }
    void sendOutfitWindow() const
    {
        if (client) {
            client->sendOutfitWindow();
        }
    }
#if GAME_FEATURE_PODIUM > 0
    void sendPodiumWindow(const Item * podium, const Position & position, uint16_t spriteId, uint8_t stackpos) {
        if (client) {
            client->sendPodiumWindow(podium, position, spriteId, stackpos);
        }
    }
#endif
    void sendCloseContainer(const uint8_t cid) const
    {
        if (client) {
            client->sendCloseContainer(cid);
        }
    }

    void sendChannel(const uint16_t channelId, const std::string & channelName, const UsersMap * channelUsers, const InvitedMap * invitedUsers) const
    {
        if (client) {
            client->sendChannel(channelId, channelName, channelUsers, invitedUsers);
        }
    }
#if GAME_FEATURE_RULEVIOLATION > 0
    void sendRuleViolationChannel(uint16_t channelId) {
        if (client) {
            client->sendRuleViolationChannel(channelId);
        }
    }
    void sendRuleViolationRemove(const std::string & target) {
        if (client) {
            client->sendRuleViolationRemove(target);
        }
    }
    void sendRuleViolationCancel(const std::string & target) {
        if (client) {
            client->sendRuleViolationCancel(target);
        }
    }
    void sendRuleViolationLock() {
        if (client) {
            client->sendRuleViolationLock();
        }
    }
    void sendChannelMessage(const Player * target, const std::string & text, SpeakClasses type, uint32_t time) {
        if (client) {
            client->sendChannelMessage(target, text, type, time);
        }
    }
#endif
    void sendTutorial(const uint8_t tutorialId) const
    {
        if (client) {
            client->sendTutorial(tutorialId);
        }
    }
    void sendAddMarker(const Position & pos, const uint8_t markType, const std::string & desc) const
    {
        if (client) {
            client->sendAddMarker(pos, markType, desc);
        }
    }
    void sendMonsterCyclopedia() const
    {
        if (client) {
            client->sendMonsterCyclopedia();
        }
    }
    void sendCyclopediaMonsters(const std::string & race) const
    {
        if (client) {
            client->sendCyclopediaMonsters(race);
        }
    }
    void sendCyclopediaRace(const uint16_t monsterId) const
    {
        if (client) {
            client->sendCyclopediaRace(monsterId);
        }
    }
    void sendCyclopediaBonusEffects() const
    {
        if (client) {
            client->sendCyclopediaBonusEffects();
        }
    }
#if GAME_FEATURE_CYCLOPEDIA_CHARACTERINFO > 0
    void sendCyclopediaCharacterNoData(CyclopediaCharacterInfoType_t characterInfoType, uint8_t errorCode) {
        if (client) {
            client->sendCyclopediaCharacterNoData(characterInfoType, errorCode);
        }
    }
    void sendCyclopediaCharacterBaseInformation() {
        if (client) {
            client->sendCyclopediaCharacterBaseInformation();
        }
    }
    void sendCyclopediaCharacterGeneralStats() {
        if (client) {
            client->sendCyclopediaCharacterGeneralStats();
        }
    }
    void sendCyclopediaCharacterCombatStats() {
        if (client) {
            client->sendCyclopediaCharacterCombatStats();
        }
    }
    void sendCyclopediaCharacterRecentDeaths(uint16_t page, uint16_t pages, const std::vector<RecentDeathEntry>&entries) {
        if (client) {
            client->sendCyclopediaCharacterRecentDeaths(page, pages, entries);
        }
    }
    void sendCyclopediaCharacterRecentPvPKills(uint16_t page, uint16_t pages, const std::vector<RecentPvPKillEntry>&entries) {
        if (client) {
            client->sendCyclopediaCharacterRecentPvPKills(page, pages, entries);
        }
    }
    void sendCyclopediaCharacterAchievements() {
        if (client) {
            client->sendCyclopediaCharacterAchievements();
        }
    }
    void sendCyclopediaCharacterItemSummary() {
        if (client) {
            client->sendCyclopediaCharacterItemSummary();
        }
    }
    void sendCyclopediaCharacterOutfitsMounts() {
        if (client) {
            client->sendCyclopediaCharacterOutfitsMounts();
        }
    }
    void sendCyclopediaCharacterStoreSummary() {
        if (client) {
            client->sendCyclopediaCharacterStoreSummary();
        }
    }
    void sendCyclopediaCharacterInspection() {
        if (client) {
            client->sendCyclopediaCharacterInspection();
        }
    }
    void sendCyclopediaCharacterBadges() {
        if (client) {
            client->sendCyclopediaCharacterBadges();
        }
    }
    void sendCyclopediaCharacterTitles() {
        if (client) {
            client->sendCyclopediaCharacterTitles();
        }
    }
#endif
#if GAME_FEATURE_HIGHSCORES > 0
    void sendHighscoresNoData() {
        if (client) {
            client->sendHighscoresNoData();
        }
    }
    void sendHighscores(const std::vector<HighscoreCharacter>&characters, uint8_t categoryId, uint32_t vocationId, uint16_t page, uint16_t pages) {
        if (client) {
            client->sendHighscores(characters, categoryId, vocationId, page, pages);
        }
    }
#endif
    void sendTournamentLeaderboard() const
    {
        if (client) {
            client->sendTournamentLeaderboard();
        }
    }
#if GAME_FEATURE_ANALYTICS > 0
    void sendImpactTracking(bool healing, int32_t impact) {
        if (client) {
            client->sendImpactTracking(healing, impact);
        }
    }
#if GAME_FEATURE_ANALYTICS_IMPACT_TRACKING_EXTENDED > 0
    void sendImpactTracking(CombatType_t combatType, int32_t impact, const std::string & cause) {
        if (client) {
            client->sendImpactTracking(combatType, impact, cause);
        }
    }
#endif
    void sendKillTracking(const std::string & name, const Outfit_t & outfit, const Container * container) {
        if (client) {
            client->sendKillTracking(name, outfit, container);
        }
    }
#endif
    void sendQuestLog() const
    {
        if (client) {
            client->sendQuestLog();
        }
    }
    void sendQuestLine(const Quest * quest) const
    {
        if (client) {
            client->sendQuestLine(quest);
        }
    }
#if GAME_FEATURE_QUEST_TRACKER > 0
    void sendTrackedQuests(uint8_t remainingQuests, std::vector<uint16_t>&quests) {
        if (client) {
            client->sendTrackedQuests(remainingQuests, quests);
        }
    }
    void sendUpdateTrackedQuest(const Mission * mission) {
        if (client) {
            client->sendUpdateTrackedQuest(mission);
        }
    }
#endif
#if CLIENT_VERSION >= 1000
    void sendFightModes() const
    {
        if (client) {
            client->sendFightModes();
        }
    }
#endif
    void sendNetworkMessage(const NetworkMessage & message) const
    {
        if (client) {
            client->writeToOutputBuffer(message);
        }
    }

    void receivePing() {
        lastPong = OTSYS_TIME();
    }

    void onThink(uint32_t interval) override;

    void postAddNotification(Thing * thing, const Cylinder * oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
    void postRemoveNotification(Thing * thing, const Cylinder * newParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;

    void setNextAction(const int64_t time) {
        if (time > nextAction) {
            nextAction = time;
        }
    }
    bool canDoAction() const {
        return nextAction <= OTSYS_TIME();
    }
    uint32_t getNextActionTime() const;

    Item* getWriteItem(uint32_t & windowTextId, uint16_t & maxWriteLen) const;
    void setWriteItem(Item * item, uint16_t maxWriteLen = 0);

    House* getEditHouse(uint32_t & windowTextId, uint32_t & listId) const;
    void setEditHouse(House * house, uint32_t listId = 0);

    void learnInstantSpell(const std::string & spellName);
    void forgetInstantSpell(const std::string & spellName);
    bool hasLearnedInstantSpell(const std::string & spellName) const;

    void addScheduledUpdates(uint32_t flags);
    bool hasScheduledUpdates(const uint32_t flags) const {
        return scheduledUpdates & flags;
    }
    void resetScheduledUpdates() {
        scheduledUpdates = 0;
        scheduledUpdate = false;
    }

    void addAsyncOngoingTask(const uint64_t flags) {
        asyncOngoingTasks |= flags;
    }
    bool hasAsyncOngoingTask(const uint64_t flags) const {
        return asyncOngoingTasks & flags;
    }
    void resetAsyncOngoingTask(const uint64_t flags) {
        asyncOngoingTasks &= ~flags;
    }

    std::string getMapShader() const { return mapShader; }
    void setMapShader(const std::string & shaderName) { mapShader = shaderName; }

private:
    std::vector<Condition*> getMuteConditions() const;

    void checkTradeState(const Item * item);
    bool hasCapacity(const Item * item, uint32_t count) const;

    void gainExperience(uint64_t gainExp, Creature * source);
    void addExperience(Creature * source, uint64_t exp, bool sendText = false);
    void removeExperience(uint64_t exp, bool sendText = false);

    void updateInventoryWeight();

    void stopNextWalkActionTask();
    void stopNextWalkTask();
    void stopNextActionTask();
    void setNextWalkActionTask(uint32_t delay, std::function<void(void)> f);
    void setNextWalkTask(uint32_t delay, std::function<void(void)> f);
    void setNextActionTask(uint32_t delay, std::function<void(void)> f);

    void death(Creature * lastHitCreature) override;
    bool dropCorpse(Creature * lastHitCreature, Creature * mostDamageCreature, bool lastHitUnjustified, bool mostDamageUnjustified) override;
    Item* getCorpse(Creature * lastHitCreature, Creature * mostDamageCreature) override;

    //cylinder implementations
    ReturnValue queryAdd(int32_t index, const Thing & thing, uint32_t count,
            uint32_t flags, Creature * actor = nullptr) const override;
    ReturnValue queryMaxCount(int32_t index, const Thing & thing, uint32_t count, uint32_t & maxQueryCount,
            uint32_t flags) const override;
    ReturnValue queryRemove(const Thing & thing, uint32_t count, uint32_t flags) const override;
    Cylinder* queryDestination(int32_t & index, const Thing & thing, Item * *destItem,
            uint32_t & flags) override;

    void addThing(Thing*) override {}
    void addThing(int32_t index, Thing * thing) override;

    void updateThing(Thing * thing, uint16_t itemId, uint32_t count) override;
    void replaceThing(uint32_t index, Thing * thing) override;

    void removeThing(Thing * thing, uint32_t count) override;

    int32_t getThingIndex(const Thing * thing) const override;
    size_t getFirstIndex() const override;
    size_t getLastIndex() const override;
    uint32_t getItemTypeCount(uint16_t itemId, int32_t subType = -1) const override;
    std::map<uint32_t, uint32_t>& getAllItemTypeCount(std::map<uint32_t, uint32_t>&countMap) const override;
    void getAllItemTypeCountAndSubtype(std::map<uint32_t, uint32_t>&countMap) const;
    Thing* getThing(size_t index) const override;

    void internalAddThing(Thing * thing) override;
    void internalAddThing(uint32_t index, Thing * thing) override;

    std::unordered_set<uint32_t> attackedSet;
    std::unordered_set<uint32_t> VIPList;

#if GAME_FEATURE_STASH > 0
    std::map<uint16_t, uint32_t> stashItems;
#endif
    std::map<uint8_t, OpenContainer> openContainers;
    std::map<uint32_t, DepotLocker*> depotLockerMap;
    std::map<uint32_t, DepotChest*> depotChests;
    std::unordered_map<uint32_t, int32_t> storageMap;

    std::vector<uint32_t> modalWindows;
    std::vector<OutfitEntry> outfits;
    GuildWarVector guildWarVector;

    std::vector<ShopInfo> shopItemList;

    std::unordered_set<std::string> learnedInstantSpellList;
    std::vector<Party*> invitePartyList;
    std::vector<Condition*> storedConditionList;

#if GAME_FEATURE_QUEST_TRACKER > 0
    std::vector<uint16_t> trackedQuests;
#endif

    std::string name;
    std::string guildNick;
    std::string mapShader;

    Skill skills[SKILL_LAST + 1];
    LightInfo itemsLight;
    Position loginPosition;
    Position lastWalkthroughPosition;

    time_t lastLoginSaved = 0;
    time_t lastLogout = 0;

    uint64_t experience = 0;
    uint64_t manaSpent = 0;
    uint64_t lastAttack = 0;
    uint64_t bankBalance = 0;
    uint64_t lastQuestlogUpdate = 0;
    uint64_t actionTaskEvent = 0;
    uint64_t nextStepEvent = 0;
    uint64_t walkTaskEvent = 0;
    uint64_t asyncOngoingTasks = 0;
    int64_t lastFailedFollow = 0;
    int64_t skullTicks = 0;
    int64_t lastWalkthroughAttempt = 0;
#if GAME_FEATURE_MOUNTS > 0
    int64_t lastToggleMount = 0;
#endif
    int64_t lastPing;
    int64_t lastPong;
    int64_t nextAction = 0;

    BedItem* bedItem = nullptr;
    Guild* guild = nullptr;
    const GuildRank* guildRank = nullptr;
    Group* group = nullptr;
#if GAME_FEATURE_MARKET > 0
    Inbox* inbox;
#endif
    Item* tradeItem = nullptr;
    Item* inventory[CONST_SLOT_LAST + 1] = {};
    Item* writeItem = nullptr;
    House* editHouse = nullptr;
    Npc* shopOwner = nullptr;
    Party* party = nullptr;
    Player* tradePartner = nullptr;
    ProtocolGame_ptr client;
    std::pair<uint32_t, std::function<void(void)>>* walkTask = nullptr;
    Town* town = nullptr;
    Vocation* vocation = nullptr;

    uint32_t scheduledUpdates = 0;
    uint32_t inventoryWeight = 0;
    uint32_t capacity = 40000;
    uint32_t damageImmunities = 0;
    uint32_t conditionImmunities = 0;
    uint32_t conditionSuppressions = 0;
    uint32_t level = 1;
    uint32_t magLevel = 0;
    uint32_t MessageBufferTicks = 0;
    uint32_t lastIP = 0;
    uint32_t accountNumber = 0;
    uint32_t guid = 0;
    uint32_t windowTextId = 0;
    uint32_t editListId = 0;
    uint32_t mana = 0;
    uint32_t manaMax = 0;
    int32_t varSkills[SKILL_LAST + 1] = {};
    int32_t varSpecialSkills[SPECIALSKILL_LAST + 1] = {};
    int32_t varStats[STAT_LAST + 1] = {};
    int32_t purchaseCallback = -1;
    int32_t saleCallback = -1;
    int32_t MessageBufferCount = 0;
    int32_t premiumDays = 0;
    int32_t bloodHitCount = 0;
    int32_t shieldBlockCount = 0;
    int32_t offlineTrainingSkill = -1;
    int32_t offlineTrainingTime = 0;
    int32_t idleTime = 0;

    uint16_t lastStatsTrainingTime = 0;
    uint16_t staminaMinutes = 2520;
    uint16_t maxWriteLen = 0;
    int16_t lastDepotId = -1;

    uint8_t soul = 0;
    uint8_t blessings = 0;
    uint8_t levelPercent = 0;
#if GAME_FEATURE_DOUBLE_PERCENT_SKILLS > 0
    uint16_t magLevelPercent = 0;
#else
    uint8_t magLevelPercent = 0;
#endif

    PlayerSex_t sex = PLAYERSEX_FEMALE;
    OperatingSystem_t operatingSystem = CLIENTOS_NONE;
    OperatingSystem_t tfcOperatingSystem = CLIENTOS_NONE;
    BlockType_t lastAttackBlockType = BLOCK_NONE;
    tradestate_t tradeState = TRADE_NONE;
    fightMode_t fightMode = FIGHTMODE_ATTACK;
    AccountType_t accountType = ACCOUNT_TYPE_NORMAL;

    bool chaseMode = false;
    bool secureMode = false;
#if GAME_FEATURE_MARKET > 0
    bool inMarket = false;
#endif
#if GAME_FEATURE_MOUNTS > 0
    bool wasMounted = false;
#endif
    bool ghostMode = false;
    bool pzLocked = false;
    bool isConnecting = false;
    bool addAttackSkillPoint = false;
    bool inventoryAbilities[CONST_SLOT_LAST + 1] = {};
    bool scheduledUpdate = false;
#if GAME_FEATURE_STASH > 0
    bool supplyStashAvailable = false;
    bool marketAvailable = false;
#endif

    static uint32_t playerAutoID;

    void updateItemsLight(bool internal = false);
    int32_t getStepSpeed() const override {
        return std::max<int32_t>(PLAYER_MIN_SPEED, std::min<int32_t>(PLAYER_MAX_SPEED, getSpeed()));
    }
    void updateBaseSpeed() {
        if (!hasFlag(PlayerFlag_SetMaxSpeed)) {
            baseSpeed = vocation->getBaseSpeed() + 2 * (level - 1);
        } else {
            baseSpeed = PLAYER_MAX_SPEED;
        }
    }

    bool isPromoted() const;

    uint32_t getAttackSpeed() const {
        return vocation->getAttackSpeed();
    }

#if GAME_FEATURE_DOUBLE_PERCENT_SKILLS > 0
    static uint16_t getPercentSkillLevel(uint64_t count, uint64_t nextLevelCount);
#else
    static uint8_t getPercentSkillLevel(uint64_t count, uint64_t nextLevelCount);
#endif
    static uint8_t getPercentLevel(uint64_t count, uint64_t nextLevelCount);
    double getLostPercent() const;
    uint64_t getLostExperience() const override {
        return skillLoss ? static_cast<uint64_t>(experience * getLostPercent()) : 0;
    }
    uint32_t getDamageImmunities() const override {
        return damageImmunities;
    }
    uint32_t getConditionImmunities() const override {
        return conditionImmunities;
    }
    uint32_t getConditionSuppressions() const override {
        return conditionSuppressions;
    }
    uint16_t getLookCorpse() const override;
    void getPathSearchParams(const Creature * creature, FindPathParams & fpp) const override;

    friend class Game;
    friend class Npc;
    friend class LuaScriptInterface;
    friend class Map;
    friend class Actions;
    friend class IOLoginData;
    friend class ProtocolGame;
};

#endif
