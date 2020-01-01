#include "serialport.h"
#include <QDebug>

SerialPort::SerialPort(QSerialPort *parent):QSerialPort(parent)
{

}


//TX Address - RX Address - SID - ...DATA... - Checksum

#define TX_ADDR_PART      0
#define RX_ADDR_PART      1
#define SID_PART          2
#define DATA_PART         3
#define CHECKSUM_PART     4

#define TX_ADDR_VAL      0x55
#define RX_ADDR_VAL      0xAA

void SerialPort::getPacket()
{

static QByteArray rxBuf;
static int packetPart = 0;
//static uint receivedSize = 0;
//static QByteArray newPacket;// = QByteArray("\x55\xAA");
//static int validPacketCtr = 0;
//static int invalidPacketCtr = 0;
//qDebug() <<"receivedSize ="<<receivedSize<<endl;



rxBuf += readAll();

bool repeat = false;

do{

   switch(packetPart)
   {
   case 0:
   {
       int res = rxBuf.indexOf(0x55,0);
       if(res == -1)
       {
           rxBuf.clear();
           repeat = false;
       }
       else
       {
           rxBuf.remove(0,res);
           packetPart = 1;
           if(rxBuf.size() > 1)
               repeat = true;
       }
   }
    break;
   case 1:
   {
       if((uchar)(rxBuf.at(1)) == 0xAA)
       {
            packetPart = 2;
            if(rxBuf.size() > 2)
              repeat = true;
       }
       else
       {
            packetPart = 0;
            repeat = true;
       }
   }
    break;
   case 2://SID
   {
        uchar sid = (uchar)(rxBuf.at(2));

        packets;
   }
    break;
   case 3://DATA

    break;
   case 4://CHECKSUM

    break;
   }

}while(repeat);


/*
do{
    repeat = false;
    if((packetPart == HEADER) )//&& (serialData.size() >= 3) )
    {

        int res = rxBuf.indexOf(0xFF,0);
        if( res!=-1 )
        {
            if(((res+2)<rxBuf.size()) && (uchar)(rxBuf.at(res+1)) == 0xFF && (uchar)(rxBuf.at(res+2)) == 0xFF )
            {
                tmpBuf.clear();
                tmpBuf.replace(0,res+2,rxBuf);
                if(res != 0)
                 qDebug() <<"extra + ";
                qDebug() <<"header ="<<tmpBuf<<endl;
                rxBuf.remove(0,res+3);
                packetPart = DATA_PART;
            }
            else
            {
               qDebug() <<"wait for receiving next byte"<<endl;
            }
        }
        else
        {
            qDebug() <<"invalid header ="<<rxBuf<<endl;
            rxBuf.clear();
        }
    }

    if(packetPart ==  DATA_PART && rxBuf.size() >= dataSize)
    {
        packetPart = CHKSUM;
        tmpBuf.clear();
        tmpBuf.replace(0,dataSize,rxBuf);
        qDebug() <<"data = "<<tmpBuf<<endl;
    }

    if(packetPart ==  CHKSUM && rxBuf.size() >= (dataSize+1))
    {
        uchar receivedCheckSum = (uchar)rxBuf.at(dataSize);
        uchar calcCheckSum = 0;
        for(int i=0 ; i<dataSize ; i++){
            calcCheckSum += (uchar)(rxBuf.at(i));
        }
         qDebug() << "data chksum = "<<receivedCheckSum <<endl;
         qDebug() << "calculated chksum = "<<calcCheckSum <<endl;
         if(calcCheckSum == receivedCheckSum)
         {
             qDebug() << "valid packet received."<<endl;

             QTableWidgetItem * cell;
             int j = 0;
             for(int i=0;i<dataSize;)
             {
                 cell = tableWidget->item(j,2);
                 bool ok;
                 uint varSize = cell->text().toUInt(&ok , 10);

                 uint res = 0;
                 uint index = 1;
                 do{
                     res = (((uchar)rxBuf.at(i))*index) + res;
                     index *= 256;
                     i++;
                     varSize--;
                 }while(varSize>0);

                 tableWidget->setItem(j++,1,new QTableWidgetItem(QString::number(res)));
             }
             validPacketCtr++;
             QString str = " valid Packet = ";
             str.append(QString::number(validPacketCtr));
             validPack->setText(str);

             QTextStream out(file);
             for(int i=0;i<tableWidget->rowCount();i++)
             {
                 cell = tableWidget->item(i,1);
                 out<<cell->text();
                 if(i+1<tableWidget->rowCount())
                    out<<',';
             }

             out<<'\n';

         }
         else
         {
             invalidPacketCtr++;
             QString str = " invalid Packet = ";
             str.append(QString::number(invalidPacketCtr));
             invalidPack->setText(str);

         }
         qDebug() << "valid packet count ="<<validPacketCtr<<"...invalid packet count=" <<invalidPacketCtr<<endl;
         rxBuf.remove(0,dataSize+1);
         packetPart = HEADER;

         if(rxBuf.size()!= 0)
             repeat = true;
    }
}while(repeat);

*/


}
