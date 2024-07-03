#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSharedPointer>

#include "emulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    Ui::MainWindow *ui;
    QTimer* mSerialTimer;
    Emulation* myEmulator;
    QList<QGroupBox*>* Stations;
    QSharedPointer<unsigned int> TimeSpeed;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cbChangeTime_currentTextChanged(const QString &timeSelected);
    void on_btnStartStop_clicked();
    void on_btnBreak_clicked();

};

#endif // MAINWINDOW_H
