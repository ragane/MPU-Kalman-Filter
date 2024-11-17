#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>
#include <QQuickView>
class SerialDevice : public QObject
{
    Q_OBJECT


public:
    explicit SerialDevice(QObject *parent = 0);
    ~SerialDevice();

    bool begin(QString portName, qint32 BaudRate, int dataBits, int parity, int stopBits);


    QString UsedPortName;
    qint32 UsedBaudRate;
    int UsedDataBits, UsedStopBits, UsedParity;

    QList<QSerialPortInfo> available_devices();
    QString SerialConnectionInfo();

    QString SerialString;
    float Pitch_Val, Yaw_Val;
    float Roll_Val;
    double KFRoll_Val, KFPitch_Val, KFYaw_Val;

    bool isOpen();
    bool close();
signals:
    void Send(QString &SerialString);

public slots:
    void readSerial();
    void ReadMsg(QString &msg);
    void ReadBaudIndex(int BaudIndex, int DataIndex, int ParityIndex, int StopIndex);
    void ReadImuScale(int &AccScale, int &GyroScale);

private:

    QSerialPort * device;

};

#endif // SERIALDEVICE_H
