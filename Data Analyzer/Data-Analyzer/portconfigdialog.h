#ifndef PORTCONFIGDIALOG_H
#define PORTCONFIGDIALOG_H


#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QLineEdit>




class PortConfigDialog:public QDialog
{
    Q_OBJECT

public:
    PortConfigDialog(QWidget * parent = 0);
public slots:
    void updatePortList();
    void onOkBtnClicked();
    void onCancelBtnClicked();
    void updatePortDescription(const QString &text);
    void getConfFilePath();
signals:
    void newPortSetting(QSerialPort *,QString );

private:
    QComboBox *PortCb;
    QComboBox *BaudrateCb;
    QComboBox *DatabitsCb;
    QComboBox *ParityCb;
    QComboBox *FlowControlCb;
    QComboBox *StopbitsCb;
    QStringList LastPortList;
    QPushButton *okBtn;
    QPushButton *CancelBtn;
    QPushButton * getConfFileBtn;
    QLabel * getConfFileLbl;
    QLineEdit * getConfFileTxdt;
    QLabel * portDiscription;
    QSerialPort SerialPort;
    QTimer * timer;
};


#endif // PORTCONFIGDIALOG_H
