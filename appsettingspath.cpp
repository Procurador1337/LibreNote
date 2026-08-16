#include "appsettingspath.h"

#include <QStandardPaths>
#include <QDir>

QString appSettingsPath()
{
    QString dir = QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation
        );

    QDir().mkpath(dir);

    return dir + "/settings.txt";
}