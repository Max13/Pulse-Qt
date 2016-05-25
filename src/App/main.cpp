#include <QApplication>
#include <QDebug>
#include <QSettings>

#include "../commons/defines.hpp"
#include "Controllers/MyController.hpp"

int main(int argc, char *argv[])
{
#ifndef QT_DEBUG
    qInstallMessageHandler(installMessageHandler);
#endif

    QApplication    a(argc, argv);
    QSettings       set(QSettings::SystemScope, ORG_DOMAIN, APP_NAME);
    MyController    *control;

    qDebug("Application start as \"%s\"", qgetenv("USERNAME").constData());
    init(set);
    control = new MyController(set);
    control->init();

    return a.exec();
}
