#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->Stations = new QList<QGroupBox*>();

    this->Stations->append(this->ui->station_1);
    this->Stations->append(this->ui->station_2);
    this->Stations->append(this->ui->station_3);
    this->Stations->append(this->ui->station_4);
    this->Stations->append(this->ui->station_5);
    this->Stations->append(this->ui->station_6);

    this->myEmulator = new Emulation(this->Stations);
    this->mSerialTimer = new QTimer(this);
    this->mSerialTimer->setInterval(_ms);

    connect(this->mSerialTimer, &QTimer::timeout, this->myEmulator, &Emulation::Sender);
    connect(this->myEmulator->myPort, &QSerialPort::readyRead, this->myEmulator, &Emulation::Reader);
    this->mSerialTimer->start();
}

MainWindow::~MainWindow() {
    this->mSerialTimer->stop();
    delete this->mSerialTimer;

    if(this->myEmulator != nullptr)
        delete this->myEmulator;

    delete this->Stations;
    delete ui;
}
