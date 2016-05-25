#ifndef     MYCONTROLLER_HPP
# define    MYCONTROLLER_HPP

# include   <QObject>
# include   <QSettings>
# include   <QSystemTrayIcon>
# include   <QTimer>

# include   "Requester.hpp"

class MyController : public QObject
{
    Q_OBJECT
    private:
        QSettings       &m_set;
        QSystemTrayIcon *m_tray;
        QTimer          m_timer;
        Requester       *m_req;

    public:
        MyController(QSettings &set, QObject *parent = 0);

        void    killRunning(void);
        void    init(void);

    signals:

    public slots:
};

#endif // MYCONTROLLER_HPP
