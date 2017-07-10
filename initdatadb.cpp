#include "initdatadb.h"

int randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

bool InitDataDb::initDataDb(const QString &hostName, int port, const QString &userName, const QString &userPassword, int countPerson)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "CallOperatorConnection");
    db.setHostName(hostName);
    db.setDatabaseName("CallOperator");
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(userPassword);
    if (!db.open()) {
        m_lastError = db.lastError().text();
        return false;
    }

    if (!clearDataTables(db)) return false;
    if (!insertDataTables(db, countPerson)) return false;
    return true;
}

QString InitDataDb::lastError() const
{
    return m_lastError;
}

bool InitDataDb::clearDataTables(QSqlDatabase db)
{
    QSqlQuery query(db);
    if (!query.exec("TRUNCATE \"User\" CASCADE")) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool InitDataDb::insertDataTables(QSqlDatabase db, int countPerson)
{
    QList<QString> listName;
    QFile file(":/name.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        while (!stream.atEnd()) {
            listName.append(stream.readLine());
        }
    } else {
        m_lastError = "Error read file with name: " + file.errorString();
        return false;
    }

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QSqlQuery insertUserQuery(db);
    if (!insertUserQuery.prepare("insert into \"User\" values (?, ?, ?, ?) ")) {
        m_lastError = insertUserQuery.lastError().text();
        return false;
    }

    QVariantList id;
    QVariantList firstName;
    QVariantList lastName;
    QVariantList phoneNumber;
    for(int i = 0; i < countPerson; ++i) {
        id << i;
        firstName << listName.at(randInt(0, listName.length() - 1));
        lastName << listName.at(randInt(0, listName.length() - 1));
        phoneNumber << QString::number(randInt(100000, 600000));
    }
    insertUserQuery.addBindValue(id);
    insertUserQuery.addBindValue(firstName);
    insertUserQuery.addBindValue(lastName);
    insertUserQuery.addBindValue(phoneNumber);

    if (!insertUserQuery.execBatch()) {
        m_lastError = insertUserQuery.lastError().text();
        return false;
    }
    insertUserQuery.finish();
    insertUserQuery.clear();

    QSqlQuery insertCallsQuery(db);
    if (!insertCallsQuery.prepare("insert into \"Calls\" values (?, ?, ?, ?) ")) {
        m_lastError = insertCallsQuery.lastError().text();
        return false;
    }

    QVariantList sourceId;
    QVariantList targetId;
    QVariantList datetime;
    QVariantList duration;

    for(int i = 0; i < 30000; ++i) {
        sourceId << randInt(1, countPerson - 1);
        targetId << randInt(1, countPerson - 1);
        datetime << QDateTime(
                        QDate(randInt(2015, 2017), randInt(1, 12), randInt(1, 28)),
                        QTime(randInt(0, 23), randInt(0, 59), randInt(0, 59))).toString("yyyy-MM-dd hh:mm:ss");
        duration << QTime(randInt(0, 23), randInt(0, 59), randInt(0, 59)).toString("hh:mm:ss");
    }
    insertCallsQuery.addBindValue(sourceId);
    insertCallsQuery.addBindValue(targetId);
    insertCallsQuery.addBindValue(datetime);
    insertCallsQuery.addBindValue(duration);

    if (!insertCallsQuery.execBatch()) {
        m_lastError = insertCallsQuery.lastError().text();
        return false;
    }
    insertCallsQuery.finish();
    insertCallsQuery.clear();

    db.close();
    return true;
}


