#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Serial Logger");
    getPortAction = new  QAction ("Set Port" , this);
    openPortAction = new  QAction ("Open Port" , this);
    QToolBar *main_ToolBar = new QToolBar("mainToolBar");
    addToolBar(main_ToolBar);
    main_ToolBar->addAction(getPortAction);
    getPortAction->setIcon(QIcon("settings.svg"));
    main_ToolBar->addAction(openPortAction);
    openPortAction->setIcon(QIcon("plug.svg"));
    openPortAction->setCheckable(true);
    main_ToolBar->setMovable(false);

    connect(getPortAction ,SIGNAL(triggered(bool)),this , SLOT(getPort()));
    connect(openPortAction,SIGNAL(triggered(bool)),this,SLOT(openPort()));





    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(serialErrorHandler(QSerialPort::SerialPortError)));

    //qDebug() <<serialPort->portName()<<endl;
    //qDebug() <<serialPort->baudRate()<<endl;

    //loadSettings();


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

    qDebug() <<serialPort->portName()<<endl;
    qDebug() <<serialPort->baudRate()<<endl;
    qDebug() <<serialPort->parity()<<endl;
    qDebug() <<serialPort->stopBits()<<endl;
    qDebug() <<serialPort->dataBits()<<endl;
    qDebug() <<serialPort->flowControl()<<endl;
}

void MainWindow::getPort()
{
    PortConfigDialog *dialog = new PortConfigDialog(this);
    connect(dialog,SIGNAL(newPortSetting(QSerialPort*)),this,SLOT(setPort(QSerialPort*)));

    if(dialog->exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted"<<endl;
    }
    else
    {
        qDebug() <<"Rejected"<<endl;
    }
}

void MainWindow::openPort()
{
    if(openPortAction->isChecked())
    {
        if(serialPort->open(QIODevice::ReadWrite))
        {
            qDebug()<< "port opened"<<endl;
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
            serialPort->close();

        getPortAction->setDisabled(false);
        openPortAction->setChecked(false);
        qDebug()<< "port closed"<<endl;

    }
}

void MainWindow::serialErrorHandler(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError)
    {
        qDebug()<< serialPort->error()<<endl;
        /*QStringList serialErrorList = {
          ""
        };*/
        QMessageBox::information(0,"error","error openning port");
    }
}



void MainWindow::loadSettings()
{
 /*
 QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
 QString sText = settings.value("text", "").toString();
 if (m_pLabel)
 {
  m_pLabel->setText(sText);
 }
 */
}

void MainWindow::saveSettings()
{
 /*
 QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
 QString sText = (m_pEdit) ? m_pEdit->text() : "";
 settings.setValue("text", sText);
 if (m_pLabel)
 {
  m_pLabel->setText(sText);
 }
 */
}
