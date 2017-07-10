#include "condreqdatamodel.h"

CondReqDataModel::CondReqDataModel(QObject *parent):QAbstractTableModel(parent)
{
}

CondReqDataModel::~CondReqDataModel()
{
    clear();
}

int CondReqDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

int CondReqDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CondReqDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    switch (section) {
    case 0:
        return QVariant("Поле");
    case 1:
        return QVariant("Отношение");
    case 2:
        return QVariant("Значение фильтра");
    default:
        return QVariant();
    }
}

QVariant CondReqDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return QVariant(m_data.at(index.row()).fieldName);
        case 1:
            return QVariant(m_data.at(index.row()).relationShip);
        case 2:
            return QVariant(m_data.at(index.row()).filterValue);
        }
    }
    return QVariant();
}

void CondReqDataModel::addCondReqItem(CondReqItem condReqItem)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.push_back(condReqItem);
    endInsertRows();
}

void CondReqDataModel::removeCondReqItem(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_data.removeAt(index.row());
    endRemoveRows();
}

QList<CondReqDataModel::CondReqItem> CondReqDataModel::getData() const
{
    return m_data;
}

CondReqDataModel::CondReqItem CondReqDataModel::getData(int row) const
{
    return m_data.at(row);
}

void CondReqDataModel::clear()
{
    m_data.clear();
}
