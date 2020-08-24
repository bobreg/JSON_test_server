#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&timer, &QTimer::timeout,
            this, &Widget::write_datetime);
    write_datetime();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::message_to_textEdit(QString str){
    ui->plainTextEdit->appendPlainText(str);
}


void Widget::write_datetime(){
    ui->label_6->setText(dateTime.currentDateTime().toString("hh:mm dd.MM.yy"));
}


void Widget::on_pushButton_clicked() // тревога
{
    emit enable_alert();
}


void Widget::on_pushButton_7_clicked() //сброс тревоги
{
    emit disable_alert();
    ui->pushButton->setEnabled(true);
}


void Widget::on_pushButton_8_clicked() //запрос пинга
{
    QByteArray a = "{\"command\":\"ping\",\"id\":1}";
    emit write_text(a);
}


void Widget::on_pushButton_9_clicked() // событие
{
    emit set_param_json("event", "Зафиксирована тревога на устройстве");
    emit set_param_json("eventId", "34");
    emit set_param_json("operatorr", "Кощей");
    emit set_param_json("type", "critical");
    emit send_event();

}


void Widget::receive_vector_sensors(QVector <sensor> sensors){
    for(auto i : sensors){
        exist_sensors = sensors;
        ui->listWidget->addItem(i.name);
    }
}


void Widget::on_listWidget_currentRowChanged(int currentRow)
{
    ui->pushButton->setEnabled(true);
    QString temp = "Сенсор : " + exist_sensors[currentRow].name + "\n";
    temp.push_back("Id сенсора : " + QString::number(exist_sensors[currentRow].id_sensor) + "\n");
    temp.push_back("Зона : " + exist_sensors[currentRow].zone + "\n");
    temp.push_back("Id зоны : " + QString::number(exist_sensors[currentRow].id_zone) + "\n");
    ui->label->setText(temp);
    QMap<QString, QString> Temp;
    Temp.insert("zone", exist_sensors[currentRow].zone);
    Temp.insert("zoneId", QString::number(exist_sensors[currentRow].id_zone));
    Temp.insert("sensor", exist_sensors[currentRow].name);
    Temp.insert("sensorId", QString::number(exist_sensors[currentRow].id_sensor));
    emit add_new_alarm(Temp);
    emit set_param_json("zone", exist_sensors[currentRow].zone);
    emit set_param_json("zoneId", QString::number(exist_sensors[currentRow].id_zone));
    emit set_param_json("sensor", exist_sensors[currentRow].name);
    emit set_param_json("sensorId", QString::number(exist_sensors[currentRow].id_sensor));
}


void Widget::on_pushButton_10_clicked()
{
    flag_relay = !flag_relay;
    if(flag_relay == true){
        emit turn_relay(flag_relay);
    }else{
        emit turn_relay(flag_relay);
    }
}

void Widget::change_relays_button(bool flag){
    if(flag == true){
        ui->pushButton_10->setText("Реле включено!");
    }else{
        ui->pushButton_10->setText("Реле вЫключено!");
    }
}

void Widget::on_pushButton_2_clicked()
{
    ui->plainTextEdit->clear();
}
