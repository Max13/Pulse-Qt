#ifndef     LAUNCHER_HPP
# define    LAUNCHER_HPP

# include   <QCoreApplication>
# include   <QSettings>
# include   <QtService>

class Launcher : public QtService<QCoreApplication>
{
    private:
        static Launcher *m_instance;
        QSettings       &m_set;

    public:
        Launcher(int argc, char **argv, QSettings &settings);
        void start(void);

    public slots:
        static void startApp(void);
};

#endif // LAUNCHER_HPP
