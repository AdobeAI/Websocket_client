#pragma once

#include "json/json.h"
#include "ProtocolBuilder.h"
#include "message.h"

class Decode {
    Decode() = delete;
public:

    void decodeLogin();

    void decodeEnterLobby();

    void decodeEnterRoom();

    void decodeItemInfo();

    void decodeInitConfig();

    void decodeShoot();

    void decodeChangeStatus();

    void decodeChangeStatusEnd();

    void decodeUpdateItem();

    void decodeHit();

    void decodekillHighMultFish();

    void decodeBuff();

    void decodeLeaveRoom();

    void decodeError();
};

class Encode {
    Encode() = delete;
public:

    static std::string encodeLogin();

    static std::string encodeJoinRoom(int level);

    static std::string encodeLeaveRoom();

    static std::string encodeInitConfig();

    static std::string encodeBet(int power_level);

    static std::string encodeHit();

    static std::string encodeUseItem();

    static std::string encodeRequestLogin(const std::string& token, int game_id);
};
