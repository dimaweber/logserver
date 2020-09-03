#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDateTime>
#include <iostream>

class QUdpSocket;

struct LogLine
{
    quint16 id;
    QDateTime logDateTime;
    QDateTime serverDateTime;
    QString hostName;
    QString processName;
    QString fileName;
    quint32 lineNum;
    QString functionName;
    bool exLog;
    QString message;
};

std::ostream& operator<< (std::ostream& stream, const LogLine& line);

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
