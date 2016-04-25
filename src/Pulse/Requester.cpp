#include <QApplication>
#include "Requester.hpp"

Requester::Requester(QObject *parent) : QObject(parent)
{
    this->m_netManager = new QNetworkAccessManager(this);
    this->m_baseUrl = this->m_set.value("baseUrl").toString();
    this->m_uuid = this->m_set.value("id").toString();
    this->m_new = this->m_uuid.isEmpty();
}

bool    Requester::init(void)
{
    bool        ok;
    QEventLoop  loop(this);
    QUrlQuery   body;

    this->m_baseUrl = QInputDialog::getText(
        0,
        tr("Nouveau coeur"),
        tr("Quel est l'adresse de base pour envoyer les pulsations ?"),
        QLineEdit::Normal,
        QStringLiteral("http://api.example.com/heartbeats"),
        &ok
    );
    if (!ok) {
        return false;
    }

    body.addQueryItem(QStringLiteral("id"), QUuid::createUuid().toString().mid(1,36));
    body.addQueryItem(QStringLiteral("name"), QSysInfo::machineHostName());
    body.addQueryItem(QStringLiteral("description"), QSysInfo::prettyProductName());

    this->m_netRequest.setUrl(this->m_baseUrl);
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
    loop.exec();

    if (this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        qDebug() << "HTTP Status:"
                 << this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << this->m_netReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Error string:" << this->m_netReply->errorString();
        qDebug() << "Response:" << this->m_netReply->readAll();
        exit(EXIT_FAILURE);
    }

    this->m_uuid = QJsonDocument::fromJson(this->m_netReply->readAll()).object()
                                                                       .value(QStringLiteral("id"))
                                                                       .toString();
    this->m_set.setValue(QStringLiteral("baseUrl"), this->m_baseUrl);
    this->m_set.setValue(QStringLiteral("id"), this->m_uuid);
    this->m_set.sync();
    delete this->m_netReply;
    return true;
}

void    Requester::process(void)
{
    this->connect(&this->m_timer, &QTimer::timeout, this, &Requester::sendPulse);
    this->m_timer.start(10 * 60 * 1000);
}

void    Requester::sendPulse(void)
{
    QEventLoop      loop(this);
    static QString  url(QString("%1/%2/pulse").arg(this->m_baseUrl, this->m_uuid));

    this->m_netRequest.setUrl(url);
    this->m_netReply = this->m_netManager->head(this->m_netRequest);
    this->connect(this->m_netManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
        qDebug() << "HTTP Status:"
                 << this->m_netReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << this->m_netReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Error string:" << this->m_netReply->errorString();
        qDebug() << "Response:" << this->m_netReply->readAll();
        qDebug() << "Pulse KO." << endl;
    } else {
        qDebug() << "Pulsed.";
    }
}
