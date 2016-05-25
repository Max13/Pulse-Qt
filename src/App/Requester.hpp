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
# include   <QUuid>
# include   <QUrlQuery>

class Requester : public QObject
{
    Q_OBJECT
    private:
        QNetworkAccessManager   *m_netManager;
        QNetworkRequest         m_netRequest;
        QNetworkReply           *m_netReply;
        QSettings               &m_set;

    public:
        Requester(QSettings &set, QObject *parent = 0);

        void                    init(void);
        bool                    needSignup(void);

    public slots:
        bool                    signup(bool force = false);
        bool                    pulse(void);
};

#endif // REQUESTER_HPP
