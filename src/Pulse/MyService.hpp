#ifndef     MYSERVICE_HPP
# define    MYSERVICE_HPP

# include   <QApplication>
# include   <QSettings>
# include   <QString>
# include   <QSystemTrayIcon>
# include   <QtService>

# include   "Requester.hpp"

class MyService : public QtService<QApplication>
{
    private:
        QSettings       *m_set;
        QSystemTrayIcon *m_tray;
        Requester       *m_req;

    public:
        MyService(int argc, char **argv, const QString &name);
        void start(void);
};

#endif // MYSERVICE_HPP
