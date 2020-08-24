#ifndef MYJSON_H
#define MYJSON_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QVector>
#include <QDebug>
#include <QIODevice>
#include <QFile>


struct sensor{  //структура хранящая в себе имя датчика с его id, id зоны и именем зоны
    QString name;
    QString zone;
    int id_zone;
    int id_sensor;

    bool operator==(const sensor &temp){
        return (temp.id_sensor == id_sensor) && (temp.name == name) && (temp.zone == zone) && (temp.id_zone == id_zone);
    }
};


class MyJson : public QObject
{
    Q_OBJECT

signals:
    void msg_for_client(QByteArray);
    void special_msg(QString);
    void send_vector_sensors_to_window(QVector <sensor>);
    void change_relays_button(bool);




public:
    MyJson();
    void send_json();



public slots:
    void msg_from_client(QByteArray);
    void change_param(QString, QString);
    void add_new_alarm(QMap<QString, QString>);
    void enable_alert();
    void send_event();
    void alert_stop();
    void relay(bool);

private:
    void ping();
    void alert();
    void my_event();


    int id = 0;
    int alertsCount = 0;
    int zoneId = 1;
    int eventId = 777;
    int sensorId = 1;

    QDate date;
    QTimer timer;
    QTime time;
    bool flag = false;
    QString state_relay = "close";

    QVector <QString> command_input;
    QVector <QString> command_output;
    QString comm;
    QString alert_sensor = "сенсор";
    QString zone = "Зона";
    QString event = "что-то произошло";
    QString operatorr = "Bobreg";
    QString type = "info";
    QByteArray msg_for_send;

    QJsonDocument msg_for_me;
    QJsonObject msg_for_me2;
    QJsonDocument msg_for_them;
    QJsonObject msg_for_them2;

    //переменные и методы для разбора json файла



    //QString current_zone = "";
    QVector <QJsonDocument> zones;  //массив всех зон
    QVector <sensor> sensors; // массив всех датчиков
    QVector <sensor> alarm_sensor; //массив аварийных датчиков
    QFile file_json;
    QByteArray data_from_file;

    //методы

    void json_parse_sensor(QByteArray, QVector <sensor> *);
    void json_parse_zone(QByteArray, QVector <QJsonDocument> *);
    void json_parse_name_zone(QJsonDocument, QVector <sensor> *);


};



#endif // MYJSON_H
