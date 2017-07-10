#ifndef DESCSORTDATAMODEL_H
#define DESCSORTDATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QDebug>
#include "filter.h"
#include "fieldtype.h"

class CondReqDataModel : public QAbstractTableModel
{
public:
    struct CondReqItem {
        QString fieldName;
        QString relationShip;
        QString filterValue;
        FieldType::Type fieldType;
    };

    CondReqDataModel(QObject *parent = nullptr);
    ~CondReqDataModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

    void addCondReqItem(CondReqItem condReqItem);
    void removeCondReqItem(const QModelIndex& index);

    QList<CondReqDataModel::CondReqItem> getData() const;
    CondReqDataModel::CondReqItem getData(int row) const;
    void clear();

private:
    QList<CondReqItem> m_data;
};

#endif // DESCSORTDATAMODEL_H
