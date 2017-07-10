#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H

#include <memory>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>
#include "condreqdatamodel.h"

class DatabaseAccessor : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseAccessor(QObject *parent = 0);
    bool connectionIsOpen() const;

signals:
    void s_connectionEstablished();
    void s_connectionFailed(const QString& errorMessage);
    void s_resultExecutedQueryFor(QUuid uuid, QSqlQuery *query);

public slots:
    bool openConnection(QString host, int port, QString user, QString userPassword);
    void executeQuery(QUuid uuid, const QString& textQuery);
    QSqlQuery *executeQuery(const QString& textQuery);

protected:
    void clear();

private:
    QSqlDatabase m_db;
};

#endif // DATABASEACCESSOR_H
