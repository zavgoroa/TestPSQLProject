#ifndef CALLINFODATAWIDGET_H
#define CALLINFODATAWIDGET_H

#include <QObject>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QHeaderView>
#include "resultsqlquerycallback.h"
#include "databaseaccessormanager.h"

namespace Ui {
class CallInfoDataWidget;
}

class CallInfoDataWidget : public QWidget, public ResultSqlQueryCallBack
{
    Q_OBJECT

public:
    explicit CallInfoDataWidget(QWidget *parent = 0);
    ~CallInfoDataWidget();

    void clear();
    void setDbAccessorManager(DatabaseAccessorManager *dbAccessorManager);
    void virtual processResultQuery(QSqlQuery *query) override;

public slots:
    void updateFilter(const QString& filter);
    void updateData();

private:
    Ui::CallInfoDataWidget *ui;
    QSqlQueryModel *m_dataModel;
    QSqlQuery *m_lastQuery;
    QString m_filter;
    DatabaseAccessorManager *m_dbAccessorManager;
};

#endif // CALLINFODATAWIDGET_H
