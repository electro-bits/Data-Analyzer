#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Data Analyzer");
    getPortAction = new  QAction ("Set Port" , this);
    openPortAction = new  QAction ("Open Port" , this);
    QToolBar *main_ToolBar = new QToolBar("mainToolBar");
    addToolBar(main_ToolBar);
    main_ToolBar->setMovable(false);
    main_ToolBar->addAction(getPortAction);
    getPortAction->setIcon(QIcon(":/icons/res/gear.svg"));
    main_ToolBar->addAction(openPortAction);
    openPortAction->setIcon(QIcon(":/icons/res/plug.svg"));
    openPortAction->setCheckable(true);


    connect(getPortAction ,SIGNAL(triggered(bool)),this , SLOT(getPort()));
    connect(openPortAction,SIGNAL(triggered(bool)),this,SLOT(openPort()));

    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(serialErrorHandler(QSerialPort::SerialPortError)));

    readSettings();
    statusBar()->showMessage(getPortConfStr() , 0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPort(QSerialPort *newPort)
{
    serialPort->setPortName(newPort->portName());
    serialPort->setBaudRate(newPort->baudRate());
    serialPort->setDataBits(newPort->dataBits());
    serialPort->setParity(newPort->parity());
    serialPort->setFlowControl(newPort->flowControl());
    serialPort->setStopBits(newPort->stopBits());

//    qDebug() <<serialPort->portName()<<endl;
//    qDebug() <<serialPort->baudRate()<<endl;
//    qDebug() <<serialPort->parity()<<endl;
//    qDebug() <<serialPort->stopBits()<<endl;
//    qDebug() <<serialPort->dataBits()<<endl;
//    qDebug() <<serialPort->flowControl()<<endl;
}

void MainWindow::getPort()
{
    PortConfigDialog *dialog = new PortConfigDialog(this);
    connect(dialog,SIGNAL(newPortSetting(QSerialPort*)),this,SLOT(setPort(QSerialPort*)));

    if(dialog->exec() == QDialog::Accepted)
    {
        statusBar()->showMessage(getPortConfStr() , 0);
        //qDebug() << "Accepted"<<endl;
    }
    else
    {
        //qDebug() <<"Rejected"<<endl;
    }
}

void MainWindow::openPort()
{
    if(openPortAction->isChecked())
    {
        if(serialPort->open(QIODevice::ReadWrite))
        {
            qDebug()<<serialPort->portName() <<"port opened"<<endl;
            getPortAction->setDisabled(true);

        }
        else
        {
            openPortAction->setChecked(false);
        }
    }
    else
    {
        if(serialPort->isOpen())
        {
            serialPort->close();
            qDebug()<<serialPort->portName()<< "port closed"<<endl;
        }
        else
        {
            qDebug()<< "port closed"<<endl;
        }

        getPortAction->setDisabled(false);
        openPortAction->setChecked(false);
    }
}

void MainWindow::serialErrorHandler(QSerialPort::SerialPortError error)
{
    QString errStr = "";

    switch (error) {
    case QSerialPort::NoError:
        break;
    case QSerialPort::DeviceNotFoundError:
        errStr += "Device Not Found\n";
        break;
    case QSerialPort::PermissionError:
        errStr += "The Selected Port Is Being Used By Another Software\n";
        break;
    case QSerialPort::OpenError:
        errStr += "Open Error\n";
        break;
    case QSerialPort::NotOpenError:
        errStr += "Not Open Error\n";
        break;
    case QSerialPort::ParityError:
        errStr += "Parity Error\n";
        break;
    case QSerialPort::FramingError:
        errStr += "Framing Error\n";
        break;
    case QSerialPort::BreakConditionError:
        errStr += "Break Condition Error\n";
        break;
    case QSerialPort::WriteError:
        errStr += "Write Error\n";
        break;
    case QSerialPort::ReadError:
        errStr += "Read Error\n";
        break;
    case QSerialPort::ResourceError:
        errStr += "Resource Error\n";
        break;
    case QSerialPort::UnsupportedOperationError:
        errStr += "Unsupported Operation Error\n";
        break;
    case QSerialPort::TimeoutError:
        errStr += "Timeout Error\n";
        break;
    case QSerialPort::UnknownError:
        errStr += "Unknown Error\n";
        break;
    default:
        break;
    }


    if(errStr != "")
        QMessageBox::information(0,"Error",errStr);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    writeSettings();
}

void MainWindow::readSettings()
{
    QSettings settings("electrobits","Data Analyzer");
    serialPort->setPortName(settings.value("port").toString());
    serialPort->setBaudRate(settings.value("baudrate").toInt());


    qDebug() <<"port is"<<serialPort->portName()<<endl;
    qDebug() <<"baudrate is" <<serialPort->baudRate()<<endl;
}

void MainWindow::writeSettings()
{
    QSettings settings("electrobits","Data Analyzer");
    settings.setValue("port",serialPort->portName());
    settings.setValue("baudrate",serialPort->baudRate());
}

QString MainWindow::getPortConfStr()
{
    QString portConfStr = "";

    portConfStr += serialPort->portName();
    foreach(QSerialPortInfo info,QSerialPortInfo::availablePorts())
    {
        //qDebug()<<info.portName()<<endl;
        //qDebug()<<info.description()<<endl;
        if(serialPort->portName() == info.portName())
        {
            portConfStr += "(";
            portConfStr += info.description();
            portConfStr += ")";
        }
    }
    portConfStr += " ";
    portConfStr += QString::number(serialPort->baudRate());
    portConfStr += "bps ";
    switch(serialPort->dataBits())
    {
        case QSerialPort::Data8:
            portConfStr += "8";
        break;
        case QSerialPort::Data7:
            portConfStr += "7";
        break;
        case QSerialPort::Data6:
            portConfStr += "6";
        break;
        case QSerialPort::Data5:
            portConfStr += "5";
        break;
        default :
            portConfStr += "?";
        break;
    }

    switch(serialPort->parity())
    {
        case QSerialPort::NoParity:
            portConfStr += "N";
        break;
        case QSerialPort::EvenParity:
            portConfStr += "E";
        break;
        case QSerialPort::OddParity:
            portConfStr += "O";
        break;
        case QSerialPort::SpaceParity:
            portConfStr += "S";
        break;
        case QSerialPort::MarkParity:
            portConfStr += "M";
        break;
        default :
            portConfStr += "?";
        break;
    }

    switch(serialPort->stopBits())
    {
        case QSerialPort::OneStop:
            portConfStr += "1";
        break;
        case QSerialPort::OneAndHalfStop:
            portConfStr += "1.5";
        break;
        case QSerialPort::TwoStop:
            portConfStr += "2";
        break;
        default :
            portConfStr += "?";
        break;
    }

    portConfStr += " ";

    switch(serialPort->flowControl())
    {
        case QSerialPort::NoFlowControl:
            portConfStr += "No flow control";
        break;
        case QSerialPort::HardwareControl:
            portConfStr += "Hardware(RTS/CTS)";
        break;
        case QSerialPort::SoftwareControl:
            portConfStr += "Software(XON/XOFF)";
        break;
        default :
            portConfStr += "?";
        break;

    }

    return portConfStr;
}
