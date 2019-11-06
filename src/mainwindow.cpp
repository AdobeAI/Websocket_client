#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "websocket.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    boost::asio::io_context ioc;

    std::make_shared<WebSocket>(ioc)->run("127.0.0.1", "68888", "hello world!");
    ioc.run();
}

MainWindow::~MainWindow() {
    delete ui;
}
