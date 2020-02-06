#pragma once
#include <map>
#include <vector>
#include "codec.h"
#include "websocket.h"
#include "ProtocolBuilder.h"

enum FISH_GROUND_TYPE : int {
    WHALES_JOURNEY_GROUND_TYPE = 1,
    DAVY_JONES_GROUND_TYPE,
    MEGA_CROCODILE_GROUND_TYPE,
    KUANGFU_MANATEE_GROUND_TYPE,
};

class Robot : public std::enable_shared_from_this<Robot>{
    std::shared_ptr<WebSocket> session;
    int room_level;
    int fish_launch_id;
    int last_fish_launch_id;
    double balance;
    std::string round_id;
    std::string player_id;
    std::map<int, int> items;
    std::map<std::string, std::function<void(const Json::Value&)>> callbacks;
    double denom;
    double bet_result;
    double bet_amount;
    std::vector<int> power_inde;
    bool ready_to_shoot;

    void addCallbacksFunction();

    void process(std::shared_ptr<WebSocket>, const std::string& buffer);

    void sendMessage(std::string message);

    void changeStatus(int fish_launch_id);

    void changeStatusEnd(std::string round_id);

    void updateItem(int item_type, int volume);

    void requestHit(int bet_serial, int weapon_serial = 0);

public:
    Robot(boost::asio::io_context& ioc);

    void connection(char const* host,char const* port);

    void init();

    void setRoomLevel(int _level);

    void setPlayer(std::string player_id, std::string round_id, std::string balance);

    void requestShoot(int power_level);
};
