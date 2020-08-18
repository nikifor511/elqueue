//#include "mainwidget.h"
#include "main.h"
#include "QDebug"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    MainWidget w;
//    w.show();



    qDebug() << "start..";
    DbAdapter *db = new DbAdapter();
    ElServer *serv = new ElServer();
    QObject::connect(serv, &ElServer::getCurrentTasks, db, &DbAdapter::getCurrentTasks);
    serv->start(58583);

    QJsonArray res = db->queryExec("SELECT * from operator");

    return a.exec();
}
