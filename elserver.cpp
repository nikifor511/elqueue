#include "elserver.h"
#include <QDebug>

ElServer::ElServer(const int port)
{
    this->port = port;
}

bool ElServer::start()
{
    if (!tcpServer)
    {
        tcpServer = new QTcpServer();
        connect(tcpServer, &QTcpServer::newConnection, this, &ElServer::newuser);
    }

    if (!tcpServer->listen(QHostAddress::Any, this->port) && server_status==0)
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

    if (datas.compare("#getFreeTasks") == 0) {

        QString message = "#freeTasks" + this->JsonArrayToString(emit getFreeTasks());
        this->writeData(message.toUtf8(), senderSocket);
    }

    if (datas.indexOf("#iwantgettask") != -1) {
        datas.replace("#iwantgettask", "");
        int taskID = datas.toInt();
        qDebug() << QString::number(taskID);

        QString hostSender = senderSocket->peerAddress().toString().replace("::ffff:", "");   //TODO Get ipv4 adress
        qDebug() << hostSender;
        if (emit setOperatorToTask(hostSender, taskID)) {
            QString message = "#yourcurrenttask" + this->JsonArrayToString(emit getTaskDataToOperator(taskID));
            this->writeData(message.toUtf8(), senderSocket);
        }

        this->updateFreeTasksForClients();
    }
    qDebug() << QString::fromUtf8("ReadClient:" + datas + "\n");
}

void ElServer::updateFreeTasksForClients()
{
    QString message = "#freeTasks" + this->JsonArrayToString(emit getFreeTasks());
    foreach (QTcpSocket *_tcpSocket, SClients) {
        this->writeData(message.toUtf8(), _tcpSocket);
    }
}

QString ElServer::JsonArrayToString(const QJsonArray dataArray)
{    
    QJsonDocument freeTasksJD;
    freeTasksJD.setArray(dataArray);
    QString message(freeTasksJD.toJson());

    return message;
}

bool ElServer::writeData(QByteArray data, QTcpSocket *_socket)
{
    _socket->write(data + "$");

}
