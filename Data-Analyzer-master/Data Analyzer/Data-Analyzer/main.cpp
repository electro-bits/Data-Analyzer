#include "mainwindow.h"
#include "portconfigdialog.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //PortConfigDialog *dialog = new PortConfigDialog;
    //dialog->show();

    return a.exec();
}
