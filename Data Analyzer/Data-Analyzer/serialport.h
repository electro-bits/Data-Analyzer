#ifndef SERIALPORT_H
#define SERIALPORT_H

//#include <QtSerialPort>
//#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVector>
#include <QString>

typedef struct {
    int sid;
    int len;
    QVector<QString> paramNames;
    QVector<int> paramSize;
}packet_t;

class SerialPort : public QSerialPort
{
    Q_OBJECT

public:
    QVector<packet_t> packets;

    SerialPort(QSerialPort *parent = 0);
    //~SePort();
    void getPacket();

private:


};

#endif // SERIALPORT_H
