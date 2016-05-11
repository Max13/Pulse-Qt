#ifndef     REQUESTER_HPP
# define    REQUESTER_HPP

# include   <QByteArray>
# include   <QEventLoop>
# include   <QInputDialog>
# include   <QJsonDocument>
# include   <QJsonObject>
# include   <QNetworkAccessManager>
# include   <QNetworkReply>
# include   <QNetworkRequest>
# include   <QObject>
# include   <QSettings>
# include   <QSysInfo>
# include   <QTimer>
# include   <QUuid>
# include   <QUrlQuery>

class Requester : public QObject
{
    Q_OBJECT
    private:
        bool                    m_new;
        QNetworkAccessManager   *m_netManager;
        QNetworkRequest         m_netRequest;
        QNetworkReply           *m_netReply;
        QSettings               &m_set;
        QString                 m_baseUrl;
        QString                 m_uuid;
        QTimer                  m_timer;

    public:
        Requester(QSettings &set, QObject *parent = 0);

        bool                    isNew(void) {return this->m_new;}

        bool                    init(void);
        void                    process(void);

    public slots:
        void                    sendPulse(void);
};

#endif // REQUESTER_HPP
