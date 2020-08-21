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

QJsonArray DbAdapter::getFreeTasks()
{
    sdb.open();
    QSqlQuery query;
    query.prepare("SELECT * FROM task WHERE operator_ID ISNULL;");
    QJsonArray res = this->queryExec(query);

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

QJsonArray DbAdapter::getTaskDataToOperator(const int taskID)
{
    sdb.open();
    QSqlQuery query;
    query.prepare("SELECT task.ID, tbegin, ticket, name "  \
                  "FROM task " \
                  "JOIN service " \
                  "ON task.service_ID = service.ID AND task.ID = :taskID");
    query.bindValue(":taskID", taskID);
    QJsonArray res = this->queryExec(query);

    sdb.close();
    return res;
}
