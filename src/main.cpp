#include <QtCore>
#include <QCoreApplication>
#include "megaserver.h"

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QCoreApplication app(argc, argv);
    MegaServer server;
    if (!server.listen(QHostAddress::LocalHost, 6178))
    {
        out << "Can't bind to port" << endl;
        return 1;
    }
    return app.exec();
}
