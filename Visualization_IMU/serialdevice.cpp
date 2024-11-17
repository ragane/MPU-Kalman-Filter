#include "serialdevice.h"

SerialDevice::SerialDevice(QObject *parent) : QObject(parent)
{
    this->device = new QSerialPort(this);

}

SerialDevice::~SerialDevice() { close(); }

bool SerialDevice::begin(QString UsedPortName, qint32 UsedBaudRate, int UsedDataBits, int UsedParity, int UsedStopBits)
{
    if (available_devices().count() < 1) { return false; }

    this->device->setPortName(UsedPortName);
    if (!isOpen())
    {
        if (this->device->open(QIODevice::ReadWrite))
        {
            qDebug() << UsedBaudRate;
            this->device->setBaudRate((QSerialPort::BaudRate)UsedBaudRate);
            this->device->setDataBits((QSerialPort::DataBits)UsedDataBits);
            this->device->setParity((QSerialPort::Parity)UsedParity);
            this->device->setStopBits((QSerialPort::StopBits)UsedStopBits);
            this->device->setFlowControl(QSerialPort::NoFlowControl);
            this->device->setDataTerminalReady(true);
            connect(this->device, SIGNAL(readyRead()), this, SLOT(readSerial()));
        }

    }
}

bool SerialDevice::isOpen() {return device->isOpen(); }

bool SerialDevice::close()
{

    disconnect(this->device, SIGNAL(readyRead()), this, SLOT(readSerial()));
    this->device->close();

}

QString SerialDevice::SerialConnectionInfo()
{
    QString outInfo;
}

QList<QSerialPortInfo> SerialDevice::available_devices()
{
    return QSerialPortInfo::availablePorts();
}


void SerialDevice::readSerial()
{
        SerialString = this->device->readAll();
        QStringList values = SerialString.split("/");
        Roll_Val = values[0].toDouble();
        emit Send(SerialString);

}

void SerialDevice::ReadMsg(QString &msg)
{
    if (device->isOpen() && device->isWritable())
    {
        this->device->write(msg.toStdString().c_str());
        qDebug() << msg;
    }
    else
    {
        qDebug() << "Unable to send message to arduino.";
    }

}
void SerialDevice::ReadImuScale(int &AccScale, int &GyroScale)
{

    QString ImuScaleMsg = "9"  + QString::number(GyroScale) + QString::number(AccScale);
    if (device->isOpen() && device->isWritable())
    {
        this->device->write(ImuScaleMsg.toStdString().c_str());
        qDebug() << ImuScaleMsg.toStdString().c_str();
    }
    else
    {
        qDebug() << "Unable to send message to arduino.";
    }

}

void SerialDevice::ReadBaudIndex(int BaudIndex, int DataIndex, int ParityIndex, int StopIndex)
{

    switch (BaudIndex)
    {
    case 0:
        UsedBaudRate = 9600;
        break;
    case 1:
        UsedBaudRate = 115200;
        break;
    }
    qDebug() << UsedBaudRate;
    switch(DataIndex)
    {
    case 0:
        UsedDataBits = 6;
        break;
    case 1:
        UsedDataBits = 7;
        break;
    case 2:
        UsedDataBits = 8;
        break;
    }
    switch(ParityIndex)
    {
    case 0:
        UsedParity = 0;
        break;
    case 1:
        UsedParity = 2;
        break;
    case 2:
        UsedParity = 3;
        break;
    }
    switch(StopIndex)
    {
    case 0:
        UsedStopBits = 1;
        break;
    case 1:
        UsedStopBits = 2;
        break;
    }
    QString msg = "8" + QString::number(BaudIndex) + QString::number(DataIndex) + QString::number(ParityIndex) + QString::number(StopIndex);
    if (device->isOpen() && device->isWritable())
    {
        this->device->write(msg.toStdString().c_str());
        qDebug() << msg.toStdString().c_str();


        begin(UsedPortName, UsedBaudRate, UsedDataBits, UsedParity, UsedStopBits);

    }
    else
    {
        qDebug() << "Unable to send message to arduino.";
    }


}
