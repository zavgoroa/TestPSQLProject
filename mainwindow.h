#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlField>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlError>
#include "inputconditionrequestdialog.h"
#include "databaseaccessormanager.h"
#include "initdatadb.h"

//#define RE_INIT_DATA_DATABASE 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static void outputMessage(const QString& header, const QString& content);

protected slots:
    void openConnection();
    void connectionEstablished();
    void connectionFailed(const QString& errorMessage);

private:
    Ui::MainWindow *ui;
    DatabaseAccessorManager *m_dbAccessorManager;
    InputConditionRequestDialog *m_inputCondReqDialog;
};

#endif // MAINWINDOW_H
