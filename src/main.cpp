#include <QtCore>
#include <QCoreApplication>
#include "megaserver.h"
#include <sys/syslog.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    MegaServer server;
    if (!server.listen(QHostAddress::LocalHost, 6178))
    {
        syslog(LOG_ERR, "Can't bind to port");
        return 1;
    }
    return app.exec();
}
