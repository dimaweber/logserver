#include "fakelogclient.h"
#include <QFile>

FakeLogClient::FakeLogClient(QObject* parent)
    :QObject(parent)
{
    pSocket = new QUdpSocket(this);

    QFile file("fake.log");
    if (file.open(QFile::ReadOnly))
    {
        while (!file.atEnd())
        {
            QByteArray line;
            line = file.readLine().trimmed();
            lines.append( QString::fromUtf8(line));
        }
    }
    else
        qWarning() << "fail to read log file";
    index = 0;
}

FakeLogClient::~FakeLogClient()
{
    pSocket->close();
    delete pSocket;
}

void FakeLogClient::onStart()
{
    pSocket->connectToHost("localhost", 5514, QAbstractSocket::WriteOnly);
    if (pSocket->waitForConnected())
        qDebug() << "fake server bind ok";
    else
        qWarning() << "fake server fail to bind";
/*    if (pSocket->open(QUdpSocket::WriteOnly))
        qDebug() << "fake server socket open ok";
    else
        qWarning() << "fake server socket open fail";
*/

    timer = new QTimer(this);
    connect (timer, &QTimer::timeout, this, &FakeLogClient::onTimer);
    timer->setSingleShot(true);
    int delay = (qrand() % 100) * 10;
    timer->start(delay);
}

void FakeLogClient::onTimer()
{
    if (index < lines.count())
    {
        pSocket->write( lines.at(index++).toUtf8());
    }

    if (index >= lines.count())
    {
        emit done();
        pSocket->close();
    }
    else
    {
        int delay = (qrand() % 100) * 10;
        timer->start(delay);
    }

}
