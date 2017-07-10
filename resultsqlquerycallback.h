#ifndef RESULTSQLQUERYCALLBACK_H
#define RESULTSQLQUERYCALLBACK_H

#include <QSqlQuery>
#include <QObject>

class ResultSqlQueryCallBack
{
public:
    void virtual processResultQuery(QSqlQuery *query) = 0;
};

#endif // RESULTSQLQUERYCALLBACK_H
