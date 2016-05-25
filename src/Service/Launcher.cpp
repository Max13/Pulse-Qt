#include <QDebug>
#include <QProcess>
#include <QString>
#include <QTimer>

#include <unistd.h>

#include "Launcher.hpp"
#include "../commons/defines.hpp"

#ifdef   Q_OS_WIN32
# define APP_EXE    "Pulse-App.exe"
#else
# define APP_EXE    "Pulse-App"
#endif

Launcher *Launcher::m_instance = NULL;

Launcher::Launcher(int argc, char **argv, QSettings &settings)
    : QtService<QCoreApplication>(argc, argv, settings.applicationName()), m_set(settings)
{
    this->setStartupType(QtServiceController::AutoStartup);
    Launcher::m_instance = this;
}

void    Launcher::start(void)
{
    init(this->m_set);

    qint64  svcPid(this->m_set.value(QStringLiteral("svc/pid")).toLongLong());

    if (svcPid && myKill(svcPid, 0) == 0) {
        qDebug() << "Service already running. Abort.";
        exit(EXIT_SUCCESS);
    }

    this->m_set.setValue(QStringLiteral("svc/pid"), qApp->applicationPid());

    QTimer::singleShot(0, &Launcher::startApp);
}

void    Launcher::startApp(void)
{
    qint64  appPid(Launcher::m_instance->m_set.value(QStringLiteral("app/pid")).toLongLong());

    if (appPid && myKill(appPid, 0) == 0) {
        qDebug() << "App is running. Waiting.";
    } else {
        qDebug() << "<- Starting app detached.";
        if (QProcess::startDetached(
                    qApp->applicationDirPath().append('/').append(APP_EXE),
                    QStringList(QStringLiteral("--service")),
                    QString(),
                    &appPid
            )
        ) {
            qDebug("-> App started with PID(%lld)", appPid);
        } else {
            qDebug("-> Couldn't start App. Waiting.");
        }
    }

    QTimer::singleShot(30000, &Launcher::startApp);
}
