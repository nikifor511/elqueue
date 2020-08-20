#ifndef ELSERVER_H
#define ELSERVER_H

#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>


class ElServer: public QObject
{
    Q_OBJECT
public:
    ElServer(const int port);
    bool start();

//    void stop();

private:
    QTcpServer *tcpServer = NULL;
    int port;
    int server_status;
    QMap<int, QTcpSocket *> SClients;

private slots:
    void newuser();
    void user_disconnect();
    void slotReadClient();
    void updateFreeTasksForClients();
    QString getFreeTasksFromDB();
    bool writeData(QByteArray data, QTcpSocket *_socket);


signals:
    QJsonArray getFreeTasks();
    bool setOperatorToTask(QString operatorIP, int taskID);

};

#endif // ELSERVER_H
