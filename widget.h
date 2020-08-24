#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "myjson.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT


signals:
    void write_text(QByteArray);
    void set_param_json(QString, QString);
    void add_new_alarm(QMap<QString, QString>);
    void enable_alert();
    void send_event();
    void disable_alert();
    void turn_relay(bool);

public:
    Widget(QWidget *parent = nullptr);
    QTimer timer;
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_pushButton_10_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QDateTime dateTime;

    bool flag_relay = false;

    QString zone;
    int zoneId;
    QVector <sensor> exist_sensors;
    int sensorId;

    void write_datetime();


public slots:
    void message_to_textEdit(QString);
    void receive_vector_sensors(QVector <sensor>);
    void change_relays_button(bool);


};
#endif // WIDGET_H
