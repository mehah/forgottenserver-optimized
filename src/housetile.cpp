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

#include "housetile.h"
#include "house.h"
#include "game.h"

HouseTile::HouseTile(const int32_t x, const int32_t y, const int32_t z, House* house) :
    DynamicTile(x, y, z), house(house) {}

void HouseTile::addThing(const int32_t index, Thing* thing)
{
    Tile::addThing(index, thing);

    if (!thing->getParent()) {
        return;
    }

    if (Item* item = thing->getItem()) {
        updateHouse(item);
    }
}

void HouseTile::internalAddThing(const uint32_t index, Thing* thing)
{
    Tile::internalAddThing(index, thing);

    if (!thing->getParent()) {
        return;
    }

    if (Item* item = thing->getItem()) {
        updateHouse(item);
    }
}

void HouseTile::updateHouse(Item* item) const
{
    if (item->getParent() != this) {
        return;
    }

    Door* door = item->getDoor();
    if (door) {
        if (door->getDoorId() != 0) {
            house->addDoor(door);
        }
    } else {
        BedItem* bed = item->getBed();
        if (bed) {
            house->addBed(bed);
        }
    }
}

ReturnValue HouseTile::queryAdd(const int32_t index, const Thing& thing, const uint32_t count, const uint32_t flags, Creature* actor/* = nullptr*/) const
{
    if (const Creature* creature = thing.getCreature()) {
        if (const Player* player = creature->getPlayer()) {
            if (!house->isInvited(player)) {
                return RETURNVALUE_PLAYERISNOTINVITED;
            }
        } else {
            return RETURNVALUE_NOTPOSSIBLE;
        }
    } else if (thing.getItem() && actor) {
        const Player* actorPlayer = actor->getPlayer();
        if (!house->isInvited(actorPlayer)) {
            return RETURNVALUE_CANNOTTHROW;
        }
    }
    return Tile::queryAdd(index, thing, count, flags, actor);
}

Tile* HouseTile::queryDestination(int32_t& index, const Thing& thing, Item** destItem, uint32_t& flags)
{
    if (const Creature* creature = thing.getCreature()) {
        if (const Player* player = creature->getPlayer()) {
            if (!house->isInvited(player)) {
                const Position& entryPos = house->getEntryPosition();
                Tile* destTile = g_game.map.getTile(entryPos);
                if (!destTile) {
                    std::cout << "Error: [HouseTile::queryDestination] House entry not correct"
                        << " - Name: " << house->getName()
                        << " - House id: " << house->getId()
                        << " - Tile not found: " << entryPos << std::endl;

                    destTile = g_game.map.getTile(player->getTemplePosition());
                    if (!destTile) {
                        destTile = &nullptr_tile;
                    }
                }

                index = -1;
                *destItem = nullptr;
                return destTile;
            }
        }
    }

    return Tile::queryDestination(index, thing, destItem, flags);
}