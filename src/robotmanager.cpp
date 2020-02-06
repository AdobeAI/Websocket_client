#include "robotmanager.h"

RobotManager::RobotManager()
    : MAX_THREADS(0)
    , host("")
    , port("")
    , game_code("") {

}

RobotManager::~RobotManager() {
    pool.stop();
}

void RobotManager::createRobot(size_t number, std::string _host, std::string _port, std::string _game_code) {
    for (size_t i = 1 ; i <= number ; i++) {
        std::shared_ptr<Robot> robot = std::make_shared<Robot>(pool.service());
        robot->init();
        robot->connection(_host.c_str(), _port.c_str());
        robots.push_back(robot);
        usleep(1 * 1000);    //wait 100 msec
    }
    game_code = _game_code;
}

void RobotManager::requestShoot() {
    for (size_t i = 0 ; i < robots.size() ; i++) {
        robots[i]->requestShoot(1);
    }
}
