#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

class ServerHandler : public QObject
{
  Q_OBJECT
public:
    ServerHandler(QObject* parent=0);
    QString verifyKey(QString key);
    QString registerKeyOnline(QString key,QString ip);
    QString getVersionNumberFile();
    //QByteArray getUpdater();

private slots:
    void replyFinished(QNetworkReply *np);
private:
    QNetworkAccessManager* manager;
    QString result;
    QByteArray resultBytes;
    bool info;
};

#endif // SERVERHANDLER_H
