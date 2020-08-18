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
    ElServer();
    bool start(const int port);
//    void stop();

private:
    QTcpServer *tcpServer = NULL;
    int server_status;
    QMap<int, QTcpSocket *> SClients;

private slots:
    void newuser();
    void user_disconnect();
    void slotReadClient();

signals:
    QJsonArray getCurrentTasks();

};

#endif // ELSERVER_H
