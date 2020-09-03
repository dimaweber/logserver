#include  "server.h"
#include "model.h"
#include "fakelogclient.h"

#include <QApplication>
#include <QTableView>
#include <QHeaderView>

#include <QThread>

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    QTableView view;

    Server server(nullptr);
    Model model(nullptr);

    QThread serverThr;
    server.moveToThread(&serverThr);

    QThread modelThr;
    model.moveToThread(&modelThr);

    FakeLogClient clientLog(nullptr);
    QThread fakeLogThr;
    clientLog.moveToThread(&fakeLogThr);
    clientLog.connect(&fakeLogThr, &QThread::started, &clientLog, &FakeLogClient::onStart);

    model.connect (&server, &Server::newLine, &model, &Model::onNewLine);
  
    view.setModel(&model);

    serverThr.start();
    modelThr.start();
    fakeLogThr.start();

    view.show();
    QHeaderView* header = view.horizontalHeader();
    header->setStretchLastSection(true);

    app.connect (&view, &QTableView::destroyed, &app, &QApplication::quit);
    modelThr.connect(&view, &QTableView::destroyed, &modelThr, &QThread::quit);
    serverThr.connect(&view, &QTableView::destroyed, &serverThr, &QThread::quit);
    fakeLogThr.connect(&view, &QTableView::destroyed, &fakeLogThr, &QThread::quit);
    app.exec();

    modelThr.wait();
    serverThr.wait();
    fakeLogThr.wait();

    return 0;
}
