#ifdef      CONSOLE
# include   <QCoreApplication>
#else
# include   <QApplication>
# include   <QIcon>
#endif

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageLogContext>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QtGlobal>

#include <stdio.h>

#include "defines.hpp"

int    myKill(pid_t pid, int signal)
{
    if (pid == 0) {
        return 0;
    }
#ifdef  Q_OS_WIN32
    if (signal == 0) {
        QProcess    kill;
        kill.start(
                    QStringLiteral("TASKLIST"),
                    QStringList() << QStringLiteral("/FI") << QString("PID eq %1").arg(pid) << QStringLiteral("/NH")
        );

        if (!kill.waitForStarted()) {
            qWarning("QProcess can't start \"%s\"", kill.program().toStdString().c_str());
            return -1;
        }
        if (!kill.waitForFinished()) {
            qWarning("\"%s\" didn't write anything", kill.program().toStdString().c_str());
            return -1;
        }

        return (kill.readAll().contains(QByteArray::number(pid)) ? 0 : -1);
    }

    QString cmd = QString("TASKKILL /F /PID %1").arg(pid);

    errno = system(cmd.toStdString().c_str());

    return (errno == 128 ? 0 : errno);
#else
    return kill(pid, signal);
#endif
}

void    killall(QSettings &set)
{
    qint64  svcPid(set.value(QStringLiteral("svc/pid")).toLongLong());
    qint64  appPid(set.value(QStringLiteral("app/pid")).toLongLong());

    myKill(svcPid, SIGTERM);
    myKill(appPid, SIGTERM);
    qDebug() << "-> Killed running Svc and App";
}

//void    installMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    //QByteArray  localMsg = msg.toLocal8Bit();
//    /*
//    static FILE *out = fopen(
//        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
//        .append("/").append(APP_NAME).append(".log").toStdString().c_str(),
//        "w"
//    );
//    */
//    //static FILE *out = fopen(APP_NAME.append(".log").toStdString().c_str(), "w");
//    QString     message(QDateTime::currentDateTime().toString(QStringLiteral("[yy-MM-dd HH:mm:ss] %'s': %'s' (%'s')\n")));

//#ifdef  QT_DEBUG
//    char    info[strlen(context.file) + strlen(line) + strlen(context.function) + 4];

//    sprintf(info, "%s:%u, %s", context.file, context.line, context.function);
//#else
//    Q_UNUSED(context);
//    const char  *info = "null";
//#endif

//    switch (type) {
//        case QtDebugMsg:
//            fprintf(out, message.toStdString().c_str(), "Debug", msg.toStdString().c_str(), info);
//            break;
//        case QtInfoMsg:
//            fprintf(out, message.toStdString().c_str(), "Info", msg.toStdString().c_str(), info);
//            break;
//        case QtWarningMsg:
//            fprintf(out, message.toStdString().c_str(), "Warning", msg.toStdString().c_str(), info);
//            break;
//        case QtCriticalMsg:
//            fprintf(out, message.toStdString().c_str(), "Critical", msg.toStdString().c_str(), info);
//            break;
//        case QtFatalMsg:
//            fprintf(out, message.toStdString().c_str(), "Fatal", msg.toStdString().c_str(), info);
//            abort();
//    }
//    fflush(out);
//}

void    installMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef  CONSOLE
    static QFile        file(QDir::tempPath().append('/').append(APP_NAME).append("_svc.log"));
#else
    static QFile        file(QDir::tempPath().append('/').append(APP_NAME).append("_app.log"));
#endif
    static QTextStream  out(&file);

    if (!file.isOpen() && !file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        abort();
    }

    out << QDateTime::currentDateTime().toString(QStringLiteral("[yy-MM-dd HH:mm:ss] "));
    switch (type) {
        case QtDebugMsg:
            out << "Debug: " << msg;
            break;
        case QtInfoMsg:
            out << "Info: " << msg;
            break;
        case QtWarningMsg:
            out << "Warning: " << msg;
            break;
        case QtCriticalMsg:
            out << "Critical: " << msg;
            break;
        case QtFatalMsg:
            out << "Fatal: " << msg;
            break;
    }

#ifdef  QT_DEBUG
    out << ' (' << context.file << ':' << context.line << ", " << context.function << ')';
#else
    Q_UNUSED(context);
#endif

    endl(out);
    if (type == QtFatalMsg) {
        abort();
    }
}

void    init(QSettings &set)
{
    Q_ASSERT(qApp != NULL);

    qDebug() << "Initiating Q(Core)Application";
    qApp->setApplicationName(APP_NAME);
    qApp->setOrganizationDomain(ORG_DOMAIN);
    qApp->setOrganizationName(ORG_DOMAIN);
#ifndef CONSOLE
    qApp->setWindowIcon(QIcon(QStringLiteral(":/icon/svg")));
#endif

    if (qApp->arguments().contains(QStringLiteral("--killall"))) {
        killall(set);
        exit(EXIT_SUCCESS);
    }

    if (qApp->arguments().contains(QStringLiteral("--reset"))) {
        qDebug() << "<- Reset invoked";
        killall(set);
        set.clear();
        set.sync();
        qDebug() << "-> Cleared settings";
    }

    qDebug() << "Settings:" << set.fileName();
}
