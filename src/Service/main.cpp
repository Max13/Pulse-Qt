#include <QDebug>
#include <QSettings>
#include <QStandardPaths>

#include "../commons/defines.hpp"
#include "Launcher.hpp"

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    //qInstallMessageHandler(installMessageHandler);
#endif

    QSettings   set(QSettings::SystemScope, ORG_DOMAIN, APP_NAME);
    Launcher    L(argc, argv, set);

    qDebug("Service start as \"%s\"", qgetenv("USERNAME").constData());

    return L.exec();
}
