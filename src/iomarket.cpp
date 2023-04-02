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

#include "iomarket.h"

#include "configmanager.h"
#include "databasetasks.h"
#include "iologindata.h"
#include "game.h"
#include "tasks.h"

extern ConfigManager g_config;
extern Game g_game;

#if GAME_FEATURE_MARKET > 0
MarketOfferList IOMarket::getActiveOffers(const MarketAction_t action, const uint16_t itemId)
{
    MarketOfferList offerList;

    std::stringExtended query(256);
    query << "SELECT `id`, `amount`, `price`, `created`, `anonymous`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `player_name` FROM `market_offers` WHERE `sale` = " << action << " AND `itemtype` = " << itemId;

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return offerList;
    }

    const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    offerList.reserve(result->countResults());
    do {
        auto price = result->getNumber<uint32_t>("price");
        uint32_t timestamp = result->getNumber<uint32_t>("created") + marketOfferDuration;
        auto amount = result->getNumber<uint16_t>("amount");
        uint16_t counter = result->getNumber<uint32_t>("id") & 0xFFFF;
        if (result->getNumber<uint16_t>("anonymous") == 0) {
            offerList.emplace_back(price, timestamp, amount, counter, static_cast<uint16_t>(0), std::move(result->getString("player_name")));
        } else {
            offerList.emplace_back(price, timestamp, amount, counter, static_cast<uint16_t>(0), "Anonymous");
        }
    } while (result->next());
    return offerList;
}

MarketOfferList IOMarket::getOwnOffers(const MarketAction_t action, const uint32_t playerId)
{
    MarketOfferList offerList;

    const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    std::stringExtended query(256);
    query << "SELECT `id`, `amount`, `price`, `created`, `itemtype` FROM `market_offers` WHERE `player_id` = " << playerId << " AND `sale` = " << action;

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return offerList;
    }

    offerList.reserve(result->countResults());
    do {
        auto price = result->getNumber<uint32_t>("price");
        uint32_t timestamp = result->getNumber<uint32_t>("created") + marketOfferDuration;
        auto amount = result->getNumber<uint16_t>("amount");
        uint16_t counter = result->getNumber<uint32_t>("id") & 0xFFFF;
        auto itemId = result->getNumber<uint16_t>("itemtype");
        offerList.emplace_back(price, timestamp, amount, counter, itemId, "");
    } while (result->next());
    return offerList;
}

HistoryMarketOfferList IOMarket::getOwnHistory(const MarketAction_t action, const uint32_t playerId)
{
    HistoryMarketOfferList offerList;

    std::stringExtended query(256);
    query << "SELECT `itemtype`, `amount`, `price`, `expires_at`, `state` FROM `market_history` WHERE `player_id` = " << playerId << " AND `sale` = " << action;

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return offerList;
    }

    offerList.reserve(result->countResults());
    do {
        auto timestamp = result->getNumber<uint32_t>("expires_at");
        auto price = result->getNumber<uint32_t>("price");
        auto itemId = result->getNumber<uint16_t>("itemtype");
        auto amount = result->getNumber<uint16_t>("amount");

        auto offerState = static_cast<MarketOfferState_t>(result->getNumber<uint16_t>("state"));
        if (offerState == OFFERSTATE_ACCEPTEDEX) {
            offerState = OFFERSTATE_ACCEPTED;
        }

        offerList.emplace_back(timestamp, price, itemId, amount, offerState);
    } while (result->next());
    return offerList;
}

void IOMarket::processExpiredOffers(const DBResult_ptr& result, bool)
{
    if (!result) {
        return;
    }

    do {
        if (!moveOfferToHistory(result->getNumber<uint32_t>("id"), OFFERSTATE_EXPIRED)) {
            continue;
        }

        const auto playerId = result->getNumber<uint32_t>("player_id");
        const auto amount = result->getNumber<uint16_t>("amount");
        if (result->getNumber<uint16_t>("sale") == 1) {
            const ItemType& itemType = Item::items[result->getNumber<uint16_t>("itemtype")];
            if (itemType.id == 0) {
                continue;
            }

            Player* player = g_game.getPlayerByGUID(playerId);
            if (!player) {
                player = new Player(nullptr);
                if (!IOLoginData::loadPlayerById(player, playerId)) {
                    delete player;
                    continue;
                }
            }

            if (itemType.stackable) {
                uint16_t tmpAmount = amount;
                while (tmpAmount > 0) {
                    const uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
                    Item* item = Item::CreateItem(itemType.id, stackCount);
                    if (g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                        delete item;
                        break;
                    }

                    tmpAmount -= stackCount;
                }
            } else {
                int32_t subType;
                if (itemType.charges != 0) {
                    subType = itemType.charges;
                } else {
                    subType = -1;
                }

                for (uint16_t i = 0; i < amount; ++i) {
                    Item* item = Item::CreateItem(itemType.id, subType);
                    if (g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
                        delete item;
                        break;
                    }
                }
            }

            if (player->isOffline()) {
                IOLoginData::savePlayer(player);
                delete player;
            }
        } else {
            const uint64_t totalPrice = result->getNumber<uint64_t>("price") * amount;

            Player* player = g_game.getPlayerByGUID(playerId);
            if (player) {
                player->setBankBalance(player->getBankBalance() + totalPrice);
            } else {
                IOLoginData::increaseBankBalance(playerId, totalPrice);
            }
        }
    } while (result->next());
}

void IOMarket::checkExpiredOffers()
{
    const time_t lastExpireDate = time(nullptr) - g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    std::stringExtended query(128);
    query << "SELECT `id`, `amount`, `price`, `itemtype`, `player_id`, `sale` FROM `market_offers` WHERE `created` <= " << lastExpireDate;
    g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)), processExpiredOffers, true);

    const int32_t checkExpiredMarketOffersEachMinutes = g_config.getNumber(ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES);
    if (checkExpiredMarketOffersEachMinutes <= 0) {
        return;
    }

    g_dispatcher.addEvent(checkExpiredMarketOffersEachMinutes * 60 * 1000, checkExpiredOffers);
}

uint32_t IOMarket::getPlayerOfferCount(const uint32_t playerId)
{
    std::stringExtended query(128);
    query << "SELECT COUNT(*) AS `count` FROM `market_offers` WHERE `player_id` = " << playerId << " LIMIT 1";

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return 0;
    }
    return result->getNumber<int32_t>("count");
}

MarketOfferEx IOMarket::getOfferByCounter(const uint32_t timestamp, const uint16_t counter)
{
    MarketOfferEx offer;

    const int32_t created = timestamp - g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    std::stringExtended query(256);
    query << "SELECT `id`, `sale`, `itemtype`, `amount`, `created`, `price`, `player_id`, `anonymous`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `player_name` FROM `market_offers` WHERE `created` = " << created << " AND (`id` & 65535) = " << counter << " LIMIT 1";

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        offer.id = 0;
        offer.playerId = 0;
        return offer;
    }

    offer.id = result->getNumber<uint32_t>("id");
    offer.type = static_cast<MarketAction_t>(result->getNumber<uint16_t>("sale"));
    offer.amount = result->getNumber<uint16_t>("amount");
    offer.counter = result->getNumber<uint32_t>("id") & 0xFFFF;
    offer.timestamp = result->getNumber<uint32_t>("created");
    offer.price = result->getNumber<uint32_t>("price");
    offer.itemId = result->getNumber<uint16_t>("itemtype");
    offer.playerId = result->getNumber<uint32_t>("player_id");
    if (result->getNumber<uint16_t>("anonymous") == 0) {
        offer.playerName = std::move(result->getString("player_name"));
    } else {
        offer.playerName = "Anonymous";
    }
    return offer;
}

void IOMarket::createOffer(const uint32_t playerId, const MarketAction_t action, const uint32_t itemId, const uint16_t amount, const uint32_t price, const bool anonymous)
{
    std::stringExtended query(256);
    query << "INSERT INTO `market_offers` (`player_id`, `sale`, `itemtype`, `amount`, `price`, `created`, `anonymous`) VALUES (";
    query << playerId << ',';
    query << action << ',';
    query << itemId << ',';
    query << amount << ',';
    query << price << ',';
    query << time(nullptr) << ',';
    query << (anonymous ? "1" : "0") << ')';
    g_database.executeQuery(query);
}

void IOMarket::acceptOffer(const uint32_t offerId, const uint16_t amount)
{
    std::stringExtended query(128);
    query << "UPDATE `market_offers` SET `amount` = `amount` - " << amount << " WHERE `id` = " << offerId;
    g_database.executeQuery(query);
}

void IOMarket::deleteOffer(const uint32_t offerId)
{
    std::stringExtended query(64);
    query << "DELETE FROM `market_offers` WHERE `id` = " << offerId;
    g_database.executeQuery(query);
}

void IOMarket::appendHistory(const uint32_t playerId, const MarketAction_t type, const uint16_t itemId, const uint16_t amount, const uint32_t price, const time_t timestamp, const MarketOfferState_t state)
{
    std::stringExtended query(256);
    query << "INSERT INTO `market_history` (`player_id`, `sale`, `itemtype`, `amount`, `price`, `expires_at`, `inserted`, `state`) VALUES (";
    query << playerId << ',';
    query << type << ',';
    query << itemId << ',';
    query << amount << ',';
    query << price << ',';
    query << timestamp << ',';
    query << time(nullptr) << ',';
    query << state << ')';
    g_databaseTasks.addTask(std::move(static_cast<std::string&>(query)));
}

bool IOMarket::moveOfferToHistory(const uint32_t offerId, const MarketOfferState_t state)
{
    const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

    std::stringExtended query(128);
    query << "SELECT `player_id`, `sale`, `itemtype`, `amount`, `price`, `created` FROM `market_offers` WHERE `id` = " << offerId << " LIMIT 1";

    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return false;
    }

    query.clear();
    query << "DELETE FROM `market_offers` WHERE `id` = " << offerId;
    if (!g_database.executeQuery(query)) {
        return false;
    }

    appendHistory(result->getNumber<uint32_t>("player_id"), static_cast<MarketAction_t>(result->getNumber<uint16_t>("sale")), result->getNumber<uint16_t>("itemtype"), result->getNumber<uint16_t>("amount"), result->getNumber<uint32_t>("price"), result->getNumber<uint32_t>("created") + marketOfferDuration, state);
    return true;
}

void IOMarket::updateStatistics()
{
    std::stringExtended query(256);
    query << "SELECT `sale`, `itemtype`, COUNT(`price`) AS `num`, MIN(`price`) AS `min`, MAX(`price`) AS `max`, SUM(`price`) AS `sum` FROM `market_history` WHERE `state` = " << OFFERSTATE_ACCEPTED << " GROUP BY `itemtype`, `sale`";
    const DBResult_ptr result = g_database.storeQuery(query);
    if (!result) {
        return;
    }

    do {
        MarketStatistics* statistics;
        if (result->getNumber<uint16_t>("sale") == MARKETACTION_BUY) {
            statistics = &purchaseStatistics[result->getNumber<uint16_t>("itemtype")];
        } else {
            statistics = &saleStatistics[result->getNumber<uint16_t>("itemtype")];
        }

        statistics->numTransactions = result->getNumber<uint32_t>("num");
        statistics->lowestPrice = result->getNumber<uint32_t>("min");
        statistics->totalPrice = result->getNumber<uint64_t>("sum");
        statistics->highestPrice = result->getNumber<uint32_t>("max");
    } while (result->next());
}

MarketStatistics* IOMarket::getPurchaseStatistics(const uint16_t itemId)
{
    const auto it = purchaseStatistics.find(itemId);
    if (it == purchaseStatistics.end()) {
        return nullptr;
    }
    return &it->second;
}

MarketStatistics* IOMarket::getSaleStatistics(const uint16_t itemId)
{
    const auto it = saleStatistics.find(itemId);
    if (it == saleStatistics.end()) {
        return nullptr;
    }
    return &it->second;
}
#endif
