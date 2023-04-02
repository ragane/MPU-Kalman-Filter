#include "dialog.h"
#include "ui_dialog.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <src/qcustomplot.h>
#include <QVector>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)

{
    ui->setupUi(this);
    this->device = new QSerialPort(this);

    Dialog::setupGraph();
    Dialog::setupAccScaleBox();
    Dialog::setupGyroScaleBox();
    Dialog::setupConnectionBox();
    Dialog::drawAccGraph();
}


void Dialog::setupGraph()
{
    ui->AccGraph->addGraph();
    ui->AccGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->AccGraph->graph(0)->setName("X-axis");

    ui->AccGraph->addGraph();
    ui->AccGraph->graph(1)->setPen(QPen(Qt::green));
    ui->AccGraph->graph(1)->setName("Y-axis");

    ui->AccGraph->addGraph();
    ui->AccGraph->graph(2)->setPen(QPen(Qt::red));
    ui->AccGraph->graph(2)->setName("Z-axis");

    ui->AccGraph->xAxis->setLabel("Time [s]");
    ui->AccGraph->yAxis->setLabel("Acceleration [g]");
    ui->AccGraph->setWindowTitle("Acceleration graph");

    // set yAxis ranges of AccGraph
    ui->AccGraph->yAxis->setRange(-10, 10);
    ui->AccGraph->yAxis2->setRange(-10, 10);

    QCPTextElement * accTitle = new QCPTextElement(AccGraph);
    accTitle->setText("Title");
    accTitle->setFont(QFont("sans", 8, QFont::Bold));
    //accTitle->setTextColor(Qt::black);
    ui->AccGraph->plotLayout()->insertRow(0);
    ui->AccGraph->plotLayout()->addElement(0, 0, accTitle);

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->GyroGraph->graph(0)->setName("Yaw");

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(1)->setPen(QPen(Qt::green));
    ui->GyroGraph->graph(1)->setName("Pitch");

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(2)->setPen(QPen(Qt::red));
    ui->GyroGraph->graph(2)->setName("Roll");

    ui->GyroGraph->xAxis->setLabel("Time [s]");
    ui->GyroGraph->yAxis->setLabel("Gyration [°/s]");
    ui->GyroGraph->setWindowTitle("Gyroscope graph");

    // set yAxis ranges of GyroGraph
    ui->GyroGraph->yAxis->setRange(-1000, 1000);
    ui->GyroGraph->yAxis2->setRange(-1000, 1000);

    QFont legendGyro = font();
    legendGyro.setPointSize(9);

    // make AccGraph interacted
    ui->AccGraph->legend->setFont(legendGyro);
    ui->AccGraph->legend->setBrush(QBrush(QColor(255, 200, 255, 255)));
    ui->AccGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);

    // make GyroGraph interacted
    ui->GyroGraph->legend->setFont(legendGyro);
    ui->GyroGraph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    ui->GyroGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);

    // create title of gyrograph
    //QCPTextElement * gyroTitle = new QCPTextElement(GyroGraph, "titleTxt", QFont("sans", 12, QFont::Bold));
    //gyroTitle->setText("Title");
    //gyroTitle->setFont(QFont("sans", 8, QFont::Bold));
    //gyroTitle->setTextColor(QColor(255, 255, 0, 0));
    //ui->GyroGraph->plotLayout()->insertRow(0);
    //ui->GyroGraph->plotLayout()->addElement(0, 0, gyroTitle);

}
void Dialog::setupAccScaleBox()
{
    ui->AccScaleBox->addItem("±2 g");
    ui->AccScaleBox->addItem("±4 g");
    ui->AccScaleBox->addItem("±8 g");
    ui->AccScaleBox->addItem("±16 g");
    ui->AccScaleBox->setCurrentText("±2 g");
// TODO zmiana skali wykresu w zaleznosci od limitu przeciazen
}

void Dialog::setupGyroScaleBox()
{
    ui->GyroScaleBox->addItem("± 250 °/s");
    ui->GyroScaleBox->addItem("± 500 °/s");
    ui->GyroScaleBox->addItem("± 1000 °/s");
    ui->GyroScaleBox->addItem("± 2000 °/s");
    ui->GyroScaleBox->setCurrentText("± 1000 °/s");
}

void Dialog::setupConnectionBox()
{
    ui->BaudrateBox->addItems(baudrateList);
    ui->BaudrateBox->setCurrentText("115200");
    ui->FlowBox->addItems(flowList);
    ui->FlowBox->setCurrentText("No flow control");
    ui->ParityBox->addItems(parityList);
    ui->ParityBox->setCurrentText("No Parity");
    ui->StopBox->addItems(stopList);
    ui->StopBox->setCurrentText("1 bit");
}





Dialog::~Dialog()
{
    delete ui;
}




void Dialog::on_SearchButton_clicked()
{
    QList<QSerialPortInfo> available_devices;
    available_devices = QSerialPortInfo::availablePorts();
    if(available_devices.isEmpty())
        this->addLogs("No available devices...");
    for (int i = 0; i < available_devices.count(); i++)
    {
        this->addLogs("Searching available devics...");
        this->addLogs("Found device: " + available_devices.at(i).portName()
                            + "\t" + available_devices.at(i).description());
        ui->PortsBox->addItem(available_devices.at(i).portName() + "\t" + available_devices.at(i).description());
    }

}

void Dialog::addLogs(QString msg)
{
    QString currentTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    ui->LogText->append(currentTime + "\t" + msg);
}

void Dialog::on_ConnectButton_clicked()
{
    if(ui->PortsBox->count() == 0)
    {
        this->addLogs("Not found any device.");
        return;
    }

    QString portName = ui->PortsBox->currentText().split("\t").first();
    this->device->setPortName(portName);

    if (!device->isOpen())
    {
        if(device->open(QSerialPort::ReadOnly))
        {
            this->device->setBaudRate(QSerialPort::Baud115200);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setFlowControl(QSerialPort::NoFlowControl);
            this->device->setParity(QSerialPort::NoParity);

            this->addLogs("Serial port is opened.");
            connect(this->device, SIGNAL(readyRead()), this, SLOT(readSerial()));
        }
        else
        {
            this->addLogs("Serial port is not opened.");
        }
    }
    else
    {
        this->addLogs("Serial port is already open.");
        return;
    }

}



void Dialog::on_DisconnectButton_clicked()
{
    if(this->device->isOpen())
    {
        this->device->close();
        this->addLogs("Serial port is closed.");
    }
    else
    {
        this->addLogs("Serial port is not available any more.");
        return;
    }
}



void Dialog::readSerial()
{
    QByteArray byteArray = device->readAll();
    qDebug() << " ****" << byteArray << "****";
    QStringList resultList = QString(byteArray).split("/");
    // TODO zapisywac do pliku CSV!!! z ktorego potem bedzie czytane do robienia wykresow!!!

    //if (x_axis.count() > x_axis.size)
    // Accelerometer results
    qDebug() << resultList[0]; //x_axis;
    qDebug() << resultList[4]; //KalRoll;
    if (!resultList.isEmpty())
    {
        // acc pitch
        //container[0].append(resultList[0].toDouble());
        // acc roll
        //container[1].append(resultList[1].toDouble());
        // acc yaw
        //container[2].append(resultList[1].toDouble());
    }

}

void Dialog::drawAccGraph()
{
    //Acc_X = container[0];
    time = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    //ui->AccGraph->graph(0)->addData(time, );
    qDebug() << Acc_X << "!!!!!!!!!!!!1";
}
