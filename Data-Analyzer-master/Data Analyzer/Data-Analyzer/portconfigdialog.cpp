#include <QtGui>
#include "portconfigdialog.h"
#include <QList>
#include <QGroupBox>
#include <QSpacerItem>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

PortConfigDialog::PortConfigDialog(QWidget *parent):QDialog(parent)
{
    PortCb = new QComboBox();
    // = { "COM1", "COM2", "COM3" , "COM4", "COM5"};
    //QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    //qDebug() << list.length()<<endl;

    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        //qDebug()<<info.portName()<<endl;
        //qDebug()<<info.description()<<endl;
        LastPortList<<info.portName();
    }

    LastPortList.sort(Qt::CaseSensitive);
    PortCb->addItems(LastPortList);


    portDiscription =  new QLabel();
    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        if(info.portName() == PortCb->currentText())
        {
            if("" != info.description())
                portDiscription->setText("Device: " + info.description());
            else
                portDiscription->setText("");
        }
    }

    BaudrateCb = new QComboBox();
    QStringList BaudrateList = { "1200", "2400", "4800" , "9600", "19200", "38400" ,"57600", "115200"};
    BaudrateCb->addItems(BaudrateList);
    BaudrateCb->setEditable(true);

    DatabitsCb = new QComboBox();
    QStringList DatabitsList = {"8", "7", "6" , "5"};
    DatabitsCb->addItems(DatabitsList);

    ParityCb = new QComboBox();
    QStringList ParityList = {"No Parity", "Even", "Odd" , "Space", "Mark"};
    ParityCb->addItems(ParityList);

    FlowControlCb = new QComboBox();
    QStringList FlowControlList = {"None", "RTS/CTS", "XON/XOFF"};
    FlowControlCb->addItems(FlowControlList);

    StopbitsCb = new QComboBox();
    QStringList StopBitsList = {"1", "1.5", "2"};
    StopbitsCb->addItems( StopBitsList);

    QLabel * PortLb = new QLabel(" Port Name: ");
    QLabel * BaudrateLb = new QLabel(" Baudrate: ");
    QLabel * DatabitsLb = new QLabel(" Databits: ");
    QLabel * ParityLb = new QLabel(" Parity: ");
    QLabel * FlowControlLb = new QLabel(" Flow Control: ");
    QLabel * StopbitsLb = new QLabel(" Stopbits: ");



    QHBoxLayout *portNameLayout = new QHBoxLayout;
    portNameLayout->addWidget(PortLb);
    portNameLayout->addWidget(PortCb);

    QHBoxLayout *BaudrateLayout = new QHBoxLayout;
    BaudrateLayout->addWidget(BaudrateLb);
    BaudrateLayout->addWidget(BaudrateCb);

    QHBoxLayout *DatabitsLayout = new QHBoxLayout;
    DatabitsLayout->addWidget(DatabitsLb);
    DatabitsLayout->addWidget(DatabitsCb);

    QHBoxLayout *ParityCbLayout = new QHBoxLayout;
    ParityCbLayout->addWidget(ParityLb);
    ParityCbLayout->addWidget(ParityCb);

    QHBoxLayout *FlowControlLayout = new QHBoxLayout;
    FlowControlLayout->addWidget(FlowControlLb);
    //FlowControlLayout->addStretch();
    FlowControlLayout->addWidget(FlowControlCb);

    QHBoxLayout *StopBitsLayout = new QHBoxLayout;
    StopBitsLayout->addWidget(StopbitsLb);
    StopBitsLayout->addWidget(StopbitsCb);


    QVBoxLayout *portLayout = new QVBoxLayout;
    portLayout->addLayout(portNameLayout);
    portLayout->addLayout(BaudrateLayout);
    portLayout->addLayout(DatabitsLayout);
    portLayout->addLayout(ParityCbLayout);
    portLayout->addLayout(FlowControlLayout);
    portLayout->addLayout(StopBitsLayout);
    portLayout->addWidget(portDiscription);
    portLayout->addStretch();

    QGroupBox *portGroupBox = new QGroupBox("Port");
    portGroupBox->setLayout(portLayout);

    //QSpacerItem * my_spacer = new QSpacerItem(0,1000, QSizePolicy::Expanding, QSizePolicy::Expanding);
    getConfFileBtn = new QPushButton("...");
    getConfFileLbl = new QLabel("Path:");
    getConfFileTxdt = new QLineEdit();
    QHBoxLayout * getConfFileLayout = new QHBoxLayout;
    getConfFileLayout->addWidget(getConfFileLbl);
    getConfFileLayout->addWidget(getConfFileTxdt);
    getConfFileLayout->addWidget(getConfFileBtn);
    QGroupBox *ConfFileGroupBox = new QGroupBox("Configuraion File");
    ConfFileGroupBox->setLayout(getConfFileLayout);
    connect(getConfFileBtn,SIGNAL(clicked(bool)),this,SLOT(getConfFilePath()));



    QHBoxLayout *BottomLayout = new QHBoxLayout;
    okBtn = new QPushButton(tr("OK"));
    CancelBtn = new QPushButton(tr("Cancel"));
    BottomLayout->addStretch();
    BottomLayout->addWidget(okBtn);
    BottomLayout->addWidget(CancelBtn);
    connect(okBtn,SIGNAL(clicked(bool)),this,SLOT(onOkBtnClicked()));
    connect(CancelBtn,SIGNAL(clicked(bool)),this,SLOT(onCancelBtnClicked()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(portGroupBox);
    mainLayout->addWidget(ConfFileGroupBox);
    //mainLayout->addLayout(topLayout);
    mainLayout->addLayout(BottomLayout);
   // mainLayout->addWidget(statusBar);

    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
    setFixedHeight(sizeHint().height());

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updatePortList()));
    timer->start(1000);

    connect(PortCb,SIGNAL(currentTextChanged(QString)),this,SLOT(updatePortDescription(QString)));
    this->setFixedSize(250,300);



}

void PortConfigDialog::updatePortList()
{

    QStringList NewPortList;

    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        NewPortList<<info.portName();
    }
    NewPortList.sort(Qt::CaseSensitive);



    if(NewPortList.length() == LastPortList.length())
    {
        int i;
        for(i = 0; i<NewPortList.length(); i++)
        {
            if(NewPortList.at(i) != LastPortList.at(i))
            {

                LastPortList.clear();
                LastPortList = NewPortList;

                QString tmpStr = PortCb->currentText();
                PortCb->clear();
                PortCb->addItems(LastPortList);
                if(PortCb->findText(tmpStr) != -1)
                    PortCb->setCurrentText(tmpStr);
                break;
            }
        }

    }
    else
    {
        //qDebug()<<LastPortList<<endl;
        //qDebug()<<NewPortList<<endl;

        LastPortList.clear();
        LastPortList = NewPortList;
        //qDebug()<<LastPortList<<endl;
        QString tmpStr = PortCb->currentText();
        PortCb->clear();
        PortCb->addItems(LastPortList);
        if(PortCb->findText(tmpStr) != -1)
            PortCb->setCurrentText(tmpStr);
    }

}

void PortConfigDialog::onOkBtnClicked()
{


    SerialPort.setPortName(PortCb->currentText());
    SerialPort.setBaudRate(BaudrateCb->currentText().toInt());

    const QList<QSerialPort::Parity> parity2Enum = {QSerialPort::NoParity
                                                    ,QSerialPort::EvenParity
                                                    ,QSerialPort::OddParity
                                                    ,QSerialPort::SpaceParity
                                                    ,QSerialPort::MarkParity };
    SerialPort.setParity(parity2Enum.at(ParityCb->currentIndex()));

    const QList<QSerialPort::DataBits> dataBits2Enum = {QSerialPort::Data8
                                                        ,QSerialPort::Data7
                                                        ,QSerialPort::Data6
                                                        ,QSerialPort::Data5 };
    SerialPort.setDataBits(dataBits2Enum.at(DatabitsCb->currentIndex()));

    const QList<QSerialPort::FlowControl> flowControl2Enum = {QSerialPort::NoFlowControl
                                                              ,QSerialPort::HardwareControl
                                                              ,QSerialPort::SoftwareControl};
    SerialPort.setFlowControl(flowControl2Enum.at(FlowControlCb->currentIndex()));

    const QList<QSerialPort::StopBits> stopBits2Enum = {QSerialPort::OneStop
                                                        ,QSerialPort::OneAndHalfStop
                                                        ,QSerialPort::TwoStop};
    SerialPort.setStopBits(stopBits2Enum.at(StopbitsCb->currentIndex()));

    if(QFile::exists(getConfFileTxdt->text()))
    {
         qDebug()<<"file exists";
         emit newPortSetting(&SerialPort,getConfFileTxdt->text());
         accept();
    }
    else
    {
         qDebug()<<"file not found";
         QMessageBox::information(this,"Error","Configuration File Not Found");
    }

}

void PortConfigDialog::onCancelBtnClicked()
{
    reject();
}

void PortConfigDialog::updatePortDescription(const QString &text)
{
    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        if(info.portName() == text)
        {
            if("" != info.description())
                portDiscription->setText("Device: " + info.description());
            else
                portDiscription->setText("");
        }
    }
}

void PortConfigDialog::getConfFilePath()
{
    QString fileName = QFileDialog::getOpenFileName(0,"Open File",".","*.xlsx");
    qDebug()<<fileName;
    getConfFileTxdt->setText(fileName);
}
