#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include "portconfigdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setUserSettings(QSerialPort *newPort,QString path);
    void getUserSettings();
    void openPort();
    void serialErrorHandler(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QAction *openPortAction;
    QAction *getPortAction;
    QString confFilePath;

    void closeEvent (QCloseEvent *event);
    void readSettings();
    void writeSettings();
    QString getPortConfStr();
};

#endif // MAINWINDOW_H
