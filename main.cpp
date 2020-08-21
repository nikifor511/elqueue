//#include "mainwidget.h"
#include "main.h"
#include "QDebug"

#include <QApplication>
#include <QTextStream>

QTextStream cout(stdout);
QTextStream cin(stdin);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    MainWidget w;
//    w.show();

    DbAdapter *db = new DbAdapter();
    QThread *serverThread = new QThread();
    ElServer *server = new ElServer(58583);
    QObject::connect(server, &ElServer::getFreeTasks, db, &DbAdapter::getFreeTasks, Qt::BlockingQueuedConnection);
    QObject::connect(server, &ElServer::setOperatorToTask, db, &DbAdapter::setOperatorToTask, Qt::BlockingQueuedConnection);
    QObject::connect(server, &ElServer::getTaskDataToOperator, db, &DbAdapter::getTaskDataToOperator, Qt::BlockingQueuedConnection);


    server->moveToThread(serverThread);
    QObject::connect(serverThread, &QThread::started, server, &ElServer::start);
    serverThread->start();
    qDebug() << "start..";

    return a.exec();
}
