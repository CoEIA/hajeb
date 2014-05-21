#ifndef APPLICATIONSTATUS_H
#define APPLICATIONSTATUS_H

#include <QObject>
#include <QDateTime>

class ApplicationStatus : public QObject
{
    Q_OBJECT
public:
    enum Status {
        TRIAL,
        BLOCK,
        END_TRIAL,
        FULL
    };

    ApplicationStatus(QObject* parent=0);
    Status applicationStatus();
    static void registerApplication(QString key,bool registerOnline);
    static bool isCleanVersion();
    static QDateTime installDate();
    static const int ALLOWED_DAYS;
    //static int allowedDays();
private slots:
    void updateCurrentTimeDate();
private:
    void readSettings();
    QDateTime m_installDateTime;
    QDateTime m_prevDateTime;
    bool m_isRegistered;
};

#endif // APPLICATIONSTATUS_H
