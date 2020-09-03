#ifndef SERVER_H
#define SERVER_H

#include "logline.h"

#include <QObject>
#include <QDateTime>

class QUdpSocket;

class Server : public QObject
{
    Q_OBJECT
    QUdpSocket* pSocket;
    const quint16 serverPort = 5514;
public:
    Server(QObject* parent = nullptr);
public slots:
    void onRead();

protected:
    LogLine parseDatagramData(QByteArray data) const;

signals:
    void newLine(LogLine*);
};

#endif
