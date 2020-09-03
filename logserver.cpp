#include <QCoreApplication>
#include  "server.h"
#include "model.h"

#include <QThread>

int main (int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    
    Server server(nullptr);
    Model model(nullptr);

    QThread serverThr;
    server.moveToThread(&serverThr);

    QThread modelThr;
    model.moveToThread(&modelThr);

    model.connect (&server, &Server::newLine, &model, &Model::onNewLine);
  
    serverThr.start();
    modelThr.start();

    return app.exec();
}
