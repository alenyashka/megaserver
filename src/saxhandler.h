#ifndef SAXHANDLER_H
#define SAXHANDLER_H

#include <QtXml>
#include "data.h"
#include "table.h"
#include "util.h"

class SaxHandler : public QXmlDefaultHandler
{
public:
    SaxHandler(Data *data);
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);

private:
    Data *data;
    Table *currentTable;
};

#endif // SAXHANDLER_H
