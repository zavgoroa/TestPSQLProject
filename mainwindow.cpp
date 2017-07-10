#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef RE_INIT_DATA_DATABASE
    InitDataDb initDatadb;
    if (!initDatadb.initDataDb("127.0.0.1", 5432, "postgres", "root", 50)) {
        outputMessage("Инициализация данных в БД", "Ошибка:" + initDatadb.lastError());
    } else {
        outputMessage("Инициализация данных в БД", "Успешно завершена");
    }
#endif

    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
    ui->leDatabaseHost->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}"), this));

    m_inputCondReqDialog = new InputConditionRequestDialog();
    m_dbAccessorManager = new DatabaseAccessorManager();
    ui->callInfoDataWidget->setDbAccessorManager(m_dbAccessorManager);

    connect(m_inputCondReqDialog, SIGNAL(s_updateCondRequest(QString)), ui->callInfoDataWidget, SLOT(updateFilter(QString)));
    connect(ui->pbOpenInpCondReqDialog, SIGNAL(released()), m_inputCondReqDialog, SLOT(show()));
    connect(ui->pbDatabaseConnect, SIGNAL(released()), this, SLOT(openConnection()));
    connect(ui->pbUpdateData, SIGNAL(released()), ui->callInfoDataWidget, SLOT(updateData()));
    connect(m_dbAccessorManager, SIGNAL(s_connectionEstablished()), this, SLOT(connectionEstablished()));
    connect(m_dbAccessorManager, SIGNAL(s_connectionFailed(QString)), this, SLOT(connectionFailed(QString)));
    connect(ui->actionExit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    m_inputCondReqDialog->deleteLater();
    m_dbAccessorManager->deleteLater();
    delete ui;
}

void MainWindow::outputMessage(const QString &header, const QString &content)
{
    QMessageBox::warning(0, header, content, QMessageBox::Ok);
}

void MainWindow::openConnection()
{
    m_dbAccessorManager->openConnection(ui->leDatabaseHost->text(), ui->spDatabasePort->value(),
                          ui->leDatabaseUser->text(), ui->leDatabaseUserPassword->text());
}

void MainWindow::connectionEstablished()
{
    ui->callInfoDataWidget->updateData();
    ui->stackedWidget->setCurrentWidget(ui->pageShowData);
}

void MainWindow::connectionFailed(const QString &errorMessage)
{
    QMessageBox::warning(0, "Connection database failed", errorMessage, QMessageBox::Ok);
    ui->callInfoDataWidget->clear();
    ui->stackedWidget->setCurrentWidget(ui->pageLogin);
}
