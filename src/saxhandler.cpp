#include "saxhandler.h"
#include <stdio.h>

SaxHandler::SaxHandler(Data *data)
{
    this->data = data;
    currentTable = NULL;
}
bool SaxHandler::startElement(const QString& /* namespaceURI */,
                              const QString& /* localName */,
                              const QString &qName,
                              const QXmlAttributes &attributes)
{
    if (qName == "table")
    {
        if (!currentTable)
        {
            QString name = attributes.value("name");
            QString comment = attributes.value("comment");
            currentTable = data->addTable(name, comment);
        }
        else
        {
            return false;
        }
    }
    else if (qName == "record")
    {
        if (currentTable)
        {
            QString title = attributes.value("title");
            QString comment = attributes.value("comment");
            bool readOnly = attributes.value("readOnly") == "true" ?
                            true : false;
            QString str = attributes.value("type");
            QVariant::Type type = str == "double" ? QVariant::Double
                                  : str == "int" ? QVariant::Int
                                  : QVariant::String;
            QVariant value(attributes.value("value"));
            currentTable->addRecord(title, comment, readOnly, type, value);
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool SaxHandler::endElement(const QString& /* namespaceURI */,
                            const QString& /* localName */,
                            const QString &qName)
{
    if (qName == "table")
    {
        currentTable = NULL;
    }
    return true;
}
