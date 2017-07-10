#ifndef DATABASEACCESSORMANAGER_H
#define DATABASEACCESSORMANAGER_H

#include <QObject>
#include <QUuid>
#include <QThread>
#include "resultsqlquerycallback.h"
#include "databaseaccessor.h"

class DatabaseSignal: public QObject {
    Q_OBJECT

signals:
    void s_openConnection(QString hostName, int port, QString userName, QString userPassword);
    void s_executeQuery(QUuid uuid, const QString& textQuery);
};

class DatabaseAccessorManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseAccessorManager(QObject *parent = 0);
    ~DatabaseAccessorManager();

signals:
    void s_connectionEstablished();
    void s_connectionFailed(const QString& errorMessage);

public slots:
    void openConnection(QString hostName, int port, QString userName, QString userPassword);
    void executeQuery(ResultSqlQueryCallBack *sourceObject, const QString& textQuery);
    void getInfoAboutCalls(ResultSqlQueryCallBack *sourceObject, const QString& filter = QString());

protected slots:
    void resultExecutedQuery(QUuid uuid, QSqlQuery *query);
    void connectionFailed(const QString& errorMessage);

protected:
    void createDatabaseAccessor();
    void destroyDatabaseAccessor();
    bool checkValidState() const;

private:
    QMap <QUuid, ResultSqlQueryCallBack*> m_recipients;
    DatabaseAccessor *m_dbAccessor;
    QThread *m_dbAccessorThread;
    DatabaseSignal *m_dbSignals;
};

#endif // DATABASEACCESSORMANAGER_H
