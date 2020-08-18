#include "elserver.h"
#include <QDebug>

ElServer::ElServer()
{

}

bool ElServer::start(const int port)
{
    if (!tcpServer)
    {
        tcpServer = new QTcpServer();
        connect(tcpServer, &QTcpServer::newConnection, this, &ElServer::newuser);
    }

    if (!tcpServer->listen(QHostAddress::Any, port) && server_status==0)
    {
        qDebug() << tcpServer->errorString();
        return false;
    } else {
        server_status=1;
        qDebug() << QStringLiteral("Сервер запущен на %1 порту").arg(tcpServer->serverPort());
        return true;
    }
}

void ElServer::newuser()
{
    emit qDebug() << QString::fromUtf8("У нас новое соединение!");

    QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
    int idusersocs = clientSocket->socketDescriptor();

    connect(clientSocket, &QTcpSocket::readyRead,this, &ElServer::slotReadClient);
    connect(clientSocket ,&QAbstractSocket::disconnected,this, &ElServer::user_disconnect);
    SClients[idusersocs] = clientSocket;
}

void ElServer::user_disconnect()
{
    qDebug() << QString::fromUtf8("Пользователь отключился!");

    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(QObject::sender());
    foreach (int key, SClients.keys())
    {
        QTcpSocket *value = SClients.value(key);
        if (value == clientSocket)
        {
            SClients.remove(key);
        }
    }
    clientSocket->deleteLater();
}

void ElServer::slotReadClient()
{
    QTcpSocket* senderSocket = (QTcpSocket*)sender();
    QByteArray datas = senderSocket->readAll();

    if (datas.compare("#getCurrentTasks") == 0) {
        QJsonArray currentTasks = emit getCurrentTasks();
        QJsonDocument currentTasksJD;
        currentTasksJD.setArray(currentTasks);

        QString message(currentTasksJD.toJson());
        message = "#currentTasks" + message;
        senderSocket->write(message.toUtf8());
    }

    if (datas.indexOf("#iwantgettask") != -1) {
        datas.replace("#iwantgettask", "");
        int taskID = datas.toInt();
        qDebug() << QString::number(taskID);


        QHostAddress hostSender = senderSocket->peerAddress();
        qDebug() << hostSender.toString();
    }


    qDebug() << QString::fromUtf8("ReadClient:" + datas + "\n");
}
