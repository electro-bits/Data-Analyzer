#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QtXlsx>
#include "portconfigdialog.h"
#include "parameterstable.h"
#include "serialport.h"

/*
typedef struct {
    int sid;
    int len;
    QVector<QString> paramNames;
    QVector<int> paramSize;
}packet_t;

*/

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
    SerialPort *serialPort;
    ParametersTable *parametersTable;
    QAction *openPortAction;
    QAction *getPortAction;
    QHBoxLayout *mainLayout;
    QString confFilePath;
//    QVector<packet_t> packet;

    void closeEvent (QCloseEvent *event);
    void readSettings();
    void writeSettings();
    QString getPortConfStr();
    void makeParametersTable();
};

#endif // MAINWINDOW_H
