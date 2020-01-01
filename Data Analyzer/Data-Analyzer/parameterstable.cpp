#include "parameterstable.h"
#include <QDebug>

ParametersTable::ParametersTable(QTableWidget *parent):QTableWidget(parent)
{
    setRowCount(0);
    setColumnCount(2);
    setHorizontalHeaderLabels(QStringList() << "Parameter"<<"Value");
}

void ParametersTable::addNewParameter(const QString &name)
{
    insertRow(rowCount());
    QTableWidgetItem *paramName = new QTableWidgetItem(name);
    paramName->setTextAlignment(Qt:: AlignCenter);
    setItem(rowCount()-1,0,paramName);

    QTableWidgetItem *paramVal = new QTableWidgetItem("--");
    paramVal->setTextAlignment(Qt:: AlignCenter);
    setItem(rowCount()-1,1,paramVal);
}

void ParametersTable::setParameter(const QString &name, const QString &val)
{
    QList<QTableWidgetItem *> itemList = findItems(name, Qt::MatchExactly);

    if(itemList.count() != 0)
    {
        QTableWidgetItem *paramVal = new QTableWidgetItem(val);
        paramVal->setTextAlignment(Qt:: AlignCenter);
        setItem(itemList.at(0)->row(),1,paramVal);
    }
}

