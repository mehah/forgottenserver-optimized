GameStore.MaxSearchResults = 25 -- Max results that can be displayed by search
GameStore.OfferTypes = {
    OFFER_TYPE_NONE = 0, -- (this will disable offer)
    OFFER_TYPE_ITEM = 1,
    OFFER_TYPE_STACKABLE = 2,
    OFFER_TYPE_OUTFIT = 3,
    OFFER_TYPE_OUTFIT_ADDON = 4,
    OFFER_TYPE_MOUNT = 5,
    OFFER_TYPE_NAMECHANGE = 6,
    OFFER_TYPE_SEXCHANGE = 7,
    OFFER_TYPE_PROMOTION = 8,
    OFFER_TYPE_HOUSE = 9,
    OFFER_TYPE_EXPBOOST = 10,
    OFFER_TYPE_PREYSLOT = 11,
    OFFER_TYPE_PREYBONUS = 12,
    OFFER_TYPE_TEMPLE = 13,
    OFFER_TYPE_BLESSINGS = 14,
    OFFER_TYPE_PREMIUM = 15,
    OFFER_TYPE_POUCH = 16,
    OFFER_TYPE_ALLBLESSINGS = 17
}

GameStore.ClientOfferTypes = {
    CLIENT_STORE_OFFER_OTHER = 0,
    CLIENT_STORE_OFFER_NAMECHANGE = 1,
    CLIENT_STORE_OFFER_WORLDTRANSFER = 9,
    CLIENT_STORE_OFFER_HIRELING = 10,
    CLIENT_STORE_OFFER_TOURNAMENTCONFIGURATION = 11,
    CLIENT_STORE_OFFER_MAINCHARACTER = 12
}

GameStore.HistoryTypes = {
    HISTORY_TYPE_NONE = 0,
    HISTORY_TYPE_GIFT = 1,
    HISTORY_TYPE_REFUND = 2
}

GameStore.States = {
    STATE_NONE = 0,
    STATE_NEW = 1,
    STATE_SALE = 2,
    STATE_TIMED = 3
}

GameStore.StoreErrors = {
    STORE_ERROR_PURCHASE = 0,
    STORE_ERROR_NETWORK = 1,
    STORE_ERROR_HISTORY = 2,
    STORE_ERROR_TRANSFER = 3,
    STORE_ERROR_INFORMATION = 4
}

GameStore.LegacyServiceTypes = {
    SERVICE_STANDERD = 0,
    SERVICE_OUTFITS = 3,
    SERVICE_MOUNTS = 4,
    SERVICE_BLESSINGS = 5
}

GameStore.ServiceTypes = {
    SERVICE_HOME = 0,
    SERVICE_CATEGORY_TYPE = 1,
    SERVICE_CATEGORY_NAME = 2,
    SERVICE_OFFER_TYPE = 3,
    SERVICE_OFFER_ID = 4,
    SERVICE_OFFER_NAME = 5
}

GameStore.SortTypes = {
    SORT_MOSTPOPULAR = 1,
    SORT_ALPHABETICALLY = 2,
    SORT_NEWEST = 3
}

GameStore.CoinTypes = {
    TIBIA_COIN = 0,
    TRANSFERABLE_TIBIA_COIN = 1,
    TOURNAMENT_COIN = 2
}

GameStore.ShowTypes = {
    SHOW_ICON = 0,
    SHOW_MOUNT = 1,
    SHOW_OUTFIT = 2,
    SHOW_ITEM = 3,
    SHOW_OUTFITMF = 4
}

GameStore.EventTypes = {
    EVENT_OFFER_CHANGED = 0,
    EVENT_FILTER_CHANGED = 1,
    EVENT_SORTTYPE_CHANGED = 2,
    EVENT_HOME = 3
}

GameStore.FeatureTypes = {
    FEATURE_TYPE_NONE = 0,
    FEATURE_TYPE_CATEGORY_TYPE = 1,
    FEATURE_TYPE_CATEGORY_NAME = 2,
    FEATURE_TYPE_OFFER_TYPE = 3,
    FEATURE_TYPE_OFFER_ID = 4,
    FEATURE_TYPE_OFFER_NAME = 5
}

GameStore.StoreErrors = {
    STORE_ERROR_PURCHASE = 0,
    STORE_ERROR_NETWORK = 1,
    STORE_ERROR_HISTORY = 2,
    STORE_ERROR_TRANSFER = 3,
    STORE_ERROR_INFORMATION = 4
}

GameStore.Home = {
    featureScrollingTime = 10, -- in about how many seconds features should scroll to next
    newOffers = {{
        icons = {'Premium_Time_360.png'},
        name = '360 Days',
        price = 3000,
        state = GameStore.States.STATE_SALE,
        item = 16101,
        id = 3360,
        type = GameStore.OfferTypes.OFFER_TYPE_PREMIUM,
        validUntil = 30
    }},
    features = {{
        icon = '64/Category_PremiumTime.png',
        categoryName = 'Premium Time'
    }, {
        icon = '64/Premium_Time_90.png',
        offerId = 3090
    }}
}

-- try to keep id value unique - so it is possible to include them in home category
GameStore.Categories = {{
    icons = {'Category_PremiumTime.png'},
    name = 'Premium Time',
    offers = {{
        icons = {'Premium_Time_30.png'},
        name = '30 Days',
        price = 250,
        state = GameStore.States.STATE_SALE,
        item = 16101,
        id = 3030,
        type = GameStore.OfferTypes.OFFER_TYPE_PREMIUM,
        validUntil = 30,
        description = 'Enhance your gaming experience by gaining additional abilities and advantages:\n\n\n- access to Premium areas\n- use Tibia\'s transport system (ships, carpet)\n- more spells\n- rent houses\n- found guilds\n- offline training\n- larger depots\n- and many more\n\n- valid for all characters on this account\n- activated at purchase'
    }, {
        icons = {'Premium_Time_90.png'},
        name = '90 Days',
        price = 750,
        state = GameStore.States.STATE_SALE,
        item = 16101,
        id = 3090,
        type = GameStore.OfferTypes.OFFER_TYPE_PREMIUM,
        validUntil = 30,
        description = 'Enhance your gaming experience by gaining additional abilities and advantages:\n\n\n- access to Premium areas\n- use Tibia\'s transport system (ships, carpet)\n- more spells\n- rent houses\n- found guilds\n- offline training\n- larger depots\n- and many more\n\n- valid for all characters on this account\n- activated at purchase'
    }, {
        icons = {'Premium_Time_180.png'},
        name = '180 Days',
        price = 1500,
        state = GameStore.States.STATE_SALE,
        item = 16101,
        id = 3180,
        type = GameStore.OfferTypes.OFFER_TYPE_PREMIUM,
        validUntil = 30,
        description = 'Enhance your gaming experience by gaining additional abilities and advantages:\n\n\n- access to Premium areas\n- use Tibia\'s transport system (ships, carpet)\n- more spells\n- rent houses\n- found guilds\n- offline training\n- larger depots\n- and many more\n\n- valid for all characters on this account\n- activated at purchase'
    }, {
        icons = {'Premium_Time_360.png'},
        name = '360 Days',
        price = 3000,
        state = GameStore.States.STATE_SALE,
        item = 16101,
        id = 3360,
        type = GameStore.OfferTypes.OFFER_TYPE_PREMIUM,
        validUntil = 30,
        description = 'Enhance your gaming experience by gaining additional abilities and advantages:\n\n\n- access to Premium areas\n- use Tibia\'s transport system (ships, carpet)\n- more spells\n- rent houses\n- found guilds\n- offline training\n- larger depots\n- and many more\n\n- valid for all characters on this account\n- activated at purchase'
    }},
    state = GameStore.States.STATE_SALVE
}, -- Blessings
{
    icons = {'Category_Blessings.png'},
    name = 'Blessings',
    offers = {{
        icons = {'All_PvE_Blessings.png'},
        name = 'All Regular Blessings',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 130,
        id = 9,
        type = GameStore.OfferTypes.OFFER_TYPE_ALLBLESSINGS,
        package = {{
            icons = {'Twist_of_Fate.png'},
            name = 'Twist of Fate'
        }, {
            icons = {'Wisdom_of_Solitude.png'},
            name = 'The Wisdom of Solitude'
        }, {
            icons = {'Spark_of_the_Phoenix.png'},
            name = 'The Spark of the Phoenix'
        }, {
            icons = {'Fire_of_the_Suns.png'},
            name = 'The Fire of the Suns'
        }, {
            icons = {'Spiritual_Shielding.png'},
            name = 'The Spiritual Shielding'
        }, {
            icons = {'Embrace_of_Tibia.png'},
            name = 'The Embrace of Tibia'
        }, {
            icons = {'Heart_of_the_Mountain.png'},
            name = 'Heart of the Mountain'
        }, {
            icons = {'Blood_of_the_Mountain.png'},
            name = 'Blood of the Mountain'
        }}
    }, {
        icons = {'Twist_of_Fate.png'},
        name = 'Twist of Fate',
        description = 'Protects your character\'s regular blessings or an Amulet of Loss if you are unfortunate enough to die in a PvP fight.\n\n- only usable by purchasing character\n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings\n- does not work for characters with a red or black skull',
        price = 8,
        id = 1,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Wisdom_of_Solitude.png'},
        name = 'The Wisdom of Solitude',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 15,
        id = 2,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Spark_of_the_Phoenix.png'},
        name = 'The Spark of the Phoenix',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 20,
        id = 3,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Fire_of_the_Suns.png'},
        name = 'The Fire of the Suns',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 15,
        id = 4,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Spiritual_Shielding.png'},
        name = 'The Spiritual Shielding',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 15,
        id = 5,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Embrace_of_Tibia.png'},
        name = 'The Embrace of Tibia',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 15,
        id = 6,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Heart_of_the_Mountain.png'},
        name = 'Heart of the Mountain',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 25,
        id = 7,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Blood_of_the_Mountain.png'},
        name = 'Blood of the Mountain',
        description = 'Reduces your character\'s chance to lose any items as well as the amount of your character\'s experience and skill loss upon death:\n\n -1 blessing = 8.00% less Skill / XP loss, 30% equipment protection\n -2 blessing = 16.00% less Skill / XP loss, 55% equipment protection\n- 3 blessing = 24.00% less Skill / XP loss, 75% equipment protection\n- 4 blessing = 32.00% less Skill / XP loss, 90% equipment protection\n- 5 blessing = 40.00% less Skill / XP loss, 100% equipment protection\n- 6 blessing = 48.00% less Skill / XP loss, 100% equipment protection\n- 7 blessing = 56.00% less Skill / XP loss, 100% equipment protection\n- only usable by purchasing character \n- maximum amount that can be owned by character: 5\n- added directly to the Record of Blessings \n- characters with a red or black skull will always lose all equipment upon death',
        price = 25,
        id = 8,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }, {
        icons = {'Death_Redemption.png'},
        name = 'Death Redemption',
        description = 'Reduces the penalty of your character\'s most recent death.\n\n\n- only usable by purchasing character\n- can only be used for the most recent death and only within 24 hours after this death',
        price = 260,
        id = 10,
        type = GameStore.OfferTypes.OFFER_TYPE_BLESSINGS
    }},
    state = GameStore.States.STATE_NONE
}, -- Casks
{
    icons = {'Category_Casks.png'},
    name = 'Casks',
    offers = {{
        icons = {'Mana_Cask.png'},
        name = 'Mana Cask',
        price = 5,
        item = 28565,
        id = 28565,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to refill your mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Strong_Mana_Cask.png'},
        name = 'Strong Mana Cask',
        price = 9,
        item = 28566,
        id = 28566,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to refill your mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Great_Mana_Cask.png'},
        name = 'Great Mana Cask',
        price = 14,
        item = 28567,
        id = 28567,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to refill your mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Ultimate_Mana_Cask.png'},
        name = 'Ultimate Mana Cask',
        price = 42,
        item = 28568,
        id = 28568,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to refill your mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Health_Cask.png'},
        name = 'Health Cask',
        price = 5,
        item = 28555,
        id = 28555,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Strong_Health_Cask.png'},
        name = 'Strong Health Cask',
        price = 11,
        item = 28556,
        id = 28556,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Great_Health_Cask.png'},
        name = 'Great Health Cask',
        price = 22,
        item = 28557,
        id = 28557,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Ultimate_Health_Cask.png'},
        name = 'Ultimate Health Cask',
        price = 36,
        item = 28558,
        id = 28558,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Supreme_Health_Cask.png'},
        name = 'Supreme Health Cask',
        price = 59,
        item = 28559,
        id = 28559,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Great_Spirit_Cask.png'},
        name = 'Great Spirit Cask',
        price = 22,
        item = 28575,
        id = 28575,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points and mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Ultimate_Spirit_Cask.png'},
        name = 'Ultimate Spirit Cask',
        price = 42,
        item = 28576,
        id = 28576,
        count = 1000,
        number = 1,
        description = 'Place it in your house and fill up potions to restore your hit points and mana!\n\n\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used to fill up potions by all characters that have access to the house\n- potions created from this cask will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n- usable 1000 times a piece\n- can only be purchased with transferable Tibia Coins',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }},
    state = GameStore.States.STATE_NONE
}, -- Exercise Weapons
{
    description = 'Buy your character a boost to speed up your character development.',
    icons = {'Category_ExerciseWeapons.png'},
    name = 'Exercise Weapons',
    offers = {{
        icons = {'Exercise_Sword.png'},
        name = 'Exercise Sword',
        price = 25,
        item = 32384,
        id = 32384,
        count = 500,
        number = 1,
        description = 'Use it to train your sword fighting skill on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your sword fighting skill\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Exercise_Axe.png'},
        name = 'Exercise Axe',
        price = 25,
        item = 32385,
        id = 32385,
        count = 500,
        number = 1,
        description = 'Use it to train your axe fighting skill on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your axe fighting skill\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Exercise_Club.png'},
        name = 'Exercise Club',
        price = 25,
        item = 32386,
        id = 32386,
        count = 500,
        number = 1,
        description = 'Use it to train your club fighting skill on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your club fighting skill\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Exercise_Rod.png'},
        name = 'Exercise Rod',
        price = 25,
        item = 32388,
        id = 32388,
        count = 500,
        number = 1,
        description = 'Use it to train your magic level on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your magic level\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Exercise_Wand.png'},
        name = 'Exercise Wand',
        price = 25,
        item = 32389,
        id = 32389,
        count = 500,
        number = 1,
        description = 'Use it to train your magic level on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your magic level\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Exercise_Bow.png'},
        name = 'Exercise Bow',
        price = 25,
        item = 32387,
        id = 32387,
        count = 500,
        number = 1,
        description = 'Use it to train your distance fighting skill on an exercise dummy!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on an exercise dummy to train your distance fighting skill\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }},
    state = GameStore.States.STATE_NONE
}, -- Kegs
{
    icons = {'Category_Kegs.png'},
    name = 'Kegs',
    offers = {{
        icons = {'Mana_Keg.png'},
        name = 'Mana Keg',
        price = 26,
        item = 28584,
        id = 28584,
        count = 500,
        number = 500,
        description = 'Fill up potions to refill your mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Mana_Keg.png'},
        name = 'Strong Mana Keg',
        price = 43,
        item = 28585,
        id = 28585,
        count = 500,
        number = 500,
        description = 'Fill up potions to refill your mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Mana_Keg.png'},
        name = 'Great Mana Keg',
        price = 66,
        item = 28586,
        id = 28586,
        count = 500,
        number = 500,
        description = 'Fill up potions to refill your mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Mana_Keg.png'},
        name = 'Ultimate Mana Keg',
        price = 202,
        item = 28587,
        id = 28587,
        count = 500,
        number = 500,
        description = 'Fill up potions to refill your mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Health_Keg.png'},
        name = 'Health Keg',
        price = 26,
        item = 28579,
        id = 28579,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Health_Keg.png'},
        name = 'Strong Health Keg',
        price = 53,
        item = 28580,
        id = 28580,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Health_Keg.png'},
        name = 'Great Health Keg',
        price = 103,
        item = 28581,
        id = 28581,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Health_Keg.png'},
        name = 'Ultimate Health Keg',
        price = 175,
        item = 28582,
        id = 28582,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Supreme_Health_Keg.png'},
        name = 'Supreme Health Keg',
        price = 288,
        item = 28583,
        id = 28583,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Spirit_Keg.png'},
        name = 'Great Spirit Keg',
        price = 105,
        item = 28589,
        id = 28589,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points and mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Spirit_Keg.png'},
        name = 'Ultimate Spirit Keg',
        price = 202,
        item = 28590,
        id = 28590,
        count = 500,
        number = 500,
        description = 'Fill up potions to restore your hit points and mana no matter where you are!\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- potions created from this keg will be sent to your Store inbox and can only be stored there and in depot box\n- usable 500 times a piece\n- saves capacity because it\'s constant weight equals only 250 potions',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }},
    state = GameStore.States.STATE_NONE
}, -- Potions
{
    icons = {'Category_Potions.png'},
    name = 'Potions',
    offers = {{
        icons = {'Health_Potion.png'},
        name = 'Health Potion',
        price = 6,
        item = 7618,
        id = 7618,
        count = 125,
        number = 125,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Health_Potion.png'},
        name = 'Health Potion',
        price = 11,
        item = 7618,
        id = 7621,
        count = 300,
        number = 300,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Health_Potion.png'},
        name = 'Strong Health Potion',
        price = 10,
        item = 7588,
        id = 7588,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Health_Potion.png'},
        name = 'Strong Health Potion',
        price = 21,
        item = 7588,
        id = 7596,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Health_Potion.png'},
        name = 'Great Health Potion',
        price = 18,
        item = 7591,
        id = 7591,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Health_Potion.png'},
        name = 'Great Health Potion',
        price = 41,
        item = 7591,
        id = 7598,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Health_Potion.png'},
        name = 'Ultimate Health Potion',
        price = 29,
        item = 8473,
        id = 8473,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Health_Potion.png'},
        name = 'Ultimate Health Potion',
        price = 68,
        item = 8473,
        id = 8478,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Supreme_Health_Potion.png'},
        name = 'Supreme Health Potion',
        price = 47,
        item = 26031,
        id = 26031,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Supreme_Health_Potion.png'},
        name = 'Supreme Health Potion',
        price = 113,
        item = 26031,
        id = 26038,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Mana_Potion.png'},
        name = 'Mana Potion',
        price = 6,
        item = 7620,
        id = 7620,
        count = 125,
        number = 125,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Mana_Potion.png'},
        name = 'Mana Potion',
        price = 12,
        item = 7620,
        id = 7625,
        count = 300,
        number = 300,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Mana_Potion.png'},
        name = 'Strong Mana Potion',
        price = 7,
        item = 7589,
        id = 7589,
        count = 100,
        number = 100,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Strong_Mana_Potion.png'},
        name = 'Strong Mana Potion',
        price = 17,
        item = 7589,
        id = 7594,
        count = 250,
        number = 250,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Mana_Potion.png'},
        name = 'Great Mana Potion',
        price = 11,
        item = 7590,
        id = 7590,
        count = 100,
        number = 100,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Mana_Potion.png'},
        name = 'Great Mana Potion',
        price = 26,
        item = 7590,
        id = 7595,
        count = 250,
        number = 250,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Mana_Potion.png'},
        name = 'Ultimate Mana Potion',
        price = 33,
        item = 26029,
        id = 26029,
        count = 100,
        number = 100,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Mana_Potion.png'},
        name = 'Ultimate Mana Potion',
        price = 79,
        item = 26029,
        id = 26035,
        count = 250,
        number = 250,
        description = 'Refills your character\'s mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Spirit_Potion.png'},
        name = 'Great Spirit Potion',
        price = 18,
        item = 8472,
        id = 8472,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points and mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Great_Spirit_Potion.png'},
        name = 'Great Spirit Potion',
        price = 41,
        item = 8472,
        id = 8475,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points and mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Spirit_Potion.png'},
        name = 'Ultimate Spirit Potion',
        price = 33,
        item = 26030,
        id = 26030,
        count = 100,
        number = 100,
        description = 'Restores your character\'s hit points and mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Spirit_Potion.png'},
        name = 'Ultimate Spirit Potion',
        price = 79,
        item = 26030,
        id = 26034,
        count = 250,
        number = 250,
        description = 'Restores your character\'s hit points and mana.\n\n\n- only usable by purchasing character\n- only buyable if fitting vocation and level of purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }},
    state = GameStore.States.STATE_NONE
}, -- Runes
{
    description = 'Buy magically filled runes to unleash their energy when in need of it.',
    icons = {'Category_Runes.png'},
    name = 'Runes',
    offers = {{
        icons = {'Animate_Dead_Rune.png'},
        name = 'Animate Dead Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nAfter a long time of research, the magicians of Edron succeeded in storing some life energy in a rune. When this energy was unleashed onto a body it was found that an undead creature arose that could be mentally controlled by the user of the rune. This rune is useful to create allies in combat.',
        price = 75,
        item = 2316,
        id = 2316,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Avalanche_Rune.png'},
        name = 'Avalanche Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThe ice damage which arises from this rune is a useful weapon in every battle but it comes in particularly handy if you fight against a horde of creatures dominated by the element fire.',
        price = 12,
        item = 2274,
        id = 2274,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThe metamorphosis caused by this rune is only superficial, and while casters who are using the rune can take on the exterior form of nearly any inanimate object, they will always retain their original smell and mental abilities. So there is no real practical use for this rune, making this largely a fun rune.',
        icons = {'Chameleon_Rune.png'},
        name = 'Chameleon Rune',
        price = 42,
        item = 2291,
        id = 2291,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nUsing this rune together with some mana, you can convince certain creatures. The needed amount of mana is determined by the power of the creature one wishes to convince, so the amount of mana to convince a rat is lower than that which is needed for an orc.',
        icons = {'Convince_Creature_Rune.png'},
        name = 'Convince Creature Rune',
        price = 16,
        item = 2290,
        id = 2290,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nIn the old days, many adventurers fell prey to poisonous creatures that were roaming the caves and forests. After many years of research druids finally succeeded in altering the cure poison spell so it could be bound to a rune. By using this rune it is possible to stop the effect of any known poison.',
        icons = {'Cure_Poison_Rune_(Item).png'},
        name = 'Cure Poison Rune',
        price = 13,
        item = 2266,
        id = 2266,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nNothing is worse than being cornered when fleeing from an enemy you just cannot beat, especially if the obstacles in your way are items you could easily remove if only you had the time! However, there is one reliable remedy: The Disintegrate rune will instantly destroy up to 500 movable items that are in your way, making room for a quick escape.',
        icons = {'Disintegrate_Rune.png'},
        name = 'Disintegrate Rune',
        price = 5,
        item = 2310,
        id = 2310,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nUsing the Energy Bomb rune will create a field of deadly energy that deals damage to all who carelessly step into it. Its area of effect is covering a full 9 square metres! Creatures that are caught in the middle of an Energy Bomb are frequently confused by the unexpected effect, and some may even stay in the field of deadly sparks for a while.',
        icons = {'Energy_Bomb_Rune.png'},
        name = 'Energy Bomb Rune',
        price = 40,
        item = 2262,
        id = 2262,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis spell creates a limited barrier made up of crackling energy that will cause electrical damage to all those passing through. Since there are few creatures that are immune to the harmful effects of energy this spell is not to be underestimated.',
        icons = {'Energy_Field_Rune.png'},
        name = 'Energy Field Rune',
        price = 8,
        item = 2277,
        id = 2277,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nCasting this spell generates a solid wall made up of magical energy. Walls made this way surpass any other magically created obstacle in width, so it is always a good idea to have an Energy Wall rune or two in one\'s pocket when travelling through the wilderness.',
        icons = {'Energy_Wall_Rune.png'},
        name = 'Energy Wall Rune',
        price = 17,
        item = 2279,
        id = 2279,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis rune must be aimed at areas rather than at specific creatures, so it is possible for explosions to be unleashed even if no targets are close at all. These explosions cause a considerable physical damage within a substantial blast radius.',
        icons = {'Explosion_Rune.png'},
        name = 'Explosion Rune',
        price = 6,
        item = 2313,
        id = 2313,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nWhen this rune is used a massive fiery ball is released which hits the aimed foe with immense power. It is especially effective against opponents of the element earth.',
        icons = {'Fireball_Rune.png'},
        name = 'Fireball Rune',
        price = 6,
        item = 2302,
        id = 2302,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis rune is a deadly weapon in the hands of the skilled user. On releasing it an area of 9 square metres is covered by searing flames that will scorch all those that are unfortunate enough to be caught in them. Worse, many monsters are confused by the unexpected blaze, and with a bit of luck a caster will even manage to trap his opponents by using the spell.',
        icons = {'Fire_Bomb_Rune.png'},
        name = 'Fire Bomb Rune',
        price = 29,
        item = 2305,
        id = 2305,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nWhen this rune is used a field of one square metre is covered by searing fire that will last for some minutes, gradually diminishing as the blaze wears down. As with all field spells, Fire Field is quite useful to block narrow passageways or to create large, connected barriers.',
        icons = {'Fire_Field_Rune.png'},
        name = 'Fire Field Rune',
        price = 6,
        item = 2301,
        id = 2301,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis rune offers reliable protection against all creatures that are afraid of fire. The exceptionally long duration of the spell as well as the possibility to form massive barriers or even protective circles out of fire walls make this a versatile, practical spell.',
        icons = {'Fire_Wall_Rune.png'},
        name = 'Fire Wall Rune',
        price = 12,
        item = 2303,
        id = 2303,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nA shot of this rune affects a huge area - up to 37 square metres! It stands to reason that the Great Fireball is a favourite of most Tibians, as it is well suited both to hit whole crowds of monsters and individual targets that are difficult to hit because they are fast or hard to spot.',
        icons = {'Great_Fireball_Rune.png'},
        name = 'Great Fireball Rune',
        price = 12,
        item = 2304,
        id = 2304,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nParticularly creatures determined by the element fire are vulnerable against this ice-cold rune. Being hit by the magic stored in this rune, an ice arrow seems to pierce the heart of the struck victim. The damage done by this rune is quite impressive which makes this a quite popular rune among Tibian mages.',
        icons = {'Icicle_Rune.png'},
        name = 'Icicle Rune',
        price = 6,
        item = 2271,
        id = 2271,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis rune is commonly used by young adventurers who are not skilled enough to use the rune\'s stronger version. Also, since the rune\'s effectiveness is determined by the user\'s magic skill, it is still popular among experienced spell casters who use it to get effective healing magic at a cheap price.',
        icons = {'Intense_Healing_Rune_(Item).png'},
        name = 'Intense Healing Rune',
        price = 19,
        item = 2265,
        id = 2265,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis spell causes all particles that are contained in the surrounding air to quickly gather and contract until a solid wall is formed that covers one full square metre. The wall that is formed that way is impenetrable to any missiles or to light and no creature or character can walk through it. However, the wall will only last for a couple of seconds.',
        icons = {'Magic_Wall_Rune.png'},
        name = 'Magic Wall Rune',
        price = 23,
        item = 2293,
        id = 2293,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThis rune causes an area of 9 square metres to be contaminated with toxic gas that will poison anybody who is caught within it. Conceivable applications include the blocking of areas or the combat against fast-moving or invisible targets. Keep in mind, however, that there are a number of creatures that are immune to poison.',
        icons = {'Poison_Bomb_Rune.png'},
        name = 'Poison Bomb Rune',
        price = 17,
        item = 2286,
        id = 2286,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nWhen this rune is used a wall of concentrated toxic fumes is created which inflicts a moderate poison on all those who are foolish enough to enter it. The effect is usually impressive enough to discourage monsters from doing so, although few of the stronger ones will hesitate if there is nothing but a poison wall between them and their dinner.',
        icons = {'Poison_Wall_Rune.png'},
        name = 'Poison Wall Rune',
        price = 10,
        item = 2289,
        id = 2289,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nSoulfire is an immensely evil spell as it directly targets a creature\'s very life essence. When the rune is used on a victim, its soul is temporarily moved out of its body, casting it down into the blazing fires of hell itself! Note that the experience and the mental strength of the caster influence the damage that is caused.',
        icons = {'Soulfire_Rune.png'},
        name = 'Soulfire Rune',
        price = 9,
        item = 2308,
        id = 2308,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Stone_Shower_Rune.png'},
        name = 'Stone Shower Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nParticularly creatures with an affection to energy will suffer greatly from this rune filled with powerful earth damage. As the name already says, a shower of stones drums on the opponents of the rune user in an area up to 37 squares.',
        price = 7,
        item = 2288,
        id = 2288,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Sudden_Death_Rune.png'},
        name = 'Sudden Death Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nNearly no other spell can compare to Sudden Death when it comes to sheer damage. For this reason it is immensely popular despite the fact that only a single target is affected. However, since the damage caused by the rune is of deadly nature, it is less useful against most undead creatures.',
        price = 28,
        item = 2268,
        id = 2268,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Thunderstorm_Rune.png'},
        name = 'Thunderstorm Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nFlashes filled with dangerous energy hit the rune user\'s opponent when this rune is being used. It is especially effective against ice dominated creatures. Covering up an area up to 37 squares, this rune is particularly useful when you meet a whole mob of opponents.',
        price = 9,
        item = 2315,
        id = 2315,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Ultimate_Healing_Rune.png'},
        name = 'Ultimate Healing Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nThe coveted Ultimate Healing rune is an all-time favourite among all vocations. No other healing enchantments that are bound into runes can compare to its salutary effect.',
        price = 35,
        item = 2273,
        id = 2273,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Wild_Growth_Rune.png'},
        name = 'Wild Growth Rune',
        description = '- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- only buyable if fitting vocation and level of purchasing character\n- cannot be purchased by characters with protection zone block or battle sign\n- cannot be purchased if capacity is exceeded\n\nBy unleashing this spell, all seeds that are lying dormant in the surrounding quickly sprout and grow into full-sized plants, thus forming an impenetrable thicket. Unfortunately, plant life created this way is short-lived and will collapse within minutes, so the magically created obstacle will not last long.',
        price = 32,
        item = 2269,
        id = 2269,
        count = 250,
        number = 250,
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }},
    state = GameStore.States.STATE_NONE
}, -- Mounts
{
    description = 'Buy your character one or more of the fabolous Mountsoffered here.',
    icons = {'Category_Mounts.png'},
    name = 'Mounts',
    offers = {{
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Arctic Unicorn lives in a deep rivalry with its cousin the Blazing Unicorn. Even though they were born in completely different areas, they somehow share the same bloodline. The eternal battle between fire and ice continues. Who will win? Tangerine vs.crystal blue! The choice is yours!',
        icons = {'Arctic_Unicorn.png'},
        name = 'Arctic Unicorn',
        price = 870,
        mount = 1018,
        id = 114,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Armoured War Horse is a dangerous black beauty! When you see its threatening, blood-red eyes coming towards you, you\'ll know trouble is on its way. Protected by its heavy armour plates, the warhorse is the perfect partner for dangerous hunting sessions and excessive enemy slaughtering.',
        icons = {'Armoured_War_Horse.png'},
        name = 'Armoured War Horse',
        price = 870,
        mount = 426,
        id = 23,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRumour has it that many years ago elder witches had gathered to hold a magical feast high up in the mountains. They had crossbred Batcat to easily conquer rocky canyons and deep valleys. Nobody knows what happened on their way up but only the mount has been seen ever since.',
        icons = {'Batcat.png'},
        name = 'Batcat',
        price = 870,
        mount = 728,
        id = 77,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nBadgers have been a staple of the Tibian fauna for a long time, and finally some daring souls have braved the challenge to tame some exceptional specimens - and succeeded! While the common badger you can encounter during your travels might seem like a rather unassuming creature, the Battle Badger, the Ether Badger, and the Zaoan Badger are fierce and mighty beasts, which are at your beck and call.',
        icons = {'Battle_Badger.png'},
        name = 'Battle Badger',
        price = 690,
        mount = 1247,
        id = 147,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTreat your character to a new travelling companion with a gentle nature and an impressive antler: The noble Black Stag will carry you through the deepest snow.',
        icons = {'Black_Stag.png'},
        name = 'Black Stag',
        price = 660,
        mount = 686,
        id = 73,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Blackpelt is out searching for the best bamboo in Tibia. Its heavy armour allows it to visit even the most dangerous places. Treat it nicely with its favourite food from time to time and it will become a loyal partner.',
        icons = {'Blackpelt.png'},
        name = 'Blackpelt',
        price = 690,
        mount = 651,
        id = 58,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Blazing Unicorn lives in a deep rivalry with its cousin the Arctic Unicorn. Even though they were born in completely different areas, they somehow share the same bloodline. The eternal battle between fire and ice continues. Who will win? Crystal blue vs. tangerine! The choice is yours!',
        icons = {'Blazing_Unicorn.png'},
        name = 'Blazing Unicorn',
        price = 870,
        mount = 1017,
        id = 113,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are fascinated by insectoid creatures and can picture yourself riding one during combat or just for travelling? The Bloodcurl will carry you through the Tibian wilderness with ease.',
        icons = {'Bloodcurl.png'},
        name = 'Bloodcurl',
        price = 750,
        mount = 869,
        id = 92,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOwls have always been a symbol of mystery, magic and wisdom in Tibian myths and fairy tales. Having one of these enigmatic creatures of the night as a trustworthy companion provides you with a silent guide whose ever-watchful eyes will cut through the shadows, help you navigate the darkness and unravel great secrets.',
        icons = {'Boreal_Owl.png'},
        name = 'Boreal Owl',
        price = 870,
        mount = 1106,
        id = 129,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYour lower back worsens with every trip you spend on the back of your mount and you are looking for a more comfortable alternative to travel through the lands? Say no more! The Bunny Dray comes with two top-performing hares that never get tired thanks to the brand new and highly innovative propulsion technology. Just keep some back-up carrots in your pocket and you will be fine!',
        icons = {'Bunny_Dray.png'},
        name = 'Bunny Dray',
        price = 870,
        mount = 1180,
        id = 139,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWhen the nights are getting longer and freezing wind brings driving snow into the land, snowmen rise and shine on every corner. Lately, a peaceful, arcane creature has found shelter in one of them and used its magical power to call the Caped Snowman into being. Wrap yourself up well and warmly and jump on the back of your new frosty companion.',
        icons = {'Caped_Snowman.png'},
        name = 'Caped Snowman',
        price = 870,
        mount = 1169,
        id = 137,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that the Cave Tarantula was born long before Banor walked the earth of Tibia. While its parents died in the war against the cruel hordes sent by Brog and Zathroth, their child survived by hiding in skulls of burned enemies. It never left its hiding spot and as it grew older, the skulls merged into its body. Now, it is fully-grown and thirsts for revenge.',
        icons = {'Cave_Tarantula.png'},
        name = 'Cave Tarantula',
        price = 690,
        mount = 1026,
        id = 117,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more of an imp than an angel, you may prefer riding out on a Cinderhoof to scare fellow Tibians on their festive strolls. Its devilish mask, claw-like hands and sharp hooves makes it the perfect companion for any daring adventurer who likes to stand out.',
        icons = {'Cinderhoof.png'},
        name = 'Cinderhoof',
        price = 870,
        mount = 851,
        id = 90,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYour lower back worsens with every trip you spend on the back of your mount and you are looking for a more comfortable alternative to travel through the lands? Say no more! The Cony Cart comes with two top-performing hares that never get tired thanks to the brand new and highly innovative propulsion technology. Just keep some back-up carrots in your pocket and you will be fine!',
        icons = {'Cony_Cart.png'},
        name = 'Cony Cart',
        price = 870,
        mount = 1181,
        id = 140,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more interested in the achievements of science, you may enjoy a ride on the Copper Fly, one of the new insect-like flying machines. Even if you do not move around, the wings of these unusual vehicles are always in motion.',
        icons = {'Copper_Fly.png'},
        name = 'Copper Fly',
        price = 870,
        mount = 671,
        id = 61,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf the Coralripper moves its fins, it generates enough air pressure that it can even float over land. Its numerous eyes allow it to quickly detect dangers even in confusing situations and eliminate them with one powerful bite. If you watch your fingers, you are going to be good friends.',
        icons = {'Coralripper.png'},
        name = 'Coralripper',
        price = 570,
        mount = 735,
        id = 79,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that the Cranium Spider was born long before Banor walked the earth of Tibia. While its parents died in the war against the cruel hordes sent by Brog and Zathroth, their child survived by hiding in skulls of burned enemies. It never left its hiding spot and as it grew older, the skulls merged into its body. Now, it is fully-grown and thirsts for revenge.',
        icons = {'Cranium_Spider.png'},
        name = 'Cranium Spider',
        price = 690,
        mount = 1025,
        id = 116,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nHave you ever dreamed of gliding through the air on the back of a winged creature? With its deep red wings, the majestic Crimson Ray is a worthy mount for courageous heroes. Feel like a king on its back as you ride into your next adventure.',
        icons = {'Crimson_Ray.png'},
        name = 'Crimson Ray',
        price = 870,
        mount = 521,
        id = 33,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = 'The Death Crawler is a scorpion that has surpassed the natural boundaries of its own kind. Way bigger, stronger and faster than ordinary scorpions, it makes a perfect companion for fearless heroes and explorers. Just be careful of his poisonous sting when you mount it.',
        icons = {'Death_Crawler.png'},
        name = 'Death Crawler',
        price = 600,
        mount = 624,
        id = 46,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIts roaring is piercing marrow and bone and can be heard over ten miles away. The Desert King is the undisputed ruler of its territory and no one messes with this animal. Show no fear and prove yourself worthy of its trust and you will get yourself a valuable companion for your adventures.',
        icons = {'Desert_King.png'},
        name = 'Desert King',
        price = 450,
        mount = 572,
        id = 41,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce captured and held captive by a mad hunter, the Doombringer is the result of sick experiments. Fed only with demon dust and concentrated demonic blood it had to endure a dreadful transformation. The demonic blood that is now running through its veins, however, provides it with incredible strength and endurance.',
        icons = {'Doombringer.png'},
        name = 'Doombringer',
        price = 780,
        mount = 644,
        id = 53,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nDo you like fluffy bunnies but think they are too small? Do you admire the majesty of stags and their antlers but are afraid of their untameable wilderness? Do not worry, the mystic creature Dreadhare consolidates the best qualities of both animals. Hop on its backs and enjoy the ride.',
        icons = {'Dreadhare.png'},
        name = 'Dreadhare',
        price = 870,
        mount = 906,
        id = 104,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that in ancient times, the sabre-tooth tiger was already used as a mount by elder warriors of Svargrond. As seafaring began to expand, this noble big cat was also transported to other regions in Tibia. Influenced by the new environment and climatic changes, the fur of the Ebony Tiger has developed its extraordinary colouring over several generations.',
        icons = {'Ebony_Tiger.png'},
        name = 'Ebony Tiger',
        price = 750,
        mount = 1091,
        id = 123,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThousands of years ago, its ancestors ruled the world. Only recently, it found its way into Tibia. The Ember Saurian has been spotted in a sea of flames and fire deep down in the depths of Kazordoon.',
        icons = {'Ember_Saurian.png'},
        name = 'Ember Saurian',
        price = 750,
        mount = 960,
        id = 111,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRide an Emerald Sphinx on your way through ancient chambers and tombs and have a loyal friend by your side while fighting countless mummies and other creatures.',
        icons = {'Emerald_Sphinx.png'},
        name = 'Emerald Sphinx',
        price = 750,
        mount = 951,
        id = 108,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWaccoons are cuddly creatures that love nothing more than to be petted and snuggled! Share a hug, ruffle the fur of the Emerald Waccoon and scratch it behind its ears to make it happy.',
        icons = {'Emerald_Waccoon.png'},
        name = 'Emerald Waccoon',
        price = 750,
        mount = 693,
        id = 70,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTreat your character to a new travelling companion with a gentle nature and an impressive antler: The noble Emperor Deer will carry you through the deepest snow.',
        icons = {'Emperor_Deer.png'},
        name = 'Emperor Deer',
        price = 660,
        mount = 687,
        id = 74,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nBadgers have been a staple of the Tibian fauna for a long time, and finally some daring souls have braved the challenge to tame some exceptional specimens - and succeeded! While the common badger you can encounter during your travels might seem like a rather unassuming creature, the Battle Badger, the Ether Badger, and the Zaoan Badger are fierce and mighty beasts, which are at your beck and call.',
        icons = {'Ether_Badger.png'},
        name = 'Ether Badger',
        price = 690,
        mount = 1248,
        id = 148,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that in ancient times, the sabre-tooth tiger was already used as a mount by elder warriors of Svargrond. As seafaring began to expand, this noble big cat was also transported to other regions in Tibia. Influenced by the new environment and climatic changes, the fur of the Feral Tiger has developed its extraordinary colouring over several generations.',
        icons = {'Feral_Tiger.png'},
        name = 'Feral Tiger',
        price = 750,
        mount = 1092,
        id = 124,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWhen the nights are getting longer and freezing wind brings driving snow into the land, snowmen rise and shine on every corner. Lately, a peaceful, arcane creature has found shelter in one of them and used its magical power to call the Festive Snowman into being. Wrap yourself up well and warmly and jump on the back of your new frosty companion.',
        icons = {'Festive_Snowman.png'},
        name = 'Festive Snowman',
        price = 900,
        mount = 1167,
        id = 135,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that the Gloom Widow was born long before Banor walked the earth of Tibia. While its parents died in the war against the cruel hordes sent by Brog and Zathroth, their child survived by hiding in skulls of burned enemies. It never left its hiding spot and as it grew older, the skulls merged into its body. Now, it is fully-grown and thirsts for revenge.',
        icons = {'Gloom_Widow.png'},
        name = 'Gloom Widow',
        price = 690,
        mount = 1027,
        id = 118,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nLegend has it that a mare and a stallion once reached the end of a rainbow and decided to stay there. Influenced by the mystical power of the rainbow, the mare gave birth to an exceptional foal: Not only the big, strong horn on its forehead but the unusual colouring of its hair makes the Prismatic Unicorn a unique mount in every respect.',
        icons = {'Prismatic_Unicorn.png'},
        name = 'Prismatic Unicorn',
        price = 870,
        mount = 1019,
        id = 115,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYour lower back worsens with every trip you spend on the back of your mount and you are looking for a more comfortable alternative to travel through the lands? Say no more! The Rabbit Rickshaw comes with two top-performing hares that never get tired thanks to the brand new and highly innovative propulsion technology. Just keep some back-up carrots in your pocket and you will be fine!',
        icons = {'Rabbit_Rickshaw.png'},
        name = 'Rabbit Rickshaw',
        price = 870,
        mount = 1179,
        id = 138,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nA wild, ancient creature, which had been hiding in the depths of the shadows for a very long time, has been spotted in Tibia again! The almighty Shadow Draptor has returned and only the bravest Tibians can control such a beast!',
        icons = {'Shadow_Draptor.png'},
        name = 'Shadow Draptor',
        price = 870,
        mount = 427,
        id = 24,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nForged by only the highest skilled blacksmiths in the depths of Kazordoon\'s furnaces, a wild animal made out of the finest steel arose from glowing embers and blazing heat. Protected by its impenetrable armour, the Steelbeak is ready to accompany its master on every battleground.',
        icons = {'Steelbeak.png'},
        name = 'Steelbeak',
        price = 870,
        mount = 522,
        id = 34,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThousands of years ago, its ancestors ruled the world. Only recently, it found its way into Tibia. The Jungle Saurian likes to hide in dense wood and overturned trees.',
        icons = {'Jungle_Saurian.png'},
        name = 'Jungle Saurian',
        price = 750,
        mount = 959,
        id = 110,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIt is said that in ancient times, the sabre-tooth tiger was already used as a mount by elder warriors of Svargrond. As seafaring began to expand, this noble big cat was also transported to other regions in Tibia. Influenced by the new environment and climatic changes, the fur of the Jungle Tiger has developed its extraordinary colouring over several generations.',
        icons = {'Jungle_Tiger.png'},
        name = 'Jungle Tiger',
        price = 750,
        mount = 1093,
        id = 125,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThousands of years ago, its ancestors ruled the world. Only recently, it found its way into Tibia. The Lagoon Saurian feels most comfortable in torrential rivers and behind dangerous waterfalls.',
        icons = {'Lagoon_Saurian.png'},
        name = 'Lagoon Saurian',
        price = 750,
        mount = 961,
        id = 112,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRide a Gold Sphinx on your way through ancient chambers and tombs and have a loyal friend by your side while fighting countless mummies and other creatures.',
        icons = {'Gold_Sphinx.png'},
        name = 'Gold Sphinx',
        price = 750,
        mount = 950,
        id = 107,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRide a Shadow Sphinx on your way through ancient chambers and tombs and have a loyal friend by your side while fighting countless mummies and other creatures.',
        icons = {'Shadow_Sphinx.png'},
        name = 'Shadow Sphinx',
        price = 750,
        mount = 952,
        id = 109,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nDo you like fluffy bunnies but think they are too small? Do you admire the majesty of stags and their antlers but are afraid of their untameable wilderness? Do not worry, the mystic creature Jackalope consolidates the best qualities of both animals. Hop on its backs and enjoy the ride.',
        icons = {'Jackalope.png'},
        name = 'Jackalope',
        price = 870,
        mount = 905,
        id = 103,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIncredible strength and smartness, an irrepressible will to survive, passionately hunting in groups. If these attributes apply to your character, we have found the perfect partner for you. Have a proper look at Ivory Fang, which stands loyally by its master\'s side in every situation. It is time to become the leader of the wolf pack!',
        icons = {'Ivory_Fang.png'},
        name = 'Ivory Fang',
        price = 750,
        mount = 901,
        id = 100,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIncredible strength and smartness, an irrepressible will to survive, passionately hunting in groups. If these attributes apply to your character, we have found the perfect partner for you. Have a proper look at Shadow Claw, which stands loyally by its master\'s side in every situation. It is time to become the leader of the wolf pack!',
        icons = {'Shadow_Claw.png'},
        name = 'Shadow Claw',
        price = 750,
        mount = 902,
        id = 101,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIncredible strength and smartness, an irrepressible will to survive, passionately hunting in groups. If these attributes apply to your character, we have found the perfect partner for you. Have a proper look at Snow Pelt, which stands loyally by its master\'s side in every situation. It is time to become the leader of the wolf pack!',
        icons = {'Snow_Pelt.png'},
        name = 'Snow Pelt',
        price = 750,
        mount = 903,
        id = 102,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOwls have always been a symbol of mystery, magic and wisdom in Tibian myths and fairy tales. Having one of these enigmatic creatures of the night as a trustworthy companion provides you with a silent guide whose ever-watchful eyes will cut through the shadows, help you navigate the darkness and unravel great secrets.',
        icons = {'Snowy_Owl.png'},
        name = 'Snowy Owl',
        price = 870,
        mount = 1105,
        id = 128,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOwls have always been a symbol of mystery, magic and wisdom in Tibian myths and fairy tales. Having one of these enigmatic creatures of the night as a trustworthy companion provides you with a silent guide whose ever-watchful eyes will cut through the shadows, help you navigate the darkness and unravel great secrets.',
        icons = {'Tawny_Owl.png'},
        name = 'Tawny Owl',
        price = 870,
        mount = 1104,
        id = 127,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTo the keen observer, the crocovile is clearly a relative of the crocodile, albeit their look suggests an even more aggressive nature. While it is true that the power of its massive and muscular body can not only crush enemies dead but also break through any gate like a battering ram, a crocovile is, above all, a steadfast companion showing unwavering loyalty to its owner.',
        icons = {'Swamp_Crocovile.png'},
        name = 'Swamp Crocovile',
        price = 750,
        mount = 1184,
        id = 142,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are intrigued by tortoises and would love to throne on a tortoise shell when travelling the Tibian wilderness? The Swamp Snapper might become your new trustworthy companion then, which will transport you safely and even carry you during combat.',
        icons = {'Swamp_Snapper.png'},
        name = 'Swamp Snapper',
        price = 690,
        mount = 886,
        id = 95,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are intrigued by tortoises and would love to throne on a tortoise shell when travelling the Tibian wilderness? The Mould Shell might become your new trustworthy companion then, which will transport you safely and even carry you during combat.',
        icons = {'Mould_Shell.png'},
        name = 'Mould Shell',
        price = 690,
        mount = 887,
        id = 96,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are intrigued by tortoises and would love to throne on a tortoise shell when travelling the Tibian wilderness? The Reed Lurker might become your new trustworthy companion then, which will transport you safely and even carry you during combat.',
        icons = {'Reed_Lurker.png'},
        name = 'Reed Lurker',
        price = 690,
        mount = 888,
        id = 97,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are fascinated by insectoid creatures and can picture yourself riding one during combat or just for travelling? The Leafscuttler will carry you through the Tibian wilderness with ease.',
        icons = {'Leafscuttler.png'},
        name = 'Leafscuttler',
        price = 750,
        mount = 870,
        id = 93,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nYou are fascinated by insectoid creatures and can picture yourself riding one during combat or just for travelling? The Mouldpincer will carry you through the Tibian wilderness with ease.',
        icons = {'Mouldpincer.png'},
        name = 'Mouldpincer',
        price = 750,
        mount = 868,
        id = 91,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWhen the nights are getting longer and freezing wind brings driving snow into the land, snowmen rise and shine on every corner. Lately, a peaceful, arcane creature has found shelter in one of them and used its magical power to call the Muffled Snowman into being. Wrap yourself up well and warmly and jump on the back of your new frosty companion.',
        icons = {'Muffled_Snowman.png'},
        name = 'Muffled Snowman',
        price = 900,
        mount = 1168,
        id = 136,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more of an imp than an angel, you may prefer riding out on a Nightdweller to scare fellow Tibians on their festive strolls. Its devilish mask, claw-like hands and sharp hooves makes it the perfect companion for any daring adventurer who likes to stand out.',
        icons = {'Nightdweller.png'},
        name = 'Nightdweller',
        price = 870,
        mount = 849,
        id = 88,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTo the keen observer, the crocovile is clearly a relative of the crocodile, albeit their look suggests an even more aggressive nature. While it is true that the power of its massive and muscular body can not only crush enemies dead but also break through any gate like a battering ram, a crocovile is, above all, a steadfast companion showing unwavering loyalty to its owner.',
        icons = {'Nightmarish_Crocovile.png'},
        name = 'Nightmarish Crocovile',
        price = 750,
        mount = 1185,
        id = 143,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more of an imp than an angel, you may prefer riding out on a Frostflare to scare fellow Tibians on their festive strolls. Its devilish mask, claw-like hands and sharp hooves makes it the perfect companion for any daring adventurer who likes to stand out.',
        icons = {'Frostflare.png'},
        name = 'Frostflare',
        price = 870,
        mount = 850,
        id = 89,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Slagsnare has external characteristics of different breeds. It is assumed that his brain is also composed of many different species, which makes it completely unpredictable. Only few have managed to approach this creature unharmed and only the best could tame it.',
        icons = {'Slagsnare.png'},
        name = 'Slagsnare',
        price = 780,
        mount = 761,
        id = 84,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Nightstinger has external characteristics of different breeds. It is assumed that his brain is also composed of many different species, which makes it completely unpredictable. Only few have managed to approach this creature unharmed and only the best could tame it.',
        icons = {'Nightstinger.png'},
        name = 'Nightstinger',
        price = 780,
        mount = 762,
        id = 85,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Razorcreep has external characteristics of different breeds. It is assumed that his brain is also composed of many different species, which makes it completely unpredictable. Only few have managed to approach this creature unharmed and only the best could tame it.',
        icons = {'Razorcreep.png'},
        name = 'Razorcreep',
        price = 780,
        mount = 763,
        id = 86,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nGet yourself a mighty travelling companion with broad shoulders and a gentle heart. Gorongra is a physically imposing creature that is much more peaceful than its relatives, Tiquanda\'s wild kongras, and will carry you safely wherever you ask it to go.',
        icons = {'Gorongra.png'},
        name = 'Gorongra',
        price = 720,
        mount = 738,
        id = 81,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nGet yourself a mighty travelling companion with broad shoulders and a gentle heart. Noctungra is a physically imposing creature that is much more peaceful than its relatives, Tiquanda\'s wild kongras, and will carry you safely wherever you ask it to go.',
        icons = {'Noctungra.png'},
        name = 'Noctungra',
        price = 720,
        mount = 739,
        id = 82,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nGet yourself a mighty travelling companion with broad shoulders and a gentle heart. Silverneck is a physically imposing creature that is much more peaceful than its relatives, Tiquanda\'s wild kongras, and will carry you safely wherever you ask it to go.',
        icons = {'Silverneck.png'},
        name = 'Silverneck',
        price = 720,
        mount = 740,
        id = 83,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf the Sea Devil moves its fins, it generates enough air pressure that it can even float over land. Its numerous eyes allow it to quickly detect dangers even in confusing situations and eliminate them with one powerful bite. If you watch your fingers, you are going to be good friends.',
        icons = {'Sea_Devil.png'},
        name = 'Sea Devil',
        price = 570,
        mount = 734,
        id = 78,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf the Plumfish moves its fins, it generates enough air pressure that it can even float over land. Its numerous eyes allow it to quickly detect dangers even in confusing situations and eliminate them with one powerful bite. If you watch your fingers, you are going to be good friends.',
        icons = {'Plumfish.png'},
        name = 'Plumfish',
        price = 570,
        mount = 736,
        id = 80,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRumour has it that many years ago elder witches had gathered to hold a magical feast high up in the mountains. They had crossbred Flitterkatzen to easily conquer rocky canyons and deep valleys. Nobody knows what happened on their way up but only the mount has been seen ever since.',
        icons = {'Flitterkatzen.png'},
        name = 'Flitterkatzen',
        price = 870,
        mount = 726,
        id = 75,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWaccoons are cuddly creatures that love nothing more than to be petted and snuggled! Share a hug, ruffle the fur of the Ringtail Waccoon and scratch it behind its ears to make it happy.',
        icons = {'Ringtail_Waccoon.png'},
        name = 'Ringtail Waccoon',
        price = 750,
        mount = 691,
        id = 68,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTo the keen observer, the crocovile is clearly a relative of the crocodile, albeit their look suggests an even more aggressive nature. While it is true that the power of its massive and muscular body can not only crush enemies dead but also break through any gate like a battering ram, a crocovile is, above all, a steadfast companion showing unwavering loyalty to its owner.',
        icons = {'River_Crocovile.png'},
        name = 'River Crocovile',
        price = 750,
        mount = 1183,
        id = 141,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nFor centuries, humans and monsters have dumped their garbage in the swamps around Venore. The combination of old, rusty weapons, stale mana and broken runes have turned some of the swamp dwellers into gigantic frogs. Benefit from those mutations and make the Sanguine Frog a faithful mount for your adventures even beyond the bounds of the swamp.',
        icons = {'Sanguine_Frog.png'},
        name = 'Sanguine Frog',
        price = 690,
        mount = 1053,
        id = 121,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWaccoons are cuddly creatures that love nothing more than to be petted and snuggled! Share a hug, ruffle the fur of the Night Waccoon and scratch it behind its ears to make it happy.',
        icons = {'NightWaccoon.png'},
        name = 'Night Waccoon',
        price = 750,
        mount = 692,
        id = 69,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Flying Divan is the perfect mount for those who are too busy to take care of an animal mount or simply like to travel on a beautiful, magic hand-woven carpet.',
        icons = {'Flying_Divan.png'},
        name = 'Flying Divan',
        price = 900,
        mount = 688,
        id = 65,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Magic Carpet is the perfect mount for those who are too busy to take care of an animal mount or simply like to travel on a beautiful, magic hand-woven carpet.',
        icons = {'Magic_Carpet.png'},
        name = 'Magic Carpet',
        price = 900,
        mount = 689,
        id = 66,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Magic Carpet is the perfect mount for those who are too busy to take cFor centuries, humans and monsters have dumped their garbage in the swamps around Venore. The combination of old, rusty weapons, stale mana and broken runes have turned some of the swamp dwellers into gigantic frogs. Benefit from those mutations and make the Marsh Toad a faithful mount for your adventures even beyond the bounds of the swamp.',
        icons = {'Marsh_Toad.png'},
        name = 'Marsh Toad',
        price = 690,
        mount = 1052,
        id = 120,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Floating Kashmir is the perfect mount for those who are too busy to take care of an animal mount or simply like to travel on a beautiful, magic hand-woven carpet.',
        icons = {'Floating_Kashmir.png'},
        name = 'Floating Kashmir',
        price = 900,
        mount = 690,
        id = 67,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nTreat your character to a new travelling companion with a gentle nature and an impressive antler: The noble Shadow Hart will carry you through the deepest snow.',
        icons = {'Shadow_Hart.png'},
        name = 'Shadow Hart',
        price = 660,
        mount = 685,
        id = 72,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWith its thick, shaggy hair, the Tundra Rambler will keep you warm even in the chilly climate of the Ice Islands. Due to its calm and peaceful nature, it is not letting itself getting worked up easily.',
        icons = {'Tundra_Rambler.png'},
        name = 'Tundra Rambler',
        price = 750,
        mount = 672,
        id = 62,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWith its thick, shaggy hair, the Highland Yak will keep you warm even in the chilly climate of the Ice Islands. Due to its calm and peaceful nature, it is not letting itself getting worked up easily.',
        icons = {'Highland_Yak.png'},
        name = 'Highland Yak',
        price = 750,
        mount = 673,
        id = 63,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nWith its thick, shaggy hair, the Glacier Vagabond will keep you warm even in the chilly climate of the Ice Islands. Due to its calm and peaceful nature, it is not letting itself getting worked up easily.',
        icons = {'Glacier_Vagabond.png'},
        name = 'Glacier Vagabond',
        price = 750,
        mount = 674,
        id = 64,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more interested in the achievements of science, you may enjoy a ride on the Golden Dragonfly, one of the new insect-like flying machines. Even if you do not move around, the wings of these unusual vehicles are always in motion.',
        icons = {'Golden_Dragonfly.png'},
        name = 'Golden Dragonfly',
        price = 600,
        mount = 669,
        id = 59,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIf you are more interested in the achievements of science, you may enjoy a ride on the Steel Bee, one of the new insect-like flying machines. Even if you do not move around, the wings of these unusual vehicles are always in motion.',
        icons = {'Steel_Bee.png'},
        name = 'Steel Bee',
        price = 600,
        mount = 670,
        id = 60,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce captured and held captive by a mad hunter, the Hailstorm Fury is the result of sick experiments. Fed only with demon dust and concentrated demonic blood it had to endure a dreadful transformation. The demonic blood that is now running through its veins, however, provides it with incredible strength and endurance.',
        icons = {'Hailstorm_Fury.png'},
        name = 'Hailtorm Fury',
        price = 780,
        mount = 648,
        id = 55,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Siegebreaker is out searching for the best bamboo in Tibia. Its heavy armour allows it to visit even the most dangerous places. Treat it nicely with its favourite food from time to time and it will become a loyal partner.',
        icons = {'Siegebreaker.png'},
        name = 'Siegebreaker',
        price = 690,
        mount = 649,
        id = 56,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Poisonbane is out searching for the best bamboo in Tibia. Its heavy armour allows it to visit even the most dangerous places. Treat it nicely with its favourite food from time to time and it will become a loyal partner.',
        icons = {'Poisonbane.png'},
        name = 'Poisonbane',
        price = 690,
        mount = 650,
        id = 57,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce a majestic and proud warhorse, the Nethersteed has fallen in a horrible battle many years ago. Driven by agony and pain, its spirit once again took possession of its rotten corpse to avenge its death. Stronger than ever, it seeks a master to join the battlefield, aiming for nothing but death and destruction.',
        icons = {'Nethersteed.png'},
        name = 'Nethersteed',
        price = 900,
        mount = 629,
        id = 50,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce a majestic and proud warhorse, the Tempest has fallen in a horrible battle many years ago. Driven by agony and pain, its spirit once again took possession of its rotten corpse to avenge its death. Stronger than ever, it seeks a master to join the battlefield, aiming for nothing but death and destruction.',
        icons = {'Tempest.png'},
        name = 'Tempest',
        price = 900,
        mount = 630,
        id = 51,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce a majestic and proud warhorse, the Flamesteed has fallen in a horrible battle many years ago. Driven by agony and pain, its spirit once again took possession of its rotten corpse to avenge its death. Stronger than ever, it seeks a master to join the battlefield, aiming for nothing but death and destruction.',
        icons = {'Flamesteed.png'},
        name = 'Flamesteed',
        price = 900,
        mount = 626,
        id = 47,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Tombstinger is a scorpion that has surpassed the natural boundaries of its own kind. Way bigger, stronger and faster than ordinary scorpions, it makes a perfect companion for fearless heroes and explorers. Just be careful of his poisonous sting when you mount it.',
        icons = {'Tombstinger.png'},
        name = 'Tombstinger',
        price = 600,
        mount = 546,
        id = 36,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nFor centuries, humans and monsters have dumped their garbage in the swamps around Venore. The combination of old, rusty weapons, stale mana and broken runes have turned some of the swamp dwellers into gigantic frogs. Benefit from those mutations and make the Toxic Toad a faithful mount for your adventures even beyond the bounds of the swamp.',
        icons = {'Toxic_Toad.png'},
        name = 'Toxic Toad',
        price = 690,
        mount = 1054,
        id = 122,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThe Jade Pincer is a scorpion that has surpassed the natural boundaries of its own kind. Way bigger, stronger and faster than ordinary scorpions, it makes a perfect companion for fearless heroes and explorers. Just be careful of his poisonous sting when you mount it.',
        icons = {'Jade_Pincer.png'},
        name = 'Jade Pincer',
        price = 600,
        mount = 628,
        id = 49,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIts roaring is piercing marrow and bone and can be heard over ten miles away. The Jade Lion is the undisputed ruler of its territory and no one messes with this animal. Show no fear and prove yourself worthy of its trust and you will get yourself a valuable companion for your adventures.',
        icons = {'Jade_Lion.png'},
        name = 'Jade Lion',
        price = 450,
        mount = 627,
        id = 48,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nRumour has it that many years ago elder witches had gathered to hold a magical feast high up in the mountains. They had crossbred Venompaw to easily conquer rocky canyons and deep valleys. Nobody knows what happened on their way up but only the mount has been seen ever since.',
        icons = {'Venompaw.png'},
        name = 'Venompaw',
        price = 870,
        mount = 727,
        id = 76,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nIts roaring is piercing marrow and bone and can be heard over ten miles away. The Winter King is the undisputed ruler of its territory and no one messes with this animal. Show no fear and prove yourself worthy of its trust and you will get yourself a valuable companion for your adventures.',
        icons = {'Winter_King.png'},
        name = 'Winter King',
        price = 450,
        mount = 631,
        id = 52,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce captured and held captive by a mad hunter, the Woodland Prince is the result of sick experiments. Fed only with demon dust and concentrated demonic blood it had to endure a dreadful transformation. The demonic blood that is now running through its veins, however, provides it with incredible strength and endurance.',
        icons = {'Wolpertinger.png'},
        name = 'Wolpertinger',
        price = 870,
        mount = 907,
        id = 105,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nOnce captured and held captive by a mad hunter, the Woodland Prince is the result of sick experiments. Fed only with demon dust and concentrated demonic blood it had to endure a dreadful transformation. The demonic blood that is now running through its veins, however, provides it with incredible strength and endurance.',
        icons = {'Woodland_Prince.png'},
        name = 'Woodland Prince',
        price = 780,
        mount = 647,
        id = 54,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nBadgers have been a staple of the Tibian fauna for a long time, and finally some daring souls have braved the challenge to tame some exceptional specimens - and succeeded! While the common badger you can encounter during your travels might seem like a rather unassuming creature, the Battle Badger, the Ether Badger, and the Zaoan Badger are fierce and mighty beasts, which are at your beck and call.',
        icons = {'Zaoan_Badger.png'},
        name = 'Zaoan Badger',
        price = 690,
        mount = 1249,
        id = 149,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThese creatures are Floating Savants whose mind has been warped and bent to focus their extraordinary mental capabilities on one single goal: to do their master\'s bidding. Instead of being filled with an endless pursuit of knowledge, their live is now one of continuous thralldom and serfhood. The Floating Sage, the Floating Scholar and the Floating Augur are at your disposal.',
        icons = {'Floating_Augur.png'},
        name = 'Floating Augur',
        price = 870,
        mount = 1266,
        id = 155,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThese creatures are Floating Savants whose mind has been warped and bent to focus their extraordinary mental capabilities on one single goal: to do their master\'s bidding. Instead of being filled with an endless pursuit of knowledge, their live is now one of continuous thralldom and serfhood. The Floating Sage, the Floating Scholar and the Floating Augur are at your disposal.',
        icons = {'Floating_Sage.png'},
        name = 'Floating Sage',
        price = 870,
        mount = 1264,
        id = 153,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nThese creatures are Floating Savants whose mind has been warped and bent to focus their extraordinary mental capabilities on one single goal: to do their master\'s bidding. Instead of being filled with an endless pursuit of knowledge, their live is now one of continuous thralldom and serfhood. The Floating Sage, the Floating Scholar and the Floating Augur are at your disposal.',
        icons = {'Floating_Scholar.png'},
        name = 'Floating Scholar',
        price = 870,
        mount = 1265,
        id = 154,
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }},
    state = GameStore.States.STATE_NONE
}, {
    -- Base outfit has addon = 0 or no defined addon. By default addon is set to 0.
    description = 'Buy your character one more of the classy outfits offered here.',
    icons = {'Category_Outfits.png'},
    name = 'Outfits',
    offers = {{
        icons = {'Outfit_Arena_Champion_Male_Addon_3.png', 'Outfit_Arena_Champion_Female_Addon_3.png'},
        name = 'Full Arena Champion Outfit',
        price = 870,
        sexId = {
            female = 885,
            male = 884
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nFight your bloody battles in the arena and become a darling of the crowd. Once you have made it to the top and everyone is cheering your name, the fashionable outfit of an Arena Champion will show the world what you are made of.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Beastmaster_Male_Addon_3.png', 'Outfit_Beastmaster_Female_Addon_3.png'},
        name = 'Full Beastmaster Outfit',
        price = 870,
        sexId = {
            female = 636,
            male = 637
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nDo you have enough authority to make wild animals subservient to you? Become a Beastmaster and surround yourself with fearsome companions. When your beasts bare their teeth, your enemies will turn tails and run.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Breezy_Garb_Male_Addon_3.png', 'Outfit_Breezy_Garb_Female_Addon_3.png'},
        name = 'Full Breezy Garb Outfit',
        price = 600,
        sexId = {
            female = 1246,
            male = 1245
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nEven the most eager adventurers and toughest warriors need some time to rest and recharge. Enjoy tranquility and peace as you picnic in good company at one of your favourite places in Tibia. Put on your Breezy Garb outfit, grab your walking stick, a basket filled with tasty snacks and then head out into nature!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Ceremonial_Garb_Male_Addon_3.png', 'Outfit_Ceremonial_Garb_Female_Addon_3.png'},
        name = 'Full Ceremonial Garb Outfit',
        price = 750,
        sexId = {
            female = 694,
            male = 695
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nIf you want to make a great entrance at a Tibian costume party, the Ceremonial Garb is certainly a good choice. With a drum over your shoulder and adorned with feathers you are perfectly dressed to lead a carnival parade through the streets of Thais.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Champion_Male_Addon_3.png', 'Outfit_Champion_Female_Addon_3.png'},
        name = 'Full Champion Outfit',
        price = 570,
        sexId = {
            female = 632,
            male = 633
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nProtect your body with heavy armour plates and spiky bones to teach your enemies the meaning of fear! The Champion outfit perfectly suits battle-hardened warriors who rely on their trusty sword and shield.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Chaos_Acolyte_Male_Addon_3.png', 'Outfit_Chaos_Acolyte_Female_Addon_3.png'},
        name = 'Full Chaos Acolyte Outfit',
        price = 900,
        sexId = {
            female = 664,
            male = 665
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYou have always felt like the cat among the pigeons and have a fable for dark magic? The Chaos Acolyte outfit is a perfect way to express your inner nature. Show your commitment for the higher cause and wreak havoc on your enemies in this unique outfit.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Conjurer_Male_Addon_3.png', 'Outfit_Conjurer_Female_Addon_3.png'},
        name = 'Full Conjurer Outfit',
        price = 750,
        sexId = {
            female = 635,
            male = 634
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYou recently graduated from the Magic Academy and want to bring your knowledge to good use? Congratulations, you are now an honourable disciple of magic! Open up a bottle of well-aged mana and treat yourself with the fashionable Conjurer outfit.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Death_Herald_Male_Addon_3.png', 'Outfit_Death_Herald_Female_Addon_3.png'},
        name = 'Full Death Herald Outfit',
        price = 600,
        sexId = {
            female = 666,
            male = 667
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYDeath and decay are your ever-present companions? Your enemies are dropping like flies and your path is covered with their bodies? However, as decency demands, you want to at least give them a proper funeral? Then the Death Herald is just the right outfit for you.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Entrepreneur_Male_Addon_3.png', 'Outfit_Entrepreneur_Female_Addon_3.png'},
        name = 'Full Entrepreneur Outfit',
        price = 750,
        sexId = {
            female = 471,
            male = 472
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nSlaughter through hordes of monsters during your early morning hunt and kiss the hand of Queen Eloise later on at the evening reception in her historical residence. With the Entrepreneur outfit you will cut a fine figure on every occasion.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Evoker_Male_Addon_3.png', 'Outfit_Evoker_Female_Addon_3.png'},
        name = 'Full Evoker Outfit',
        price = 840,
        sexId = {
            female = 724,
            male = 725
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nDance around flickering fires in the Evoker outfit while singing unholy chants to praise witchcraft and wizardry. Your faithful bat will always be by your side.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Grove_Keeper_Male_Addon_3.png', 'Outfit_Grove_Keeper_Female_Addon_3.png'},
        name = 'Full Groove Keeper Outfit',
        price = 870,
        sexId = {
            female = 909,
            male = 908
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nFeeling the springy grass under your feet and inhaling the spicy air of the forest is pure satisfaction for your soul? Every animal is your friend and you caringly look after trees and plants all the time? Then it is time to become one with nature: Become a Grove Keeper!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Guidon_Bearer_Male_Addon_3.png', 'Outfit_Guidon_Bearer_Female_Addon_3.png'},
        name = 'Full Guidon Bearer Outfit',
        price = 870,
        sexId = {
            female = 1187,
            male = 1186
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nCarrying the guidon of a unit, always marching in front, is not only an honour but also comes with great responsibility. Guidon bearers wield great power, they lead where others follow and keep the spirits of the troops up as they wave their flag against the golden suns of Tibia.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Herbalist_Male_Addon_3.png', 'Outfit_Herbalist_Female_Addon_3.png'},
        name = 'Full Herbalist Outfit',
        price = 750,
        sexId = {
            female = 1020,
            male = 1021
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe Herbalist outfit is the perfect outfit for all herbs collectors. Those of you who are aware that you do not necessarily have to reach into the mouth of a hydra to get a hydra tongue and those who know exactly where to get blood- and shadow-herbs will find a matching outfit for their daily hobby. Show the world your affinity for herbs and impress your friends with your knowledge of medicine and potions.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Herder_Male_Addon_3.png', 'Outfit_Herder_Female_Addon_3.png'},
        name = 'Full Herder Outfit',
        price = 750,
        sexId = {
            female = 1280,
            male = 1279
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe Herder is one with nature, being outside all day, watching carefully over his flock. If you like to spend time on picturesque meadows and are always looking for greener pastures, then this outfit is for you.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Lupine_Warden_Male_Addon_3.png', 'Outfit_Lupine_Warden_Female_Addon_3.png'},
        name = 'Full Lupine Warden Outfit',
        price = 840,
        sexId = {
            female = 900,
            male = 899
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nDo you feel the adrenaline rushing through your veins when the sun goes down and a full moon lightens the night? Do you have the urge to hunt down your target no matter what? Unleash the beast inside of you and lead your friends to battle with the Lupine Warden outfit!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Mercenary_Male_Addon_3.png', 'Outfit_Mercenary_Female_Addon_3.png'},
        name = 'Full Mercenary Outfit',
        price = 870,
        sexId = {
            female = 1057,
            male = 1056
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe Mercenary carries a powerful, razor-sharp axe on his shoulders that effortlessly cuts through any armour and bone. You should better tell your friends to keep a safe distance, since heads will roll over the blood-soaked battleground after a powerful swing of yours.\nConsidering the sheer size of this axe, it might even be possible to chop onions without shedding a tear.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Owl_Keeper_Male_Addon_3.png', 'Outfit_Owl_Keeper_Female_Addon_3.png'},
        name = 'Full Owl Keeper Outfit',
        price = 600,
        sexId = {
            female = 1174,
            male = 1173
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nOwl Keepers are often referred to as spirits walking through the forest at night, mere shadows during the day. They are also said to be shamans, protecting the flora and fauna of the Tibian lands. You often see them wearing a stag\'s antlers on their head and in the company of an owl, for they are as wise and mysterious as these intriguing creatures.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Pharaoh_Male_Addon_3.png', 'Outfit_Pharaoh_Female_Addon_3.png'},
        name = 'Full Pharaoh Outfit',
        price = 750,
        sexId = {
            female = 956,
            male = 955
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYou know how to read hieroglyphs? You admire the exceptional architectural abilities and the unsolved mysteries of an ancient high culture? Next time you pay a visit to your friends, tell them to prepare a bathtub full of milk and honey for you because a Pharaoh is now walking through the streets of Ankrahmun!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Philosopher_Male_Addon_3.png', 'Outfit_Philosopher_Female_Addon_3.png'},
        name = 'Full Philosopher Outfit',
        price = 750,
        sexId = {
            female = 874,
            male = 873
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nDo you feel the urge to tell people what is really going on in the world? Do you know all answers to the important questions of life? Are you a true philosopher? Then dress like one to showcase the latest fashion for all wise theorists.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Pumpkin_Mummy_Male_Addon_3.png', 'Outfit_Pumpkin_Mummy_Female_Addon_3.png'},
        name = 'Full Pumpkin Mummy Outfit',
        price = 870,
        sexId = {
            female = 1128,
            male = 1127
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nIf you cannot decide whether to wrap yourself up as a mummy or flaunt an enormous pumpkin head for your next hunting party, why not combine both? The Pumpkin Mummy outfit is the perfect costume for scary nights and spooky days.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Puppeteer_Male_Addon_3.png', 'Outfit_Puppeteer_Female_Addon_3.png'},
        name = 'Full Puppeteer Outfit',
        price = 870,
        sexId = {
            female = 696,
            male = 697
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nAre you a fan of puppetry? You like to travel the world together with one or two little acting fellows? Or are you simply the one who likes to pull the strings? Then the Puppeteer outfit is the right choice for you.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Ranger_Male_Addon_3.png', 'Outfit_Ranger_Female_Addon_3.png'},
        name = 'Full Ranger Outfit',
        price = 750,
        sexId = {
            female = 683,
            male = 684
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nMost of the day, the Ranger is looking over his forest. He is taking care of all animals and plants and tries to keep everything in balance. Intruders are greeted by a warning shot from his deadly longbow. It is the perfect outfit for Paladins who live in close touch with nature.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Royal_Pumpkin_Male_Addon_3.png', 'Outfit_Royal_Pumpkin_Female_Addon_3.png'},
        name = 'Full Royal Pumpkin Outfit',
        price = 840,
        sexId = {
            male = 760,
            female = 759
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe mutated pumpkin is too weak for your mighty weapons? Time to show that evil vegetable how to scare the living daylight out of people! Put on a scary looking pumpkin on your head and spread terror and fear amongst the Tibian population.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Sea_Dog_Male_Addon_3.png', 'Outfit_Sea_Dog_Female_Addon_3.png'},
        name = 'Full Sea Dog Outfit',
        price = 600,
        sexId = {
            female = 749,
            male = 750
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nAhoy mateys! Flaunt the swashbuckling Sea Dog outfit and strike a pose with your hook to impress both landlubbers and fellow pirates. Board your next ship in style!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Seaweaver_Male_Addon_3.png', 'Outfit_Seaweaver_Female_Addon_3.png'},
        name = 'Full Seaweaver Outfit',
        price = 570,
        sexId = {
            female = 732,
            male = 733
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe Seaweaver outfit is the perfect choice if you want to show the world that you are indeed a son or a daughter of the submarine kingdom. You can almost feel the salty taste and the rough wind of the sea when wearing it.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Siege_Master_Male_Addon_3.png', 'Outfit_Siege_Master_Female_Addon_3.png'},
        name = 'Full Siege Master Outfit',
        price = 600,
        sexId = {
            female = 1050,
            male = 1051
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nNeither thick stone walls nor heavily armoured gates can stop the Siege Master, who brings down hostile fortifications in the blink of an eye. Whenever he tenses his muscular arms to lift the powerful battering ram, his enemies\' knees begin to buckle. It is the perfect outfit for those who also stand for brute strength and immense destruction.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Sinister_Archer_Male_Addon_3.png', 'Outfit_Sinister_Archer_Female_Addon_3.png'},
        name = 'Full Sinister Archer Outfit',
        price = 120,
        sexId = {
            female = 1103,
            male = 1102
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nFrom an early age, the Sinister Archer has been fascinated by people\'s dark machinations and perversions. Sinister Archers claim that they advocate the good and that they only use their arrows to pierce the hearts of those who have committed many crimes and misdeeds. However, they are still viewed by the public with much suspicion due to their dubious appearance. To keep their identity secret, they often hide themselves behind a skull-like face guard that can easily withstand even axe and club blows.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Spirit_Caller_Male_Addon_3.png', 'Outfit_Spirit_Caller_Female_Addon_3.png'},
        name = 'Full Spirit Caller Outfit',
        price = 600,
        sexId = {
            female = 698,
            male = 699
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYou are in love with the deep soul of Mother Earth and prefer to walk in the shadows of her wooden children? Choose the Spirit Caller outfit to live in harmony with nature.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Sun_Priest_Male_Addon_3.png', 'Outfit_Sun_Priest_Female_Addon_3.png'},
        name = 'Full Sun Priest Outfit',
        price = 750,
        sexId = {
            female = 1024,
            male = 1023
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nDo you worship warm temperatures and are opposed to the thought of long and dark winter nights? Do you refuse to spend countless evenings in front of your chimney while ice-cold wind whistles through the cracks and niches of your house? It is time to stop freezing and to become an honourable Sun Priest! With this stylish outfit, you can finally show the world your unconditional dedication and commitment to the sun!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Trophy_Hunter_Male_Addon_3.png', 'Outfit_Trophy_Hunter_Female_Addon_3.png'},
        name = 'Full Trophy Hunter Outfit',
        price = 870,
        sexId = {
            female = 900,
            male = 899
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nYou spend hours in the woods in search of wild and rare animals? Countless stuffed skulls of deer, wolves and other creatures are decorating your walls? Now you have the chance to present your trophies in public. Become a Trophy Hunter and cover your shoulders with the finest bear skulls!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Winter_Warden_Male_Addon_3.png', 'Outfit_Winter_Warden_Female_Addon_3.png'},
        name = 'Full Winter Warden Outfit',
        price = 870,
        sexId = {
            female = 852,
            male = 853
        },
        addon = 3,
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\nThe warm and cosy cloak of the Winter Warden outfit will keep you warm in every situation. Best thing, it is not only comfortable but fashionable as well. You will be the envy of any snow queen or king, guaranteed!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Citizen_Male.png', 'Outfit_Retro_Citizen_Female.png'},
        name = 'Retro Citizen',
        price = 870,
        sexId = {
            female = 975,
            male = 974
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nDo you still remember your first stroll through the streets of Thais? For old times\' sake, walk the paths of Nostalgia as a Retro Citizen!',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Hunter_Male.png', 'Outfit_Retro_Hunter_Female.png'},
        name = 'Retro Hunter',
        price = 870,
        sexId = {
            female = 973,
            male = 972
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nWhenever you pick up your bow and spears, you walk down memory lane and think of your early days? Treat yourself with the fashionable Retro Hunter outfit and hunt some good old monsters from your childhood.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Knight_Male.png', 'Outfit_Retro_Knight_Female.png'},
        name = 'Retro Knight',
        price = 870,
        sexId = {
            female = 971,
            male = 970
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nWho needs a fancy looking sword with bling-bling and ornaments? Back in the days, we survived without such unnecessary accessories! Time to show those younkers what a Retro Knight is made of.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Mage_Male.png', 'Outfit_Retro_Mage_Female.png'},
        name = 'Retro Wizzard',
        price = 870,
        sexId = {
            female = 969,
            male = 968
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nDress up as a Retro Mage and you will always cut a fine figure on the battleground while eliminating your enemies with your magical powers the old-fashioned way.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Nobleman_Male.png', 'Outfit_Retro_Nobleman_Female.png'},
        name = 'Retro Noblewoman',
        price = 870,
        sexId = {
            female = 967,
            male = 966
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nKing Tibianus has invited you to a summer ball and you have nothing to wear for this special event? Do not worry, the Retro Noble(wo)man outfit makes you a real eye catcher on every festive occasion.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Summoner_Male.png', 'Outfit_Retro_Summoner_Female.png'},
        name = 'Retro Summoner',
        price = 870,
        sexId = {
            female = 965,
            male = 964
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nWhile the Retro Mage usually throws runes and mighty spells directly at the enemies, the Retro Summoner outfit might be the better choice for Tibians that prefer to send mighty summons to the battlefield to keep their enemies at distance.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Retro_Warrior_Male.png', 'Outfit_Retro_Warrior_Female.png'},
        name = 'Retro Warrior',
        price = 870,
        sexId = {
            female = 963,
            male = 962
        },
        description = '- only usable by purchasing character\n- colours can be changed using the Outfit dialog\n\nYou are fearless and strong as a behemoth but have problems finding the right outfit for your adventures? The Retro Warrior outfit is a must-have for all fashion-conscious old-school Tibians out there.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }},
    state = GameStore.States.STATE_NONE
}, -- House items
{
    description = 'Buy exceptional equipment to upgrade your Tibia House',
    icons = {'Category_HouseTools.png'},
    name = 'Houses',
    offers = { -- Decorations
    {
        count = 1,
        icons = {'Alchemistic_Bookstand.png'},
        name = 'Alchemistic Bookstand',
        price = 100,
        item = 32028,
        id = 32028,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Alchemistic_Cupboard.png'},
        name = 'Alchemistic Cupboard',
        price = 50,
        item = 32038,
        id = 32038,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Alchemistic_Scales.png'},
        name = 'Alchemistic Scales',
        price = 120,
        item = 32032,
        id = 32032,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'All-Seeing_Tapestry.png'},
        name = 'All-Seeing Tapestry',
        price = 60,
        item = 26106,
        id = 26106,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Anglerfish_Lamp_(Lit).png'},
        name = 'Anglerfish Lamp',
        price = 120,
        item = 32259,
        id = 32259,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Arrival_at_Thais_Painting.png'},
        name = 'Arrival The Thais Paint',
        price = 50,
        item = 32046,
        id = 32046,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Azure_Carpet.png'},
        name = 'Azure Carpet',
        price = 35,
        item = 26366,
        id = 26366,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Baby_Dragon.png'},
        name = 'Baby Dragon',
        price = 250,
        item = 26098,
        id = 26098,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Baby_Rotworm_(Store).png'},
        name = 'Baby Rotworm',
        price = 150,
        item = 32390,
        id = 32390,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Baby_Unicorn.png'},
        name = 'Baby Unicorn',
        price = 250,
        item = 36538,
        id = 36538,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Bamboo_Mat.png'},
        name = 'Bamboo Mat',
        price = 25,
        item = 26089,
        id = 26089,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Barrel_&_Anchor_Lamp.png'},
        name = 'Barrel & Anchor Lamp',
        price = 80,
        item = 36772,
        id = 36772,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Bath_Tub.png'},
        name = 'Bath Tub',
        price = 250,
        item = 29336,
        id = 29336,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Bellflower.png'},
        name = 'Bellflower',
        price = 50,
        item = 32396,
        id = 32396,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Bitter-Smack_Leaf.png'},
        name = 'Bitter-Smack Leaf',
        price = 50,
        item = 27893,
        id = 27893,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Blooming_Cactus.png'},
        name = 'Blooming Cactus',
        price = 50,
        item = 27892,
        id = 27892,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Blue_Round_Cushion.png'},
        name = 'Blue Round Cushion',
        price = 50,
        item = 36057,
        id = 36057,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Blue_Square_Cushion.png'},
        name = 'Blue Square Cushion',
        price = 50,
        item = 36054,
        id = 36054,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Brocade_Tapestry.png'},
        name = 'Brocade Tapestry',
        price = 50,
        item = 26381,
        id = 26381,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Carnivorous_Plant.png'},
        name = 'Carnivorous Plant',
        price = 50,
        item = 33417,
        id = 33417,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Cat_in_a_Basket.png'},
        name = 'Cat in a Basket',
        price = 150,
        item = 26107,
        id = 26107,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Chameleon_(Item).png'},
        name = 'Chamaleon',
        price = 250,
        item = 27889,
        id = 27889,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Chest_of_Abundance.png'},
        name = 'Chest of Abundance',
        price = 120,
        item = 33516,
        id = 33516,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Colourful_Carpet.png'},
        name = 'Colourful Carpet',
        price = 35,
        item = 27085,
        id = 27085,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Crested_Carpet.png'},
        name = 'Crested Carpet',
        price = 25,
        item = 29388,
        id = 29388,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Crimson_Carpet.png'},
        name = 'Crimson Carpet',
        price = 35,
        item = 26363,
        id = 26363,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Crystal_Lamp_(Store).png'},
        name = 'Crystal Lamp',
        price = 80,
        item = 36031,
        id = 36031,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Curly_Hortensis_Lamp_(Lit).png'},
        name = 'Curly Hortensis Lamp',
        price = 120,
        item = 36530,
        id = 36530,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Dark_Parquet.png'},
        name = 'Dark Parquet',
        price = 30,
        item = 26369,
        id = 26369,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Decorated_Carpet.png'},
        name = 'Decorated Carpet',
        price = 35,
        item = 29390,
        id = 29390,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Demon_(Pet).png'},
        name = 'Demon Pet',
        price = 250,
        item = 29409,
        id = 29409,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Demon_Skull.png'},
        name = 'Demon Skull',
        price = 50,
        item = 36047,
        id = 36047,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Diamond_Carpet.png'},
        name = 'Diamond Carpet',
        price = 25,
        item = 27088,
        id = 27088,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Dog_House.png'},
        name = 'Dog House',
        price = 150,
        item = 26353,
        id = 26353,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Dungeon_Scene_Painting.png'},
        name = 'Dungeon Scene Painting',
        price = 100,
        item = 32045,
        id = 32045,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Emerald_Carpet.png'},
        name = 'Emerald Carpet',
        price = 35,
        item = 26367,
        id = 26367,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Fennec.png'},
        name = 'Fennec',
        price = 150,
        item = 32394,
        id = 32394,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- to trigger an animation feed it with meat, ham, dragon ham, haunch of a boar, roasted meat or bug meat\n- can be fed once every 65 seconds\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferumbras_Bust.png'},
        name = 'Ferumbras Dust',
        price = 70,
        item = 32040,
        id = 32040,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner\n-house owner can use it to display a duplicate of an owned Ferumbras\' Hat on this bust - also works if the character has already turned Ferumbras\' hat in to earn the outfit addon',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferumbras_Portrait.png'},
        name = 'Ferumbras Portrait',
        price = 100,
        item = 32048,
        id = 32048,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Filled_Shoes.png'},
        name = 'Festive Filled Shoes',
        price = 50,
        item = 35021,
        id = 35021,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Fireplace.png'},
        name = 'Festive Fireplace',
        price = 180,
        item = 35027,
        id = 35027,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Pile_of_Presents.png'},
        name = 'Festive Pile of Presents',
        price = 50,
        item = 35039,
        id = 35039,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Pyramid.png'},
        name = 'Festive Pyramid',
        price = 120,
        item = 35042,
        id = 35042,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Rocking_Chair.png'},
        name = 'Festive Rocking Chair',
        price = 50,
        item = 35035,
        id = 35035,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Sack_of_Presents.png'},
        name = 'Festive Sack of Presents',
        price = 50,
        item = 35041,
        id = 35041,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Sleigh.png'},
        name = 'Festive Sleigh',
        price = 50,
        item = 35038,
        id = 35038,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Table.png'},
        name = 'Festive Table',
        price = 100,
        item = 35023,
        id = 35023,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Festive_Tree.png'},
        name = 'Festive Tree',
        price = 180,
        item = 35031,
        id = 35031,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Fish_in_a_Tank.png'},
        name = 'Fish Tank',
        price = 180,
        item = 26347,
        id = 26347,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Flowery_Carpet.png'},
        name = 'Flowery Carpet',
        price = 35,
        item = 27084,
        id = 27084,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Fluorescent_Fungi_(Lit).png'},
        name = 'Fluorescent Fungi',
        price = 60,
        item = 33491,
        id = 33491,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Forget-Me-Not.png'},
        name = 'Forget-Me-Not',
        price = 50,
        item = 32397,
        id = 32397,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Fur_Carpet.png'},
        name = 'Fur Carpet',
        price = 30,
        item = 27087,
        id = 27087,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Gloomy_Poisonous_Fungi_(Lit).png'},
        name = 'Gloomy Poisonous Fungi',
        price = 60,
        item = 33497,
        id = 33497,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Glowing_Sulphur_Fungi_(Lit).png'},
        name = 'Glowing Sulphur Fungi',
        price = 60,
        item = 33495,
        id = 33495,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Golden_Demon_Skull.png'},
        name = 'Golden Demon Skull',
        price = 80,
        item = 36046,
        id = 36046,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Golden Dragon Tapestry to decorate your home.',
        icons = {'Golden_Dragon_Tapestry.png'},
        name = 'Golden Dragon Tapestry',
        price = 70,
        item = 26379,
        id = 26379,
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Golden_Minotaur_Skull.png'},
        name = 'Golden Minotaur Skull',
        price = 100,
        item = 36044,
        id = 36044,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Green_Round_Cushion.png'},
        name = 'Green Round Cushion',
        price = 50,
        item = 36056,
        id = 36056,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Green_Square_Cushion.png'},
        name = 'Green Square Cushion',
        price = 50,
        item = 36053,
        id = 36053,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hamster_in_a_Wheel.png'},
        name = 'Hamster in a Wheel',
        price = 180,
        item = 26100,
        id = 26100,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Baby_Hedgehog.png'},
        name = 'Hedgehog',
        price = 150,
        item = 36515,
        id = 36515,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hrodmiran_Weapons_Rack.png'},
        name = 'Hrodmiran Weapons Rack',
        price = 90,
        item = 29317,
        id = 29317,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Idol_Lamp.png'},
        name = 'Idol Lamp',
        price = 80,
        item = 36049,
        id = 36049,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Incomprehensible_Riches.png'},
        name = 'Incomprehensible Riches',
        price = 90,
        item = 33515,
        id = 33515,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'King_Tibianus_Bust.png'},
        name = 'King Tibianus Bust',
        price = 50,
        item = 32049,
        id = 32049,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Light_of_Change.png'},
        name = 'Light of Change',
        price = 180,
        item = 32023,
        id = 32023,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Light_Parquet.png'},
        name = 'Light Parquet',
        price = 30,
        item = 26368,
        id = 26368,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Lit_Predator_Lamp.png'},
        name = 'Lit Predator Lamp',
        price = 60,
        item = 26092,
        id = 26092,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Lit_Protectress_Lamp.png'},
        name = 'Lit Protectress Lamp',
        price = 90,
        item = 26096,
        id = 26096,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Lit_Skull_Lamp.png'},
        name = 'Lit Skull Lamp',
        price = 90,
        item = 27103,
        id = 27103,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Little_Big_Flower_Lamp_(Lit).png'},
        name = 'Little Big Flower Lamp',
        price = 80,
        item = 36532,
        id = 36532,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Lordly_Tapestry.png'},
        name = 'Lordly Tapestry',
        price = 50,
        item = 26104,
        id = 26104,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Luminescent_Fungi_(Lit).png'},
        name = 'Luminescent Fungi',
        price = 60,
        item = 33493,
        id = 33493,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Marble_Floor_(Furniture).png'},
        name = 'Marble Floor',
        price = 30,
        item = 26376,
        id = 26376,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Menacing_Tapestry.png'},
        name = 'Menacing Tapestry',
        price = 70,
        item = 26105,
        id = 26105,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Mermaid_Figure_Head.png'},
        name = 'Mermaid Figure Head',
        price = 120,
        item = 26105,
        id = 26105,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Minotaur_Skull_(Store).png'},
        name = 'Minotaur Skull',
        price = 70,
        item = 36045,
        id = 36045,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Model_Ship_Lamp.png'},
        name = 'Model Ship Lamp',
        price = 80,
        item = 36777,
        id = 36777,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Monkey_(Store).png'},
        name = 'Monkey',
        price = 180,
        item = 36790,
        id = 36790,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Mystic_Carpet.png'},
        name = 'Mystic Carpet',
        price = 35,
        item = 29354,
        id = 29354,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Night_Sky_Carpet.png'},
        name = 'Night Sky Carpet',
        price = 25,
        item = 27090,
        id = 27090,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Painting_of_Tibiasula.png'},
        name = 'Painting of Tibiasula',
        price = 250,
        item = 33518,
        id = 33518,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Painting_of_Tibiasula.png'},
        name = 'Painting of Tibiasula',
        price = 250,
        item = 33519,
        id = 33519,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Parrot_(Item).png'},
        name = 'Parrot',
        price = 180,
        item = 27100,
        id = 27100,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Patterned_Carpet.png'},
        name = 'Patterned Carpet',
        price = 30,
        item = 27089,
        id = 27089,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pile_of_Alchemistic_Books.png'},
        name = 'Pile of Alchemistic Books',
        price = 120,
        item = 32036,
        id = 32036,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pink_Roses.png'},
        name = 'Pink Roses',
        price = 50,
        item = 27894,
        id = 27894,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pirate_Flag.png'},
        name = 'Pirate Flag',
        price = 50,
        item = 36780,
        id = 36780,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pirate_Ship_Ballista.png'},
        name = 'Pirate Ship Ballista',
        price = 120,
        item = 36768,
        id = 36768,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pirate_Skeleton_Cage.png'},
        name = 'Pirate Skeleton Cage',
        price = 120,
        item = 36782,
        id = 36782,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pirate_Treasure_Chest.png'},
        name = 'Pirate Treasure Chest',
        price = 120,
        item = 36771,
        id = 36771,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Pirate_Treasure_Map_(Store).png'},
        name = 'Pirate Treasure Map',
        price = 50,
        item = 36781,
        id = 36781,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Queen_Eloise_Bust.png'},
        name = 'Queen Eloise Bust',
        price = 50,
        item = 32043,
        id = 32043,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Red_Geranium.png'},
        name = 'Red Geranium',
        price = 50,
        item = 32398,
        id = 32398,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Red_Roses.png'},
        name = 'Red Roses',
        price = 50,
        item = 27895,
        id = 27895,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Exalted_Sarcophagus.png'},
        name = 'Sarcophagus',
        price = 120,
        item = 36518,
        id = 36518,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Shaggy_Carpet.png'},
        name = 'Shaggy Carpet',
        price = 30,
        item = 29352,
        id = 29352,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ship\'s_Wheel.png'},
        name = 'Ship\'s Wheel',
        price = 50,
        item = 36783,
        id = 36783,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Star Carpet to decorate your home.',
        icons = {'Rolled-up_Star_Carpet.png'},
        name = 'Star Carpet',
        price = 25,
        item = 27091,
        id = 27091,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Piled-up_Stone_Tiles.png'},
        name = 'Stone Tiles',
        price = 25,
        item = 29357,
        id = 29357,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Striped Carpet to decorate your home.',
        icons = {'Rolled-up_Striped_Carpet .png'},
        name = 'Striped Carpet',
        price = 30,
        item = 27086,
        id = 27086,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Stuffed_Bear_Display.png'},
        name = 'Stuffed Bear Display',
        price = 90,
        item = 33499,
        id = 33499,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Stuffed_Teddy_Display.png'},
        name = 'Stuffed Teddy Display',
        price = 50,
        item = 33501,
        id = 33501,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Sulphur_Blossom_Lamp_(Lit).png'},
        name = 'Sulphur Blossom Lamp',
        price = 80,
        item = 36558,
        id = 36558,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Sword Tapestry to decorate your home.',
        icons = {'Sword_Tapestry.png'},
        name = 'Sword Tapestry',
        price = 60,
        item = 26380,
        id = 26380,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Snake_(Pet).png'},
        name = 'Terrarium Snake',
        price = 180,
        item = 29407,
        id = 29407,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Spider_in_a_Terrarium.png'},
        name = 'Terrarium Spider',
        price = 180,
        item = 29314,
        id = 29314,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Tibia_Streets_Painting.png'},
        name = 'Tibia Streets Painting',
        price = 100,
        item = 32047,
        id = 32047,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Torch_of_Change.png'},
        name = 'Torch of Change',
        price = 120,
        item = 33175,
        id = 33175,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'VengothicLamp.png'},
        name = 'Vengothic Lamp',
        price = 180,
        item = 27886,
        id = 27886,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Venorean_Table_Clock.png'},
        name = 'Venorean Table Clock',
        price = 120,
        item = 29348,
        id = 29348,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Verdant_Carpet.png'},
        name = 'Verdant Carpet',
        price = 30,
        item = 29350,
        id = 29350,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Violet_Round_Cushion.png'},
        name = 'Violet Round Cushion',
        price = 50,
        item = 36055,
        id = 36055,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Violet_Square_Cushion.png'},
        name = 'Violet Square Cushion',
        price = 50,
        item = 36052,
        id = 36052,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Wheat_Carpet.png'},
        name = 'Wheat Carpet',
        price = 30,
        item = 29387,
        id = 29387,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_White_Fur_Carpet.png'},
        name = 'White Fur Carpet',
        price = 30,
        item = 26088,
        id = 26088,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'White_Shark_Trophy.png'},
        name = 'White Shark Trophy',
        price = 80,
        item = 36786,
        id = 36786,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Wooden_Planks.png'},
        name = 'Wooden Planks',
        price = 25,
        item = 29359,
        id = 29359,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rolled-up_Yalaharian_Carpet.png'},
        name = 'Yalaharian Carpet',
        price = 35,
        item = 26087,
        id = 26087,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Yellow Roses to decorate your home.',
        icons = {'Yellow_Roses.png'},
        name = 'Yellow Roses',
        price = 50,
        item = 27896,
        id = 27896,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, -- Furniture
    {
        icons = {'Alchemistic_Cabinet.png'},
        name = 'Alchemistic Cabinet',
        price = 100,
        item = 32020,
        id = 32020,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Alchemistic_Chair.png'},
        name = 'Alchemistic Chair',
        price = 50,
        item = 32018,
        id = 32018,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Alchemistic_Table.png'},
        name = 'Alchemistic Table',
        price = 80,
        item = 32021,
        id = 32021,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Comfy_Cabinet.png'},
        name = 'Comfy Cabinet',
        price = 100,
        item = 33513,
        id = 33513,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Comfy_Chair.png'},
        name = 'Comfy Chair',
        price = 70,
        item = 33505,
        id = 33505,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Comfy_Chest.png'},
        name = 'Comfy Chest',
        price = 60,
        item = 33509,
        id = 33509,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Comfy_Table.png'},
        name = 'Comfy Table',
        price = 60,
        item = 33507,
        id = 33507,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Dwarven_Stone_Cabinet.png'},
        name = 'Dwarven Stone Cabinet',
        price = 100,
        item = 36027,
        id = 36027,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Dwarven_Stone_Chair.png'},
        name = 'Dwarven Stone Chair',
        price = 50,
        item = 36020,
        id = 36020,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Dwarven_Stone_Chest.png'},
        name = 'Dwarven Stone Chest',
        price = 80,
        item = 36022,
        id = 36022,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Dwarven_Stone_Table.png'},
        name = 'Dwarven Stone Table',
        price = 50,
        item = 36026,
        id = 36026,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to open up some storage space\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferocious_Cabinet.png'},
        name = 'Ferocious Cabinet',
        price = 110,
        item = 26077,
        id = 26077,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferocious_Chair.png'},
        name = 'Ferocious Chair',
        price = 50,
        item = 26065,
        id = 26065,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferocious_Table.png'},
        name = 'Ferocious Table',
        price = 50,
        item = 26070,
        id = 26070,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferocious_Trunk.png'},
        name = 'Ferocious Trunk',
        price = 80,
        item = 26079,
        id = 26079,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hrodmiran_Chair.png'},
        name = 'Hrodmir Chair',
        price = 50,
        item = 36528,
        id = 36528,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hrodmiran_Chest.png'},
        name = 'Hrodmir Chest',
        price = 80,
        item = 36522,
        id = 36522,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hrodmiran_Cupboard.png'},
        name = 'Hrodmir Cupboard',
        price = 100,
        item = 36540,
        id = 36540,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Hrodmiran_Table.png'},
        name = 'Hrodmir Table',
        price = 50,
        item = 36514,
        id = 36514,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Magnificent_Cabinet.png'},
        name = 'Magnificent Cabinet',
        price = 100,
        item = 26075,
        id = 26075,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Magnificent_Chair.png'},
        name = 'Magnificent Chair',
        price = 60,
        item = 26061,
        id = 26061,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Magnificent_Table.png'},
        name = 'Magnificent Table',
        price = 60,
        item = 26074,
        id = 26074,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Magnificent_Trunk.png'},
        name = 'Magnificent Trunk',
        price = 70,
        item = 26083,
        id = 26083,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ornate_Cabinet.png'},
        name = 'Ornate Cabinet',
        price = 100,
        item = 29398,
        id = 29398,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ornate_Chair.png'},
        name = 'Ornate Chair',
        price = 50,
        item = 29394,
        id = 29394,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ornate_Chest.png'},
        name = 'Ornate Chest',
        price = 80,
        item = 29401,
        id = 29401,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ornate_Table.png'},
        name = 'Ornate Table',
        price = 50,
        item = 29397,
        id = 29397,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Round_Side_Table.png'},
        name = 'Round Side Table',
        price = 50,
        item = 36043,
        id = 36043,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rustic_Cabinet.png'},
        name = 'Rustic Cabinet',
        price = 100,
        item = 26356,
        id = 26356,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rustic_Chair.png'},
        name = 'Rustic Chair',
        price = 50,
        item = 26351,
        id = 26351,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rustic_Table.png'},
        name = 'Rustic Table',
        price = 50,
        item = 26354,
        id = 26354,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Rustic_Trunk.png'},
        name = 'Rustic Trunk',
        price = 80,
        item = 26358,
        id = 26358,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Skeletal_Cabinet.png'},
        name = 'Skeletal Cabinet',
        price = 100,
        item = 33415,
        id = 33415,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Skeletal_Chair.png'},
        name = 'Skeletal Chair',
        price = 50,
        item = 32260,
        id = 32260,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Skeletal_Chest.png'},
        name = 'Skeletal Chest',
        price = 80,
        item = 32266,
        id = 32266,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Skeletal_Table.png'},
        name = 'Skeletal Table',
        price = 50,
        item = 32264,
        id = 32264,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Square_Side_Table.png'},
        name = 'Square Side Table',
        price = 50,
        item = 29397,
        id = 29397,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Vengothic_Cabinet.png'},
        name = 'Vengothic Cabinet',
        price = 100,
        item = 27903,
        id = 27903,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Vengothic_Chair.png'},
        name = 'Vengothic Chair',
        price = 50,
        item = 27899,
        id = 27899,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Vengothic_Chest.png'},
        name = 'Vengothic Chest',
        price = 80,
        item = 27905,
        id = 27905,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Vengothic_Table.png'},
        name = 'Vengothic Table',
        price = 50,
        item = 27901,
        id = 27901,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Verdant_Cabinet.png'},
        name = 'Verdant Cabinet',
        price = 100,
        item = 29341,
        id = 29341,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Verdant_Chair.png'},
        name = 'Verdant Chair',
        price = 50,
        item = 29339,
        id = 29339,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Verdant_Table.png'},
        name = 'Verdant Table',
        price = 80,
        item = 29347,
        id = 29347,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Verdant_Trunk.png'},
        name = 'Verdant Trunk',
        price = 50,
        item = 29343,
        id = 29343,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Wooden_Bookcase.png'},
        name = 'Wooden Bookcase',
        price = 50,
        item = 36029,
        id = 36029,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, -- Upgrades
    {
        count = 1,
        icons = {'Reward_Shrine_(Item).png'},
        name = 'Daily Reward Shrine',
        price = 150,
        item = 29022,
        id = 29022,
        description = 'Pick up your daily reward comfortably in your own four walls!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- use it to open the reward wall\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Demon_Exercise_Dummy.png'},
        name = 'Demon Exercise Dummy',
        price = 900,
        item = 32145,
        id = 32145,
        description = 'Train your skills more effectively at home than in public on this expert exercise dummy!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- can only be used by one character at a time\n- use one of the exercise weapons on this dummy\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Ferumbras_Exercise_Dummy.png'},
        name = 'Ferumbras Exercise Dummy',
        price = 900,
        item = 32143,
        id = 32143,
        description = 'Train your skills more effectively at home than in public on this expert exercise dummy!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- can only be used by one character at a time\n- use one of the exercise weapons on this dummy\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Gilded_Imbuing_Shrine.png'},
        name = 'Gilded Imbuing Shrine',
        price = 200,
        item = 27851,
        id = 27851,
        description = 'Enhance your equipment comfortably in your own four walls!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- use it with an imbuable item to open the imbuing dialog\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Imbuing_Shrine.png'},
        name = 'Imbuing Shrine',
        price = 150,
        item = 27843,
        id = 27843,
        description = 'Enhance your equipment comfortably in your own four walls!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- use it with an imbuable item to open the imbuing dialog\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Royal Mailbox to decorate your home.',
        icons = {'Mailbox_(Furniture).png'},
        name = 'Mailbox',
        price = 150,
        item = 26055,
        id = 26055,
        description = 'Send your letters and parcels right from your own home!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Monk_Exercise_Dummy.png'},
        name = 'Monk Exercise Dummy',
        price = 900,
        item = 32147,
        id = 32147,
        description = 'Train your skills more effectively at home than in public on this expert exercise dummy!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- can only be used by one character at a time\n- use one of the exercise weapons on this dummy\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        description = 'Buy an incredible Ornate Mailbox to decorate your home.',
        icons = {'Ornate_Mailbox.png'},
        name = 'Ornate Mailbox',
        price = 200,
        item = 26057,
        id = 26057,
        description = 'Send your letters and parcels right from your own home!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        count = 1,
        icons = {'Shiny_Reward_Shrine.png'},
        name = 'Shiny Daily Reward Shrine',
        price = 200,
        item = 29024,
        id = 29024,
        description = 'Pick up your daily reward comfortably in your own four walls!\n- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can be used by all characters that have access to the house\n- use it to open the reward wall\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }},
    state = GameStore.States.STATE_NONE
}, -- Extras
{
    description = 'Buy an Extra Services.',
    icons = {'Category_Extras.png'},
    name = 'Extra',
    offers = { -- Extras Services
    {
        icons = {'Name_Change.png'},
        name = 'Name Change',
        price = 250,
        id = 65002,
        description = 'Tired of your current character name? Purchase a new one!\n\n\n- only usable by purchasing character\n- relog required after purchase to finalise the name change',
        type = GameStore.OfferTypes.OFFER_TYPE_NAMECHANGE
    }, {
        icons = {'Sex_Change.png'},
        name = 'Sex Change',
        price = 120,
        id = 65003,
        description = 'Turns your female character into a male one - or vice versa.\n\n\n- only usable by purchasing character\n- activated at purchase\n- you will keep all outfits you have purchased or earned in quest',
        type = GameStore.OfferTypes.OFFER_TYPE_SEXCHANGE
    }, -- Usefull Things
    {
        icons = {'Gold_Converter.png'},
        name = 'Gold Converter',
        item = 26378,
        id = 26378,
        count = 500,
        number = 1,
        price = 5,
        description = 'Changes either a stack of 100 gold pieces into 1 platinum coin, or a stack of 100 platinum coins into 1 crystal coin!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it on a stack of 100 to change it to the superior currency\n- usable 500 times a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Gold_Pouch.png'},
        name = 'Gold Pouch',
        item = 26377,
        id = 26377,
        count = 1,
        number = 1,
        price = 900,
        description = 'Carries as many gold, platinum or crystal coins as your capacity allows, however, no other items.\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- can only be purchased once\n- use it to open it\n- always placed on the first position of your Store inbox',
        type = GameStore.OfferTypes.OFFER_TYPE_POUCH
    }, {
        icons = {'Instant_Reward_Access.png'},
        name = 'Instant Reward Access',
        id = 65006,
        count = 30,
        number = 1,
        price = 100,
        description = 'No matter where you are in Tibia, claim your daily reward on the spot!\n\n\n- only usable by purchasing character\n- added to your reward wall\n- maximum amount that can be owned by character: 90',
        type = GameStore.OfferTypes.OFFER_TYPE_ITEM
    }, {
        icons = {'Magic_Gold_Converter.png'},
        name = 'Magic Gold Converter',
        item = 32109,
        id = 32109,
        count = 500,
        number = 1,
        price = 15,
        description = 'Changes automatically either a stack of 100 gold pieces into 1 platinum coin, or a stack of 100 platinum coins into 1 crystal coin!\n\n\n- only usable by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to activate or deactivate the automatic conversion\n- converts all stacks of 100 gold or platinum in the inventory whenever it is activated\n- deactivated upon purchase\n- usable for 500 conversions a piece',
        type = GameStore.OfferTypes.OFFER_TYPE_STACKABLE
    }, {
        icons = {'Permanent_Prey_Slot.png'},
        name = 'Permanent Prey Slot',
        id = 65007,
        price = 900,
        description = 'Get an additional prey slot to activate additional prey!\n\n- only usable by purchasing character\n- maximum amount that can be owned by character: 3\n- added directly to Prey dialog',
        type = GameStore.OfferTypes.OFFER_TYPE_PREYSLOT
    }, {
        icons = {'Prey_Bonus_Reroll.png'},
        name = '5x Prey Willcard',
        id = 65008,
        count = 5,
        price = 50,
        description = 'Use Prey Wildcards to reroll the bonus of an active prey, to lock your active prey or to select a prey of your choice.\n\n- only usable by purchasing character\n- added directly to Prey dialog\n- maximum amount that can be owned by character: 50',
        type = GameStore.OfferTypes.OFFER_TYPE_PREYBONUS
    }, {
        icons = {'Prey_Bonus_Reroll.png'},
        name = '20x Prey Willcard',
        id = 65009,
        count = 20,
        price = 200,
        description = 'Use Prey Wildcards to reroll the bonus of an active prey, to lock your active prey or to select a prey of your choice.\n\n- only usable by purchasing character\n- added directly to Prey dialog\n- maximum amount that can be owned by character: 50',
        type = GameStore.OfferTypes.OFFER_TYPE_PREYBONUS
    }, {
        icons = {'Temple_Teleport.png'},
        name = 'Temple Teleport',
        item = 29019,
        id = 29019,
        count = 1,
        price = 15,
        description = 'Teleports you instantly to your home temple.\n\n- only usable by purchasing character\n- use it to teleport you to your home temple\n- cannot be used while having a battle sign or a protection zone block\n- does not work in no-logout zones or close to a character\'s home temple',
        type = GameStore.OfferTypes.OFFER_TYPE_ITEM
    }, -- Boost
    {
        icons = {'XP_Boost.png'},
        name = 'XP Boost',
        price = 30,
        id = 65010,
        type = GameStore.OfferTypes.OFFER_TYPE_EXPBOOST,
        description = 'Purchase a boost that increases the experience points your character gains from hunting by 50%!\n\n\n- only usable by purchasing character\n- lasts for 1 hour hunting time\n- paused if stamina falls under 14 hours\n- can be purchased up to 5 times between 2 server saves\n- price increases with every purchase\n- cannot be purchased if an XP boost is already active'
    }},
    state = GameStore.States.STATE_NONE
}, -- Tournament
{
    icons = {'Category_Tournament.png'},
    name = 'Tournament',
    offers = { -- Tickets
    -- Exclusive Offers
    {
        icons = {'Cerberus_Champion.png'},
        name = 'Cerberus Champion',
        price = 1250,
        mount = 1209,
        id = 146,
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nA fierce and grim guardian of the underworld has risen to fight side by side with the bravest warriors in order to send evil creatures into the realm of the dead. The three headed Cerberus Champion is constantly baying for blood and using its sharp fangs it easily rips apart even the strongest armour and shield.',
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        icons = {'Jousting_Eagle.png'},
        name = 'Jousting Eagle',
        price = 1250,
        mount = 1208,
        id = 145,
        description = '- only usable by purchasing character\n- provides character with a speed boost\n\nHigh above the clouds far away from dry land, the training of giant eagles takes place. Only the cream of the crop is able to survive in such harsh environment long enough to call themselves Jousting Eagles while the weaklings find themselves at the bottom of the sea. The tough ones become noble and graceful mounts that are well known for their agility and endurance.',
        type = GameStore.OfferTypes.OFFER_TYPE_MOUNT
    }, {
        icons = {'Outfit_Lion_of_War_Male_Addon_3.png', 'Outfit_Lion_of_War_Female_Addon_3.png'},
        name = 'Full Lion of War Outfit',
        price = 1750,
        sexId = {
            female = 1207,
            male = 1206
        },
        addon = 3,
        description = '- usable by all characters of the account\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\n- The Lion of War has fought on countless battlefields and never lost once. Enemies tremble with fear when he batters his sword against his almighty shield. Realising that a Lion of War knows no mercy, his opponents often surrender before the battle even begins.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Veteran_Paladin_Male_Addon_3.png', 'Outfit_Veteran_Paladin_Female_Addon_3.png'},
        name = 'Full Veteran Paladin Outfit',
        price = 1750,
        sexId = {
            female = 1205,
            male = 1204
        },
        addon = 3,
        description = '- usable by all characters of the account\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\n- A Veteran Paladin has mastered the art of distance fighting. No matter how far away his prey may be, a marksman like the Veteran Paladin will always hit with extraordinary precision. No one can escape his keen hawk-eyed vision and even small stones become deadly weapons in his hands.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Outfit_Void_Master_Male_Addon_3.png', 'Outfit_Void_Master_Female_Addon_3.png'},
        name = 'Full Void Master Outfit',
        price = 1750,
        sexId = {
            female = 1203,
            male = 1202
        },
        addon = 3,
        description = '- usable by all characters of the account\n- colours can be changed using the Outfit dialog\n- includes basic outfit and 2 addons which can be selected individually\n\n- According to ancient rumours, the pulsating orb that the Void Master balances skilfully on the tip of his staff consists of powerful cosmic spheres. If you gaze too long into the infinite emptiness inside the orb, its powers will absorb your mind.',
        type = GameStore.OfferTypes.OFFER_TYPE_OUTFIT
    }, {
        icons = {'Cerberus_Champion_Puppy.png'},
        name = 'Cerberus Champion Puppy',
        price = 800,
        item = 36299,
        id = 36299,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Jousting_Eagle_Baby.png'},
        name = 'Jousting Eagle Baby',
        price = 800,
        item = 36297,
        id = 36297,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use it to trigger an animation\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Sublime_Tournament_Accolade.png'},
        name = 'Sublime Tournament Accolade',
        price = 500,
        item = 36307,
        id = 36307,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Sublime_Tournament_Carpet.png'},
        name = 'Sublime Tournament Carpet',
        price = 70,
        item = 36302,
        id = 36302,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use an unwrapped carpet to roll it out or up\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Tournament_Accolade.png'},
        name = 'Tournament Accolade',
        price = 500,
        item = 36305,
        id = 36305,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }, {
        icons = {'Tournament_Carpet.png'},
        name = 'Tournament Carpet',
        price = 70,
        item = 36301,
        id = 36301,
        count = 1,
        description = '- can only be unwrapped in a house owned by the purchasing character\n- comes in a box which can only be unwrapped by purchasing character\n- will be sent to your Store inbox and can only be stored there and in depot box\n- use an unwrapped carpet to roll it out or up\n- will be wrapped back and sent to inbox if the purchasing character is no longer the house owner',
        type = GameStore.OfferTypes.OFFER_TYPE_HOUSE
    }},
    state = GameStore.States.STATE_NONE
}}

-- Here we dynamically assign ids for offers that doesn't have one. These ids must be unique.
local runningId = 70000
for i = 1, #GameStore.Categories do
    local category = GameStore.Categories[i]
    if (category.offers) then
        for j = 1, #category.offers do
            local offer = category.offers[j]
            if (offer.id == nil) then
                offer.id = runningId
                runningId = runningId + 1
            end

            if (offer.type == nil) then
                offer.type = GameStore.OfferTypes.OFFER_TYPE_NONE
            end
        end
    end
end
