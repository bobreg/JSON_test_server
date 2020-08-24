#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>

struct sensor{  //структура хранящая в себе имя датчика с его id, id зоны и именем зоны
    QString name;
    QString zone;
    int id_zone;
    int id_sensor;
};

#endif // SENSOR_H
