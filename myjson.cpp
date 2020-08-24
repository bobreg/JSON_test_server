#include "myjson.h"

MyJson::MyJson()
{
    command_output = {"pingAnswer", "alert", "event", "alertPassed", "relay"};
    connect(&timer, &QTimer::timeout,
            this, &MyJson::alert);

    file_json.setFileName("my_model_json.json");
    file_json.open(QIODevice::ReadOnly);
    data_from_file = file_json.readAll();
    file_json.close();

    json_parse_sensor(data_from_file, &sensors);

}

//------------------------- метод для разбора json файла---------------------------------

void MyJson:: json_parse_sensor(QByteArray data, QVector <sensor> * sensors){
    sensor temp;
    QJsonDocument document_json;
    document_json = QJsonDocument::fromJson(data);
    QJsonArray arr = document_json.array();
    for(auto i : arr){
        temp.name = i.toObject()["sensor"].toString();
        temp.zone = i.toObject()["zone"].toString();
        temp.id_sensor = i.toObject()["sensorId"].toInt();
        temp.id_zone = i.toObject()["zoneId"].toInt();
        sensors->push_back(temp);
        }
}

void MyJson::send_json(){
    emit send_vector_sensors_to_window(sensors);
}

//------------------------- методы для разбора json сообщений из сети и формирования
//                                                           ответных json сообщений---------------------------------

//приём запроса из сети пинга или сброса тревог
// и запроса от кнопок для установки события или тревоги
void MyJson::msg_from_client(QByteArray byteArray){
    msg_for_me = QJsonDocument::fromJson(byteArray);
    if(msg_for_me.isEmpty()){
        emit special_msg("----Ошибка приёма сообщения!-----\n");
    }else{
        emit special_msg("-----сообщение клиента-----\n"
                            + msg_for_me.toJson() + "\n");
        msg_for_me2 = msg_for_me.object();
        comm = msg_for_me2["command"].toString();
        if(comm == "ping"){
            ping();
        }else if(comm == "relay"){
            state_relay = msg_for_me2["mode"].toString();
            if(state_relay == "opened"){
                emit change_relays_button(true);
            }else if(state_relay == "closed"){
                emit change_relays_button(false);
            }
            emit special_msg("-----нам сообщают о\n"
                             " переключение реле-----\n");
        }else if(comm == "alertPassed"){
            alert_stop();
        }else{
            emit special_msg("-----Фигня какая-то-----\n"
                                + msg_for_me.toJson() + "\n");
        }
    }
}

//отправка тревог до сброса
void MyJson::enable_alert(){
    timer.start(5000);
}

//отправить событие
void MyJson::send_event(){
    my_event();
}

// формирование json ответа на запрос пинга
void MyJson::ping(){
    msg_for_them2 = QJsonObject();
    id++;
    msg_for_them2.insert("command", command_output[0]);
    msg_for_them2.insert("id", id);
    msg_for_them2.insert("time", time.currentTime().
                        toString("hh:mm"));
    msg_for_them2.insert("date", date.currentDate().
                        toString("dd.MM.yyyy"));
    msg_for_them2.insert("alertsCount", alertsCount);
    msg_for_them.setObject(msg_for_them2);
    msg_for_send = msg_for_them.toJson();
    //бросаем сигнал со сформированным ответом в окно и сеть
    emit special_msg("-----сообщение сервера----\n");
    emit msg_for_client(msg_for_send);

}

//формирование json запроса для изменения реле
void MyJson::relay(bool status_relay){
    msg_for_them2 = QJsonObject();
    id++;
    msg_for_them2.insert("command", command_output[4]);
    msg_for_them2.insert("id", id);
    msg_for_them2.insert("relayId", 77);
    msg_for_them2.insert("relayExternalId", 22);
    if(status_relay == true){
        msg_for_them2.insert("mode", "open");
    }else{
        msg_for_them2.insert("mode", "close");
    }
    msg_for_them.setObject(msg_for_them2);
    msg_for_send = msg_for_them.toJson();
    //бросаем сигнал со сформированным ответом в окно и сеть
    emit special_msg("-----сообщение сервера----\n");
    emit msg_for_client(msg_for_send);

}

// формирование json ответа тревог
void MyJson::alert(){
    for(auto i : alarm_sensor){
        msg_for_them2 = QJsonObject();
        id++;
        alertsCount++;
        msg_for_them2.insert("command", command_output[1]);
        msg_for_them2.insert("id", id);
        msg_for_them2.insert("time", time.currentTime().
                            toString("hh:mm"));
        msg_for_them2.insert("date", date.currentDate().
                            toString("dd.MM.yyyy"));
        msg_for_them2.insert("alertId", i.id_sensor);
        msg_for_them2.insert("sensor", i.name);
        msg_for_them2.insert("sensorId", i.id_sensor);
        msg_for_them2.insert("zone", i.zone);
        msg_for_them2.insert("zoneId", i.id_zone);
        msg_for_them.setObject(msg_for_them2);
        msg_for_send = msg_for_them.toJson();
        //бросаем сигнал со сформированным ответом в окно и сеть
        emit special_msg("-----сообщение сервера----\n");
        emit msg_for_client(msg_for_send);
    }
//    msg_for_them2 = QJsonObject();
//    id++;
//    alertsCount++;
//    msg_for_them2.insert("command", command_output[1]);
//    msg_for_them2.insert("id", id);
//    msg_for_them2.insert("time", time.currentTime().
//                        toString("hh:mm"));
//    msg_for_them2.insert("date", date.currentDate().
//                        toString("dd.MM.yyyy"));
//    msg_for_them2.insert("alertId", sensorId);
//    msg_for_them2.insert("sensor", alert_sensor);
//    msg_for_them2.insert("sensorId", sensorId);
//    msg_for_them2.insert("zone", zone);
//    msg_for_them2.insert("zoneId", zoneId);
//    msg_for_them.setObject(msg_for_them2);
//    msg_for_send = msg_for_them.toJson();
//    //бросаем сигнал со сформированным ответом в окно и сеть
//    emit special_msg("-----сообщение сервера----\n");
//    emit msg_for_client(msg_for_send);
}

//сброс тревоги
void MyJson::alert_stop(){
    msg_for_them2 = QJsonObject();
    id++;
    msg_for_them2.insert("command", command_output[3]);
    msg_for_them2.insert("id", id);
    msg_for_them2.insert("alertId", sensorId);
    msg_for_them.setObject(msg_for_them2);
    msg_for_send = msg_for_them.toJson();
    //бросаем сигнал со сформированным ответом в окно и сеть
    emit special_msg("-----сообщение сервера----\n");
    emit msg_for_client(msg_for_send);
    int count = 0;
    qDebug() << "------------------";
    for(auto i : alarm_sensor){
        qDebug() << i.name;
    }
    qDebug() << "------------------";
    for(auto i : alarm_sensor){
        if(i.id_sensor == sensorId){
            qDebug() << i.id_sensor;
            qDebug() << sensorId;
            qDebug() << i.name << count;
            alarm_sensor.remove(alarm_sensor.indexOf(i));
        }else{
            count++;
        }
    }
    qDebug() << "**************";
    for(auto i : alarm_sensor){
        qDebug() << i.name;
    }
    qDebug() << "***************";
    if(alarm_sensor.isEmpty()){
        timer.stop();
        alertsCount = 0;
    }
}

// формирование json ответа на произвольное событие
void MyJson::my_event(){
    id++;
    msg_for_them2 = QJsonObject();
    msg_for_them2.insert("command", command_output[2]);
    msg_for_them2.insert("id", id);
    msg_for_them2.insert("time", time.currentTime().
                        toString("hh:mm"));
    msg_for_them2.insert("date", date.currentDate().
                        toString("dd.MM.yyyy"));
    msg_for_them2.insert("eventId", eventId);
    msg_for_them2.insert("event", event);
    msg_for_them2.insert("operator", operatorr);
    msg_for_them2.insert("type", type);
    msg_for_them.setObject(msg_for_them2);
    msg_for_send = msg_for_them.toJson();
    //бросаем сигнал со сформированным ответом в окно и сеть
    emit special_msg("-----сообщение сервера----\n");
    emit msg_for_client(msg_for_send);
}

//изменение индификаторов для будущего json ответа
void MyJson::change_param(QString param, QString value){
    if(param == "alert_sensor"){
        alert_sensor = value;
    }else if(param == "zone"){
        zone = value;
    }else if(param == "event"){
        event = value;
    }else if(param == "operatorr"){
        operatorr = value;
    }else if(param == "zoneId"){
        zoneId = value.toInt();
    }else if(param == "eventId"){
        eventId = value.toInt();
    }else if(param == "sensorId"){
        sensorId = value.toInt();
    }else if(param == "sensor"){
        alert_sensor = value;
    }else if(param == "type"){
        type = value;
    }

}


void MyJson::add_new_alarm(QMap<QString, QString> alarm){
    sensor temp;
    temp.name = alarm["sensor"];
    temp.zone = alarm["zone"];
    temp.id_zone = alarm["zoneId"].toInt();
    temp.id_sensor = alarm["sensorId"].toInt();
    if(alarm_sensor.indexOf(temp) == -1){
        alarm_sensor.append(temp);
    }
}
