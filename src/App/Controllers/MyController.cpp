#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>

#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "../commons/defines.hpp"

#include "MyController.hpp"


MyController::MyController(QSettings &set, QObject *parent) : QObject(parent), m_set(set)
{
    this->m_req = new Requester(this->m_set, this);
    this->m_tray = new QSystemTrayIcon(qApp->windowIcon(), this);
}

void    MyController::killRunning(void)
{
    qint64 prevPid = this->m_set.value(QStringLiteral("app/pid")).toLongLong();
    int    killStatus;

    if (myKill(prevPid, 0) != 0) {
        qDebug() << "App doesn't seem to be running";
        return;
    }

    qDebug("<- Killing previous PID(%lld) brutally", prevPid);
    if ((killStatus = myKill(prevPid, SIGTERM)) != 0) {
        qDebug("-> KO, errno(%d). Exiting.", killStatus);
        exit(EXIT_FAILURE);
    }
    qDebug() << "-> OK";
    sleep(1);
}

void    MyController::init(void)
{
    this->killRunning();

    bool    isService = qApp->arguments().contains(QStringLiteral("--service"));

    this->m_set.setValue(QStringLiteral("app/service"), isService);
    this->m_set.setValue(QStringLiteral("app/pid"), qApp->applicationPid());

    this->m_tray->show();
    if (this->m_req->needSignup()) {
        qDebug() << "Need signup. So signing-up";
        if (isService) {
            qDebug() << "Is service, abort.";
            exit(EXIT_SUCCESS);
        }

        QString url;
        do {
            bool    ok;
            url = QInputDialog::getText(
                      0,
                      tr("Need URL"),
                      tr("What is the base URL of your API ?"),
                      QLineEdit::Normal,
                      QStringLiteral("http://api.example.com/heartbeats"),
                      &ok
            );

            if (!ok) {
                this->m_tray->hide();
                exit(EXIT_SUCCESS);
            }
        } while (url.isEmpty());

        this->m_set.setValue(QStringLiteral("baseUrl"), url);

        if (!this->m_req->signup()) {
            QMessageBox::critical(0, tr("Error"), tr("Can't signup. Look at the log."));
            this->m_tray->hide();
            exit(EXIT_FAILURE);
        }
    }

    connect(&this->m_timer, &QTimer::timeout, this->m_req, &Requester::pulse);
    this->m_timer.start(5 * 60 * 1000);
    qDebug() << "Pulse timer started";
    this->m_req->pulse();
}

