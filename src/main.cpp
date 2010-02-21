#include <QtCore>
#include <QCoreApplication>
#include "src/data.h"
#include "src/table.h"

int main(int , char**)
{
    QTextStream out(stdout);
    Data data("data.xml");
    data.delTable("table2");
    Table *t = data.addTable("table4", "comment4");
    if (t)
    {
        t->addRecord("rec1", "com1", true, QVariant::Double, 1.0);
        t->addRecord("rec2", "com2", true, QVariant::Int, 2);
    }
    t = data.getTable("table1");
    if (t)
    {
        t->addRecord("rec2", "com2", false, QVariant::String, "bla-bla-bla");
    }
    data.save();
    return 0;
}
