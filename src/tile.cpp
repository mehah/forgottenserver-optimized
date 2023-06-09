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

#include "tile.h"

#include "creature.h"
#include "combat.h"
#include "game.h"
#include "mailbox.h"
#include "monster.h"
#include "movement.h"
#include "teleport.h"
#include "trashholder.h"

extern MoveEvents* g_moveEvents;

StaticTile real_nullptr_tile(0xFFFF, 0xFFFF, 0xFF);
Tile& Tile::nullptr_tile = real_nullptr_tile;

bool Tile::hasProperty(const ITEMPROPERTY prop) const
{
    if (ground && ground->hasProperty(prop)) {
        return true;
    }

    if (const TileItemVector* items = getItemList()) {
        for (const Item* item : *items) {
            if (item->hasProperty(prop)) {
                return true;
            }
        }
    }
    return false;
}

bool Tile::hasProperty(const Item* exclude, const ITEMPROPERTY prop) const
{
    assert(exclude);

    if (ground && exclude != ground && ground->hasProperty(prop)) {
        return true;
    }

    if (const TileItemVector* items = getItemList()) {
        for (const Item* item : *items) {
            if (item != exclude && item->hasProperty(prop)) {
                return true;
            }
        }
    }

    return false;
}

bool Tile::hasHeight(const uint32_t n) const
{
    uint32_t height = 0;

    if (ground) {
        if (ground->hasProperty(CONST_PROP_HASHEIGHT)) {
            ++height;
        }

        if (n == height) {
            return true;
        }
    }

    if (const TileItemVector* items = getItemList()) {
        for (const Item* item : *items) {
            if (item->hasProperty(CONST_PROP_HASHEIGHT)) {
                ++height;
            }

            if (n == height) {
                return true;
            }
        }
    }
    return false;
}

size_t Tile::getCreatureCount() const
{
    if (const CreatureVector* creatures = getCreatures()) {
        return creatures->size();
    }
    return 0;
}

size_t Tile::getItemCount() const
{
    if (const TileItemVector* items = getItemList()) {
        return items->size();
    }
    return 0;
}

uint32_t Tile::getTopItemCount() const
{
    if (const TileItemVector* items = getItemList()) {
        return items->getTopItemCount();
    }
    return 0;
}

uint32_t Tile::getDownItemCount() const
{
    if (const TileItemVector* items = getItemList()) {
        return items->getDownItemCount();
    }
    return 0;
}

std::string Tile::getDescription(int32_t) const
{
    return "You dont know why, but you cant see anything!";
}

Teleport* Tile::getTeleportItem() const
{
    if (!hasFlag(TILESTATE_TELEPORT)) {
        return nullptr;
    }

    if (const TileItemVector* items = getItemList()) {
        for (Item* item : *items) {
            if (item->getTeleport()) {
                return item->getTeleport();
            }
        }
    }
    return nullptr;
}

MagicField* Tile::getFieldItem() const
{
    if (!hasFlag(TILESTATE_MAGICFIELD)) {
        return nullptr;
    }

    if (ground && ground->getMagicField()) {
        return ground->getMagicField();
    }

    if (const TileItemVector* items = getItemList()) {
        for (Item* item : *items) {
            if (item->getMagicField()) {
                return item->getMagicField();
            }
        }
    }
    return nullptr;
}

TrashHolder* Tile::getTrashHolder() const
{
    if (!hasFlag(TILESTATE_TRASHHOLDER)) {
        return nullptr;
    }

    if (ground && ground->getTrashHolder()) {
        return ground->getTrashHolder();
    }

    if (const TileItemVector* items = getItemList()) {
        for (Item* item : *items) {
            if (item->getTrashHolder()) {
                return item->getTrashHolder();
            }
        }
    }
    return nullptr;
}

Mailbox* Tile::getMailbox() const
{
    if (!hasFlag(TILESTATE_MAILBOX)) {
        return nullptr;
    }

    if (ground && ground->getMailbox()) {
        return ground->getMailbox();
    }

    if (const TileItemVector* items = getItemList()) {
        for (Item* item : *items) {
            if (item->getMailbox()) {
                return item->getMailbox();
            }
        }
    }
    return nullptr;
}

BedItem* Tile::getBedItem() const
{
    if (!hasFlag(TILESTATE_BED)) {
        return nullptr;
    }

    if (ground && ground->getBed()) {
        return ground->getBed();
    }

    if (const TileItemVector* items = getItemList()) {
        for (Item* item : *items) {
            if (item->getBed()) {
                return item->getBed();
            }
        }
    }
    return nullptr;
}

Creature* Tile::getTopCreature() const
{
    if (const CreatureVector* creatures = getCreatures()) {
        if (!creatures->empty()) {
            return *creatures->begin();
        }
    }
    return nullptr;
}

const Creature* Tile::getBottomCreature() const
{
    if (const CreatureVector* creatures = getCreatures()) {
        if (!creatures->empty()) {
            return *creatures->rbegin();
        }
    }
    return nullptr;
}

Creature* Tile::getTopVisibleCreature(const Creature* creature) const
{
    if (const CreatureVector* creatures = getCreatures()) {
        if (creature) {
            const Player* player = creature->getPlayer();
            if (player && player->isAccessPlayer()) {
                return getTopCreature();
            }

            for (Creature* tileCreature : *creatures) {
                if (creature->canSeeCreature(tileCreature)) {
                    return tileCreature;
                }
            }
        } else {
            for (Creature* tileCreature : *creatures) {
                if (!tileCreature->isInvisible()) {
                    const Player* player = tileCreature->getPlayer();
                    if (!player || !player->isInGhostMode()) {
                        return tileCreature;
                    }
                }
            }
        }
    }
    return nullptr;
}

const Creature* Tile::getBottomVisibleCreature(const Creature* creature) const
{
    if (const CreatureVector* creatures = getCreatures()) {
        if (creature) {
            const Player* player = creature->getPlayer();
            if (player && player->isAccessPlayer()) {
                return getBottomCreature();
            }

            for (auto it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
                if (creature->canSeeCreature(*it)) {
                    return *it;
                }
            }
        } else {
            for (auto it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
                if (!(*it)->isInvisible()) {
                    const Player* player = (*it)->getPlayer();
                    if (!player || !player->isInGhostMode()) {
                        return *it;
                    }
                }
            }
        }
    }
    return nullptr;
}

Item* Tile::getTopDownItem() const
{
    if (const TileItemVector* items = getItemList()) {
        return items->getTopDownItem();
    }
    return nullptr;
}

Item* Tile::getTopTopItem() const
{
    if (const TileItemVector* items = getItemList()) {
        return items->getTopTopItem();
    }
    return nullptr;
}

Item* Tile::getItemByTopOrder(const int32_t topOrder)
{
    //topOrder:
    //1: borders
    //2: ladders, signs, splashes
    //3: doors etc
    //4: creatures
    if (TileItemVector* items = getItemList()) {
        for (auto it = ItemVector::const_reverse_iterator(items->getEndTopItem()), end = ItemVector::const_reverse_iterator(items->getBeginTopItem()); it != end; ++it) {
            if (Item::items[(*it)->getID()].alwaysOnTopOrder == topOrder) {
                return *it;
            }
        }
    }
    return nullptr;
}

Thing* Tile::getTopVisibleThing(const Creature* creature)
{
    Thing* thing = getTopVisibleCreature(creature);
    if (thing) {
        return thing;
    }

    TileItemVector* items = getItemList();
    if (items) {
        for (auto it = ItemVector::const_reverse_iterator(items->getEndDownItem()), end = ItemVector::const_reverse_iterator(items->getBeginDownItem()); it != end; ++it) {
            const ItemType& iit = Item::items[(*it)->getID()];
            if (!iit.lookThrough) {
                return *it;
            }
        }

        for (auto it = ItemVector::const_reverse_iterator(items->getEndTopItem()), end = ItemVector::const_reverse_iterator(items->getBeginTopItem()); it != end; ++it) {
            const ItemType& iit = Item::items[(*it)->getID()];
            if (!iit.lookThrough) {
                return *it;
            }
        }
    }

    return ground;
}

void Tile::onAddTileItem(Item* item)
{
#if GAME_FEATURE_BROWSEFIELD > 0
    if (item->hasProperty(CONST_PROP_MOVEABLE) || item->getContainer()) {
        const auto it = g_game.browseFields.find(this);
        if (it != g_game.browseFields.end()) {
            it->second->addItemBack(item);
            item->setParent(this);
        }
    }
#endif

    setTileFlags(item);

    const Position& cylinderMapPos = getPosition();

    SpectatorVector spectators;
    g_game.map.getSpectators(spectators, cylinderMapPos, true);

    //send to client
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendAddTileItem(this, cylinderMapPos, item);
        }
    }

    //event methods
    for (Creature* spectator : spectators) {
        spectator->onAddTileItem(this, cylinderMapPos);
    }
}

void Tile::onUpdateTileItem(Item* oldItem, const ItemType& oldType, Item* newItem, const ItemType& newType)
{
#if GAME_FEATURE_BROWSEFIELD > 0
    if (newItem->hasProperty(CONST_PROP_MOVEABLE) || newItem->getContainer()) {
        const auto it = g_game.browseFields.find(this);
        if (it != g_game.browseFields.end()) {
            const int32_t index = it->second->getThingIndex(oldItem);
            if (index != -1) {
                it->second->replaceThing(index, newItem);
                newItem->setParent(this);
            }
        }
    } else if (oldItem->hasProperty(CONST_PROP_MOVEABLE) || oldItem->getContainer()) {
        const auto it = g_game.browseFields.find(this);
        if (it != g_game.browseFields.end()) {
            Cylinder* oldParent = oldItem->getParent();
            it->second->removeThing(oldItem, oldItem->getItemCount());
            oldItem->setParent(oldParent);
        }
    }
#endif

    const Position& cylinderMapPos = getPosition();

    SpectatorVector spectators;
    g_game.map.getSpectators(spectators, cylinderMapPos, true);

    //send to client
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendUpdateTileItem(this, cylinderMapPos, newItem);
        }
    }

    //event methods
    for (Creature* spectator : spectators) {
        spectator->onUpdateTileItem(this, cylinderMapPos, oldItem, oldType, newItem, newType);
    }
}

void Tile::onRemoveTileItem(const SpectatorVector& spectators, const std::vector<int32_t>& oldStackPosVector, Item* item)
{
#if GAME_FEATURE_BROWSEFIELD > 0
    if (item->hasProperty(CONST_PROP_MOVEABLE) || item->getContainer()) {
        const auto it = g_game.browseFields.find(this);
        if (it != g_game.browseFields.end()) {
            it->second->removeThing(item, item->getItemCount());
        }
    }
#endif

    resetTileFlags(item);

    const Position& cylinderMapPos = getPosition();
    const ItemType& iType = Item::items[item->getID()];

    //send to client + event method
    auto i = static_cast<size_t>(-1); //Start index at -1 to avoid copying it
    for (Creature* spectator : spectators) {
        if (const Player* tmpPlayer = spectator->getPlayer()) {
            tmpPlayer->sendRemoveTileThing(cylinderMapPos, oldStackPosVector[++i]);
        }

        spectator->onRemoveTileItem(this, cylinderMapPos, iType, item);
    }
}

void Tile::onUpdateTile(const SpectatorVector& spectators) const
{
    const Position& cylinderMapPos = getPosition();

    //send to clients
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->sendUpdateTile(this, cylinderMapPos);
    }
}

ReturnValue Tile::queryAdd(int32_t, const Thing& thing, uint32_t, const uint32_t flags, Creature*) const
{
    if (const Creature* creature = thing.getCreature()) {
        if (hasBitSet(FLAG_NOLIMIT, flags)) {
            return RETURNVALUE_NOERROR;
        }

        if (hasBitSet(FLAG_PATHFINDING, flags) && hasFlag(TILESTATE_FLOORCHANGE | TILESTATE_TELEPORT)) {
            return RETURNVALUE_NOTPOSSIBLE;
        }

        if (ground == nullptr) {
            return RETURNVALUE_NOTPOSSIBLE;
        }

        if (const Monster* monster = creature->getMonster()) {
            if (hasFlag(TILESTATE_PROTECTIONZONE | TILESTATE_FLOORCHANGE | TILESTATE_TELEPORT)) {
                return RETURNVALUE_NOTPOSSIBLE;
            }

            const CreatureVector* creatures = getCreatures();
            if (monster->canPushCreatures() && !monster->isSummon()) {
                if (creatures) {
                    for (Creature* tileCreature : *creatures) {
                        if (tileCreature->getPlayer() && tileCreature->getPlayer()->isInGhostMode()) {
                            continue;
                        }

                        const Monster* creatureMonster = tileCreature->getMonster();
                        if (!creatureMonster || !tileCreature->isPushable() ||
                                (creatureMonster->isSummon() && creatureMonster->getMaster()->getPlayer())) {
                            return RETURNVALUE_NOTPOSSIBLE;
                        }
                    }
                }
            } else if (creatures && !creatures->empty()) {
                for (const Creature* tileCreature : *creatures) {
                    if (!tileCreature->isInGhostMode()) {
                        return RETURNVALUE_NOTENOUGHROOM;
                    }
                }
            }

            if (hasFlag(TILESTATE_IMMOVABLEBLOCKSOLID)) {
                return RETURNVALUE_NOTPOSSIBLE;
            }

            if (hasBitSet(FLAG_PATHFINDING, flags) && hasFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH)) {
                return RETURNVALUE_NOTPOSSIBLE;
            }

            if (hasFlag(TILESTATE_BLOCKSOLID) || (hasBitSet(FLAG_PATHFINDING, flags) && hasFlag(TILESTATE_NOFIELDBLOCKPATH))) {
                if (!(monster->canPushItems() || hasBitSet(FLAG_IGNOREBLOCKITEM, flags))) {
                    return RETURNVALUE_NOTPOSSIBLE;
                }
            }

            const MagicField* field = getFieldItem();
            if (!field || field->isBlocking() || field->getDamage() == 0) {
                return RETURNVALUE_NOERROR;
            }

            const CombatType_t combatType = field->getCombatType();

            //There is 3 options for a monster to enter a magic field
            //1) Monster is immune
            if (!monster->isImmune(combatType)) {
                //1) Monster is able to walk over field type
                //2) Being attacked while random stepping will make it ignore field damages
                if (hasBitSet(FLAG_IGNOREFIELDDAMAGE, flags)) {
                    if (!(monster->canWalkOnFieldType(combatType) || monster->isIgnoringFieldDamage())) {
                        return RETURNVALUE_NOTPOSSIBLE;
                    }
                } else {
                    return RETURNVALUE_NOTPOSSIBLE;
                }
            }

            return RETURNVALUE_NOERROR;
        }

        const CreatureVector* creatures = getCreatures();
        if (const Player* player = creature->getPlayer()) {
            if (creatures && !creatures->empty() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags) && !player->isAccessPlayer()) {
                for (const Creature* tileCreature : *creatures) {
                    if (!player->canWalkthrough(tileCreature)) {
                        return RETURNVALUE_NOTPOSSIBLE;
                    }
                }
            }

            if (player->getParent() == nullptr && hasFlag(TILESTATE_NOLOGOUT)) {
                //player is trying to login to a "no logout" tile
                return RETURNVALUE_NOTPOSSIBLE;
            }

            const Tile* playerTile = player->getTile();
            if (playerTile && player->isPzLocked()) {
                if (!playerTile->hasFlag(TILESTATE_PVPZONE)) {
                    //player is trying to enter a pvp zone while being pz-locked
                    if (hasFlag(TILESTATE_PVPZONE)) {
                        return RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE;
                    }
                } else if (!hasFlag(TILESTATE_PVPZONE)) {
                    // player is trying to leave a pvp zone while being pz-locked
                    return RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE;
                }

                if ((!playerTile->hasFlag(TILESTATE_NOPVPZONE) && hasFlag(TILESTATE_NOPVPZONE)) ||
                    (!playerTile->hasFlag(TILESTATE_PROTECTIONZONE) && hasFlag(TILESTATE_PROTECTIONZONE))) {
                    // player is trying to enter a non-pvp/protection zone while being pz-locked
                    return RETURNVALUE_PLAYERISPZLOCKED;
                }
            }
        } else if (creatures && !creatures->empty() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags)) {
            for (const Creature* tileCreature : *creatures) {
                if (!tileCreature->isInGhostMode()) {
                    return RETURNVALUE_NOTENOUGHROOM;
                }
            }
        }

        if (!hasBitSet(FLAG_IGNOREBLOCKITEM, flags)) {
            //If the FLAG_IGNOREBLOCKITEM bit isn't set we dont have to iterate every single item
            if (hasFlag(TILESTATE_BLOCKSOLID)) {
                return RETURNVALUE_NOTENOUGHROOM;
            }
        } else {
            //FLAG_IGNOREBLOCKITEM is set
            if (ground) {
                const ItemType& iiType = Item::items[ground->getID()];
                if (iiType.blockSolid && (!iiType.moveable || ground->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID))) {
                    return RETURNVALUE_NOTPOSSIBLE;
                }
            }

            if (const auto items = getItemList()) {
                for (const Item* item : *items) {
                    const ItemType& iiType = Item::items[item->getID()];
                    if (iiType.blockSolid && (!iiType.moveable || item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID))) {
                        return RETURNVALUE_NOTPOSSIBLE;
                    }
                }
            }
        }
    } else if (const Item* item = thing.getItem()) {
        const TileItemVector* items = getItemList();
        if (items && items->size() >= 0xFFFF) {
            return RETURNVALUE_NOTPOSSIBLE;
        }

        if (hasBitSet(FLAG_NOLIMIT, flags)) {
            return RETURNVALUE_NOERROR;
        }

        const bool itemIsHangable = item->isHangable();
        if (ground == nullptr && !itemIsHangable) {
            return RETURNVALUE_NOTPOSSIBLE;
        }

        const CreatureVector* creatures = getCreatures();
        if (creatures && !creatures->empty() && item->isBlocking() && !hasBitSet(FLAG_IGNOREBLOCKCREATURE, flags)) {
            for (const Creature* tileCreature : *creatures) {
                if (!tileCreature->isInGhostMode()) {
                    return RETURNVALUE_NOTENOUGHROOM;
                }
            }
        }

        if (itemIsHangable && hasFlag(TILESTATE_SUPPORTS_HANGABLE)) {
            if (items) {
                for (const Item* tileItem : *items) {
                    if (tileItem->isHangable()) {
                        return RETURNVALUE_NEEDEXCHANGE;
                    }
                }
            }
        } else {
            if (ground) {
                const ItemType& iiType = Item::items[ground->getID()];
                if (iiType.blockSolid) {
                    if (!iiType.allowPickupable || item->isMagicField() || item->isBlocking()) {
                        if (!item->isPickupable()) {
                            return RETURNVALUE_NOTENOUGHROOM;
                        }

                        if (!iiType.hasHeight || iiType.pickupable || iiType.isBed()) {
                            return RETURNVALUE_NOTENOUGHROOM;
                        }
                    }
                }
            }

            if (items) {
                for (const Item* tileItem : *items) {
                    const ItemType& iiType = Item::items[tileItem->getID()];
                    if (!iiType.blockSolid) {
                        continue;
                    }

                    if (iiType.allowPickupable && !item->isMagicField() && !item->isBlocking()) {
                        continue;
                    }

                    if (!item->isPickupable()) {
                        return RETURNVALUE_NOTENOUGHROOM;
                    }

                    if (!iiType.hasHeight || iiType.pickupable || iiType.isBed()) {
                        return RETURNVALUE_NOTENOUGHROOM;
                    }
                }
            }
        }
    }
    return RETURNVALUE_NOERROR;
}

ReturnValue Tile::queryMaxCount(int32_t, const Thing&, const uint32_t count, uint32_t& maxQueryCount, uint32_t) const
{
    maxQueryCount = std::max<uint32_t>(1, count);
    return RETURNVALUE_NOERROR;
}

ReturnValue Tile::queryRemove(const Thing& thing, const uint32_t count, const uint32_t flags) const
{
    const int32_t index = getThingIndex(&thing);
    if (index == -1) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    const Item* item = thing.getItem();
    if (item == nullptr) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
        return RETURNVALUE_NOTMOVEABLE;
    }

    return RETURNVALUE_NOERROR;
}

Tile* Tile::queryDestination(int32_t&, const Thing&, Item** destItem, uint32_t& flags)
{
    Tile* destTile = nullptr;
    *destItem = nullptr;

    if (hasFlag(TILESTATE_FLOORCHANGE_DOWN)) {
        uint16_t dx = tilePos.x;
        uint16_t dy = tilePos.y;
        const uint8_t dz = tilePos.z + 1;

        const Tile* southDownTile = g_game.map.getTile(dx, dy - 1, dz);
        if (southDownTile && southDownTile->hasFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT)) {
            dy -= 2;
            destTile = g_game.map.getTile(dx, dy, dz);
        } else {
            const Tile* eastDownTile = g_game.map.getTile(dx - 1, dy, dz);
            if (eastDownTile && eastDownTile->hasFlag(TILESTATE_FLOORCHANGE_EAST_ALT)) {
                dx -= 2;
                destTile = g_game.map.getTile(dx, dy, dz);
            } else {
                const Tile* downTile = g_game.map.getTile(dx, dy, dz);
                if (downTile) {
                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_NORTH)) {
                        ++dy;
                    }

                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_SOUTH)) {
                        --dy;
                    }

                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT)) {
                        dy -= 2;
                    }

                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_EAST)) {
                        --dx;
                    }

                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_EAST_ALT)) {
                        dx -= 2;
                    }

                    if (downTile->hasFlag(TILESTATE_FLOORCHANGE_WEST)) {
                        ++dx;
                    }

                    destTile = g_game.map.getTile(dx, dy, dz);
                }
            }
        }
    } else if (hasFlag(TILESTATE_FLOORCHANGE)) {
        uint16_t dx = tilePos.x;
        uint16_t dy = tilePos.y;
        const uint8_t dz = tilePos.z - 1;

        if (hasFlag(TILESTATE_FLOORCHANGE_NORTH)) {
            --dy;
        }

        if (hasFlag(TILESTATE_FLOORCHANGE_SOUTH)) {
            ++dy;
        }

        if (hasFlag(TILESTATE_FLOORCHANGE_EAST)) {
            ++dx;
        }

        if (hasFlag(TILESTATE_FLOORCHANGE_WEST)) {
            --dx;
        }

        if (hasFlag(TILESTATE_FLOORCHANGE_SOUTH_ALT)) {
            dy += 2;
        }

        if (hasFlag(TILESTATE_FLOORCHANGE_EAST_ALT)) {
            dx += 2;
        }

        destTile = g_game.map.getTile(dx, dy, dz);
    }

    if (destTile == nullptr) {
        destTile = this;
    } else {
        flags |= FLAG_NOLIMIT;    //Will ignore that there is blocking items/creatures
    }

    if (destTile) {
        Thing* destThing = destTile->getTopDownItem();
        if (destThing) {
            *destItem = destThing->getItem();
        }
    }
    return destTile;
}

void Tile::addThing(Thing* thing)
{
    addThing(0, thing);
}

void Tile::addThing(int32_t, Thing* thing)
{
    Creature* creature = thing->getCreature();
    if (creature) {
        g_game.map.clearSpectatorCache(creature->getPlayer());
        creature->setParent(this);
        CreatureVector* creatures = makeCreatures();
#if CLIENT_VERSION >= 853
        creatures->insert(creatures->begin(), creature);
#else
        creatures->push_back(creature);
#endif
    } else {
        Item* item = thing->getItem();
        if (item == nullptr) {
            return /*RETURNVALUE_NOTPOSSIBLE*/;
        }

        TileItemVector* items = getItemList();
        if (items && items->size() >= 0xFFFF) {
            return /*RETURNVALUE_NOTPOSSIBLE*/;
        }

        item->setParent(this);

        const ItemType& itemType = Item::items[item->getID()];
        if (itemType.isGroundTile()) {
            if (ground == nullptr) {
                ground = item;
                onAddTileItem(item);
            } else {
                const ItemType& oldType = Item::items[ground->getID()];

                Item* oldGround = ground;
                ground->setParent(nullptr);
                g_game.ReleaseItem(ground);
                ground = item;
                resetTileFlags(oldGround);
                setTileFlags(item);
                onUpdateTileItem(oldGround, oldType, item, itemType);
                postRemoveNotification(oldGround, nullptr, 0);
            }
        } else if (itemType.alwaysOnTop) {
            if (itemType.isSplash() && items) {
                //remove old splash if exists
                for (ItemVector::const_iterator it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
                    Item* oldSplash = *it;
                    if (!Item::items[oldSplash->getID()].isSplash()) {
                        continue;
                    }

                    removeThing(oldSplash, 1);
                    oldSplash->setParent(nullptr);
                    g_game.ReleaseItem(oldSplash);
                    postRemoveNotification(oldSplash, nullptr, 0);
                    break;
                }
            }

            bool isInserted = false;
            if (items) {
                for (auto it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
                    //Note: this is different from internalAddThing
                    if (itemType.alwaysOnTopOrder <= Item::items[(*it)->getID()].alwaysOnTopOrder) {
                        items->insert(it, item);
                        isInserted = true;
                        break;
                    }
                }
            } else {
                items = makeItemList();
            }

            if (!isInserted) {
                items->insert(items->getEndTopItem(), item);
            }

            items->addTopItemCount(1);
            onAddTileItem(item);
        } else {
            if (itemType.isMagicField()) {
                //remove old field item if exists
                if (items) {
                    for (ItemVector::const_iterator it = items->getBeginDownItem(), end = items->getEndDownItem(); it != end; ++it) {
                        MagicField* oldField = (*it)->getMagicField();
                        if (oldField) {
                            if (oldField->isReplaceable()) {
                                removeThing(oldField, 1);

                                oldField->setParent(nullptr);
                                g_game.ReleaseItem(oldField);
                                postRemoveNotification(oldField, nullptr, 0);
                                break;
                            }
                            //This magic field cannot be replaced.
                            item->setParent(nullptr);
                            g_game.ReleaseItem(item);
                            return;
                        }
                    }
                }
            }

            items = makeItemList();
            items->push_back(item);
            onAddTileItem(item);
        }
    }
}

void Tile::updateThing(Thing* thing, const uint16_t itemId, const uint32_t count)
{
    const int32_t index = getThingIndex(thing);
    if (index == -1) {
        return /*RETURNVALUE_NOTPOSSIBLE*/;
    }

    Item* item = thing->getItem();
    if (item == nullptr) {
        return /*RETURNVALUE_NOTPOSSIBLE*/;
    }

    const ItemType& oldType = Item::items[item->getID()];
    const ItemType& newType = Item::items[itemId];
    resetTileFlags(item);
    item->setID(itemId);
    item->setSubType(count);
    setTileFlags(item);
    onUpdateTileItem(item, oldType, item, newType);
}

void Tile::replaceThing(const uint32_t index, Thing* thing)
{
    int32_t pos = index;

    Item* item = thing->getItem();
    if (item == nullptr) {
        return /*RETURNVALUE_NOTPOSSIBLE*/;
    }

    Item* oldItem = nullptr;
    bool isInserted = false;

    if (ground) {
        if (pos == 0) {
            oldItem = ground;
            ground = item;
            isInserted = true;
        }

        --pos;
    }

    TileItemVector* items = getItemList();
    if (items && !isInserted) {
        const int32_t topItemSize = getTopItemCount();
        if (pos < topItemSize) {
            auto it = items->getBeginTopItem() + pos;
            oldItem = *it;
            it = items->erase(it);
            items->insert(it, item);
            isInserted = true;
        }

        pos -= topItemSize;
    }

    const CreatureVector* creatures = getCreatures();
    if (creatures) {
        if (!isInserted && pos < static_cast<int32_t>(creatures->size())) {
            return /*RETURNVALUE_NOTPOSSIBLE*/;
        }

        pos -= static_cast<uint32_t>(creatures->size());
    }

    if (items && !isInserted) {
        const int32_t downItemSize = getDownItemCount();
        if (pos < downItemSize) {
            auto it = items->getEndDownItem() - (pos + 1);
            oldItem = *it;
            it = items->erase(it);
            items->insert(it, item);
            isInserted = true;
        }
    }

    if (isInserted) {
        item->setParent(this);

        resetTileFlags(oldItem);
        setTileFlags(item);
        const ItemType& oldType = Item::items[oldItem->getID()];
        const ItemType& newType = Item::items[item->getID()];
        onUpdateTileItem(oldItem, oldType, item, newType);

        oldItem->setParent(nullptr);
    }
}

void Tile::removeThing(Thing* thing, const uint32_t count)
{
    Creature* creature = thing->getCreature();
    if (creature) {
        CreatureVector* creatures = getCreatures();
        if (creatures) {
            const auto it = std::find(creatures->begin(), creatures->end(), thing);
            if (it != creatures->end()) {
                g_game.map.clearSpectatorCache(creature->getPlayer());
                creatures->erase(it);
            }
        }
        return;
    }

    Item* item = thing->getItem();
    if (!item) {
        return;
    }

    const int32_t index = getThingIndex(item);
    if (index == -1) {
        return;
    }

    if (item == ground) {
        ground->setParent(nullptr);
        ground = nullptr;

        SpectatorVector spectators;
        g_game.map.getSpectators(spectators, getPosition(), true);
        onRemoveTileItem(spectators, std::vector<int32_t>(spectators.size(), 0), item);
        return;
    }

    TileItemVector* items = getItemList();
    if (!items) {
        return;
    }

    const ItemType& itemType = Item::items[item->getID()];
    if (itemType.alwaysOnTop) {
        const auto it = std::find(items->getBeginTopItem(), items->getEndTopItem(), item);
        if (it == items->getEndTopItem()) {
            return;
        }

        SpectatorVector spectators;
        g_game.map.getSpectators(spectators, getPosition(), true);

        std::vector<int32_t> oldStackPosVector(spectators.size());
        auto i = static_cast<size_t>(-1); //Start index at -1 to avoid copying it
        for (Creature* spectator : spectators) {
            if (const Player* tmpPlayer = spectator->getPlayer()) {
                oldStackPosVector[++i] = getStackposOfItem(tmpPlayer, item);
            }
        }

        item->setParent(nullptr);
        items->erase(it);
        items->addTopItemCount(-1);
        onRemoveTileItem(spectators, oldStackPosVector, item);
    } else {
        const auto end = ItemVector::reverse_iterator(items->getBeginDownItem());
        const auto it = std::find(ItemVector::reverse_iterator(items->getEndDownItem()), end, item);
        if (it == end) {
            return;
        }

        if (itemType.stackable && count != item->getItemCount()) {
            const uint8_t newCount = static_cast<uint8_t>(std::max<int32_t>(0, static_cast<int32_t>(item->getItemCount() - count)));
            item->setItemCount(newCount);
            onUpdateTileItem(item, itemType, item, itemType);
        } else {
            SpectatorVector spectators;
            g_game.map.getSpectators(spectators, getPosition(), true);

            std::vector<int32_t> oldStackPosVector(spectators.size());
            auto i = static_cast<size_t>(-1); //Start index at -1 to avoid copying it
            for (Creature* spectator : spectators) {
                if (const Player* tmpPlayer = spectator->getPlayer()) {
                    oldStackPosVector[++i] = getStackposOfItem(tmpPlayer, item);
                }
            }

            item->setParent(nullptr);
            items->erase(std::next(it).base());
            onRemoveTileItem(spectators, oldStackPosVector, item);
        }
    }
}

#if GAME_FEATURE_FASTER_CLEAN > 0
void Tile::cleanItem(Item* item, const int32_t index, const uint32_t count)
{
    if (index == -1) {
        return;
    }

    if (item == ground) {
        ground->setParent(nullptr);
        ground = nullptr;
        return;
    }

    TileItemVector* items = getItemList();
    if (!items) {
        return;
    }

    const ItemType& itemType = Item::items[item->getID()];
    if (itemType.alwaysOnTop) {
        const auto it = std::find(items->getBeginTopItem(), items->getEndTopItem(), item);
        if (it == items->getEndTopItem()) {
            return;
        }

        item->setParent(nullptr);
        items->erase(it);
        items->addTopItemCount(-1);
    } else {
        const auto end = ItemVector::reverse_iterator(items->getBeginDownItem());
        const auto it = std::find(ItemVector::reverse_iterator(items->getEndDownItem()), end, item);
        if (it == end) {
            return;
        }

        if (itemType.stackable && count != item->getItemCount()) {
            const uint8_t newCount = static_cast<uint8_t>(std::max<int32_t>(0, static_cast<int32_t>(item->getItemCount() - count)));
            item->setItemCount(newCount);
            return;
        }
        item->setParent(nullptr);
        items->erase(std::next(it).base());
    }

    resetTileFlags(item);
}
#endif

void Tile::removeCreature(Creature* creature)
{
    g_game.map.getMapSector(tilePos.x, tilePos.y)->removeCreature(creature);
    removeThing(creature, 0);
}

int32_t Tile::getThingIndex(const Thing* thing) const
{
    int32_t n = -1;
    if (ground) {
        if (ground == thing) {
            return 0;
        }
        ++n;
    }

    const TileItemVector* items = getItemList();
    if (items) {
        const Item* item = thing->getItem();
        if (item && item->isAlwaysOnTop()) {
            for (auto it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
                ++n;
                if (*it == item) {
                    return n;
                }
            }
            return -1;
        }
        n += items->getTopItemCount();
    }

    if (const CreatureVector* creatures = getCreatures()) {
        if (thing->getCreature()) {
            for (const Creature* creature : *creatures) {
                ++n;
                if (creature == thing) {
                    return n;
                }
            }
        } else {
            n += creatures->size();
        }
    }

    if (items) {
        const Item* item = thing->getItem();
        if (item && !item->isAlwaysOnTop()) {
            for (auto it = ItemVector::const_reverse_iterator(items->getEndDownItem()), end = ItemVector::const_reverse_iterator(items->getBeginDownItem()); it != end; ++it) {
                ++n;
                if (*it == item) {
                    return n;
                }
            }
        }
    }
    return -1;
}

int32_t Tile::getClientIndexOfCreature(const Player* player, const Creature* creature) const
{
    int32_t n;
    if (ground) {
        n = 1;
    } else {
        n = 0;
    }

    const TileItemVector* items = getItemList();
    if (items) {
        n += items->getTopItemCount();
    }

    if (const CreatureVector* creatures = getCreatures()) {
        for (auto it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
            const Creature* c = *it;
            if (c == creature) {
                return n;
            }
            if (player->canSeeCreature(c)) {
                ++n;
            }
        }
    }
    return -1;
}

int32_t Tile::getStackposOfCreature(const Player* player, const Creature* creature) const
{
    int32_t n;
    if (ground) {
        n = 1;
    } else {
        n = 0;
    }

    const TileItemVector* items = getItemList();
    if (items) {
        n += items->getTopItemCount();
        if (n >= 10) {
            return -1;
        }
    }

    if (const CreatureVector* creatures = getCreatures()) {
        for (auto it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
            const Creature* c = *it;
            if (c == creature) {
                return n;
            }
            if (player->canSeeCreature(c)) {
                if (++n >= 10) {
                    return -1;
                }
            }
        }
    }
    return -1;
}

int32_t Tile::getStackposOfItem(const Player* player, const Item* item) const
{
    int32_t n = 0;
    if (ground) {
        if (ground == item) {
            return n;
        }
        ++n;
    }

    const TileItemVector* items = getItemList();
    if (items) {
        if (item->isAlwaysOnTop()) {
            for (auto it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
                if (*it == item) {
                    return n;
                }
                if (++n == 10) {
                    return -1;
                }
            }
            return -1;
        }
        n += items->getTopItemCount();
        if (n >= 10) {
            return -1;
        }
    }

    if (const CreatureVector* creatures = getCreatures()) {
        for (const Creature* creature : *creatures) {
            if (player->canSeeCreature(creature)) {
                if (++n >= 10) {
                    return -1;
                }
            }
        }
    }

    if (items && !item->isAlwaysOnTop()) {
        for (auto it = ItemVector::const_reverse_iterator(items->getEndDownItem()), end = ItemVector::const_reverse_iterator(items->getBeginDownItem()); it != end; ++it) {
            if (*it == item) {
                return n;
            }
            if (++n >= 10) {
                return -1;
            }
        }
    }
    return -1;
}

size_t Tile::getFirstIndex() const
{
    return 0;
}

size_t Tile::getLastIndex() const
{
    return getThingCount();
}

uint32_t Tile::getItemTypeCount(const uint16_t itemId, const int32_t subType /*= -1*/) const
{
    uint32_t count = 0;
    if (ground && ground->getID() == itemId) {
        count += Item::countByType(ground, subType);
    }

    const TileItemVector* items = getItemList();
    if (items) {
        for (const Item* item : *items) {
            if (item->getID() == itemId) {
                count += Item::countByType(item, subType);
            }
        }
    }
    return count;
}

Thing* Tile::getThing(size_t index) const
{
    if (ground) {
        if (index == 0) {
            return ground;
        }

        --index;
    }

    const TileItemVector* items = getItemList();
    if (items) {
        const uint32_t topItemSize = items->getTopItemCount();
        if (index < topItemSize) {
            return (*items)[index];
        }
        index -= topItemSize;
    }

    if (const CreatureVector* creatures = getCreatures()) {
        if (index < creatures->size()) {
            return (*creatures)[index];
        }
        index -= creatures->size();
    }

    if (items && index < items->getDownItemCount()) {
        return (*items)[items->size() - (index + 1)];
    }
    return nullptr;
}

void Tile::postAddNotification(Thing* thing, const Cylinder* oldParent, const int32_t index, const cylinderlink_t link /*= LINK_OWNER*/)
{
    SpectatorVector spectators;
    g_game.map.getSpectators(spectators, getPosition(), true, true);
    for (Creature* spectator : spectators) {
        spectator->getPlayer()->postAddNotification(thing, oldParent, index, LINK_NEAR);
    }

    //add a reference to this item, it may be deleted after being added (mailbox for example)
    Creature* creature = thing->getCreature();
    Item* item;
    if (creature) {
        creature->incrementReferenceCounter();
        item = nullptr;
    } else {
        item = thing->getItem();
        if (item) {
            item->incrementReferenceCounter();
        }
    }

    if (link == LINK_OWNER) {
        if (hasFlag(TILESTATE_TELEPORT)) {
            Teleport* teleport = getTeleportItem();
            if (teleport) {
                teleport->addThing(thing);
            }
        } else if (hasFlag(TILESTATE_TRASHHOLDER)) {
            TrashHolder* trashholder = getTrashHolder();
            if (trashholder) {
                trashholder->addThing(thing);
            }
        } else if (hasFlag(TILESTATE_MAILBOX)) {
            Mailbox* mailbox = getMailbox();
            if (mailbox) {
                mailbox->addThing(thing);
            }
        }

        //calling movement scripts
        if (creature) {
            g_moveEvents->onCreatureMove(creature, this, MOVE_EVENT_STEP_IN);
        } else if (item) {
            g_moveEvents->onItemMove(item, this, true);
        }
    }

    //release the reference to this item onces we are finished
    if (creature) {
        g_game.ReleaseCreature(creature);
    } else if (item) {
        g_game.ReleaseItem(item);
    }
}

void Tile::postRemoveNotification(Thing* thing, const Cylinder* newParent, const int32_t index, const cylinderlink_t link /*= LINK_OWNER*/)
{
    SpectatorVector spectators;
    g_game.map.getSpectators(spectators, getPosition(), true, true);

#if GAME_FEATURE_FASTER_CLEAN > 0
    if (getThingCount() > 8 && link != LINK_CLEAN) {
#else
    if (getThingCount() > 8) {
#endif
        onUpdateTile(spectators);
    }

    for (Creature* spectator : spectators) {
        spectator->getPlayer()->postRemoveNotification(thing, newParent, index, LINK_NEAR);
    }

#if GAME_FEATURE_FASTER_CLEAN > 0
    if (link != LINK_CLEAN) {
#else
        {
#endif
            //calling movement scripts
            Creature* creature = thing->getCreature();
            if (creature) {
                g_moveEvents->onCreatureMove(creature, this, MOVE_EVENT_STEP_OUT);
            } else {
                Item* item = thing->getItem();
                if (item) {
                    g_moveEvents->onItemMove(item, this, false);
                }
            }
        }
    }

void Tile::internalAddThing(Thing * thing)
{
    internalAddThing(0, thing);
}

void Tile::internalAddThing(uint32_t, Thing * thing)
{
    thing->setParent(this);

    Creature* creature = thing->getCreature();
    if (creature) {
        g_game.map.clearSpectatorCache(creature->getPlayer());
        CreatureVector* creatures = makeCreatures();
#if CLIENT_VERSION >= 853
        creatures->insert(creatures->begin(), creature);
#else
        creatures->push_back(creature);
#endif
    } else {
        Item* item = thing->getItem();
        if (item == nullptr) {
            return;
        }

        const ItemType& itemType = Item::items[item->getID()];
        if (itemType.isGroundTile()) {
            if (ground == nullptr) {
                ground = item;
                setTileFlags(item);
            }
            return;
        }

        TileItemVector* items = makeItemList();
        if (items->size() >= 0xFFFF) {
            return /*RETURNVALUE_NOTPOSSIBLE*/;
        }

        if (itemType.alwaysOnTop) {
            bool isInserted = false;
            for (auto it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
                if (Item::items[(*it)->getID()].alwaysOnTopOrder > itemType.alwaysOnTopOrder) {
                    items->insert(it, item);
                    isInserted = true;
                    break;
                }
            }

            if (!isInserted) {
                items->insert(items->getEndTopItem(), item);
            }
            items->addTopItemCount(1);
        } else {
            items->push_back(item);
        }

        setTileFlags(item);
    }
}

void Tile::setTileFlags(const Item * item)
{
    if (!hasFlag(TILESTATE_FLOORCHANGE)) {
        const ItemType& it = Item::items[item->getID()];
        if (it.floorChange != 0) {
            setFlag(it.floorChange);
        }
    }

    if (item->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID)) {
        setFlag(TILESTATE_IMMOVABLEBLOCKSOLID);
    }

    if (item->hasProperty(CONST_PROP_BLOCKPATH)) {
        setFlag(TILESTATE_BLOCKPATH);
    }

    if (item->hasProperty(CONST_PROP_NOFIELDBLOCKPATH)) {
        setFlag(TILESTATE_NOFIELDBLOCKPATH);
    }

    if (item->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)) {
        setFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
    }

    if (item->hasProperty(CONST_PROP_BLOCKPROJECTILE)) {
        setFlag(TILESTATE_BLOCKPROJECTILE);
    }

    if (item->getTeleport()) {
        setFlag(TILESTATE_TELEPORT);
    }

    if (item->getMagicField()) {
        setFlag(TILESTATE_MAGICFIELD);
    }

    if (item->getMailbox()) {
        setFlag(TILESTATE_MAILBOX);
    }

    if (item->getTrashHolder()) {
        setFlag(TILESTATE_TRASHHOLDER);
    }

    if (item->hasProperty(CONST_PROP_BLOCKSOLID)) {
        setFlag(TILESTATE_BLOCKSOLID);
    }

    if (item->getBed()) {
        setFlag(TILESTATE_BED);
    }

    const Container* container = item->getContainer();
    if (container && container->getDepotLocker()) {
        setFlag(TILESTATE_DEPOT);
    }

    if (item->hasProperty(CONST_PROP_SUPPORTHANGABLE)) {
        setFlag(TILESTATE_SUPPORTS_HANGABLE);
    }
}

void Tile::resetTileFlags(const Item * item)
{
    const ItemType& it = Item::items[item->getID()];
    if (it.floorChange != 0) {
        resetFlag(TILESTATE_FLOORCHANGE);
    }

#define checkLoop																																			\
	do {																																						\
		if ((blockSolid | immovableBlockSolid | blockPath | noFieldBlockPath | immovableBlockPath | immovableNoFieldBlockPath | blockProjectile) == false) {	\
			break;																																				\
		}																																						\
	} while(0)

    bool blockSolid = item->hasProperty(CONST_PROP_BLOCKSOLID);
    bool immovableBlockSolid = item->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID);
    bool blockPath = item->hasProperty(CONST_PROP_BLOCKPATH);
    bool noFieldBlockPath = item->hasProperty(CONST_PROP_NOFIELDBLOCKPATH);
    bool immovableBlockPath = item->hasProperty(CONST_PROP_IMMOVABLEBLOCKPATH);
    bool immovableNoFieldBlockPath = item->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH);
    bool blockProjectile = item->hasProperty(CONST_PROP_BLOCKPROJECTILE);
    if ((blockSolid | immovableBlockSolid | blockPath | noFieldBlockPath | immovableBlockPath | immovableNoFieldBlockPath | blockProjectile) != false) {
        if (ground && item != ground) {
            if (blockSolid && ground->hasProperty(CONST_PROP_BLOCKSOLID)) {
                blockSolid = false;
            }
            if (immovableBlockSolid && ground->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID)) {
                immovableBlockSolid = false;
            }
            if (blockPath && ground->hasProperty(CONST_PROP_BLOCKPATH)) {
                blockPath = false;
            }
            if (noFieldBlockPath && ground->hasProperty(CONST_PROP_NOFIELDBLOCKPATH)) {
                noFieldBlockPath = false;
            }
            if (immovableBlockPath && ground->hasProperty(CONST_PROP_IMMOVABLEBLOCKPATH)) {
                immovableBlockPath = false;
            }
            if (immovableNoFieldBlockPath && ground->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)) {
                immovableNoFieldBlockPath = false;
            }
            if (blockProjectile && ground->hasProperty(CONST_PROP_BLOCKPROJECTILE)) {
                blockProjectile = false;
            }
        }

        if (const TileItemVector* items = getItemList()) {
            for (const Item* checkItem : *items) {
                if (item != checkItem) {
                    if (blockSolid && checkItem->hasProperty(CONST_PROP_BLOCKSOLID)) {
                        blockSolid = false;
                        checkLoop;
                    }
                    if (immovableBlockSolid && checkItem->hasProperty(CONST_PROP_IMMOVABLEBLOCKSOLID)) {
                        immovableBlockSolid = false;
                        checkLoop;
                    }
                    if (blockPath && checkItem->hasProperty(CONST_PROP_BLOCKPATH)) {
                        blockPath = false;
                        checkLoop;
                    }
                    if (noFieldBlockPath && checkItem->hasProperty(CONST_PROP_NOFIELDBLOCKPATH)) {
                        noFieldBlockPath = false;
                        checkLoop;
                    }
                    if (immovableBlockPath && checkItem->hasProperty(CONST_PROP_IMMOVABLEBLOCKPATH)) {
                        immovableBlockPath = false;
                        checkLoop;
                    }
                    if (immovableNoFieldBlockPath && checkItem->hasProperty(CONST_PROP_IMMOVABLENOFIELDBLOCKPATH)) {
                        immovableNoFieldBlockPath = false;
                        checkLoop;
                    }
                    if (blockProjectile && checkItem->hasProperty(CONST_PROP_BLOCKPROJECTILE)) {
                        blockProjectile = false;
                        checkLoop;
                    }
                }
            }
        }
    }
#undef checkLoop

    if (blockSolid) {
        resetFlag(TILESTATE_BLOCKSOLID);
    }

    if (immovableBlockSolid) {
        resetFlag(TILESTATE_IMMOVABLEBLOCKSOLID);
    }

    if (blockPath) {
        resetFlag(TILESTATE_BLOCKPATH);
    }

    if (noFieldBlockPath) {
        resetFlag(TILESTATE_NOFIELDBLOCKPATH);
    }

    if (immovableBlockPath) {
        resetFlag(TILESTATE_IMMOVABLEBLOCKPATH);
    }

    if (immovableNoFieldBlockPath) {
        resetFlag(TILESTATE_IMMOVABLENOFIELDBLOCKPATH);
    }

    if (blockProjectile) {
        resetFlag(TILESTATE_BLOCKPROJECTILE);
    }

    if (item->getTeleport()) {
        resetFlag(TILESTATE_TELEPORT);
    }

    if (item->getMagicField()) {
        resetFlag(TILESTATE_MAGICFIELD);
    }

    if (item->getMailbox()) {
        resetFlag(TILESTATE_MAILBOX);
    }

    if (item->getTrashHolder()) {
        resetFlag(TILESTATE_TRASHHOLDER);
    }

    if (item->getBed()) {
        resetFlag(TILESTATE_BED);
    }

    const Container* container = item->getContainer();
    if (container && container->getDepotLocker()) {
        resetFlag(TILESTATE_DEPOT);
    }

    if (item->hasProperty(CONST_PROP_SUPPORTHANGABLE)) {
        resetFlag(TILESTATE_SUPPORTS_HANGABLE);
    }
}

bool Tile::isMoveableBlocking() const
{
    return !ground || hasFlag(TILESTATE_BLOCKSOLID);
}

Item* Tile::getUseItem(int32_t index) const
{
    const TileItemVector* items = getItemList();
    if (!items || items->size() == 0) {
        return ground;
    }

#if CLIENT_VERSION >= 1230
    // Might be some versions before but I don't have access to them
    // Cipsoft probably omits creatures in stackpos for some micro-optimizations to avoid unnecessary cache-misses
    if (ground) {
        if (index == 0) {
            return ground;
        }

        --index;
    }

    int32_t topItemSize = items->getTopItemCount();
    if (index < topItemSize) {
        return (*items)[index];
    }
    index -= topItemSize;
    if (index < static_cast<int32_t>(items->getDownItemCount())) {
        return (*items)[items->size() - (index + 1)];
    }
#else
    if (Thing* thing = getThing(index)) {
        return thing->getItem();
    }
#endif

    return nullptr;
}