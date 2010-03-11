#ifndef DATA_H
#define DATA_H

#include <QtCore>
#include <QtXml>
#include "singleton.h"

class Table;

class Data: public Singleton<Data>
{
protected:
    Data();
    friend class Singleton<Data>;
public:
    void setFileName(const QString&);
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
