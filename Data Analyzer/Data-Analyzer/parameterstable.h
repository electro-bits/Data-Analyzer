#ifndef PARAMETERSTABLE_H
#define PARAMETERSTABLE_H

#include <QTableWidget>
#include <QString>


class ParametersTable : public QTableWidget
{
    Q_OBJECT

public:
    ParametersTable(QTableWidget *parent = 0);
    void addNewParameter(const QString &name);
    void setParameter(const QString &name , const QString &val);

};

#endif // PARAMETERSTABLE_H
