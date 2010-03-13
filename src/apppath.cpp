#include "apppath.h"

const QString AppPath::getDataPath()
{
    QString path;

    path.append(QCoreApplication::applicationDirPath() + QDir::separator() +
                "data.xml");

    if (QFile::exists(path))
    {
        return path;
    }
    path.clear();
    path.append("/var/lib/megaserver/data.xml");
    return path;
}

const QString AppPath::getSettingsPath()
{
    QString path;
    path.append(QCoreApplication::applicationDirPath() + QDir::separator() +
                "megaserver.conf");

    if (QFile::exists(path))
    {
        return path;
    }
    path.clear();
    path.append("/etc/megaserver.conf");
    return path;
}
