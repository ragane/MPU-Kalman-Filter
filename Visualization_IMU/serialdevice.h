#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(QObject *parent = 0);

signals:

public slots:
};

#endif // SERIALDEVICE_H