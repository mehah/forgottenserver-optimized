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

#ifndef FS_ENUMS_H_003445999FEE4A67BCECBE918B0124CE
#define FS_ENUMS_H_003445999FEE4A67BCECBE918B0124CE

enum RuleViolationType_t : uint8_t
{
    REPORT_TYPE_NAME = 0,
    REPORT_TYPE_STATEMENT = 1,
    REPORT_TYPE_BOT = 2
};

enum RuleViolationReasons_t : uint8_t
{
    REPORT_REASON_NAMEINAPPROPRIATE = 0,
    REPORT_REASON_NAMEPOORFORMATTED = 1,
    REPORT_REASON_NAMEADVERTISING = 2,
    REPORT_REASON_NAMEUNFITTING = 3,
    REPORT_REASON_NAMERULEVIOLATION = 4,
    REPORT_REASON_INSULTINGSTATEMENT = 5,
    REPORT_REASON_SPAMMING = 6,
    REPORT_REASON_ADVERTISINGSTATEMENT = 7,
    REPORT_REASON_UNFITTINGSTATEMENT = 8,
    REPORT_REASON_LANGUAGESTATEMENT = 9,
    REPORT_REASON_DISCLOSURE = 10,
    REPORT_REASON_RULEVIOLATION = 11,
    REPORT_REASON_STATEMENT_BUGABUSE = 12,
    REPORT_REASON_UNOFFICIALSOFTWARE = 13,
    REPORT_REASON_PRETENDING = 14,
    REPORT_REASON_HARASSINGOWNERS = 15,
    REPORT_REASON_FALSEINFO = 16,
    REPORT_REASON_ACCOUNTSHARING = 17,
    REPORT_REASON_STEALINGDATA = 18,
    REPORT_REASON_SERVICEATTACKING = 19,
    REPORT_REASON_SERVICEAGREEMENT = 20
};

enum BugReportType_t : uint8_t
{
    BUG_CATEGORY_MAP = 0,
    BUG_CATEGORY_TYPO = 1,
    BUG_CATEGORY_TECHNICAL = 2,
    BUG_CATEGORY_OTHER = 3
};

enum ThreadState
{
    THREAD_STATE_RUNNING,
    THREAD_STATE_CLOSING,
    THREAD_STATE_TERMINATED,
};

#if GAME_FEATURE_SESSIONEND > 0
enum SessionEndInformations : uint8_t
{
    // I'm guessing unknown types are ban/protocol error or something
    // but since there aren't any difference from logout should we care?
    SESSION_END_LOGOUT,
    SESSION_END_UNK2,
    SESSION_END_FORCECLOSE,
    SESSION_END_UNK3,
};
#endif

enum itemAttrTypes : uint32_t
{
    ITEM_ATTRIBUTE_NONE,

    ITEM_ATTRIBUTE_ACTIONID = 1 << 0,
    ITEM_ATTRIBUTE_UNIQUEID = 1 << 1,
    ITEM_ATTRIBUTE_DESCRIPTION = 1 << 2,
    ITEM_ATTRIBUTE_TEXT = 1 << 3,
    ITEM_ATTRIBUTE_DATE = 1 << 4,
    ITEM_ATTRIBUTE_WRITER = 1 << 5,
    ITEM_ATTRIBUTE_NAME = 1 << 6,
    ITEM_ATTRIBUTE_ARTICLE = 1 << 7,
    ITEM_ATTRIBUTE_PLURALNAME = 1 << 8,
    ITEM_ATTRIBUTE_WEIGHT = 1 << 9,
    ITEM_ATTRIBUTE_ATTACK = 1 << 10,
    ITEM_ATTRIBUTE_DEFENSE = 1 << 11,
    ITEM_ATTRIBUTE_EXTRADEFENSE = 1 << 12,
    ITEM_ATTRIBUTE_ARMOR = 1 << 13,
    ITEM_ATTRIBUTE_HITCHANCE = 1 << 14,
    ITEM_ATTRIBUTE_SHOOTRANGE = 1 << 15,
    ITEM_ATTRIBUTE_OWNER = 1 << 16,
    ITEM_ATTRIBUTE_DURATION = 1 << 17,
    ITEM_ATTRIBUTE_DECAYSTATE = 1 << 18,
    ITEM_ATTRIBUTE_CORPSEOWNER = 1 << 19,
    ITEM_ATTRIBUTE_CHARGES = 1 << 20,
    ITEM_ATTRIBUTE_FLUIDTYPE = 1 << 21,
    ITEM_ATTRIBUTE_DOORID = 1 << 22,
    ITEM_ATTRIBUTE_DURATION_TIMESTAMP = 1 << 23,

    ITEM_ATTRIBUTE_CUSTOM = 1U << 31
};

enum VipStatus_t : uint8_t
{
    VIPSTATUS_OFFLINE = 0,
    VIPSTATUS_ONLINE = 1,
    VIPSTATUS_PENDING = 2
};

enum MarketAction_t
{
    MARKETACTION_BUY = 0,
    MARKETACTION_SELL = 1,
};

enum MarketRequest_t
{
    MARKETREQUEST_OWN_OFFERS = 0xFFFE,
    MARKETREQUEST_OWN_HISTORY = 0xFFFF,
};

enum MarketOfferState_t
{
    OFFERSTATE_ACTIVE = 0,
    OFFERSTATE_CANCELLED = 1,
    OFFERSTATE_EXPIRED = 2,
    OFFERSTATE_ACCEPTED = 3,

    OFFERSTATE_ACCEPTEDEX = 255,
};

enum ChannelEvent_t : uint8_t
{
    CHANNELEVENT_JOIN = 0,
    CHANNELEVENT_LEAVE = 1,
    CHANNELEVENT_INVITE = 2,
    CHANNELEVENT_EXCLUDE = 3,
};

enum CreatureType_t : uint8_t
{
    CREATURETYPE_PLAYER = 0,
    CREATURETYPE_MONSTER = 1,
    CREATURETYPE_NPC = 2,
    CREATURETYPE_SUMMON_OWN = 3,
    CREATURETYPE_SUMMON_OTHERS = 4,
    CREATURETYPE_HIDDEN = 5,
};

enum OperatingSystem_t : uint8_t
{
    CLIENTOS_NONE = 0,

    CLIENTOS_LINUX = 1,
    CLIENTOS_WINDOWS = 2,
    CLIENTOS_FLASH = 3,

    CLIENTOS_NEW_LINUX = 4,
    CLIENTOS_NEW_WINDOWS = 5,
    CLIENTOS_NEW_MACOS = 6,

    CLIENTOS_OTCLIENT_LINUX = 10,
    CLIENTOS_OTCLIENT_WINDOWS = 11,
    CLIENTOS_OTCLIENT_MAC = 12,

    CLIENTOS_TFC_ANDROID = 100,
    CLIENTOS_TFC_IPHONEOS = 101,
    CLIENTOS_TFC_MACOSX = 102,
    CLIENTOS_TFC_WIZ = 103,
    CLIENTOS_TFC_PANDORA = 104,
    CLIENTOS_TFC_PSP = 105,
    CLIENTOS_TFC_WINDOWS = 106,
    CLIENTOS_TFC_WINDOWSRT = 107,
    CLIENTOS_TFC_HAIKU = 108,
    CLIENTOS_TFC_NACL = 109,
    CLIENTOS_TFC_EMSCRIPTEN = 110,
    CLIENTOS_TFC_RASPBERRYPI = 111,
    CLIENTOS_TFC_UNIX = 112,
};

enum SpellGroup_t : uint8_t
{
    SPELLGROUP_NONE = 0,
    SPELLGROUP_ATTACK = 1,
    SPELLGROUP_HEALING = 2,
    SPELLGROUP_SUPPORT = 3,
    SPELLGROUP_SPECIAL = 4,
};

enum SpellType_t : uint8_t
{
    SPELL_UNDEFINED = 0,
    SPELL_INSTANT = 1,
    SPELL_RUNE = 2,
};

enum AccountType_t : uint8_t
{
    ACCOUNT_TYPE_NORMAL = 1,
    ACCOUNT_TYPE_TUTOR = 2,
    ACCOUNT_TYPE_SENIORTUTOR = 3,
    ACCOUNT_TYPE_GAMEMASTER = 4,
    ACCOUNT_TYPE_GOD = 5
};

enum RaceType_t : uint8_t
{
    RACE_NONE,
    RACE_VENOM,
    RACE_BLOOD,
    RACE_UNDEAD,
    RACE_FIRE,
    RACE_ENERGY,
};

enum CombatType_t : uint16_t
{
    COMBAT_NONE = 0,

    COMBAT_PHYSICALDAMAGE = 1 << 0,
    COMBAT_ENERGYDAMAGE = 1 << 1,
    COMBAT_EARTHDAMAGE = 1 << 2,
    COMBAT_FIREDAMAGE = 1 << 3,
    COMBAT_UNDEFINEDDAMAGE = 1 << 4,
    COMBAT_LIFEDRAIN = 1 << 5,
    COMBAT_MANADRAIN = 1 << 6,
    COMBAT_HEALING = 1 << 7,
    COMBAT_DROWNDAMAGE = 1 << 8,
    COMBAT_ICEDAMAGE = 1 << 9,
    COMBAT_HOLYDAMAGE = 1 << 10,
    COMBAT_DEATHDAMAGE = 1 << 11,

    COMBAT_COUNT = 12
};

enum CombatParam_t
{
    COMBAT_PARAM_TYPE,
    COMBAT_PARAM_EFFECT,
    COMBAT_PARAM_DISTANCEEFFECT,
    COMBAT_PARAM_BLOCKSHIELD,
    COMBAT_PARAM_BLOCKARMOR,
    COMBAT_PARAM_TARGETCASTERORTOPMOST,
    COMBAT_PARAM_CREATEITEM,
    COMBAT_PARAM_AGGRESSIVE,
    COMBAT_PARAM_DISPEL,
    COMBAT_PARAM_USECHARGES,
};

enum CallBackParam_t
{
    CALLBACK_PARAM_LEVELMAGICVALUE,
    CALLBACK_PARAM_SKILLVALUE,
    CALLBACK_PARAM_TARGETTILE,
    CALLBACK_PARAM_TARGETCREATURE,
};

enum ConditionParam_t
{
    CONDITION_PARAM_OWNER = 1,
    CONDITION_PARAM_TICKS = 2,
    //CONDITION_PARAM_OUTFIT = 3,
    CONDITION_PARAM_HEALTHGAIN = 4,
    CONDITION_PARAM_HEALTHTICKS = 5,
    CONDITION_PARAM_MANAGAIN = 6,
    CONDITION_PARAM_MANATICKS = 7,
    CONDITION_PARAM_DELAYED = 8,
    CONDITION_PARAM_SPEED = 9,
    CONDITION_PARAM_LIGHT_LEVEL = 10,
    CONDITION_PARAM_LIGHT_COLOR = 11,
    CONDITION_PARAM_SOULGAIN = 12,
    CONDITION_PARAM_SOULTICKS = 13,
    CONDITION_PARAM_MINVALUE = 14,
    CONDITION_PARAM_MAXVALUE = 15,
    CONDITION_PARAM_STARTVALUE = 16,
    CONDITION_PARAM_TICKINTERVAL = 17,
    CONDITION_PARAM_FORCEUPDATE = 18,
    CONDITION_PARAM_SKILL_MELEE = 19,
    CONDITION_PARAM_SKILL_FIST = 20,
    CONDITION_PARAM_SKILL_CLUB = 21,
    CONDITION_PARAM_SKILL_SWORD = 22,
    CONDITION_PARAM_SKILL_AXE = 23,
    CONDITION_PARAM_SKILL_DISTANCE = 24,
    CONDITION_PARAM_SKILL_SHIELD = 25,
    CONDITION_PARAM_SKILL_FISHING = 26,
    CONDITION_PARAM_STAT_MAXHITPOINTS = 27,
    CONDITION_PARAM_STAT_MAXMANAPOINTS = 28,
    // CONDITION_PARAM_STAT_SOULPOINTS = 29,
    CONDITION_PARAM_STAT_MAGICPOINTS = 30,
    CONDITION_PARAM_STAT_MAXHITPOINTSPERCENT = 31,
    CONDITION_PARAM_STAT_MAXMANAPOINTSPERCENT = 32,
    // CONDITION_PARAM_STAT_SOULPOINTSPERCENT = 33,
    CONDITION_PARAM_STAT_MAGICPOINTSPERCENT = 34,
    CONDITION_PARAM_PERIODICDAMAGE = 35,
    CONDITION_PARAM_SKILL_MELEEPERCENT = 36,
    CONDITION_PARAM_SKILL_FISTPERCENT = 37,
    CONDITION_PARAM_SKILL_CLUBPERCENT = 38,
    CONDITION_PARAM_SKILL_SWORDPERCENT = 39,
    CONDITION_PARAM_SKILL_AXEPERCENT = 40,
    CONDITION_PARAM_SKILL_DISTANCEPERCENT = 41,
    CONDITION_PARAM_SKILL_SHIELDPERCENT = 42,
    CONDITION_PARAM_SKILL_FISHINGPERCENT = 43,
    CONDITION_PARAM_BUFF_SPELL = 44,
    CONDITION_PARAM_SUBID = 45,
    CONDITION_PARAM_FIELD = 46,
    CONDITION_PARAM_DISABLE_DEFENSE = 47,
    CONDITION_PARAM_SPECIALSKILL_CRITICALHITCHANCE = 48,
    CONDITION_PARAM_SPECIALSKILL_CRITICALHITAMOUNT = 49,
    CONDITION_PARAM_SPECIALSKILL_LIFELEECHCHANCE = 50,
    CONDITION_PARAM_SPECIALSKILL_LIFELEECHAMOUNT = 51,
    CONDITION_PARAM_SPECIALSKILL_MANALEECHCHANCE = 52,
    CONDITION_PARAM_SPECIALSKILL_MANALEECHAMOUNT = 53,
};

enum BlockType_t : uint8_t
{
    BLOCK_NONE,
    BLOCK_DEFENSE,
    BLOCK_ARMOR,
    BLOCK_IMMUNITY
};

enum skills_t : uint8_t
{
    SKILL_FIST = 0,
    SKILL_CLUB = 1,
    SKILL_SWORD = 2,
    SKILL_AXE = 3,
    SKILL_DISTANCE = 4,
    SKILL_SHIELD = 5,
    SKILL_FISHING = 6,

    SKILL_MAGLEVEL = 7,
    SKILL_LEVEL = 8,

    SKILL_FIRST = SKILL_FIST,
    SKILL_LAST = SKILL_FISHING
};

enum stats_t
{
    STAT_MAXHITPOINTS,
    STAT_MAXMANAPOINTS,
    STAT_SOULPOINTS, // unused
    STAT_MAGICPOINTS,

    STAT_FIRST = STAT_MAXHITPOINTS,
    STAT_LAST = STAT_MAGICPOINTS
};

enum SpecialSkills_t
{
    SPECIALSKILL_CRITICALHITCHANCE,
    SPECIALSKILL_CRITICALHITAMOUNT,
    SPECIALSKILL_LIFELEECHCHANCE,
    SPECIALSKILL_LIFELEECHAMOUNT,
    SPECIALSKILL_MANALEECHCHANCE,
    SPECIALSKILL_MANALEECHAMOUNT,

    SPECIALSKILL_FIRST = SPECIALSKILL_CRITICALHITCHANCE,
    SPECIALSKILL_LAST = SPECIALSKILL_MANALEECHAMOUNT
};

enum formulaType_t
{
    COMBAT_FORMULA_UNDEFINED,
    COMBAT_FORMULA_LEVELMAGIC,
    COMBAT_FORMULA_SKILL,
    COMBAT_FORMULA_DAMAGE,
};

enum ConditionType_t
{
    CONDITION_NONE,

    CONDITION_POISON = 1 << 0,
    CONDITION_FIRE = 1 << 1,
    CONDITION_ENERGY = 1 << 2,
    CONDITION_BLEEDING = 1 << 3,
    CONDITION_HASTE = 1 << 4,
    CONDITION_PARALYZE = 1 << 5,
    CONDITION_OUTFIT = 1 << 6,
    CONDITION_INVISIBLE = 1 << 7,
    CONDITION_LIGHT = 1 << 8,
    CONDITION_MANASHIELD = 1 << 9,
    CONDITION_INFIGHT = 1 << 10,
    CONDITION_DRUNK = 1 << 11,
    CONDITION_EXHAUST_WEAPON = 1 << 12, // unused
    CONDITION_REGENERATION = 1 << 13,
    CONDITION_SOUL = 1 << 14,
    CONDITION_DROWN = 1 << 15,
    CONDITION_MUTED = 1 << 16,
    CONDITION_CHANNELMUTEDTICKS = 1 << 17,
    CONDITION_YELLTICKS = 1 << 18,
    CONDITION_ATTRIBUTES = 1 << 19,
    CONDITION_FREEZING = 1 << 20,
    CONDITION_DAZZLED = 1 << 21,
    CONDITION_CURSED = 1 << 22,
    CONDITION_EXHAUST_COMBAT = 1 << 23, // unused
    CONDITION_EXHAUST_HEAL = 1 << 24, // unused
    CONDITION_PACIFIED = 1 << 25,
    CONDITION_SPELLCOOLDOWN = 1 << 26,
    CONDITION_SPELLGROUPCOOLDOWN = 1 << 27,
};

enum ConditionId_t : int8_t
{
    CONDITIONID_DEFAULT = -1,
    CONDITIONID_COMBAT,
    CONDITIONID_HEAD,
    CONDITIONID_NECKLACE,
    CONDITIONID_BACKPACK,
    CONDITIONID_ARMOR,
    CONDITIONID_RIGHT,
    CONDITIONID_LEFT,
    CONDITIONID_LEGS,
    CONDITIONID_FEET,
    CONDITIONID_RING,
    CONDITIONID_AMMO,
};

enum PlayerSex_t : uint8_t
{
    PLAYERSEX_FEMALE = 0,
    PLAYERSEX_MALE = 1,

    PLAYERSEX_LAST = PLAYERSEX_MALE
};

enum Vocation_t : uint16_t
{
    VOCATION_NONE = 0
};

enum ReturnValue
{
    RETURNVALUE_NOERROR,
    RETURNVALUE_NOTPOSSIBLE,
    RETURNVALUE_NOTENOUGHROOM,
    RETURNVALUE_PLAYERISPZLOCKED,
    RETURNVALUE_PLAYERISNOTINVITED,
    RETURNVALUE_CANNOTTHROW,
    RETURNVALUE_THEREISNOWAY,
    RETURNVALUE_DESTINATIONOUTOFREACH,
    RETURNVALUE_CREATUREBLOCK,
    RETURNVALUE_NOTMOVEABLE,
    RETURNVALUE_DROPTWOHANDEDITEM,
    RETURNVALUE_BOTHHANDSNEEDTOBEFREE,
    RETURNVALUE_CANONLYUSEONEWEAPON,
    RETURNVALUE_NEEDEXCHANGE,
    RETURNVALUE_CANNOTBEDRESSED,
    RETURNVALUE_PUTTHISOBJECTINYOURHAND,
    RETURNVALUE_PUTTHISOBJECTINBOTHHANDS,
    RETURNVALUE_TOOFARAWAY,
    RETURNVALUE_FIRSTGODOWNSTAIRS,
    RETURNVALUE_FIRSTGOUPSTAIRS,
    RETURNVALUE_CONTAINERNOTENOUGHROOM,
    RETURNVALUE_NOTENOUGHCAPACITY,
    RETURNVALUE_CANNOTPICKUP,
    RETURNVALUE_THISISIMPOSSIBLE,
    RETURNVALUE_DEPOTISFULL,
    RETURNVALUE_CREATUREDOESNOTEXIST,
    RETURNVALUE_CANNOTUSETHISOBJECT,
    RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE,
    RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE,
    RETURNVALUE_YOUAREALREADYTRADING,
    RETURNVALUE_THISPLAYERISALREADYTRADING,
    RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT,
    RETURNVALUE_DIRECTPLAYERSHOOT,
    RETURNVALUE_NOTENOUGHLEVEL,
    RETURNVALUE_NOTENOUGHMAGICLEVEL,
    RETURNVALUE_NOTENOUGHMANA,
    RETURNVALUE_NOTENOUGHSOUL,
    RETURNVALUE_YOUAREEXHAUSTED,
    RETURNVALUE_PLAYERISNOTREACHABLE,
    RETURNVALUE_CANONLYUSETHISRUNEONCREATURES,
    RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE,
    RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER,
    RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE,
    RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE,
    RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE,
    RETURNVALUE_YOUCANONLYUSEITONCREATURES,
    RETURNVALUE_CREATUREISNOTREACHABLE,
    RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS,
    RETURNVALUE_YOUNEEDPREMIUMACCOUNT,
    RETURNVALUE_YOUNEEDTOLEARNTHISSPELL,
    RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL,
    RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL,
    RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE,
    RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE,
    RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE,
    RETURNVALUE_YOUCANNOTLOGOUTHERE,
    RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL,
    RETURNVALUE_CANNOTCONJUREITEMHERE,
    RETURNVALUE_YOUNEEDTOSPLITYOURSPEARS,
    RETURNVALUE_NAMEISTOOAMBIGUOUS,
    RETURNVALUE_CANONLYUSEONESHIELD,
    RETURNVALUE_NOPARTYMEMBERSINRANGE,
    RETURNVALUE_YOUARENOTTHEOWNER,
    RETURNVALUE_NOSUCHRAIDEXISTS,
    RETURNVALUE_ANOTHERRAIDISALREADYEXECUTING,
    RETURNVALUE_TRADEPLAYERFARAWAY,
    RETURNVALUE_YOUDONTOWNTHISHOUSE,
    RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE,
    RETURNVALUE_TRADEPLAYERHIGHESTBIDDER,
    RETURNVALUE_YOUCANNOTTRADETHISHOUSE,
};

enum SpeechBubble_t
{
    SPEECHBUBBLE_NONE = 0,
    SPEECHBUBBLE_NORMAL = 1,
    SPEECHBUBBLE_TRADE = 2,
    SPEECHBUBBLE_QUEST = 3,
    SPEECHBUBBLE_QUESTTRADER = 4,
};

enum MapMark_t
{
    MAPMARK_TICK = 0,
    MAPMARK_QUESTION = 1,
    MAPMARK_EXCLAMATION = 2,
    MAPMARK_STAR = 3,
    MAPMARK_CROSS = 4,
    MAPMARK_TEMPLE = 5,
    MAPMARK_KISS = 6,
    MAPMARK_SHOVEL = 7,
    MAPMARK_SWORD = 8,
    MAPMARK_FLAG = 9,
    MAPMARK_LOCK = 10,
    MAPMARK_BAG = 11,
    MAPMARK_SKULL = 12,
    MAPMARK_DOLLAR = 13,
    MAPMARK_REDNORTH = 14,
    MAPMARK_REDSOUTH = 15,
    MAPMARK_REDEAST = 16,
    MAPMARK_REDWEST = 17,
    MAPMARK_GREENNORTH = 18,
    MAPMARK_GREENSOUTH = 19,
};

enum MagicEffectsType_t : uint8_t
{
    MAGIC_EFFECTS_END_LOOP = 0,//ends magic effect loop
    MAGIC_EFFECTS_DELTA = 1,//needs uint8_t delta after type to adjust position
    MAGIC_EFFECTS_DELAY = 2,//needs uint16_t delay after type to delay in miliseconds effect display
    MAGIC_EFFECTS_CREATE_EFFECT = 3,//needs uint8_t effectid after type
    MAGIC_EFFECTS_CREATE_DISTANCEEFFECT = 4,//needs uint8_t and deltaX(int8_t), deltaY(int8_t) after type
    MAGIC_EFFECTS_CREATE_DISTANCEEFFECT_REVERSED = 5,//needs uint8_t and deltaX(int8_t), deltaY(int8_t) after type
};

enum PremiumTrigger_t : uint8_t
{
    PREMIUM_TRIGGER_PREMIUM_AREAS = 0,
    PREMIUM_TRIGGER_OFFLINE_TRAINING = 1,
    PREMIUM_TRIGGER_SPELLS = 2,
    PREMIUM_TRIGGER_INSTANT_TRAVEL = 3,
    PREMIUM_TRIGGER_TAME_MOUNTS = 4,
    PREMIUM_TRIGGER_OUTFITS = 5,
    PREMIUM_TRIGGER_XPBOOST = 6,
    PREMIUM_TRIGGER_PROMOTION_DEATH = 7,
    PREMIUM_TRIGGER_MARKET = 8,
    PREMIUM_TRIGGER_DECORATIONS = 9,
    PREMIUM_TRIGGER_EXTENDED_VIPLIST = 10,
    PREMIUM_TRIGGER_EXTENDED_DEPOT = 11,
    PREMIUM_TRIGGER_PRIVATE_CHATS = 12,
    PREMIUM_TRIGGER_PROMOTION_STRONGER = 13,
    PREMIUM_TRIGGER_PREMIUM_EXPIRATIONS = 14,
    PREMIUM_TRIGGER_PVP_ARENAS = 15,
    PREMIUM_TRIGGER_FREE_PREY_SLOTS = 16,
    PREMIUM_TRIGGER_IMBUING = 17,
    PREMIUM_TRIGGER_DAILY_REWARDS = 18,
    PREMIUM_TRIGGER_ANALYTICS = 19,
    PREMIUM_TRIGGER_EXCLUSIVE_QUESTS = 20,
    PREMIUM_TRIGGER_PRIORITY_LOGIN = 21,
    PREMIUM_TRIGGER_MIGHTY_SUMMON = 22,
};

enum InspectObjectTypes : uint8_t
{
    INSPECT_NORMALOBJECT = 0,
    INSPECT_NPCTRADE = 1,
    INSPECT_UNKNOWN = 2,
    INSPECT_CYCLOPEDIA = 3
};

enum Bestiary_Progress_t : uint8_t
{
    BESTIARY_PROGRESS_NONE = 0, // unknown monster
    BESTIARY_PROGRESS_FIRST = 1, // 0 / 3
    BESTIARY_PROGRESS_SECOND = 2, // 1 / 3
    BESTIARY_PROGRESS_THIRD = 3, // 2 / 3
    BESTIARY_PROGRESS_COMPLETED = 4 // 3 / 3
};

enum Bestiary_Rarity_t : uint8_t
{
    BESTIARY_RARITY_COMMON = 0,
    BESTIARY_RARITY_UNCOMMON = 1,
    BESTIARY_RARITY_SEMIRARE = 2,
    BESTIARY_RARITY_RARE = 3,
    BESTIARY_RARITY_VERYRARE = 4
};

enum Bestiary_Difficulty_t : uint8_t
{
    BESTIARY_DIFFICULTY_HARMLESS = 0,
    BESTIARY_DIFFICULTY_TRIVIAL = 1,
    BESTIARY_DIFFICULTY_EASY = 2,
    BESTIARY_DIFFICULTY_MEDIUM = 3,
    BESTIARY_DIFFICULTY_HARD = 4
};

enum Bestiary_Occurence_t : uint8_t
{
    BESTIARY_OCCURENCE_COMMON = 0,
    BESTIARY_OCCURENCE_UNCOMMON = 1,
    BESTIARY_OCCURENCE_RARE = 2,
    BESTIARY_OCCURENCE_VERYRARE = 3
};

enum Bestiary_AttackType_t : uint8_t
{
    BESTIARY_ATTACKTYPE_MELEE = 0,
    BESTIARY_ATTACKTYPE_DISTANCE = 1,
    BESTIARY_ATTACKTYPE_NONE = 2
};

enum Cipbia_Elementals_t : uint8_t
{
    CIPBIA_ELEMENTAL_PHYSICAL = 0,
    CIPBIA_ELEMENTAL_FIRE = 1,
    CIPBIA_ELEMENTAL_EARTH = 2,
    CIPBIA_ELEMENTAL_ENERGY = 3,
    CIPBIA_ELEMENTAL_ICE = 4,
    CIPBIA_ELEMENTAL_HOLY = 5,
    CIPBIA_ELEMENTAL_DEATH = 6,
    CIPBIA_ELEMENTAL_HEALING = 7,
    CIPBIA_ELEMENTAL_DROWN = 8,
    CIPBIA_ELEMENTAL_LIFEDRAIN = 9,
    CIPBIA_ELEMENTAL_UNDEFINED = 10
};

#if GAME_FEATURE_STASH > 0
enum Supply_Stash_Actions_t : uint8_t
{
    SUPPLY_STASH_ACTION_STOW_ITEM = 0,
    SUPPLY_STASH_ACTION_STOW_CONTAINER = 1,
    SUPPLY_STASH_ACTION_STOW_STACK = 2,
    SUPPLY_STASH_ACTION_WITHDRAW = 3
};
#endif

#if GAME_FEATURE_HIGHSCORES > 0
enum HighscoreType_t : uint8_t
{
    HIGHSCORE_GETENTRIES = 0,
    HIGHSCORE_OURRANK = 1
};

enum HighscoreCategories_t : uint8_t
{
    HIGHSCORE_CATEGORY_EXPERIENCE = 0,
    HIGHSCORE_CATEGORY_FIST_FIGHTING,
    HIGHSCORE_CATEGORY_CLUB_FIGHTING,
    HIGHSCORE_CATEGORY_SWORD_FIGHTING,
    HIGHSCORE_CATEGORY_AXE_FIGHTING,
    HIGHSCORE_CATEGORY_DISTANCE_FIGHTING,
    HIGHSCORE_CATEGORY_SHIELDING,
    HIGHSCORE_CATEGORY_FISHING,
    HIGHSCORE_CATEGORY_MAGIC_LEVEL
};

struct HighscoreCategory
{
    HighscoreCategory(const char* name, uint8_t id) :
        name(name), id(id) {}

    const char* name;
    uint8_t id;
};

struct HighscoreCharacter
{
    HighscoreCharacter(std::string name, uint64_t points, uint32_t id, uint32_t rank, uint16_t level, uint8_t vocation) :
        name(std::move(name)), points(points), id(id), rank(rank), level(level), vocation(vocation) {}

    std::string name;
    uint64_t points;
    uint32_t id;
    uint32_t rank;
    uint16_t level;
    uint8_t vocation;
};
#endif

#if GAME_FEATURE_CYCLOPEDIA_CHARACTERINFO > 0
enum CyclopediaCharacterInfoType_t : uint8_t
{
    CYCLOPEDIA_CHARACTERINFO_BASEINFORMATION = 0,
    CYCLOPEDIA_CHARACTERINFO_GENERALSTATS = 1,
    CYCLOPEDIA_CHARACTERINFO_COMBATSTATS = 2,
    CYCLOPEDIA_CHARACTERINFO_RECENTDEATHS = 3,
    CYCLOPEDIA_CHARACTERINFO_RECENTPVPKILLS = 4,
    CYCLOPEDIA_CHARACTERINFO_ACHIEVEMENTS = 5,
    CYCLOPEDIA_CHARACTERINFO_ITEMSUMMARY = 6,
    CYCLOPEDIA_CHARACTERINFO_OUTFITSMOUNTS = 7,
    CYCLOPEDIA_CHARACTERINFO_STORESUMMARY = 8,
    CYCLOPEDIA_CHARACTERINFO_INSPECTION = 9,
    CYCLOPEDIA_CHARACTERINFO_BADGES = 10,
    CYCLOPEDIA_CHARACTERINFO_TITLES = 11
};

enum CyclopediaCharacterInfo_OutfitType_t : uint8_t
{
    CYCLOPEDIA_CHARACTERINFO_OUTFITTYPE_NONE = 0,
    CYCLOPEDIA_CHARACTERINFO_OUTFITTYPE_QUEST = 1,
    CYCLOPEDIA_CHARACTERINFO_OUTFITTYPE_STORE = 2
};

enum CyclopediaCharacterInfo_RecentKillStatus_t : uint8_t
{
    CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_JUSTIFIED = 0,
    CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_UNJUSTIFIED = 1,
    CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_GUILDWAR = 2,
    CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_ASSISTED = 3,
    CYCLOPEDIA_CHARACTERINFO_RECENTKILLSTATUS_ARENA = 4
};

struct RecentDeathEntry
{
    RecentDeathEntry(std::string cause, uint32_t timestamp) :
        cause(std::move(cause)), timestamp(timestamp) {}

    std::string cause;
    uint32_t timestamp;
};

struct RecentPvPKillEntry
{
    RecentPvPKillEntry(std::string description, uint32_t timestamp, uint8_t status) :
        description(std::move(description)), timestamp(timestamp), status(status) {}

    std::string description;
    uint32_t timestamp;
    uint8_t status;
};
#endif

struct Outfit_t
{
    uint16_t lookTypeEx = 0;
#if GAME_FEATURE_MOUNTS > 0
    uint16_t lookMount = 0;
#endif
#if GAME_FEATURE_LOOKTYPE_U16 > 0
    uint16_t lookType = 0;
#else
    uint8_t lookType = 0;
#endif
    uint8_t lookHead = 0;
    uint8_t lookBody = 0;
    uint8_t lookLegs = 0;
    uint8_t lookFeet = 0;
    uint8_t lookAddons = 0;
#if GAME_FEATURE_MOUNT_COLORS > 0
    uint8_t lookMountHead = 0;
    uint8_t lookMountBody = 0;
    uint8_t lookMountLegs = 0;
    uint8_t lookMountFeet = 0;
#endif
};

struct LightInfo
{
    uint8_t level = 0;
    uint8_t color = 0;
    constexpr LightInfo() = default;
    constexpr LightInfo(uint8_t level, uint8_t color) : level(level), color(color) {}
};

struct ShopInfo
{
    ShopInfo() = default;
    ShopInfo(uint16_t itemId, int32_t subType = 0, uint32_t buyPrice = 0, uint32_t sellPrice = 0, std::string realName = "") :
        itemId(itemId), subType(subType), buyPrice(buyPrice), sellPrice(sellPrice), realName(std::move(realName)) {}

    // copyable
    ShopInfo(const ShopInfo& rhs) :
        itemId(rhs.itemId), subType(rhs.subType), buyPrice(rhs.buyPrice), sellPrice(rhs.sellPrice), realName(rhs.realName) {}
    ShopInfo& operator=(const ShopInfo& rhs) {
        if (this != &rhs) {
            itemId = rhs.itemId;
            subType = rhs.subType;
            buyPrice = rhs.buyPrice;
            sellPrice = rhs.sellPrice;
            realName = rhs.realName;
        }
        return *this;
    }

    // moveable
    ShopInfo(ShopInfo&& rhs) noexcept :
        itemId(rhs.itemId), subType(rhs.subType), buyPrice(rhs.buyPrice), sellPrice(rhs.sellPrice), realName(std::move(rhs.realName)) {}
    ShopInfo& operator=(ShopInfo&& rhs) noexcept {
        if (this != &rhs) {
            itemId = rhs.itemId;
            subType = rhs.subType;
            buyPrice = rhs.buyPrice;
            sellPrice = rhs.sellPrice;
            realName = std::move(rhs.realName);
        }
        return *this;
    }

    uint16_t itemId = 0;
    int32_t subType = 1;
    uint32_t buyPrice = 0;
    uint32_t sellPrice = 0;
    std::string realName;
};

struct MarketOffer
{
    MarketOffer() = default;
    MarketOffer(uint32_t price, uint32_t timestamp, uint16_t amount, uint16_t counter, uint16_t itemId, std::string playerName) :
        price(price), timestamp(timestamp), amount(amount), counter(counter), itemId(itemId), playerName(std::move(playerName)) {}

    // copyable
    MarketOffer(const MarketOffer& rhs) :
        price(rhs.price), timestamp(rhs.timestamp), amount(rhs.amount), counter(rhs.counter), itemId(rhs.itemId), playerName(rhs.playerName) {}
    MarketOffer& operator=(const MarketOffer& rhs) {
        if (this != &rhs) {
            price = rhs.price;
            timestamp = rhs.timestamp;
            amount = rhs.amount;
            counter = rhs.counter;
            itemId = rhs.itemId;
            playerName = rhs.playerName;
        }
        return *this;
    }

    // moveable
    MarketOffer(MarketOffer&& rhs) noexcept :
        price(rhs.price), timestamp(rhs.timestamp), amount(rhs.amount), counter(rhs.counter), itemId(rhs.itemId), playerName(std::move(rhs.playerName)) {}
    MarketOffer& operator=(MarketOffer&& rhs) noexcept {
        if (this != &rhs) {
            price = rhs.price;
            timestamp = rhs.timestamp;
            amount = rhs.amount;
            counter = rhs.counter;
            itemId = rhs.itemId;
            playerName = std::move(rhs.playerName);
        }
        return *this;
    }

    uint32_t price;
    uint32_t timestamp;
    uint16_t amount;
    uint16_t counter;
    uint16_t itemId;
    std::string playerName;
};

struct MarketOfferEx
{
    MarketOfferEx() = default;
    MarketOfferEx(MarketOfferEx&& other) :
        id(other.id), playerId(other.playerId), timestamp(other.timestamp), price(other.price),
        amount(other.amount), counter(other.counter), itemId(other.itemId), type(other.type),
        playerName(std::move(other.playerName)) {}

    uint32_t id;
    uint32_t playerId;
    uint32_t timestamp;
    uint32_t price;
    uint16_t amount;
    uint16_t counter;
    uint16_t itemId;
    MarketAction_t type;
    std::string playerName;
};

struct HistoryMarketOffer
{
    HistoryMarketOffer() = default;
    HistoryMarketOffer(uint32_t timestamp, uint32_t price, uint16_t itemId, uint16_t amount, MarketOfferState_t state) :
        timestamp(timestamp), price(price), itemId(itemId), amount(amount), state(state) {}

    uint32_t timestamp;
    uint32_t price;
    uint16_t itemId;
    uint16_t amount;
    MarketOfferState_t state;
};

struct MarketStatistics
{
    uint32_t numTransactions = 0;
    uint32_t highestPrice = 0;
    uint64_t totalPrice = 0;
    uint32_t lowestPrice = 0;
};

struct ModalWindow
{
    std::vector<std::pair<std::string, uint8_t>> buttons, choices;
    std::string title, message;
    uint32_t id;
    uint8_t defaultEnterButton, defaultEscapeButton;
    bool priority;

    ModalWindow(uint32_t id, std::string title, std::string message) :
        title(std::move(title)), message(std::move(message)), id(id), defaultEnterButton(0xFF), defaultEscapeButton(0xFF), priority(false) {}
};

enum CombatOrigin
{
    ORIGIN_NONE,
    ORIGIN_CONDITION,
    ORIGIN_SPELL,
    ORIGIN_MELEE,
    ORIGIN_RANGED,
};

struct CombatDamage
{
    struct
    {
        CombatType_t type = COMBAT_NONE;
        int32_t value = 0;
    } primary, secondary;

    CombatOrigin origin = ORIGIN_NONE;
    BlockType_t blockType = BLOCK_NONE;
};

using MarketOfferList = std::vector<MarketOffer>;
using HistoryMarketOfferList = std::vector<HistoryMarketOffer>;
using ShopInfoList = std::vector<ShopInfo>;

enum MonstersEvent_t : uint8_t
{
    MONSTERS_EVENT_NONE = 0,
    MONSTERS_EVENT_THINK = 1,
    MONSTERS_EVENT_APPEAR = 2,
    MONSTERS_EVENT_DISAPPEAR = 3,
    MONSTERS_EVENT_MOVE = 4,
    MONSTERS_EVENT_SAY = 5,
};

#endif
