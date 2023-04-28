#pragma once

#define STATIC_ASSERT_OFFSET(struct, member, offset)    static_assert(offsetof(struct, member) == offset, "Offset check")

namespace game
{
    enum DWOnlineStatus
    {
        DW_WAITING_FOR_USERNAME = 0x0,
        DW_LIVE_ERROR = 0x1,
        DW_LIVE_DISCONNECTED = 0x2,
        DW_LIVE_CONNECTING = 0x3,
        DW_LIVE_CONNECTED = 0x4,
        DW_LIVE_PLATFORM_AUTH_ERROR = 0x5,
    };

    enum taskCompleteResults
    {
        TASK_NOTCOMPLETE = 0x0,
        TASK_COMPLETE = 0x1,
        TASK_ERROR = 0x2
    };

    struct overlappedTask
    {
        void* overlappedIO;
        int finalStatus;
        int errorCode;
        int type;
        bool active;
        int controllerIndex;
        void* data;
        void* miscData;
        float startMS;
    };
    static_assert(sizeof(overlappedTask) == 0x24);

    enum netadrtype_t
    {
        NA_BOT = 0x0,
        NA_BAD = 0x1,
        NA_LOOPBACK = 0x2,
        NA_BROADCAST = 0x3,
        NA_IP = 0x4
    };

    struct netadr_t
    {
        netadrtype_t type;
        unsigned __int8 ip[4];
        unsigned __int16 port;
        int addrHandleIndex;
    };

    struct bdSecurityID
    {
        unsigned __int8 ab[8];
    };

    struct XNADDR
    {
        unsigned __int8 addrBuff[25];
    };

    struct bdSecurityKey
    {
        unsigned __int8 ab[16];
    };

    struct XSESSION_INFO
    {
        bdSecurityID sessionID;
        XNADDR hostAddress;
        bdSecurityKey keyExchangeKey;
    };

    struct RemoteTalkerInfo
    {
        bool registered;
        unsigned __int64 xuid;
        int natType;
        int voiceConnectivityBits;
        int lastConnectivityTestTime[1];
        bool muted;
    };

    struct RegisteredUser
    {
        bool active;
        unsigned __int64 xuid;
        netadr_t addr;
        bool privateSlot;
        int performanceValue;
    };

    struct LocalTalkerInfo
    {
        bool enabled;
    };

    struct SessionData_s
    {
        char* sessionName;
        bool sessionHandle;
        XSESSION_INFO sessionInfo;
        bool keysGenerated;
        bool sessionStartCalled;
        bool registerUsersWithVoice;
        unsigned __int64 sessionNonce;
        int privateSlots;
        int publicSlots;
        int flags;
        void* ps3VoiceEngine;
        bool qosListenEnabled;
        RegisteredUser registeredUsers[4];
        RemoteTalkerInfo remoteTalkers[4];
        LocalTalkerInfo localTalkers[4];
        int voiceConnectivityBits;
        int sessionCreateController;
        int ackedMeetBits;
        int lastMeetSendTime;
    };
    STATIC_ASSERT_OFFSET(SessionData_s, keysGenerated, 0x36);
    STATIC_ASSERT_OFFSET(SessionData_s, registeredUsers, 0x60);
    STATIC_ASSERT_OFFSET(SessionData_s, remoteTalkers, 0x100);
    STATIC_ASSERT_OFFSET(SessionData_s, localTalkers, 0x180);
    STATIC_ASSERT_OFFSET(SessionData_s, voiceConnectivityBits, 0x184);
    static_assert(sizeof(SessionData_s) == 0x198);

    struct MatchMakingInfo
    {
        char __pad0[0x21C];
        int m_dirty;
        int m_active;
        int m_membertimesincelastupdate;
    };
    static_assert(sizeof(MatchMakingInfo) == 0x228);

    struct PartyData_s
    {
        char __pad0[0x1848];
        int areWeHost;
        char __pad1[0x4];
        int inParty;
        int party_systemActive;
        char __pad2[0x1000];
    };
    static_assert(sizeof(PartyData_s) == 0x2858);

    enum dvarType_t
    {
        DVAR_TYPE_BOOL = 0x0,
        DVAR_TYPE_FLOAT = 0x1,
        DVAR_TYPE_FLOAT_2 = 0x2,
        DVAR_TYPE_FLOAT_3 = 0x3,
        DVAR_TYPE_FLOAT_4 = 0x4,
        DVAR_TYPE_INT = 0x5,
        DVAR_TYPE_ENUM = 0x6,
        DVAR_TYPE_STRING = 0x7,
        DVAR_TYPE_COLOR = 0x8,
        DVAR_TYPE_INT64 = 0x9,
        DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
        DVAR_TYPE_COLOR_XYZ = 0xB,
        DVAR_TYPE_COUNT = 0xC
    };

    struct DvarLimits_enumeration
    {
        int stringCount;
        const char** strings;
    };

    struct DvarLimits_integer
    {
        int min;
        int max;
    };

    struct DvarLimits_integer64
    {
        __int64 min;
        __int64 max;
    };

    struct DvarLimits_value
    {
        float min;
        float max;
    };

    struct DvarLimits_vector
    {
        float min;
        float max;
    };

    union DvarLimits
    {
        DvarLimits_enumeration enumeration;
        DvarLimits_integer integer;
        DvarLimits_integer64 integer64;
        DvarLimits_value value;
        DvarLimits_vector vector;
    };

    union DvarValue
    {
        bool enabled;
        int integer;
        unsigned int unsignedInt;
        __int64 integer64;
        unsigned __int64 unsignedInt64;
        float value;
        float vector[4];
        const char* string;
        unsigned __int8 color[4];
    };

    struct dvar_s
    {
        const char* name;
        const char* description;
        int hash;
        unsigned int flags;
        dvarType_t type;
        bool modified;
        bool loadedFromSaveGame;
        DvarValue current;
        DvarValue latched;
        DvarValue reset;
        DvarValue saved;
        DvarLimits domain;
        dvar_s* hashNext;
    };
    static_assert(sizeof(dvar_s) == 0x70);

    enum consoleChannel
    {
        CON_CHANNEL_DONT_FILTER = 0x0,
        CON_CHANNEL_ERROR = 0x1,
        CON_CHANNEL_GAMENOTIFY = 0x2,
        CON_CHANNEL_BOLDGAME = 0x3,
        CON_CHANNEL_SUBTITLE = 0x4,
        CON_CHANNEL_OBITUARY = 0x5,
        CON_CHANNEL_LOGFILEONLY = 0x6,
        CON_CHANNEL_CONSOLEONLY = 0x7,
        CON_CHANNEL_GFX = 0x8,
        CON_CHANNEL_SOUND = 0x9,
        CON_CHANNEL_FILES = 0xA,
        CON_CHANNEL_DEVGUI = 0xB,
        CON_CHANNEL_PROFILE = 0xC,
        CON_CHANNEL_UI = 0xD,
        CON_CHANNEL_CLIENT = 0xE,
        CON_CHANNEL_SERVER = 0xF,
        CON_CHANNEL_SYSTEM = 0x10,
        CON_CHANNEL_PLAYERWEAP = 0x11,
        CON_CHANNEL_AI = 0x12,
        CON_CHANNEL_ANIM = 0x13,
        CON_CHANNEL_PHYS = 0x14,
        CON_CHANNEL_FX = 0x15,
        CON_CHANNEL_LEADERBOARDS = 0x16,
        CON_CHANNEL_LIVE = 0x17,
        CON_CHANNEL_PARSERSCRIPT = 0x18,
        CON_CHANNEL_SCRIPT = 0x19,
        CON_CHANNEL_SPAWNSYSTEM = 0x1A,
        CON_CHANNEL_COOPINFO = 0x1B,
        CON_CHANNEL_SERVERDEMO = 0x1C,
        CON_CHANNEL_DDL = 0x1D,
        CON_CHANNEL_NETWORK = 0x1E,
        CON_CHANNEL_SCHEDULER = 0x1F,
        CON_FIRST_DEBUG_CHANNEL = 0x1F,
        CON_CHANNEL_TASK = 0x20,
        CON_CHANNEL_SPU = 0x21,

        CON_BUILTIN_CHANNEL_COUNT = 0x22
    };

    enum bdLobbyErrorCode
    {
        BD_NO_ERROR = 0x0,
        BD_TOO_MANY_TASKS = 0x1,
        BD_NOT_CONNECTED = 0x2,
        BD_SEND_FAILED = 0x3,
        BD_HANDLE_TASK_FAILED = 0x4,
        BD_START_TASK_FAILED = 0x5,
        BD_RESULT_EXCEEDS_BUFFER_SIZE = 0x64,
        BD_ACCESS_DENIED = 0x65,
        BD_EXCEPTION_IN_DB = 0x66,
        BD_MALFORMED_TASK_HEADER = 0x67,
        BD_INVALID_ROW = 0x68,
        BD_EMPTY_ARG_LIST = 0x69,
        BD_PARAM_PARSE_ERROR = 0x6A,
        BD_PARAM_MISMATCHED_TYPE = 0x6B,
        BD_SERVICE_NOT_AVAILABLE = 0x6C,
        BD_CONNECTION_RESET = 0x6D,
        BD_INVALID_USER_ID = 0x6E,
        BD_LOBBY_PROTOCOL_VERSION_FAILURE = 0x6F,
        BD_LOBBY_INTERNAL_FAILURE = 0x70,
        BD_LOBBY_PROTOCOL_ERROR = 0x71,
        BD_ASYNCHRONOUS_ERROR = 0xC8,
        BD_MEMBER_NO_PROPOSAL = 0x12C,
        BD_TEAMNAME_ALREADY_EXISTS = 0x12D,
        BD_MAX_TEAM_MEMBERSHIPS_LIMITED = 0x12E,
        BD_MAX_TEAM_OWNERSHIPS_LIMITED = 0x12F,
        BD_NOT_A_TEAM_MEMBER = 0x130,
        BD_INVALID_TEAM_ID = 0x131,
        BD_INVALID_TEAM_NAME = 0x132,
        BD_NOT_A_TEAM_OWNER = 0x133,
        BD_NOT_AN_ADMIN_OR_OWNER = 0x134,
        BD_MEMBER_PROPOSAL_EXISTS = 0x135,
        BD_MEMBER_EXISTS = 0x136,
        BD_TEAM_FULL = 0x137,
        BD_VULGAR_TEAM_NAME = 0x138,
        BD_TEAM_USERID_BANNED = 0x139,
        BD_TEAM_EMPTY = 0x13A,
        BD_INVALID_TEAM_PROFILE_QUERY_ID = 0x13B,
        BD_INVALID_LEADERBOARD_ID = 0x190,
        BD_INVALID_STATS_SET = 0x191,
        BD_EMPTY_STATS_SET_IGNORED = 0x193,
        BD_NO_DIRECT_ACCESS_TO_ARBITRATED_LBS = 0x194,
        BD_STATS_WRITE_PERMISSION_DENIED = 0x195,
        BD_STATS_WRITE_TYPE_DATA_TYPE_MISMATCH = 0x196,
        BD_NO_STATS_FOR_USER = 0x197,
        BD_INVALID_ACCESS_TO_UNRANKED_LB = 0x198,
        BD_INVALID_TITLE_ID = 0x1F4,
        BD_MESSAGING_INVALID_MAIL_ID = 0x258,
        BD_SELF_BLOCK_NOT_ALLOWED = 0x259,
        BD_GLOBAL_MESSAGE_ACCESS_DENIED = 0x25A,
        BD_AUTH_NO_ERROR = 0x2BC,
        BD_AUTH_BAD_REQUEST = 0x2BD,
        BD_AUTH_SERVER_CONFIG_ERROR = 0x2BE,
        BD_AUTH_BAD_TITLE_ID = 0x2BF,
        BD_AUTH_BAD_ACCOUNT = 0x2C0,
        BD_AUTH_ILLEGAL_OPERATION = 0x2C1,
        BD_AUTH_INCORRECT_LICENSE_CODE = 0x2C2,
        BD_AUTH_CREATE_USERNAME_EXISTS = 0x2C3,
        BD_AUTH_CREATE_USERNAME_ILLEGAL = 0x2C4,
        BD_AUTH_CREATE_USERNAME_VULGAR = 0x2C5,
        BD_AUTH_CREATE_MAX_ACC_EXCEEDED = 0x2C6,
        BD_AUTH_MIGRATE_NOT_SUPPORTED = 0x2C7,
        BD_AUTH_TITLE_DISABLED = 0x2C8,
        BD_AUTH_ACCOUNT_EXPIRED = 0x2C9,
        BD_AUTH_ACCOUNT_LOCKED = 0x2CA,
        BD_AUTH_UNKNOWN_ERROR = 0x2CB,
        BD_AUTH_INCORRECT_PASSWORD = 0x2CC,
        BD_AUTH_IP_NOT_IN_ALLOWED_RANGE = 0x2CD,
        BD_AUTH_WII_TOKEN_VERIFICATION_FAILED = 0x2CE,
        BD_AUTH_WII_AUTHENTICATION_FAILED = 0x2CF,
        BD_AUTH_IP_KEY_LIMIT_REACHED = 0x2D0,
        BD_NO_PROFILE_INFO_EXISTS = 0x320,
        BD_FRIENDSHIP_NOT_REQUSTED = 0x384,
        BD_NOT_A_FRIEND = 0x385,
        BD_SELF_FRIENDSHIP_NOT_ALLOWED = 0x387,
        BD_FRIENDSHIP_EXISTS = 0x388,
        BD_PENDING_FRIENDSHIP_EXISTS = 0x389,
        BD_USERID_BANNED = 0x38A,
        BD_FRIENDS_FULL = 0x38C,
        BD_FRIENDS_NO_RICH_PRESENCE = 0x38D,
        BD_RICH_PRESENCE_TOO_LARGE = 0x38E,
        BD_NO_FILE = 0x3E8,
        BD_PERMISSION_DENIED = 0x3E9,
        BD_FILESIZE_LIMIT_EXCEEDED = 0x3EA,
        BD_FILENAME_MAX_LENGTH_EXCEEDED = 0x3EB,
        BD_CHANNEL_DOES_NOT_EXIST = 0x44D,
        BD_CHANNEL_ALREADY_SUBSCRIBED = 0x44E,
        BD_CHANNEL_NOT_SUBSCRIBED = 0x44F,
        BD_CHANNEL_FULL = 0x450,
        BD_CHANNEL_SUBSCRIPTIONS_FULL = 0x451,
        BD_CHANNEL_NO_SELF_WHISPERING = 0x452,
        BD_EVENT_DESC_TRUNCATED = 0x4B0,
        BD_CONTENT_UNLOCK_UNKNOWN_ERROR = 0x514,
        BD_UNLOCK_KEY_INVALID = 0x515,
        BD_UNLOCK_KEY_ALREADY_USED_UP = 0x516,
        BD_KEY_ARCHIVE_INVALID_WRITE_TYPE = 0x5DC,
        BD_BANDWIDTH_TEST_TRY_AGAIN = 0x712,
        BD_BANDWIDTH_TEST_STILL_IN_PROGRESS = 0x713,
        BD_BANDWIDTH_TEST_NOT_PROGRESS = 0x714,
        BD_BANDWIDTH_TEST_SOCKET_ERROR = 0x715,
        BD_INVALID_SESSION_NONCE = 0x76D,
        BD_ARBITRATION_FAILURE = 0x76F,
        BD_ARBITRATION_USER_NOT_REGISTERED = 0x771,
        BD_ARBITRATION_NOT_CONFIGURED = 0x772,
        BD_CONTENTSTREAMING_FILE_NOT_AVAILABLE = 0x7D0,
        BD_CONTENTSTREAMING_STORAGE_SPACE_EXCEEDED = 0x7D1,
        BD_CONTENTSTREAMING_NUM_FILES_EXCEEDED = 0x7D2,
        BD_CONTENTSTREAMING_UPLOAD_BANDWIDTH_EXCEEDED = 0x7D3,
        BD_CONTENTSTREAMING_FILENAME_MAX_LENGTH_EXCEEDED = 0x7D4,
        BD_CONTENTSTREAMING_MAX_THUMB_DATA_SIZE_EXCEEDED = 0x7D5,
        BD_CONTENTSTREAMING_DOWNLOAD_BANDWIDTH_EXCEEDED = 0x7D6,
        BD_CONTENTSTREAMING_NOT_ENOUGH_DOWNLOAD_BUFFER_SPACE = 0x7D7,
        BD_CONTENTSTREAMING_SERVER_NOT_CONFIGURED = 0x7D8,
        BD_CONTENTSTREAMING_INVALID_APPLE_RECEIPT = 0x7DA,
        BD_CONTENTSTREAMING_APPLE_STORE_NOT_AVAILABLE = 0x7DB,
        BD_CONTENTSTREAMING_APPLE_RECEIPT_FILENAME_MISMATCH = 0x7DC,
        BD_CONTENTSTREAMING_HTTP_ERROR = 0x7E4,
        BD_CONTENTSTREAMING_FAILED_TO_START_HTTP = 0x7E5,
        BD_VOTERANK_ERROR_EMPTY_RATING_SUBMISSION = 0x7EE,
        BD_VOTERANK_ERROR_MAX_VOTES_EXCEEDED = 0x7EF,
        BD_VOTERANK_ERROR_INVALID_RATING = 0x7F0,
        BD_MAX_NUM_TAGS_EXCEEDED = 0x82A,
        BD_TAGGED_COLLECTION_DOES_NOT_EXIST = 0x82B,
        BD_INVALID_QUERY_ID = 0x834,
        BD_NO_ENTRY_TO_UPDATE = 0x835,
        BD_SESSION_INVITE_EXISTS = 0x836,
        BD_INVALID_SESSION_ID = 0x837,
        BD_ATTACHMENT_TOO_LARGE = 0x838,
        BD_INVALID_GROUP_ID = 0xAF0,
        BD_TWITTER_AUTH_ATTEMPT_FAILED = 0xDAD,
        BD_TWITTER_AUTH_TOKEN_INVALID = 0xDAE,
        BD_TWITTER_UPDATE_LIMIT_REACHED = 0xDAF,
        BD_TWITTER_UNAVAILABLE = 0xDB0,
        BD_TWITTER_ERROR = 0xDB1,
        BD_TWITTER_TIMED_OUT = 0xDB2,
        BD_TWITTER_DISABLED_FOR_USER = 0xDB3,
        BD_TWITTER_ACCOUNT_AMBIGUOUS = 0xDB4,
        BD_TWITTER_MAXIMUM_ACCOUNTS_REACHED = 0xDB5,
        BD_FACEBOOK_AUTH_ATTEMPT_FAILED = 0xE11,
        BD_FACEBOOK_AUTH_TOKEN_INVALID = 0xE12,
        BD_FACEBOOK_PHOTO_DOES_NOT_EXIST = 0xE13,
        BD_FACEBOOK_PHOTO_INVALID = 0xE14,
        BD_FACEBOOK_PHOTO_ALBUM_FULL = 0xE15,
        BD_FACEBOOK_UNAVAILABLE = 0xE16,
        BD_FACEBOOK_ERROR = 0xE17,
        BD_FACEBOOK_TIMED_OUT = 0xE18,
        BD_FACEBOOK_DISABLED_FOR_USER = 0xE19,
        BD_FACEBOOK_ACCOUNT_AMBIGUOUS = 0xE1A,
        BD_FACEBOOK_MAXIMUM_ACCOUNTS_REACHED = 0xE1B,
        BD_APNS_USER_NOT_REGISTERED = 0xE74,
        BD_APNS_INVALID_PAYLOAD = 0xE75,
        BD_CRUX_ERROR = 0xF3D,
        BD_CRUX_EMAIL_PASSWORD_INVALID = 0xF3E,
        BD_CRUX_EMAIL_INVALID = 0xF3F,
        BD_CRUX_BIRTH_DATE_INVALID = 0xF40,
        BD_CRUX_BIRTH_DATE_NOT_ALLOWED = 0xF41,
        BD_CRUX_PASSWORD_INVALID = 0xF42,
        BD_CRUX_PLATFORM_ID_INVALID = 0xF43,
        BD_CRUX_PLATFORM_UID_EXISTS = 0xF44,
        BD_CRUX_COMMUNITY_ID_INVALID = 0xF45,
        BD_CRUX_COMMUNITY_USERNAME_INVALID = 0xF46,
        BD_CRUX_COMMUNITY_USERNAME_EXISTS = 0xF47,
        BD_CRUX_TITLE_ID_INVALID = 0xF48,
        BD_CRUX_TITLE_UID_EXISTS = 0xF49,

        BD_MAX_ERROR_CODE = 0xF4A
    };
}