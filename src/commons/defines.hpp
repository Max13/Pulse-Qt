#ifndef     DEFINES_HPP
# define    DEFINES_HPP

# include   <QMessageLogContext>
# include   <QSettings>
# include   <QString>

# include  <stdlib.h>

# define    APP_NAME    QStringLiteral("Pulse")
# define    ORG_DOMAIN  QStringLiteral("rihan.fr")

int     myKill(pid_t pid, int signal);
void    installMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void    init(QSettings &set);
bool    isLoggedIn(void);

#endif
