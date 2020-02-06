#include "codec.h"

std::string Encode::encodeRequestLogin(const std::string& token, int game_id) {
    std::string pack = "";
    pack += ProtocolBuilder::Encode_FromInt(MSG_SYSTEM_TYPE);
    pack += ProtocolBuilder::Encode_FromByte(MSG_SYS_LOGIN);
    pack += ProtocolBuilder::Encode_FromInt(game_id);
    pack += ProtocolBuilder::Encode_FromString(token);
    return pack;
}

std::string make_json_string(Json::Value _root) {
    std::string json = Json::FastWriter().write(_root);
    std::string pack = "";
    pack += ProtocolBuilder::Encode_FromInt(MessageType::MSG_GAME_TYPE);
    pack += ProtocolBuilder::Encode_FromByte(MessageType::MSG_GAME_TYPE);
    pack += ProtocolBuilder::Encode_FromString(json);
    return pack;
}

std::string Encode::encodeLogin() {
    Json::Value root(Json::objectValue);
    root["req_id"] = 1;
    root["route"] = "fish.login";

    return make_json_string(root);
}

std::string Encode::encodeJoinRoom(int level) {
    Json::Value root(Json::objectValue);
    root["req_id"] = 1;
    root["route"] = "fish.joinroom";
    root["data"]["Level"] = level;
    return make_json_string(root);
}

std::string Encode::encodeLeaveRoom() {
    std::string pack = "";
    return pack;
}

std::string Encode::encodeInitConfig() {
    Json::Value root(Json::objectValue);
    root["req_id"] = 5;
    root["route"] = "fish.iniconfig";
    root["data"]["GameID"] = 65;

    return make_json_string(root);
}

std::string Encode::encodeBet(int power_level) {
    Json::Value root(Json::objectValue);
    root["req_id"] = 0;
    root["route"] = "fish.bet";
    //"{\"x\":467,\"y\":589,\"betButtonIndex\":0,\"isMobile\":false}"
    root["data"]["ClientData"] = "{\"x\":336,\"y\":525,\"isMobile\":false,\"betButtonIndex\":0}";
    Json::Value bet_list(Json::arrayValue);
    bet_list.append(power_level);
    root["data"]["PlayerBetMultiples"] = bet_list;

    return make_json_string(root);
}

std::string Encode::encodeHit() {
    std::string pack = "";
    return pack;
}

std::string Encode::encodeUseItem() {
    std::string pack = "";
    return pack;
}
