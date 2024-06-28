#ifndef EMULATION_H
#define EMULATION_H

#include <QObject>
#include <QSerialPort>
#include <QList>
#include <QStack>
#include <QDebug>

#include "station.h"

class Emulation : public QObject {
    Q_OBJECT
public:
    Emulation(QList<QGroupBox*>* Stations_, QLabel* status_);
    ~Emulation();
    QSerialPort* myPort;
public slots:
    void Reader();
    void Sender();
private:
    void stationStarter(const QList<QByteArray> substring);
    void stationsStateController(QList<QByteArray>& substring);

    QList<QGroupBox*>* uiStations_;
    QList<Station*> myStations;
    QByteArray buffer;
};

#endif // EMULATION_H
