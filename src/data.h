#ifndef DATA_H
#define DATA_H

#include <QtCore>
#include <QtXml>

class Table;

class Data
{
public:
    Data(const QString&);
    QList<Table> getTables() const;
    Table* getTable (const QString&);
    Table* addTable(const QString&, const QString&);
    bool delTable(const QString&);
    int save();
private:
    QList<Table> tables;
    QFile *file;
};

#endif // DATA_H
