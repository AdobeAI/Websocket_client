#include "robot.h"


static std::vector<double> denoms = {0, 0.01, 0.1, 0.1, 0.2};

Robot::Robot(boost::asio::io_context& ioc)
    : room_level(FISH_GROUND_TYPE::WHALES_JOURNEY_GROUND_TYPE)
    , fish_launch_id(0)
    , last_fish_launch_id(0)
    , balance(0.0)
    , round_id("")
    , player_id("")
    , denom(0.01)
    , bet_result(0.0)
    , bet_amount(0.0)
    , ready_to_shoot(false) {
    session = std::make_shared<WebSocket>(ioc);
    callbacks.clear();
    items.clear();
    power_inde.clear();
    for (int i = 1 ; i <= 5 ; i++) {
        items[i] = 0;
    }
}

void Robot::init() {
    auto bot = shared_from_this();

    session->onConnected([bot](std::shared_ptr<WebSocket> conn) {
        if (bot) {
            std::string message = Encode::encodeRequestLogin("GUESTPASS", 10003);
            std::cout << "connect success" << std::endl;
            conn->send(message);
            conn->receicve();
        }
    });

    session->onSend([bot](std::shared_ptr<WebSocket>) {
    });

    session->onReceive([bot](std::shared_ptr<WebSocket> conn, const std::string& buffer) {
        if (bot) {
            bot->process(conn, buffer);
        }
    });

    session->onError([bot](std::shared_ptr<WebSocket> conn, OperatorError error, boost::system::error_code ec) {
        if (bot) {
            std::cout << conn->getErrorType(error) << " message : " << ec.message() << std::endl;
        }
    });

    session->onClose([bot](std::shared_ptr<WebSocket>) {
        if (bot) {

        }
    });

    session->onTimeout([bot](std::shared_ptr<WebSocket>) {
        if (bot) {

        }
    });

    addCallbacksFunction();
}

void Robot::addCallbacksFunction() {

    callbacks["fish.updateitem"] = [this](const Json::Value& msg) {
        const Json::Value arr_item = msg["data"]["ItemList"];
        for (Json::Value::ArrayIndex i = 0 ; i < arr_item.size() ; i++) {
            updateItem(arr_item[i]["ItemType"].asInt(), arr_item[i]["Volume"].asInt());
        }
    };

    callbacks["fish.leaveroom"] = [this](const Json::Value& msg) {
        std::string _player_id = msg["data"]["PlayerID"].asString();
    };

    callbacks["fish.changestatusend"] = [this](const Json::Value& msg) {
        const Json::Value arr_player = msg["data"]["UserInfos"];
        for (Json::Value::ArrayIndex i = 0 ; i < arr_player.size() ; i++) {
            if (arr_player[i]["PlayerID"].asString() == player_id) {
                std::string _round_id = arr_player[i]["PlayerID"].asString();
                changeStatusEnd(_round_id);
            }
        }
    };

    //{"body":{"FishLaunchID":8,"GapSecs":2,"SceneType":0,"StartTime":1578303360},"compressRoute":1,"id":0,"route":"fish.changestatus","type":4}
    callbacks["fish.changestatus"] = [this](const Json::Value& msg) {
        int fish_launch_id = msg["data"]["FishLaunchID"].asInt();
        changeStatus(fish_launch_id);
    };

    callbacks["fish.buff"] = [this](const Json::Value& msg) {

    };

    callbacks["fish.boradcast"] = [this](const Json::Value& msg) {

    };

    callbacks["fish.iniconfig"] = [this](const Json::Value& msg) {
        //{"body":{"BetButton":[1,2,3,5,10,20,30,50,80,100,200,300,500,800,1000],
        //"FishLaunchDataString":"","ItemSettings":[{"ColdDownSecs":18,"ItemID":1,"isOpen":true},{"ColdDownSecs":10,"ItemID":2,"isOpen":true},{"ColdDownSecs":20,"ItemID":3,"isOpen":true},{"ColdDownSecs":8,"ItemID":4,"isOpen":true},{"ColdDownSecs":15,"ItemID":5,"isOpen":true}]},
        //"compressRoute":1,"id":5,"route":"fish.iniconfig","type":4}

        const Json::Value arr_bet_button = msg["body"]["BetButton"];
        for (Json::ArrayIndex i = 0 ; i < arr_bet_button.size() ; i++) {
            power_inde.push_back(arr_bet_button[i].asInt());
        }
        ready_to_shoot = true;
    };

    callbacks["fish.itemsetting"] = [this](const Json::Value& msg) {

    };

    callbacks["fish.joinroom"] = [this](const Json::Value& msg) {
        const Json::Value arr_player = msg["body"]["SeatStatus"];
        const Json::Value arr_balance = msg["body"]["Balance"];
        for (Json::Value::ArrayIndex i = 0 ; i < arr_player.size() ; i++) {
            if (arr_player[i].asString() == msg["body"]["PlayerID"].asString()) {
                std::string _balance = arr_balance[i].asString();
                std::string _player_id = msg["body"]["PlayerID"].asString();
                std::string _round_id = msg["body"]["RollOutID"].asString();
                setPlayer(_player_id, _round_id, _balance);
                break;
            }
        }
        int fish_launch_id = msg["body"]["FishLaunchID"].asInt();
        sendMessage(Encode::encodeInitConfig());
    };

    callbacks["fish.login"] = [this](const Json::Value& msg) {
        sendMessage(Encode::encodeJoinRoom(room_level));
    };

    callbacks["fish.first"] = [this](const Json::Value& msg) {
        sendMessage(Encode::encodeLogin());
    };

    callbacks["fish.bet"] = [this](const Json::Value& msg) {
        const int bet_serial = msg["data"]["bet_serial"].asInt();

        requestHit(bet_serial);
    };

    callbacks["fish.hit"] = [this](const Json::Value& msg) {
        //const int bet_serial = msg["data"]["bet_serial"].asInt();
    };

    callbacks["error"] = [this](const Json::Value& msg) {

    };
}

void Robot::connection(char const* host, char const* port) {
    session->run(host, port);
}

void Robot::sendMessage(std::string message) {
    session->send(message);
}

void Robot::process(std::shared_ptr<WebSocket> conn, const std::string& buffer) {
    std::string binary_message = buffer;

    const int message_type = ProtocolBuilder::Decode_ToInt(binary_message);

    if (message_type == MSG_SYSTEM_TYPE) {
        const int message_id = ProtocolBuilder::Decode_ToByte(binary_message);
        switch (message_id) {
        case MSG_SYS_SELECT_SERVER:
            break;
        case MSG_SYS_LOGIN:
            break;
        case MSG_SYS_SELECT_GAME:
            break;
        case MSG_SYS_UPDATE_BALANCE:
            break;
        }
    } else {
        ProtocolBuilder::Decode_ToInt(binary_message); //AB3 never use
        const std::string json_message = ProtocolBuilder::Decode_ToString(binary_message);
        std::cout << json_message << std::endl;

        Json::Value root(Json::objectValue);
        Json::Reader reader;

        reader.parse(json_message, root);
        const std::string route = root["route"].asString();

        auto itr = callbacks.find(route);
        if (itr != callbacks.end()) {
            (*itr).second(root);
        }
    }
    conn->receicve();
}

//{"req_id":0,"route":"fish.hit","data":{"WeaponSerial":0,"BetSerial":1,"FishIDs":[100023,100030]}}
void Robot::requestHit(int bet_serial, int weapon_serial) {

}

//{"body":{"Balance":"2000000.39","BetSerial":1,"BuffTypes":[],
//"ClientData":"{\"x\":467,\"y\":589,\"betButtonIndex\":0,\"isMobile\":false}","PlayerID":"10000003630"},
//"compressRoute":1,"id":0,"route":"fish.bet","type":4}
void Robot::requestShoot(int power_level) {
    if (!ready_to_shoot) {
        return;
    }

    if ((balance - (power_level * denom)) >= 0.0) {
        balance -= (power_level * denom);
        sendMessage(Encode::encodeBet(power_level));
        bet_result += (power_level * denom);
    }
}

void Robot::setRoomLevel(int _level) {
    room_level = _level;
    denom = denoms[_level];
}

void Robot::setPlayer(std::string _player_id, std::string _round_id, std::string _balance) {
    player_id = _player_id;
    round_id = _round_id;
    balance = std::stod(_balance);
}

void Robot::changeStatusEnd(std::string _round_id) {
    round_id = _round_id;
}

void Robot::changeStatus(int _fish_launch_id) {
    last_fish_launch_id = fish_launch_id;
    fish_launch_id = _fish_launch_id;
}

void Robot::updateItem(int item_type, int volume) {
    items[item_type] = volume;
}
