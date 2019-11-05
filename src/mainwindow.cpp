#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "websocket.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    boost::asio::io_context ioc;

    std::make_shared<Session>(ioc)->run("localhost", "51231", "hello world!");
    ioc.run();
    std::cout << "test" << std::endl;
}

MainWindow::~MainWindow() {
    delete ui;
}
