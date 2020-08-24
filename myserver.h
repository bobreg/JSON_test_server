#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>



class MyServer : public QObject
{
    Q_OBJECT
public:
    MyServer();

private slots:
    void close_connect();
    void new_connect();
    void read_data();

public slots:
    void send_data(QByteArray);

signals:
    void new_message(QByteArray);
    void disconnectt();
    void special_msg(QString);
    void connect_is();
    void connect_lose();

private:
    QTcpServer server;
    QTcpSocket* socket;
    QByteArray message_recived;
    //QByteArray message_will_send;




};

#endif // MYSERVER_H
