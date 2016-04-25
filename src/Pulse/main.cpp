#include <QApplication>
#include <QSettings>
#include <QString>
#include <QSystemTrayIcon>

#include "Requester.hpp"

int main(int argc, char *argv[])
{
    QApplication    a(argc, argv);
    QSettings       *set;
    QSystemTrayIcon s(QIcon(QStringLiteral(":/icon/svg")), &a);
    Requester       *r;

    a.setApplicationName(QStringLiteral("Pulse"));
    a.setOrganizationDomain(QStringLiteral("rihan.fr"));
    a.setWindowIcon(s.icon());
    s.show();

    Q_UNUSED(set);
    set = new QSettings;
    if (a.arguments().contains(QStringLiteral("--reset"), Qt::CaseInsensitive)) {
        set->clear();
    }

    r = new Requester;
    a.connect(&a, &QApplication::aboutToQuit, r, &Requester::deleteLater);
    if (r->isNew()) {
        qDebug() << "New Heart !";
        if (!r->init()) {
            delete r;
            s.hide();
            exit(EXIT_FAILURE);
        }
        qDebug() << "Heart registered.";
    } else {
        r->sendPulse();
    }
    qDebug() << "Processing timer.";
    r->process();

    return a.exec();
}
