#include <QApplication>
#include "Requester.hpp"

Requester::Requester(QSettings &set, QObject *parent) : QObject(parent), m_set(set)
{
    this->m_netManager = new QNetworkAccessManager(this);
}

bool    Requester::needSignup(void)
{
    return this->m_set.value(QStringLiteral("baseUrl")).isNull()
           || this->m_set.value(QStringLiteral("id")).isNull();
}

bool    Requester::signup(bool force)
{
    if (!this->needSignup() && !force) {
        return true;
    }

    QEventLoop      loop(this);
    QUrlQuery       body;

    body.addQueryItem(QStringLiteral("id"), QUuid::createUuid().toString().mid(1,36));
    body.addQueryItem(QStringLiteral("name"), QSysInfo::machineHostName());
    body.addQueryItem(QStringLiteral("description"), QSysInfo::prettyProductName());

    this->m_netRequest.setUrl(this->m_set.value(QStringLiteral("baseUrl")).toString());
    this->m_netRequest.setHeader(
                QNetworkRequest::ContentTypeHeader,
                QStringLiteral("application/x-www-form-urlencoded")
    );
    this->m_netRequest.setRawHeader("Accept", "application/json");

    this->m_netReply = this->m_netManager->post(
                           this->m_netRequest,
                           body.toString(QUrl::FullyEncoded).toUtf8()
    );
    this->connect(this->m_netManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    qDebug() << "<- Sign-up";
    loop.exec();

    if (this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        qDebug(
                    "-> KO: Status(%d, %s), Error(%s), Response(%s)",
                    this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                    this->m_netReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toStdString().c_str(),
                    this->m_netReply->errorString().toStdString().c_str(),
                    this->m_netReply->readAll().data()
        );
        return false;
    }

    qDebug() << "-> OK";
    this->m_set.setValue(
                QStringLiteral("id"),
                QJsonDocument::fromJson(this->m_netReply->readAll()).object()
                                                                    .value(QStringLiteral("id"))
                                                                    .toString()
    );
    return true;
}

bool    Requester::pulse(void)
{
    if (this->needSignup()) {
        qDebug() << "Need signup, can't pulse";
        return false;
    }

    QEventLoop      loop(this);
    static QString  url(QString("%1/%2/pulse").arg(
                            this->m_set.value(QStringLiteral("baseUrl")).toString(),
                            this->m_set.value(QStringLiteral("id")).toString()
    ));

    this->m_netRequest.setUrl(url);
    this->m_netReply = this->m_netManager->head(this->m_netRequest);
    this->connect(this->m_netManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    qDebug() << "<- Pulsed";
    loop.exec();

    if (this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        qDebug(
                    "-> KO: Status(%d, %s), Error(%s), Response(%s)",
                    this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
                    this->m_netReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toStdString().c_str(),
                    this->m_netReply->errorString().toStdString().c_str(),
                    this->m_netReply->readAll().data()
        );
        return false;
    }

    qDebug() << "-> OK";
    return true;
}
