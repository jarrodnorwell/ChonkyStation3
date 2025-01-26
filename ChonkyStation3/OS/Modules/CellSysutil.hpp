#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellSysutil {
public:
    CellSysutil(PlayStation3* ps3) : ps3(ps3) {
        // Reset callbacks
        for (int i = 0; i < 4; i++) callbacks[i] = { 0, 0 };
    }
    PlayStation3* ps3;

    struct CellSysutilCallback {
        u32 func_ptr;
        u32 userdata_ptr;
    };
    CellSysutilCallback callbacks[4];
    
    enum SYSTEM_PARAM : u32 {
        // Integers
        CELL_SYSUTIL_SYSTEMPARAM_ID_LANG = 0x0111,
        CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN = 0x0112,
        CELL_SYSUTIL_SYSTEMPARAM_ID_DATE_FORMAT = 0x0114,
        CELL_SYSUTIL_SYSTEMPARAM_ID_TIME_FORMAT = 0x0115,
        CELL_SYSUTIL_SYSTEMPARAM_ID_TIMEZONE = 0x0116,
        CELL_SYSUTIL_SYSTEMPARAM_ID_SUMMERTIME = 0x0117,
        CELL_SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL = 0x0121,
        CELL_SYSUTIL_SYSTEMPARAM_ID_GAME_PARENTAL_LEVEL0_RESTRICT = 0x0123,
        CELL_SYSUTIL_SYSTEMPARAM_ID_CURRENT_USER_HAS_NP_ACCOUNT = 0x0141,
        CELL_SYSUTIL_SYSTEMPARAM_ID_CAMERA_PLFREQ = 0x0151,
        CELL_SYSUTIL_SYSTEMPARAM_ID_PAD_RUMBLE = 0x0152,
        CELL_SYSUTIL_SYSTEMPARAM_ID_KEYBOARD_TYPE = 0x0153,
        CELL_SYSUTIL_SYSTEMPARAM_ID_JAPANESE_KEYBOARD_ENTRY_METHOD = 0x0154,
        CELL_SYSUTIL_SYSTEMPARAM_ID_CHINESE_KEYBOARD_ENTRY_METHOD = 0x0155,
        CELL_SYSUTIL_SYSTEMPARAM_ID_PAD_AUTOOFF = 0x0156,

        // Strings
        CELL_SYSUTIL_SYSTEMPARAM_ID_NICKNAME = 0x0113,
        CELL_SYSUTIL_SYSTEMPARAM_ID_CURRENT_USERNAME = 0x0131,
    };

    enum SYSTEM_LANG : u32 {
        CELL_SYSUTIL_LANG_JAPANESE = 0,
        CELL_SYSUTIL_LANG_ENGLISH_US = 1,
        CELL_SYSUTIL_LANG_FRENCH = 2,
        CELL_SYSUTIL_LANG_SPANISH = 3,
        CELL_SYSUTIL_LANG_GERMAN = 4,
        CELL_SYSUTIL_LANG_ITALIAN = 5,
        CELL_SYSUTIL_LANG_DUTCH = 6,
        CELL_SYSUTIL_LANG_PORTUGUESE_PT = 7,
        CELL_SYSUTIL_LANG_RUSSIAN = 8,
        CELL_SYSUTIL_LANG_KOREAN = 9,
        CELL_SYSUTIL_LANG_CHINESE_T = 10,
        CELL_SYSUTIL_LANG_CHINESE_S = 11,
        CELL_SYSUTIL_LANG_FINNISH = 12,
        CELL_SYSUTIL_LANG_SWEDISH = 13,
        CELL_SYSUTIL_LANG_DANISH = 14,
        CELL_SYSUTIL_LANG_NORWEGIAN = 15,
        CELL_SYSUTIL_LANG_POLISH = 16,
        CELL_SYSUTIL_LANG_PORTUGUESE_BR = 17,
        CELL_SYSUTIL_LANG_ENGLISH_GB = 18,
    };

    enum ENTER_BUTTON : u32 {
        CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE = 0,
        CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS = 1,
    };

    u64 cellSysutilUnregisterCallback();
    u64 cellSysutilCheckCallback();
    u64 cellSysutilGetSystemParamInt();
    u64 cellSysutilRegisterCallback();

private:
    MAKE_LOG_FUNCTION(log, cellSysutil);
};