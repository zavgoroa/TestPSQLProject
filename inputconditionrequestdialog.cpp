#include "inputconditionrequestdialog.h"
#include "ui_inputconditionrequestdialog.h"

InputConditionRequestDialog::InputConditionRequestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputConditionRequestDialog)
{
    ui->setupUi(this);
    m_condReqDataModel = new CondReqDataModel(this);
    ui->dataCondTableView->setModel(m_condReqDataModel);
    ui->dataCondTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dataCondTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dtDateTime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    connect(ui->pbAddNewCondReqItem, SIGNAL(released()), this, SLOT(addNewConReqItem()));
    connect(ui->pbRemoveCondReqItem, SIGNAL(released()), this, SLOT(removeCondReqItem()));
    connect(ui->pbApplyCondReq, SIGNAL(released()), this, SLOT(applyCondReq()));
    connect(ui->pbCancelCondReq, SIGNAL(released()), this, SLOT(close()));
    connect(ui->cbField, SIGNAL(activated(int)), this, SLOT(updateRelationForType(int)));
    connect(ui->cbField, SIGNAL(activated(int)), this, SLOT(updatePageWithInputWidget(int)));

    m_fieldInfo.insert(0, FieldType::Type::TEXT);
    m_fieldInfo.insert(1, FieldType::Type::TEXT);
    m_fieldInfo.insert(2, FieldType::Type::TEXT);
    m_fieldInfo.insert(3, FieldType::Type::TEXT);
    m_fieldInfo.insert(4, FieldType::Type::TEXT);
    m_fieldInfo.insert(5, FieldType::Type::TEXT);
    m_fieldInfo.insert(6, FieldType::Type::DATETIME);
    m_fieldInfo.insert(7, FieldType::Type::TIME);

    updateRelationForType(0);
}

InputConditionRequestDialog::~InputConditionRequestDialog()
{
    delete ui;
}

void InputConditionRequestDialog::addNewConReqItem()
{
    if (checkNewCondReqValues()) {
        CondReqDataModel::CondReqItem item;
        item.fieldName = ui->cbField->currentText();
        item.relationShip = ui->cbSignCompare->currentText();
        item.filterValue = getFilterValueForField();
        item.fieldType = m_fieldInfo.value(ui->cbField->currentIndex(), FieldType::Type::UNKNOW);
        m_condReqDataModel->addCondReqItem(item);
    } else {
        QMessageBox::warning(0, tr("Невалидное значение"), tr("Не должно быть пустых значений"));
    }
}

void InputConditionRequestDialog::removeCondReqItem()
{
    if (ui->dataCondTableView->currentIndex().isValid()) {
        m_condReqDataModel->removeCondReqItem(ui->dataCondTableView->currentIndex());
    }
}

void InputConditionRequestDialog::applyCondReq()
{
    emit s_updateCondRequest(formTextFilter());
    this->close();
}

void InputConditionRequestDialog::updateRelationForType(int index)
{
    ui->cbSignCompare->clear();
    if (m_fieldInfo.contains(index)) {
        FieldType::Type fieldType = m_fieldInfo.value(ui->cbField->currentIndex(), FieldType::Type::UNKNOW);
        ui->cbSignCompare->addItems(FieldType::getRelationForType(fieldType));
    }
    ui->cbSignCompare->setCurrentIndex(0);
}

void InputConditionRequestDialog::updatePageWithInputWidget(int index)
{
    FieldType::Type fieldType = m_fieldInfo.value(index, FieldType::Type::UNKNOW);
    switch (fieldType) {
    case FieldType::Type::TEXT:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case  FieldType::Type::TIME:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case FieldType::Type::DATETIME:
        ui->stackedWidget->setCurrentIndex(2);
        break;
    default:
        break;
    }
}

bool InputConditionRequestDialog::checkNewCondReqValues() const
{
    return ui->cbField->currentIndex() != -1 && !ui->cbField->currentText().isEmpty()
            && ui->cbSignCompare->currentIndex() != -1 && !ui->cbSignCompare->currentText().isEmpty()
            && !getFilterValueForField().isEmpty();
}

QString InputConditionRequestDialog::getFilterValueForField() const
{
    switch (ui->stackedWidget->currentIndex()) {
    case 0:
        return ui->leTextValue->text();
    case 1:
        return ui->teDuration->time().toString("hh:mm:ss");
    case 2:
        return ui->dtDateTime->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    default:
        return QString();
    }
}

QString InputConditionRequestDialog::formTextFilter() const
{
    QList<CondReqDataModel::CondReqItem> filterData = m_condReqDataModel->getData();
    QString textFilter;
    for (auto it = filterData.cbegin(); it != filterData.cend(); ++it) {
        textFilter.append(QString(" \"%1\" %2 ").arg((*it).fieldName).arg((*it).relationShip));
        textFilter.append(FieldType::getTemplateForType((*it).fieldType).arg((*it).filterValue));
        if (it + 1 != filterData.cend()) {
            textFilter.append(" AND ");
        }
    }
    if (!textFilter.isEmpty()) {
        textFilter = "where " + textFilter;
    }
    return textFilter;
}
