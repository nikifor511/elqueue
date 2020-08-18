#include "dbadapter.h"
#include <QDebug>

DbAdapter::DbAdapter()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
//    sdb.setDatabaseName("/home/nikifor/qtProjects/s_eq/s_eq//eq.db");
    sdb.setDatabaseName("D:/qt_projects/s_eq/s_eq/eq.db");
}

QJsonArray DbAdapter::queryExec(QString sql_str)
{
    if (sdb.open()) {
          qDebug() << "open db!!!!";
    }

    QJsonArray recordsArray;
    QSqlQuery query(sql_str);
//    query.last();

    if (sql_str.indexOf("INSERT") > -1)
    {
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

QJsonArray DbAdapter::getCurrentTasks()
{
    QJsonArray res = this->queryExec("SELECT * FROM task WHERE operator_ID ISNULL;");
    return res;
}
