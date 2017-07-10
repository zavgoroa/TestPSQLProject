#include "fieldtype.h"

QStringList FieldType::getRelationForType(FieldType::Type type)
{
    switch (type) {
    case FieldType::Type::TEXT:
        return QStringList() << "=" << "LIKE";
    case FieldType::Type::DATETIME: case FieldType::Type::TIME:
        return QStringList() << "=" << "<" << ">" << "<=" << ">=" << "!=";
    default:
        return QStringList() << "=";
    }
}

QString FieldType::getTemplateForType(FieldType::Type type)
{
    switch (type) {
    case FieldType::Type::TEXT: case FieldType::Type::DATETIME: case FieldType::Type::TIME:
        return QString("'%1'");
    default:
        return QString("%1");
    }
}
