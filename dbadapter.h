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
    QJsonArray queryExec(const QString sql_str);


private:
    QSqlDatabase sdb;

public slots:
    QJsonArray getCurrentTasks();
    bool setOperatorToTask(int taskID, int operatorID);

};

#endif // DBADAPTER_H
