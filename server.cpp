#include "server.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include  <QRegExp>

#include <QDebug>
#include <iomanip>

Server::Server(QObject* parent):QObject(parent), pSocket(nullptr)
{
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

std::ostream& operator<<(std::ostream& stream, const LogLine& line)
{
    auto output = [&stream](const char*name, const QString& value)
    {
#if MULTILINE_OUTPUT
        stream <<  '\t' << std::setw(10) << std::left << name  << ':' << qPrintable(value) << std::endl;
#else
        Q_UNUSED(name);
        stream <<  std::setw(10) <<  std::left << qPrintable(value);
#endif
    };
    output("id", QString::number(line.id));
    output("date", line.serverDateTime.toString("hh:mm:ss.zzz"));
    output("host", line.hostName);
    output("process",  line.processName);
    //if (line.exLog)
    {
        output("filename", line.fileName);
        output("linenum", QString::number(line.lineNum));
        output("function", line.functionName);
    }
    output("message", line.message);
    return stream;
}

LogLine Server::parseDatagramData(QByteArray data) const
{
    QString str = QString::fromUtf8(data.constData());
    LogLine line;
    QRegExp lineRegExp("<([0-9]{1,4})>([a-zA-Z]{3}\\s+[0-9]{1,2}\\s+[0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2})\\s*([a-zA-z]*)\\s*([a-zA-Z_]*):(.*)");
    QRegExp extendedLogging("\\[(.*):([0-9]*)\\s+(.*)\\] (.*)");

    if (lineRegExp.indexIn(str) != -1)
    {
        line.id = lineRegExp.cap(1).toInt();
        line.logDateTime = QDateTime::fromString(lineRegExp.cap(2).simplified(), "MMM d HH:mm:ss");
        line.hostName = lineRegExp.cap(3);
        line.processName = lineRegExp.cap(4);
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
