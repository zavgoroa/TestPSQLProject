#include "callinfodatawidget.h"
#include "ui_callinfodatawidget.h"
#include "mainwindow.h"

CallInfoDataWidget::CallInfoDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallInfoDataWidget)
{
    ui->setupUi(this);
    m_dbAccessorManager = nullptr;
    m_dataModel = new QSqlQueryModel(this);
    m_lastQuery = nullptr;
    ui->tableView->setModel(m_dataModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CallInfoDataWidget::~CallInfoDataWidget()
{
    clear();
    delete ui;
}

void CallInfoDataWidget::clear()
{
    m_dataModel->clear();
    if (m_lastQuery != nullptr) delete m_lastQuery;
}

void CallInfoDataWidget::updateFilter(const QString &filter)
{
    if (m_filter != filter) {
        m_filter = filter;
        updateData();
    }
}

void CallInfoDataWidget::setDbAccessorManager(DatabaseAccessorManager *dbAccessorManager)
{
    m_dbAccessorManager = dbAccessorManager;
}

void CallInfoDataWidget::processResultQuery(QSqlQuery *query)
{
    m_dataModel->clear();
    if (m_lastQuery != nullptr) {
        delete m_lastQuery;
        m_lastQuery = nullptr;
    }
    m_lastQuery = query;

    if (m_lastQuery->lastError().type() == QSqlError::NoError) {
        m_dataModel->setQuery(*m_lastQuery);
    } else {
        MainWindow::outputMessage("Sql query error", m_lastQuery->lastError().text());
        m_lastQuery->clear();
        delete m_lastQuery;
        m_lastQuery = nullptr;
    }
}

void CallInfoDataWidget::updateData()
{
    if (m_dbAccessorManager) {
        m_dbAccessorManager->getInfoAboutCalls(this, m_filter);
    }
}
