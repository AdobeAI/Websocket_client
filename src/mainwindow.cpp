#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    robot_manager.createRobot(3, "10.28.24.106", "51231", "AB3");
}

MainWindow::~MainWindow() {
    delete ui;
}
