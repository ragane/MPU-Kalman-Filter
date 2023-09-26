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
    Dialog::drawGraph();
}


void Dialog::setupGraph()
{
    ui->AccGraph->addGraph();
    ui->AccGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->AccGraph->graph(0)->setName("Real data");

    ui->AccGraph->addGraph();
    ui->AccGraph->graph(1)->setPen(QPen(Qt::green));
    ui->AccGraph->graph(1)->setName("KF data");


    ui->AccGraph->xAxis->setLabel("Time [s]");
    ui->AccGraph->yAxis->setLabel("Roll");
    ui->AccGraph->setWindowTitle("Acceleration graph");

    // set yAxis ranges of AccGraph
    ui->AccGraph->yAxis->setRange(-20, 20);
    ui->AccGraph->yAxis2->setRange(-20, 20);

    // set title of roll graph
    ui->AccGraph->plotLayout()->insertRow(0);
    QCPTextElement * accTitle = new QCPTextElement(ui->AccGraph, "Roll graph", QFont("arial", 12, QFont::Bold));
    ui->AccGraph->plotLayout()->addElement(0, 0, accTitle);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->AccGraph->xAxis->setTicker(timeTicker);
    ui->GyroGraph->xAxis->setTicker(timeTicker);
    connect(ui->AccGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->AccGraph->xAxis, SLOT(setRange(QCPRange)));
    connect(ui->AccGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->AccGraph->yAxis, SLOT(setRange(QCPRange)));


    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->GyroGraph->graph(0)->setName("Real data");

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(1)->setPen(QPen(Qt::green));
    ui->GyroGraph->graph(1)->setName("KF data");


    ui->GyroGraph->xAxis->setLabel("Time [s]");
    ui->GyroGraph->yAxis->setLabel("Pitch");
    ui->GyroGraph->setWindowTitle("Pitch graph");

    //set yAxis ranges of GyroGraph
    ui->GyroGraph->yAxis->setRange(-20, 20);
    ui->GyroGraph->yAxis2->setRange(-20, 20);
    connect(ui->GyroGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->GyroGraph->xAxis, SLOT(setRange(QCPRange)));
    connect(ui->GyroGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->GyroGraph->yAxis, SLOT(setRange(QCPRange)));

    QFont legendGyro = font();
    legendGyro.setPointSize(9);

    // create title of gyrograph
    ui->GyroGraph->plotLayout()->insertRow(0);
    QCPTextElement * gyroTitle = new QCPTextElement(ui->GyroGraph, "Pitch Graph", QFont("arial", 12, QFont::Bold));
    ui->GyroGraph->plotLayout()->addElement(0, 0, gyroTitle);


    ui->YawGraph->addGraph();
    ui->YawGraph->graph(0)->setPen(QPen(Qt::blue));
    ui->YawGraph->graph(0)->setName("Real data");

    ui->YawGraph->addGraph();
    ui->YawGraph->graph(1)->setPen(QPen(Qt::green));
    ui->YawGraph->graph(1)->setName("KF data");

    ui->YawGraph->xAxis->setLabel("Time [s]");
    ui->YawGraph->yAxis->setLabel("Yaw");
    ui->YawGraph->yAxis->setRange(-20, 20);
    ui->YawGraph->yAxis2->setRange(-20, 20);
    connect(ui->YawGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->YawGraph->xAxis, SLOT(setRange(QCPRange)));
    connect(ui->YawGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->YawGraph->yAxis, SLOT(setRange(QCPRange)));

    QFont legendYaw = font();
    legendYaw.setPointSize(9);

    // create title of yaw graph
    ui->YawGraph->plotLayout()->insertRow(0);
    QCPTextElement * yawTitle = new QCPTextElement(ui->YawGraph, "Yaw Graph", QFont("arial", 12, QFont::Bold));
    ui->YawGraph->plotLayout()->addElement(0, 0, yawTitle);


    // make AccGraph interacted
    ui->AccGraph->legend->setFont(legendGyro);
    ui->AccGraph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    ui->AccGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->AccGraph->legend->setVisible(true);
    // make GyroGraph interacted
    ui->GyroGraph->legend->setFont(legendGyro);
    ui->GyroGraph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    ui->GyroGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->GyroGraph->legend->setVisible(true);
    // make YawGraph interacted
    ui->YawGraph->legend->setFont(legendYaw);
    ui->YawGraph->legend->setBrush(QBrush(QColor(255, 255, 255, 233)));
    ui->YawGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->YawGraph->legend->setVisible(true);

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
\

    QString SerialString = device->readAll();
    this->addLogs("Serial data: " + SerialString);
    QStringList values = SerialString.split("/");


    if (values.size() < 6)
    {
        QThread::sleep(0.001);

    }
    else
    {
        qDebug() << "Serial data: " << values;
        Pitch_Val = values[0].toDouble();
        KFPitch_Val = values[3].toDouble();
        Roll_Val = values[1].toDouble();
        KFRoll_Val = values[4].toDouble();
        Yaw_Val = values[2].toDouble();
        KFYaw_Val = values[5].toDouble();
    }
    drawGraph();

}

void Dialog::drawGraph()
{

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->AccGraph->graph(0)->addData(key, Roll_Val);
      ui->AccGraph->graph(1)->addData(key, KFRoll_Val);

      ui->GyroGraph->graph(0)->addData(key, Pitch_Val);
      ui->GyroGraph->graph(1)->addData(key, KFPitch_Val);

      ui->YawGraph->graph(0)->addData(key, Yaw_Val);
      ui->YawGraph->graph(1)->addData(key, KFYaw_Val);

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->AccGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->AccGraph->replot();
    ui->GyroGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->GyroGraph->replot();
    ui->YawGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->YawGraph->replot();


}

