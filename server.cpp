#include "server.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include  <QRegExp>

#include <QDebug>
#include <iomanip>

Server::Server(QObject* parent):QObject(parent), pSocket(nullptr)
{
    logLocale = new QLocale(QLocale::English, QLocale::UnitedStates);
    pSocket = new QUdpSocket(this);

    if (!pSocket->bind(serverPort))
    {
        qWarning() << "Fail to bing udp port " << serverPort;    
    }
    else
    {
        qDebug() << "udp port " << serverPort << " bind -- OK";
    }

    connect (pSocket, &QUdpSocket::readyRead, this, &Server::onRead);
}
    
void Server::onRead()
{
    while (pSocket->hasPendingDatagrams())
    {
        QNetworkDatagram dgram = pSocket->receiveDatagram();
            
        LogLine* line = new LogLine(parseDatagramData(dgram.data()));
              
        emit newLine(line);
    }
}

LogLine Server::parseDatagramData(QByteArray data) const
{
    QString str = QString::fromUtf8(data.constData());
    LogLine line;
    QRegExp lineRegExp("<([0-9]{1,4})>([a-zA-Z]{3}\\s+[0-9]{1,2}\\s+[0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2})\\s*([a-zA-z]*)\\s*([a-zA-Z_]*):(.*)");
    QRegExp extendedLogging("\\[(.*):([0-9]*)\\s+(.*)\\] (.*)");

    if (lineRegExp.indexIn(str) != -1)
    {
        line.priority = lineRegExp.cap(1).toInt();
        line.logDateTime = logLocale->toDateTime(lineRegExp.cap(2).simplified(), "MMM d HH:mm:ss");
        line.hostName = lineRegExp.cap(3);
        line.processName = lineRegExp.cap(4);
        if (line.processName.isEmpty())
        {
            line.processName = line.hostName;
            line.hostName = "localhost";
        }
        line.message = lineRegExp.cap(5);

        if (extendedLogging.indexIn(line.message) != -1)
        {
            line.exLog = true;
            line.fileName = extendedLogging.cap(1);
            line.lineNum = extendedLogging.cap(2).toInt();
            line.functionName = extendedLogging.cap(3);
            line.message = extendedLogging.cap(4);
        }
        else
        {
            line.exLog = false;
            line.fileName.clear();
            line.lineNum = 0;
            line.functionName.clear();
        }
    }
    line.serverDateTime = QDateTime::currentDateTime();

    return line;
}
