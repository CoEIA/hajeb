#include <QSettings>
#include <QDebug>
#include "applicationstatus.h"

const int ApplicationStatus::ALLOWED_DAYS = 356;

ApplicationStatus::ApplicationStatus(QObject *parent):
    QObject(parent)
{
    readSettings();
}

void ApplicationStatus::readSettings() {
    QSettings settings(QSettings::SystemScope,"HajebSystem");
    if (!settings.contains("registered-online")) {
        settings.setValue("installDateTime",QDateTime::currentDateTime());
        settings.setValue("prevDateTime",QDateTime::currentDateTime());
        settings.setValue("sn","");
        settings.setValue("registered-online",false);
    }

    m_installDateTime = settings.value("installDateTime").toDateTime();
    m_prevDateTime = settings.value("prevDateTime").toDateTime();
    m_isRegistered = settings.value("registered-online").toBool();
    QString sn = settings.value("sn").toString();

    if (m_isRegistered)
        qDebug() << "Licensed version: " << sn;
}

ApplicationStatus::Status ApplicationStatus::applicationStatus() {
    if (m_isRegistered)
        return FULL;

    if (QDateTime::currentDateTime() < m_prevDateTime)
        return BLOCK;

    if (QDateTime::currentDateTime() < m_installDateTime.addDays(ALLOWED_DAYS))
        return TRIAL;
    else
        return END_TRIAL;
}

void ApplicationStatus::updateCurrentTimeDate() {
    QSettings settings(QSettings::SystemScope,"HajebSystem");
    settings.setValue("prevDateTime",QDateTime::currentDateTime());
}

void ApplicationStatus::registerApplication(QString key,bool registerOnline) {
    QSettings settings(QSettings::SystemScope,"HajebSystem");
    settings.setValue("registered-online",registerOnline);
    settings.setValue("sn",key);
}

bool ApplicationStatus::isCleanVersion() {
    QSettings settings(QSettings::SystemScope,"HajebSystem");
    return settings.value("registered-online").toBool();
}

QDateTime ApplicationStatus::installDate() {
    QSettings settings(QSettings::SystemScope,"HajebSystem");
    return settings.value("installDateTime").toDateTime();
}



