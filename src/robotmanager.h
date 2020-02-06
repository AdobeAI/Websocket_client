#pragma once

#include <thread>
#include "robot/robot.h"
#include "iocontextpool.h"

class RobotManager {
    IOContextPool pool;
    std::vector<std::shared_ptr<Robot>> robots;
    unsigned int MAX_THREADS;
    std::string host;
    std::string port;
    std::string game_code;

public:
    RobotManager();

    ~RobotManager();

    void createRobot(size_t number, std::string _host, std::string _port, std::string game_code);

    void requestShoot();

};

