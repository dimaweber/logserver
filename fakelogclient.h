#ifndef FAKELOGCLIENT_H
#define FAKELOGCLIENT_H

#include <QObject>
#include <QStringList>
#include <QUdpSocket>
#include <QTimer>

class FakeLogClient  : public QObject
{
    Q_OBJECT
    QUdpSocket* pSocket;
    QStringList lines;
    QTimer* timer;
    int index;
public:
    FakeLogClient(QObject* parent = nullptr);
    ~FakeLogClient();

public slots:
    void onStart();
    void onTimer();

signals:
    void done();
};

#endif // FAKELOGCLIENT_H
