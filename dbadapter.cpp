#include "dbadapter.h"
#include <QDebug>

DbAdapter::DbAdapter()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("/home/nikifor/qtProjects/elqueue/elqueue/eq.db");
    sdb.setDatabaseName("D:/qt_projects/elqueue/elqueue/eq.db");

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
    query.prepare("SELECT task.ID, task.tbegin, task.taccept, task.tend, task.ticket, task.operator_ID, service.name " \
                  "FROM task " \
                  "JOIN service " \
                  "ON task.service_ID = service.ID AND task.operator_ID ISNULL ");
    query.exec();
    QList<Task*> res;
    while (query.next()) {
        QString tBeginStr = query.value("tbegin").toString();
        QDateTime tBeginDT = QDateTime::fromString(tBeginStr, "yyyy-MM-dd hh:mm:ss");
        qDebug() << "tBeginDT: " <<tBeginDT.toString("hh:mm:ss yyyy-MM-dd");
        Task *someTask = new Task(query.value("ID").toInt(),
                                  tBeginDT,
                                  QDateTime::fromString(query.value("taccept").toString(), "yyyy-MM-dd hh:mm:ss"),
                                  QDateTime::fromString(query.value("tend").toString(), "yyyy-MM-dd hh:mm:ss"),
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
    query.prepare("SELECT ID FROM operator WHERE host = :host");
    query.bindValue(":host", operatorIP);
    int operatorID = this->queryExec(query)[0].toObject()["ID"].toInt();
    QDateTime now = QDateTime::currentDateTime();
    QString nowStr = now.toString("yyyy-MM-dd hh:mm:ss");

    //QSqlQuery query;
    query.prepare("UPDATE task SET" \
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
