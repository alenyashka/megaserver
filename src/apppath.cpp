#include "apppath.h"

const QString AppPath::getDataPath()
{
    QString path;

    path.append(QCoreApplication::applicationDirPath() + QDir::separator());

    if (QFile::exists(path + "data.xml"))
    {
        return path;
    }
    path.clear();
    path.append("/var/lib/megaserver/");
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
