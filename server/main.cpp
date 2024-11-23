#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    Server server;
    server.startServer(12345); // Use port 12345 for communication

    return app.exec();
}
