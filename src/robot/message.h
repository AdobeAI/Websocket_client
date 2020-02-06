#pragma once

#include <cstdint>

enum MessageType : int {
    MSG_SYSTEM_TYPE = 0,
    MSG_GAME_TYPE,
};

enum SystemMessageId : uint8_t {
    MSG_SYS_SELECT_SERVER = 0,
    MSG_SYS_LOGIN = 1,
    MSG_SYS_SELECT_GAME = 2,
    MSG_SYS_UPDATE_BALANCE = 255,
};
