#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QSettings>
#include <iostream>

class Configuration
{
    public:
        Configuration(QString settings = "settings.ini");
        virtual ~Configuration();

        void init();

        bool isMuted();
        long getInterval();
        QString greeting();
        QString getDatabaseName();
    private:
        QSettings *settings;

        bool muted;          // is it muted when alarming?
        long interval;       // how long should it lock screen and force user to rest
        long prenotify;      // how long should it lock screen and force user to rest
        QString greetings;
        QString database;
};

#endif // CONFIGURATION_H
