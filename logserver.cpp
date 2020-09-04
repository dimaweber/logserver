#include "server.h"
#include "model.h"
#include "fakelogclient.h"
#include "gui.h"

#include <QApplication>

#include <QThread>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<LogLine::Fields>("LogLine::Fields");
    
    Server server(nullptr);
    Model model(nullptr);

    QThread serverThr;
    server.moveToThread(&serverThr);

    QThread modelThr;
    model.moveToThread(&modelThr);

    FakeLogClient fakeLog(nullptr);
    QThread fakeLogThr;
    fakeLog.moveToThread(&fakeLogThr);
    fakeLog.connect(&fakeLogThr, &QThread::started, &fakeLog, &FakeLogClient::onStart);

    model.connect (&server, &Server::newLine, &model, &Model::onNewLine);
  
    Gui gui(&model);

    gui.show();

    app.connect (&fakeLog, &FakeLogClient::done, [&model]()
    {
        qDebug() << "Model possible priorities: " << model.possibleValues[LogLine::Priority];
        qDebug() << "Model possible hosts: " << model.possibleValues[LogLine::HostName];
        qDebug() << "Model possible modules: " << model.possibleValues[LogLine::ProcessName];
        qDebug() << "log timestamps range: "  << model.timestampRanges[LogLine::LogTimestamp].first << " - " << model.timestampRanges[LogLine::LogTimestamp].second;
    });

    modelThr.connect(&app, &QApplication::aboutToQuit, &modelThr, &QThread::quit);
    serverThr.connect(&app, &QApplication::aboutToQuit, &serverThr, &QThread::quit);
    fakeLogThr.connect(&app, &QApplication::aboutToQuit, &fakeLogThr, &QThread::quit);
    fakeLog.connect(&fakeLogThr, &QThread::finished, &fakeLog, &FakeLogClient::onStop);

    serverThr.start();
    modelThr.start();
    fakeLogThr.start();

    int retVal = app.exec();

    modelThr.wait();
    serverThr.wait();
    fakeLogThr.wait();

    return retVal;
}
