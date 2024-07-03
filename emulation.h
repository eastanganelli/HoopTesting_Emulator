#ifndef EMULATION_H
#define EMULATION_H

#include <QObject>
#include <QSerialPort>
#include <QComboBox>
#include <QList>
#include <QStack>

#include "station.h"

class Emulation : public QObject {
    Q_OBJECT

    void stationStarter(const QList<QByteArray> substring);
    void stationsStateController(QList<QByteArray>& substring);
    QStringList getListActiveStations();

    QList<QGroupBox*>* uiStations_;
    QList<Station*>    myStations;
    QComboBox*         cbActiveStation;
    QByteArray         buffer;

public:
    Emulation(QList<QGroupBox*>* Stations_, QLabel* status_, QComboBox* cbActiveStation_);
    ~Emulation();
    QList<Station*> getStations();

    QSerialPort* myPort;

public slots:
    void Reader();
    void Sender();
};
#endif // EMULATION_H
