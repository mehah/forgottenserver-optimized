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

#include "depotchest.h"
#include "tools.h"

DepotChest::DepotChest(const uint16_t type) :
    Container(type), maxDepotItems(1500) {}

ReturnValue DepotChest::queryAdd(const int32_t index, const Thing& thing, const uint32_t count,
                                 const uint32_t flags, Creature* actor/* = nullptr*/) const
{
    const Item* item = thing.getItem();
    if (item == nullptr) {
        return RETURNVALUE_NOTPOSSIBLE;
    }

    const bool skipLimit = hasBitSet(FLAG_NOLIMIT, flags);
    if (!skipLimit) {
        int32_t addCount = 0;

        if (item->isStackable() && item->getItemCount() != count) {
            addCount = 1;
        }

        if (item->getTopParent() != this) {
            if (const Container* container = item->getContainer()) {
                addCount = container->getItemHoldingCount() + 1;
            } else {
                addCount = 1;
            }
        }

        if (getItemHoldingCount() + addCount > maxDepotItems) {
            return RETURNVALUE_DEPOTISFULL;
        }
    }

    return Container::queryAdd(index, thing, count, flags, actor);
}

void DepotChest::postAddNotification(Thing* thing, const Cylinder* oldParent, const int32_t index, cylinderlink_t)
{
    Cylinder* parent = getParent();
    if (parent != nullptr) {
        parent->postAddNotification(thing, oldParent, index, LINK_PARENT);
    }
}

void DepotChest::postRemoveNotification(Thing* thing, const Cylinder* newParent, const int32_t index, cylinderlink_t)
{
    Cylinder* parent = getParent();
    if (parent != nullptr) {
        parent->postRemoveNotification(thing, newParent, index, LINK_PARENT);
    }
}

#if GAME_FEATURE_MARKET > 0
Cylinder* DepotChest::getParent() const
{
    if (parent) {
        return parent->getParent();
    }
    return nullptr;
}
#endif