#include "myserver.h"

MyServer::MyServer()
{

    bool stat = server.listen(QHostAddress::Any, 8237);

    if(stat == false){
        qDebug() << "Не могу подключиться к порту";
        //emit special_msg("Не могу подключиться к порту");

    }else{
        qDebug() << "Ожидание подключения...";
        //emit special_msg("Ожидание подключения...");
    }
    connect(&server, &QTcpServer::newConnection,
            this, &MyServer::new_connect);


}


void MyServer::read_data(){
    message_recived = socket->readAll();
    emit new_message(message_recived);
}


void MyServer::send_data(QByteArray message_will_send){
    socket->write(message_will_send);
    socket->waitForReadyRead(100);
}


void MyServer::new_connect(){
    emit special_msg("Есть соединение!");
    socket = server.nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead,
            this, &MyServer::read_data);
    connect(socket, &QTcpSocket::disconnected,
            this, &MyServer::close_connect);
    emit connect_is();
}


void MyServer::close_connect(){
    socket->close();
    emit disconnectt();
    emit connect_lose();
    emit special_msg("Клиент отключился!");
}
