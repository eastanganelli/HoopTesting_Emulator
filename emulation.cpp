#include "emulation.h"

/**
 * @brief Emulation::Emulation
 * @param Stations_
 */
Emulation::Emulation(QList<QGroupBox*>* Stations_) {
    this->uiStations_ = Stations_;
    this->myPort = new QSerialPort();
    this->myPort->setPortName("COM28");
    this->myPort->setBaudRate(QSerialPort::Baud9600);
    this->myPort->setDataBits(QSerialPort::Data8);
    this->myPort->setParity(QSerialPort::NoParity);
    this->myPort->setStopBits(QSerialPort::OneStop);
    this->myPort->open(QIODevice::ReadWrite);
}

/**
 * @brief Emulation::~Emulation
 */
Emulation::~Emulation() {
    if(this->myStations.length() > 0) {
        for(Station* aux : this->myStations) { delete aux; }
    }
    this->uiStations_ = nullptr;
    delete this->myPort;
}

/**
 * @brief Emulation::Reader
 */
void Emulation::Reader() {
    this->buffer.append(this->myPort->readAll());
    int index = this->buffer.indexOf('\n');
    if (index == -1) { return; }

    QByteArray line = this->buffer.mid(0, index);
    buffer.remove(0, index + 1);

    //qDebug() << line;
    QList<QByteArray> substring = line.split(',');
    if(substring.length() > 2) {
        this->stationStarter(substring);
    } else if (substring.length() == 2) {
        this->stationsStateController(substring);
    }
}

/**
 * @brief Emulation::Sender
 */
void Emulation::Sender() {
    this->myPort->clear();
    QList<QByteArray> msgs;
    if(this->myStations.length() < 1) { msgs.append(QByteArray("Active!\n")); }
    else { for(Station* activeStation : this->myStations) { msgs.append(activeStation->emulate()); } }
    for(QByteArray msg : msgs) { this->myPort->write(msg); }
    msgs.clear();
    this->myPort->waitForBytesWritten(_ms);
}

/**
 * @brief Emulation::stationStarter
 * @param substring
 */
void Emulation::stationStarter(const QList<QByteArray> substring) {
    u_int ID = substring[1].toUInt(),
        pressure = substring[2].toUInt(),
        temperature = substring[3].toUInt();
    QGroupBox* stationUI = this->uiStations_->at(ID - 1);
    Station* aux = ((substring.length() == 4 && substring[0].toStdString() == "start") ? new Station(stationUI, ID, pressure, temperature) : new Station(stationUI, ID, pressure, temperature, substring[3].toUInt()));
    this->myStations.append(aux);
}

/**
 * @brief Emulation::stationsStateController
 * @param substring
 */
void Emulation::stationsStateController(QList<QByteArray> &substring) {
    u_int ID = substring[1].toUInt();

    if(substring[0].toStdString() == "stop") {
        unsigned int i = 0;
        for(Station* myStation : this->myStations) {
            if(myStation->getID() == ID) {
                delete myStation;
                this->myStations.removeAt(i);
                qDebug() << "Station " << ID << " stopped";
            } i++;
        }
    }
}
