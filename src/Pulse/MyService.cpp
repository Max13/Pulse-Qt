#include "MyService.hpp"

MyService::MyService(int argc, char **argv, const QString &name)
    : QtService<QApplication>(argc, argv, name)
{
    this->setStartupType(QtServiceController::AutoStartup);
}

void    MyService::start(void)
{
    this->m_tray = new QSystemTrayIcon(QIcon(QStringLiteral(":/icon/svg")), qApp);

    qApp->setApplicationName(QStringLiteral("Pulse"));
    qApp->setOrganizationDomain(QStringLiteral("rihan.fr"));
    qApp->setOrganizationName(qApp->organizationDomain());
    qApp->setWindowIcon(this->m_tray->icon());
    this->m_tray->show();

    this->m_set = new QSettings(
                      QSettings::SystemScope,
                      qApp->organizationName(),
                      qApp->applicationName()
    );
    if (qApp->arguments().contains(QStringLiteral("--reset"), Qt::CaseInsensitive)) {
        this->m_set->clear();
    }

    this->m_req = new Requester(*this->m_set);
    qApp->connect(qApp, &QApplication::aboutToQuit, this->m_req, &Requester::deleteLater);
    if (this->m_req->isNew()) {
        qDebug() << "New Heart !";
        if (!this->m_req->init()) {
            this->m_tray->hide();
            exit(EXIT_FAILURE);
        }
        qDebug() << "Heart registered.";
    } else {
        this->m_req->sendPulse();
    }
    qDebug() << "Processing timer.";
    this->m_req->process();
}
