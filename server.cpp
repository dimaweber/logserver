#include "server.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include  <QRegExp>

#include <QDebug>
#include <iomanip>

Server::Server(QObject* parent):QObject(parent), pSocket(nullptr), logLocale(nullptr)
{
    logLocale = new QLocale(QLocale::English, QLocale::UnitedStates);
    pSocket = new QUdpSocket(this);

    if (!pSocket->bind(serverPort))
    {
        qWarning() << "Fail to bind udp port " << serverPort;
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
            
        LogLine* line = new LogLine(dgram.data(), logLocale);
              
        emit newLine(line);
    }
}


