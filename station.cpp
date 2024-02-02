#include "station.h"

Station::Station(QGroupBox *uiStation, const u_int ID_, const u_int tPressure_, const u_int tTemp) : ID(ID_), targetPressure(tPressure_), targetTemp(tTemp), condPeriod(0) {
    this->i = 0;
    this->myUI = uiStation;
    QLabel* Status = this->myUI->findChild<QLabel*>("lblStation_"   + QString::number(ID_));
    Status->setText("Status: On");
    this->myUI->findChild<QLabel*>("lblStationTP_" + QString::number(ID_))->setText("Target Pressure: " + QString::number(tPressure_) + " bar");
    this->myUI->findChild<QLabel*>("lblStationTT_" + QString::number(ID_))->setText("Target Temp: " + QString::number(tTemp) + " C");
    this->myUI->setStyleSheet("QGroupBox { border: 2px solid green; border-radius: 20%; } QGroupBox:title { margin-top: 10px; background-color: rgba(255, 255, 255, 0%); } font-size: 11px; QLabel { font-size: 11px; }");

    this->actualPresure = 0;
    this->actualTemp = 0;
}

Station::Station(QGroupBox *uiStation, const u_int ID_, const u_int tPressure_, const u_int tTemp, const u_int CPeriod) : ID(ID_), targetPressure(tPressure_), targetTemp(tTemp), condPeriod(CPeriod) {
    this->i = 0;
    this->myUI = uiStation;
    QLabel* Status = this->myUI->findChild<QLabel*>("lblStation_"   + QString::number(ID_));
    Status->setText("Status: On");
    this->myUI->findChild<QLabel*>("lblStationTP_" + QString::number(ID_))->setText("Target Pressure: " + QString::number(tPressure_) + " bar");
    this->myUI->findChild<QLabel*>("lblStationTT_" + QString::number(ID_))->setText("Target Temp: " + QString::number(tTemp) + " C");
    this->myUI->setStyleSheet("QGroupBox { border: 2px solid green; border-radius: 20%; } QGroupBox:title { margin-top: 10px; background-color: rgba(255, 255, 255, 0%); } font-size: 11px; QLabel { font-size: 11px; }");

    this->actualPresure = 0;
    this->actualTemp = 0;

}

Station::~Station() {
    this->myUI->findChild<QLabel*>("lblStation_"   + QString::number(this->ID))->setText("Status: Off");
    this->myUI->findChild<QLabel*>("lblStationTP_" + QString::number(this->ID))->setText("Target Pressure: 0 bar");
    this->myUI->findChild<QLabel*>("lblStationRP_" + QString::number(this->ID))->setText("Pressure: 0 bar");
    this->myUI->findChild<QLabel*>("lblStationTT_" + QString::number(this->ID))->setText("Target Temp: 0 C");
    this->myUI->findChild<QLabel*>("lblStationRT_" + QString::number(this->ID))->setText("Temp: 0 C");
    this->myUI->setStyleSheet("QGroupBox { border: 2px solid red; border-radius: 20%; } QGroupBox:title { margin-top: 10px; background-color: rgba(255, 255, 255, 0%); } font-size: 11px; QLabel { font-size: 11px; }");
    this->myUI = nullptr;
}

QByteArray Station::emulate() {
    //(void)this->myTime->addMSecs(_ms);
    this->i++;
    QString msg;

    if((this->actualPresure <= this->targetPressure || this->actualTemp <= this->targetTemp) && this->condPeriod > 0) {
        this->actualPresure = Station::logCurve(this->targetPressure, this->i, this->condPeriod);
        this->actualTemp    = Station::logCurve(this->targetTemp, this->i, this->condPeriod);;
    } else {
        this->actualPresure = Station::linealCurve(this->targetPressure);
        this->actualTemp    = Station::linealCurve(this->targetTemp);
    }

    msg = QString::number(this->ID) + "," + QString::number(this->actualPresure, 'f', 2) + "," + QString::number(this->actualTemp, 'f', 2) + "\n";

    this->myUI->findChild<QLabel*>("lblStationRP_" + QString::number(this->ID))->setText("Pressure: " + QString::number(this->actualPresure, 'f', 2) + " bar");
    this->myUI->findChild<QLabel*>("lblStationRT_" + QString::number(this->ID))->setText("Temp: " + QString::number(this->actualTemp, 'f', 2) + " C");

    //qDebug() << msg;

    return QByteArray(msg.toStdString().c_str());
}

const u_int Station::getID(){
    return this->ID;
}

float Station::logCurve(const float _value, const u_int  _time, const u_int _condP) {
    return _value * log10(_time)/log10(_condP) + (pow(-1, rand()%(2)+1) * (rand()%10)/100);
}

float Station::linealCurve(const float _value) {
    return _value + (pow(-1, rand()%(2)+1) * (rand()%10)/100);
}
