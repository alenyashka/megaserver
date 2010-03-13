#ifndef APPPATH_H
#define APPPATH_H

#include <QCoreApplication>
#include <QString>
#include <QDir>

class AppPath
{
public:
    static const QString getDataPath();
    static const QString getSettingsPath();
};

#endif // APPPATH_H
