#include "dbadapter.h"
#include <QDebug>
 #include <QPluginLoader>
#include <QApplication>
#include <QDir>

const QString dateFormat = "yyyy-MM-ddThh:mm:ss.zzz";

DbAdapter::DbAdapter()
{
//    sdb = QSqlDatabase::addDatabase("QSQLITE");
////    sdb.setDatabaseName("/home/nikifor/qtProjects/elqueue/elqueue/eq.db");

//    sdb.setDatabaseName("D:/qt_projects/elqueue/elqueue/eq.db");
//    QPluginLoader pluginLoader_;
//    if(!pluginLoader_.isLoaded())
//        {
//            pluginLoader_.setFileName("");

//            if (!pluginLoader_.load())
//            {
//                //// Loading SQL Driver failed.;
//                bool isInitialized_ = false;
//                //return false;
//            }
//        }

    sdb = QSqlDatabase::addDatabase("QIBASE");
    //sdb.setHostName("localhost");

//    sdb.setUserName("SYSDBA");
//    sdb.setPassword("123456789");
    QString cfg = "Database=\"D:\\qt_projects\\elqueue\\elqueue\\ELQ_E.FDB\"; DataSource=localhost; " \
                "Port=3050; Dialect=3; Charset=NONE; Role=; Connection lifetime=15; " \
                "Pooling=true; MinPoolSize=0; MaxPoolSize=50; Packet Size=8192; " \
                "ServerType=1; ";



//            "server type=Embedded; auto_commit=True; auto_commit_level=4096; "
//            "connection lifetime=1; ISC_DPB_LC_CTYPE=UTF8; DataBase=\"D:\\qt_projects\\elqueue\\elqueue\\ELQ_E.FDB\"");
    sdb.setConnectOptions(cfg);
//    sdb.setDatabaseName("D:\\qt_projects\\elqueue\\elqueue\\ELQ_E.FDB");
    if (sdb.open()) {
        int y =0;
    }
}

QJsonArray DbAdapter::queryExec(QSqlQuery query)
{
    QJsonArray recordsArray;
    query.exec();

    while (query.next()) {
        QJsonObject recordObject;
        for (int column = 0; column < query.record().count(); column++)
        {
            recordObject.insert( query.record().fieldName(column),
                QJsonValue::fromVariant(query.value(column)) );
        }
        recordsArray.push_back(recordObject);
    }
    return recordsArray;
}

QList<Task*> DbAdapter::getFreeTasks()
{
    sdb.open();
    QSqlQuery query;
    query.prepare("SELECT TASK.ID, TASK.tbegin, TASK.taccept, TASK.tend, TASK.ticket, TASK.operator_ID, SERVICE.name " \
                  "FROM TASK " \
                  "JOIN SERVICE " \
                  "ON TASK.SERVICE_ID = SERVICE.ID AND TASK.OPERATOR_ID IS NULL ");
    query.exec();
    QList<Task*> res;
    while (query.next()) {
        QString tBeginStr = query.value("tbegin").toString();
        QDateTime tBeginDT = QDateTime::fromString(tBeginStr, dateFormat);
        //tBeginDT.setTimeSpec(Qt::UTC);
        Task *someTask = new Task(query.value("ID").toInt(),
                                  QDateTime::fromString(query.value("tbegin").toString(), dateFormat),
                                  QDateTime::fromString(query.value("taccept").toString(), dateFormat),
                                  QDateTime::fromString(query.value("tend").toString(), dateFormat),
                                  query.value("ticket").toString(),
                                  query.value("operator_ID").toInt(),
                                  query.value("name").toString());

        res.append(someTask);
    }
    sdb.close();
    return res;
}

bool DbAdapter::setOperatorToTask(QString operatorIP, int taskID)
{
    sdb.open();
    QSqlQuery query;
    query.prepare("SELECT ID FROM OPERATOR WHERE host = :host");
    query.bindValue(":host", operatorIP);
    int operatorID = this->queryExec(query)[0].toObject()["ID"].toInt();
    QDateTime now = QDateTime::currentDateTime();
    QString nowStr = now.toString(dateFormat);

    //QSqlQuery query;
    query.prepare("UPDATE TASK SET" \
                  " operator_ID = :operator_ID," \
                  " taccept = :taccept" \
                  " WHERE ID = :ID");

    query.bindValue(":operator_ID", operatorID);
    query.bindValue(":taccept", nowStr);
    query.bindValue(":ID", taskID);
    this->queryExec(query);

    sdb.close();
    return true;
}

Task* DbAdapter::getTaskDataToOperator(const int taskID)
{
    sdb.open();
    QSqlQuery query;
    query.prepare("SELECT task.ID, task.tbegin, task.taccept, task.tend, task.ticket, task.operator_ID, service.name "  \
                  "FROM task " \
                  "JOIN service " \
                  "ON task.service_ID = service.ID AND task.ID = :taskID");
    query.bindValue(":taskID", taskID);
    query.exec();
    query.next();

    Task *res = new Task(query.value(0).toInt(),
                         query.value(1).toDateTime(),
                         query.value(2).toDateTime(),
                         query.value(3).toDateTime(),
                         query.value(4).toString(),
                         query.value(5).toInt(),
                         query.value(6).toString());
    sdb.close();
    return res;
}
