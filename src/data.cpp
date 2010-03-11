#include "data.h"
#include "table.h"
#include "saxhandler.h"
Data::Data()
{
}

void Data::setFileName(const QString &fileName)
{
    QTextStream out(stdout);
    file = new QFile(fileName);
    if (!file->exists())
    {
        out << "Error: File doest'n exist" << endl;
        return;
    }
    if (file->isReadable())
    {
        out << "Error: File could not be read" << endl;
        return;
    }
    if (file->isWritable())
    {
        out << "Error: File could not be write" << endl;
        return;
    }
    QXmlInputSource inputSource(file);
    QXmlSimpleReader reader;
    SaxHandler handler(this);
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    reader.parse(inputSource);
    file->close();
    out.flush();
}

QList<Table> Data::getTables() const
{
    return this->tables;
}

int Data::save()
{
    file->open(QIODevice::WriteOnly);
    QTextStream out(file);
    out << "<data>" << endl;
    for (int i = 0; i < tables.size(); ++i)
    {
        out << "    <table name = \"" << tables[i].getName()
        << "\" comment = \"" << tables[i].getComment()
        << "\">" << endl;
        QList<Record> records = tables[i].getRecords();
        for (int j = 0; j < records.size(); ++j)
        {
            QString readOnly = records[j].isReadOnly() ? "true" : "false";
            QString type = records[j].getType() == QVariant::Double ? "Double"
                           : records[j].getType() == QVariant::Int ? "Int"
                           : "String";
            out << "        <record title = \"" << records[j].getTitle()
            << "\" comment = \"" << records[j].getComment()
            << "\" readOnly = \"" << readOnly
            << "\" type = \"" << type
            << "\" value= \"" << records[j].getValue().toString()
            << "\" />" << endl;
        }
        out << "    </table>" << endl;
    }
    out << "</data>" << endl;
    file->close();
    return 0;
}

Table* Data::addTable(const QString &name, const QString &comment)
{
    Table table(name, comment, this);
    if (!tables.contains(table))
    {
        this->tables.append(table);
        return &tables[tables.indexOf(table)];
    }
    else
    {
        return NULL;
    }
}

bool Data::delTable(const QString &name)
{
    return this->tables.removeOne(Table(name));
}

Table* Data::getTable(const QString &name)
{
    int index = this->tables.indexOf(Table(name));
    if (index == -1) return NULL;
    return &this->tables[index];
}
