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

#ifndef FS_MOVEMENT_H_5E0D2626D4634ACA83AC6509518E5F49
#define FS_MOVEMENT_H_5E0D2626D4634ACA83AC6509518E5F49

#include "baseevents.h"
#include "item.h"
#include "luascript.h"
#include "vocation.h"

extern Vocations g_vocations;

enum MoveEvent_t
{
    MOVE_EVENT_STEP_IN,
    MOVE_EVENT_STEP_OUT,
    MOVE_EVENT_EQUIP,
    MOVE_EVENT_DEEQUIP,
    MOVE_EVENT_ADD_ITEM,
    MOVE_EVENT_REMOVE_ITEM,
    MOVE_EVENT_ADD_ITEM_ITEMTILE,
    MOVE_EVENT_REMOVE_ITEM_ITEMTILE,

    MOVE_EVENT_LAST,
    MOVE_EVENT_NONE
};

class MoveEvent;
using MoveEvent_ptr = std::shared_ptr<MoveEvent>;

struct MoveEventList
{
    MoveEvent_ptr moveEvent[MOVE_EVENT_LAST];
};

using VocEquipMap = std::map<uint16_t, bool>;

class MoveEvents final : public BaseEvents
{
public:
    MoveEvents();
    ~MoveEvents() override;

    // non-copyable
    MoveEvents(const MoveEvents&) = delete;
    MoveEvents& operator=(const MoveEvents&) = delete;

    uint32_t onCreatureMove(Creature* creature, const Tile* tile, MoveEvent_t eventType);
    uint32_t onPlayerEquip(Player* player, Item* item, slots_t slot, bool isCheck);
    uint32_t onPlayerDeEquip(Player* player, Item* item, slots_t slot);
    uint32_t onItemMove(Item* item, const Tile* tile, bool isAdd);

    MoveEvent* getEvent(const Item* item, MoveEvent_t eventType);

    bool registerLuaEvent(const MoveEvent_ptr& event);
    bool registerLuaFunction(const MoveEvent_ptr& event);
    void clear(bool fromLua) override;

private:
    using MoveListMap = std::map<uint16_t, MoveEventList>;
    using MovePosListMap = std::map<Position, MoveEventList>;
    static void clearMap(MoveListMap& map, bool fromLua);
    static void clearPosMap(MovePosListMap& map, bool fromLua);

    LuaScriptInterface& getScriptInterface() override;
    std::string getScriptBaseName() const override;
    Event_ptr getEvent(const std::string& nodeName) override;
    bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

    static void addEvent(MoveEvent_ptr moveEvent, uint16_t id, MoveListMap& map);
    static void addEvent(MoveEvent_ptr moveEvent, const Position& pos, MovePosListMap& map);

    MoveEvent* getEvent(const Tile* tile, MoveEvent_t eventType);

    MoveEvent* getEvent(const Item* item, MoveEvent_t eventType, slots_t slot);

    MoveListMap uniqueIdMap;
    MoveListMap actionIdMap;
    MoveListMap itemIdMap;
    MovePosListMap positionMap;

    LuaScriptInterface scriptInterface;
};

using StepFunction = std::function<uint32_t(Creature* creature, Item* item, const Position& pos)>;
using MoveFunction = std::function<uint32_t(Item* item, Item* tileItem, const Position& pos)>;
using EquipFunction = std::function<uint32_t(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean)>;

class MoveEvent final : public Event
{
public:
    explicit MoveEvent(LuaScriptInterface* interface);

    MoveEvent_t getEventType() const;
    void setEventType(MoveEvent_t type);

    bool configureEvent(const pugi::xml_node& node) override;
    bool loadFunction(const pugi::xml_attribute& attr, bool isScripted) override;

    uint32_t fireStepEvent(Creature* creature, Item* item, const Position& pos) const;
    uint32_t fireAddRemItem(Item* item, Item* tileItem, const Position& pos);
    uint32_t fireEquip(Player* player, Item* item, slots_t slot, bool isCheck);

    uint32_t getSlot() const {
        return slot;
    }

    //scripting
    bool executeStep(Creature* creature, Item* item, const Position& pos) const;
    bool executeEquip(Player* player, Item* item, slots_t slot, bool isCheck) const;
    bool executeAddRemItem(Item* item, Item* tileItem, const Position& pos) const;
    //

    //onEquip information
    uint32_t getReqLevel() const {
        return reqLevel;
    }
    uint32_t getReqMagLv() const {
        return reqMagLevel;
    }
    bool isPremium() const {
        return premium;
    }
    const std::string& getVocationString() const {
        return vocationString;
    }
    void setVocationString(std::string str) {
        vocationString = std::move(str);
    }
    uint32_t getWieldInfo() const {
        return wieldInfo;
    }
    const VocEquipMap& getVocEquipMap() const {
        return vocEquipMap;
    }
    void addVocEquipMap(const std::string& vocName) {
        const int32_t vocationId = g_vocations.getVocationId(vocName);
        if (vocationId != -1) {
            vocEquipMap[vocationId] = true;
        }
    }
    bool getTileItem() const {
        return tileItem;
    }
    void setTileItem(const bool b) {
        tileItem = b;
    }
    std::vector<uint16_t>& getItemIdRange() {
        return itemIdRange;
    }
    void addItemId(uint16_t id) {
        itemIdRange.emplace_back(id);
    }
    std::vector<uint16_t>& getActionIdRange() {
        return actionIdRange;
    }
    void addActionId(uint16_t id) {
        actionIdRange.emplace_back(id);
    }
    std::vector<uint16_t>& getUniqueIdRange() {
        return uniqueIdRange;
    }
    void addUniqueId(uint16_t id) {
        uniqueIdRange.emplace_back(id);
    }
    std::vector<Position>& getPosList() {
        return posList;
    }
    void addPosList(Position pos) {
        posList.emplace_back(pos);
    }
    void setSlot(const uint32_t s) {
        slot = s;
    }
    uint32_t getRequiredLevel() const
    {
        return reqLevel;
    }
    void setRequiredLevel(const uint32_t level) {
        reqLevel = level;
    }
    uint32_t getRequiredMagLevel() const
    {
        return reqMagLevel;
    }
    void setRequiredMagLevel(const uint32_t level) {
        reqMagLevel = level;
    }
    bool needPremium() const
    {
        return premium;
    }
    void setNeedPremium(const bool b) {
        premium = b;
    }

    void setWieldInfo(const WieldInfo_t info) {
        wieldInfo |= info;
    }

    static uint32_t StepInField(Creature* creature, Item* item, const Position& pos);
    static uint32_t StepOutField(Creature* creature, Item* item, const Position& pos);

    static uint32_t AddItemField(Item* item, Item* tileItem, const Position& pos);
    static uint32_t RemoveItemField(Item* item, Item* tileItem, const Position& pos);

    static uint32_t EquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean);
    static uint32_t DeEquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean);

    MoveEvent_t eventType = MOVE_EVENT_NONE;
    StepFunction stepFunction;
    MoveFunction moveFunction;
    EquipFunction equipFunction;

private:
    std::string getScriptEventName() const override;

    uint32_t slot = SLOTP_WHEREEVER;

    //onEquip information
    uint32_t reqLevel = 0;
    uint32_t reqMagLevel = 0;
    bool premium = false;
    std::string vocationString;
    uint32_t wieldInfo = 0;
    VocEquipMap vocEquipMap;
    bool tileItem = false;

    std::vector<uint16_t> itemIdRange;
    std::vector<uint16_t> actionIdRange;
    std::vector<uint16_t> uniqueIdRange;
    std::vector<Position> posList;
};

#endif
