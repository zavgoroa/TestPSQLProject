#ifndef INFOABOUTPSQLFIELDTYPE_H
#define INFOABOUTPSQLFIELDTYPE_H

#include <QString>
#include <QStringList>

class FieldType
{

public:
    enum class Type {
        TEXT,
        TIME,
        DATETIME,
        UNKNOW
    };

    static QStringList getRelationForType(FieldType::Type type);
    static QString getTemplateForType(FieldType::Type type);
};

#endif // INFOABOUTPSQLFIELDTYPE_H
