#ifndef INITDATADB_H
#define INITDATADB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QSqlError>
#include <QTextStream>
#include <QTime>
#include <QDebug>

int randInt(int low, int high);

class InitDataDb
{
public:
    bool initDataDb(const QString& hostName, int port, const QString& userName, const QString& userPassword, int countPerson);
    QString lastError() const;

protected:
    bool clearDataTables(QSqlDatabase db);
    bool insertDataTables(QSqlDatabase db, int countPerson);

private:
    QString m_lastError;
};

#endif // INITDATADB_H
