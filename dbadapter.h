#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlRecord>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


class DbAdapter: public QObject
{

public:
    DbAdapter();
    QJsonArray queryExec(QSqlQuery);


private:
    QSqlDatabase sdb;

public slots:
    QJsonArray getFreeTasks();
    bool setOperatorToTask(QString operatorIP, int taskID);
    QJsonArray getTaskDataToOperator(const int taskID);


};

#endif // DBADAPTER_H
