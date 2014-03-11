#include "Configuration.h"

Configuration::Configuration(QString s)
{
    settings = new QSettings(s, QSettings::IniFormat);
}

Configuration::~Configuration()
{
    delete settings;
}


void Configuration::init()
{
    this->muted = settings->value("common/muted", 0).toBool();                                         // read mute settings
    this->interval = settings->value("common/interval", 120).toInt() * 60 * 1000;                      // read rest interval in minutes
    this->greetings= settings->value("common/greeting", QString("Hello, World!")).toString();          // read greetings

    this->database = settings->value("database/file", QString("mytasks.sqlite")).toString();           // read database
}

long Configuration::getInterval()
{
    return this->interval;
}

bool Configuration::isMuted()
{
    return this->muted;
}

QString Configuration::greeting()
{
    return this->greetings;
}

QString Configuration::getDatabaseName()
{
    return this->database;
}
