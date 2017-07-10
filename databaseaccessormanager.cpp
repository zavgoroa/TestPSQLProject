#include "databaseaccessormanager.h"

DatabaseAccessorManager::DatabaseAccessorManager(QObject *parent) : QObject(parent)
{
    m_dbSignals = new DatabaseSignal();
    m_dbAccessor = nullptr;
    m_dbAccessorThread = nullptr;
}

DatabaseAccessorManager::~DatabaseAccessorManager()
{
    destroyDatabaseAccessor();
    m_dbSignals->deleteLater();
    m_recipients.clear();
}

void DatabaseAccessorManager::openConnection(QString hostName, int port, QString userName, QString userPassword)
{
    createDatabaseAccessor();
    emit m_dbSignals->s_openConnection(hostName, port, userName, userPassword);
}

void DatabaseAccessorManager::executeQuery(ResultSqlQueryCallBack *sourceObject, const QString &textQuery)
{
    if (checkValidState()) {
        QUuid newUuid = QUuid::createUuid();
        m_recipients.insert(newUuid, sourceObject);
        emit m_dbSignals->s_executeQuery(newUuid, textQuery);
    }
}

void DatabaseAccessorManager::getInfoAboutCalls(ResultSqlQueryCallBack *sourceObject, const QString &filter)
{
    QString textQuery = "SELECT * FROM getinfoaboutcalls " + filter;
    executeQuery(sourceObject, textQuery);
}

void DatabaseAccessorManager::resultExecutedQuery(QUuid uuid, QSqlQuery *query)
{
    if (query->lastError().type() == QSqlError::ConnectionError) {
        query->clear();
        delete query;
        connectionFailed("Connection error.");
        return;
    }

    if (m_recipients.contains(uuid)) {
        ResultSqlQueryCallBack *recipient = m_recipients.value(uuid);
        recipient->processResultQuery(query);
        m_recipients.remove(uuid);
    } else {
        query->finish();
        delete query;
    }
}

void DatabaseAccessorManager::connectionFailed(const QString &errorMessage)
{
    emit s_connectionFailed(errorMessage);
    destroyDatabaseAccessor();
}

void DatabaseAccessorManager::createDatabaseAccessor()
{
    destroyDatabaseAccessor();
    m_dbAccessorThread = new QThread();
    connect(m_dbAccessorThread, SIGNAL(finished()), m_dbAccessorThread, SLOT(deleteLater()));
    m_dbAccessor = new DatabaseAccessor();
    m_dbAccessor->moveToThread(m_dbAccessorThread);
    connect(m_dbSignals, SIGNAL(s_openConnection(QString,int,QString,QString)), m_dbAccessor, SLOT(openConnection(QString,int,QString,QString)));
    connect(m_dbSignals, SIGNAL(s_executeQuery(QUuid,QString)), m_dbAccessor, SLOT(executeQuery(QUuid,QString)));
    connect(m_dbAccessor, SIGNAL(s_connectionFailed(QString)), this, SLOT(connectionFailed(QString)));
    connect(m_dbAccessor, SIGNAL(s_connectionEstablished()), this, SIGNAL(s_connectionEstablished()));
    connect(m_dbAccessor, SIGNAL(s_resultExecutedQueryFor(QUuid,QSqlQuery*)), this, SLOT(resultExecutedQuery(QUuid,QSqlQuery*)));
    m_dbAccessorThread->start();
}

void DatabaseAccessorManager::destroyDatabaseAccessor()
{
    if (m_dbAccessorThread != nullptr) {
        m_dbAccessorThread->quit();
        m_dbAccessorThread->deleteLater();
        m_dbAccessorThread = nullptr;
    }

    if (m_dbAccessor != nullptr) {
        m_dbAccessor->deleteLater();
        m_dbAccessor = nullptr;
    }
}

bool DatabaseAccessorManager::checkValidState() const
{
    return m_dbAccessor && m_dbAccessorThread;
}
