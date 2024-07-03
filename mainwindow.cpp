#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->Stations = new QList<QGroupBox*>();
    this->TimeSpeed = QSharedPointer<unsigned int>(new unsigned int(1000));

    this->Stations->append(this->ui->station_1);
    this->Stations->append(this->ui->station_2);
    this->Stations->append(this->ui->station_3);
    this->Stations->append(this->ui->station_4);
    this->Stations->append(this->ui->station_5);
    this->Stations->append(this->ui->station_6);

    this->myEmulator = new Emulation(this->Stations, this->ui->lblSerialPort, this->ui->cbChangeTime);
    this->mSerialTimer = new QTimer(this);
    this->mSerialTimer->setInterval(*(this->TimeSpeed.data()));

    connect(this->mSerialTimer, &QTimer::timeout, this->myEmulator, &Emulation::Sender);
    connect(this->myEmulator->myPort, &QSerialPort::readyRead, this->myEmulator, &Emulation::Reader);

    this->ui->cbChangeTime->setCurrentText("1000");
    this->ui->btnStartStop->setText("Start");
    this->ui->btnStartStop->setStyleSheet("background-color: Green");
}

MainWindow::~MainWindow() {
    this->mSerialTimer->stop();
    delete this->mSerialTimer;

    if(this->myEmulator != nullptr)
        delete this->myEmulator;

    delete this->Stations;
    delete ui;
}

void MainWindow::on_cbChangeTime_currentTextChanged(const QString &timeSelected) { this->TimeSpeed = QSharedPointer<unsigned int>(new unsigned int(timeSelected.toUInt())); }

void MainWindow::on_btnStartStop_clicked() {
    if(this->mSerialTimer->isActive()) {
        this->mSerialTimer->stop();
        this->myEmulator->myPort->close();
        this->ui->btnStartStop->setText("Start");
        this->ui->btnStartStop->setStyleSheet("background-color: Green");
    } else {
        this->myEmulator->myPort->open(QIODevice::ReadWrite);
        this->mSerialTimer->start();
        this->ui->btnStartStop->setText("Stop");
        this->ui->btnStartStop->setStyleSheet("background-color: Red");
    }
}

void MainWindow::on_btnBreak_clicked() {
    if(!this->ui->cbActiveStation->currentText().isEmpty()) {
        uint auxID = this->ui->cbActiveStation->currentText().toUInt();
        qDebug() << "ID Station -> " << this->ui->cbActiveStation->currentText().toUInt();
        for(Station* aux : this->myEmulator->getStations()) {
            if(aux->getID() == auxID) { aux->breakTime(); }
        }
    }
}
