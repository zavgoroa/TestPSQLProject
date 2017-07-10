#ifndef INPUTCONDITIONREQUESTDIALOG_H
#define INPUTCONDITIONREQUESTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QStringBuilder>
#include <QMessageBox>
#include "condreqdatamodel.h"
#include "fieldtype.h"

namespace Ui {
class InputConditionRequestDialog;
}

class InputConditionRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputConditionRequestDialog(QWidget *parent = 0);
    ~InputConditionRequestDialog();

private slots:
    void addNewConReqItem();
    void removeCondReqItem();
    void applyCondReq();

    void updateRelationForType(int index);
    void updatePageWithInputWidget(int index);

signals:
    void s_updateCondRequest(const QString& textFilter);

protected:
    bool checkNewCondReqValues() const;
    QString getFilterValueForField() const;
    QString formTextFilter() const;

private:
    Ui::InputConditionRequestDialog *ui;
    CondReqDataModel *m_condReqDataModel;
    QMap<int, FieldType::Type> m_fieldInfo;
};


#endif // INPUTCONDITIONREQUESTDIALOG_H
