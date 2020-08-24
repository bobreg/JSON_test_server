#include "widget.h"

#include <QApplication>
#include "myserver.h"
#include "myjson.h"

void connect_is(MyServer * server, MyJson * json){
    QApplication::connect(server, &MyServer::new_message,
                          json, &MyJson::msg_from_client);

    QApplication::connect(json, &MyJson::msg_for_client,
                          server, &MyServer::send_data);

    qDebug() << "соединение установлено";

}


void connect_lose(MyServer * server, MyJson * json){
    QApplication::disconnect(server, &MyServer::new_message,
                             json, &MyJson::msg_from_client);

    QApplication::disconnect(json, &MyJson::msg_for_client,
                          server, &MyServer::send_data);
    qDebug() << "соединение сброшено";
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    MyServer server;
    MyJson json;
    w.show();

    QApplication::connect(&server, &MyServer::special_msg,
                          &w, &Widget::message_to_textEdit);

    QApplication::connect(&json, &MyJson::special_msg,            //отправка специальных сообщений в окно от json объекта
                          &w, &Widget::message_to_textEdit);

    QApplication::connect(&w, &Widget::write_text,                // отправка сообщения из окна в json  объект
                          &json, &MyJson::msg_from_client);

    QApplication::connect(&json, &MyJson::msg_for_client,         // сформированное сообщение для клиента написать в окне
                          &w, &Widget::message_to_textEdit);

    QApplication::connect(&w, &Widget::set_param_json,            // установить параметры json объекта для формирования сообщения
                          &json, &MyJson::change_param);          // клиенту в соответствии с выбранным датчиком в окне

    QApplication::connect(&w, &Widget::add_new_alarm,             // добавка новой аварии для отправки клиенту
                          &json, &MyJson::add_new_alarm);

    QApplication::connect(&server, &MyServer::connect_is,         //если есть соединение с клиентом, то соединим json объект с
                          [&server, &json](){connect_is(&server, &json);});  // объектом сервера

    QApplication::connect(&server, &MyServer::connect_lose,       //если соединение с клиентом сброшено, то разъединим json объект с
                          [&server, &json](){connect_lose(&server, &json);});  // объектом сервера


    QApplication::connect(&json, &MyJson::send_vector_sensors_to_window,  //передача набора датчиков в объект окна
                          &w, &Widget::receive_vector_sensors);

    QApplication::connect(&json, &MyJson::change_relays_button,  //изменение надписи кнопки в зависимости
                          &w, &Widget::change_relays_button);    //от истинного состояния реле


    QApplication::connect(&w, &Widget::send_event,                // обработка кнопки json объектом
                          &json, &MyJson::send_event);

    QApplication::connect(&w, &Widget::turn_relay,                // обработка кнопки json объектом
                          &json, &MyJson::relay);

    QApplication::connect(&w, &Widget::enable_alert,              // обработка кнопки json объектом
                          &json, &MyJson::enable_alert);

    QApplication::connect(&w, &Widget::disable_alert,              // обработка кнопки json объектом
                          &json, &MyJson::alert_stop);

    json.send_json();   // запуск разбора json файла
    w.timer.start(60000);  // запуск таймера в окне

    return a.exec();
}


