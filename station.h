#ifndef STATION_H
#define STATION_H

#include <QObject>
#include <QtMath>
#include <QTextStream>
#include <QGroupBox>
#include <QLabel>

#define _ms 1000

typedef unsigned int u_int;

class Station : public QObject{
    Q_OBJECT

    float logCurve(const float _value, const u_int _time, const u_int _condP);
    float linealCurve(const float _value);
    float HoopBreak(const float _value, const u_int _time);

    QGroupBox* myUI;
    u_int i;
    const float ID, targetPressure, targetTemp, condPeriod;
    float actualPresure, actualTemp;
    bool timeToBreak;

public:
    Station(QGroupBox* uiStation, const float ID_, const float tPressure_, const u_int tTemp);
    Station(QGroupBox* uiStation, const u_int ID_, const float tPressure_, const float tTemp, const u_int CPeriod);
    ~Station();
    QByteArray emulate();
    void breakTime();
    const u_int getID();
};

#endif // STATION_H
