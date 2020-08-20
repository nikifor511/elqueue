#include "dbadapter.h"
#include <QDebug>

DbAdapter::DbAdapter()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("/home/nikifor/qtProjects/elqueue/elqueue/eq.db");
    sdb.setDatabaseName("D:/qt_projects/elqueue/elqueue/eq.db");
}

QJsonArray DbAdapter::queryExec(QString sql_str)
{
    if (sdb.open()) {
          qDebug() << "open db!!!!";
    }

    QJsonArray recordsArray;
    QSqlQuery query(sql_str);
//    query.last();

    if (sql_str.indexOf("INSERT") > -1) {
        bool last =  query.last();
        int y=8;
        int r = 9;

    }

    while (query.next()) {
        QJsonObject recordObject;
        for (int column = 0; column < query.record().count(); column++)
        {
            recordObject.insert( query.record().fieldName(column),
                QJsonValue::fromVariant(query.value(column)) );
        }
        recordsArray.push_back(recordObject);
    }

    sdb.close();
    return recordsArray;
}

QJsonArray DbAdapter::getFreeTasks()
{
    QJsonArray res = this->queryExec("SELECT * FROM task WHERE operator_ID ISNULL;");

    return res;
}

bool DbAdapter::setOperatorToTask(QString operatorIP, int taskID)
{
//    QJsonArray operatorID_JA = this->queryExec("SELECT ID FROM operator WHERE host = '" + operatorIP + "'");
//    QJsonObject operatorID_obj = operatorID_JA[0].toObject();
//    int id = operatorID_obj["ID"].toInt();
    int operatorID = this->queryExec("SELECT ID FROM operator WHERE host = '" + operatorIP + "'")[0].toObject()["ID"].toInt();
    this->queryExec("UPDATE task SET operator_ID = " + QString::number(operatorID) + " WHERE ID = " + QString::number(taskID));
    return true;
}
