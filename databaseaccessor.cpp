#include "databaseaccessor.h"

DatabaseAccessor::DatabaseAccessor(QObject *parent) : QObject(parent)
{
    clear();
}

bool DatabaseAccessor::openConnection(QString host, int port, QString user, QString userPassword)
{
    clear();
    m_db = QSqlDatabase::addDatabase("QPSQL", "CallOperatorConnection");
    m_db.setHostName(host);
    m_db.setDatabaseName(tr("CallOperator"));
    m_db.setPort(port);
    m_db.setUserName(user);
    m_db.setPassword(userPassword);

    if (m_db.open()) {
        emit s_connectionEstablished();
    } else {
        emit s_connectionFailed(m_db.lastError().text());
    }
    return m_db.isOpen();
}

bool DatabaseAccessor::connectionIsOpen() const
{
    return m_db.isOpen();
}

QSqlQuery* DatabaseAccessor::executeQuery(const QString &textQuery)
{
    QSqlQuery *query = new QSqlQuery(m_db);
    query->exec(textQuery);
    return query;
}

void DatabaseAccessor::executeQuery(QUuid uuid, const QString &textQuery)
{
    QSqlQuery *query = new QSqlQuery(m_db);
    query->exec(textQuery);
    emit s_resultExecutedQueryFor(uuid, query);
}

void DatabaseAccessor::clear()
{
    QSqlDatabase::removeDatabase(m_db.connectionName());
    m_db.close();
}
