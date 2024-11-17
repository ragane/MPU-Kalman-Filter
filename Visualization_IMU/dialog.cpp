#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <src/qcustomplot.h>
#include <QVector>
#include <QQuickItem>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Imu app");

    Dialog::setupGraph();
    Dialog::setupAccScaleBox();
    Dialog::setupGyroScaleBox();
    Dialog::setupConnectionBox();
    Dialog::drawGraph();
    ui->quickWidget->setSource(QUrl("qrc:/3DMoveVisualization.qml"));
    ui->quickWidget->show();
    auto obj = ui->quickWidget->rootObject();
    QObject::connect(this, SIGNAL(changeRoll(QString)), obj, SLOT(changeRoll(QString)));
    QObject::connect(this, SIGNAL(changePitch(QString)), obj, SLOT(changePitch(QString)));
    QObject::connect(this, SIGNAL(changeYaw(QString)), obj, SLOT(changeYaw(QString)));


}

void Dialog::setupGraph()
{

    ui->textEdit->setDisabled(true);
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
    ui->YawGraph->xAxis->setTicker(timeTicker);
    ui->TempGraph->xAxis->setTicker(timeTicker);
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

    ui->TempGraph->addGraph();
    ui->TempGraph->graph(0)->setPen(QPen(Qt::red));


    ui->TempGraph->graph(0)->setName("Temperature");

    ui->TempGraph->xAxis->setLabel("Time [s]");
    ui->TempGraph->yAxis->setLabel("Temperature [C]");
    ui->TempGraph->setWindowTitle("Temperature graph");
    ui->TempGraph->yAxis->setRange(10, 30);

    connect(ui->TempGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->TempGraph->xAxis, SLOT(setRange(QCPRange)));
    connect(ui->TempGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->TempGraph->yAxis, SLOT(setRange(QCPRange)));

    // create title of temp graph
    ui->TempGraph->plotLayout()->insertRow(0);
    QCPTextElement * tempTitle = new QCPTextElement(ui->TempGraph, "Temperature Graph", QFont("arial", 12, QFont::Bold));
    ui->TempGraph->plotLayout()->addElement(0, 0, tempTitle);
    // make tempGraph interacted
    ui->TempGraph->legend->setFont(legendGyro);
    ui->TempGraph->legend->setBrush(QBrush(QColor(255, 255, 255, 230)));
    ui->TempGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->TempGraph->legend->setVisible(true);



}

void Dialog::setupAccScaleBox()
{
    ui->AccScaleBox->addItem("±2 g");
    ui->AccScaleBox->addItem("±4 g");
    ui->AccScaleBox->addItem("±8 g");
    ui->AccScaleBox->addItem("±16 g");
    ui->AccScaleBox->setCurrentText("±2 g");
}

void Dialog::setupGyroScaleBox()
{
    ui->GyroScaleBox->addItem("± 250 °/s");
    ui->GyroScaleBox->addItem("± 500 °/s");
    ui->GyroScaleBox->addItem("± 1000 °/s");
    ui->GyroScaleBox->addItem("± 2000 °/s");
    ui->GyroScaleBox->setCurrentText("± 2000 °/s");
}

void Dialog::setupConnectionBox()
{
    ui->BaudrateBox->addItems(baudrateList);
    ui->BaudrateBox->setCurrentText("9600");
    ui->ParityBox->addItems(ParityList);
    ui->ParityBox->setCurrentText("No Parity");
    ui->StopBox->addItems(StopList);
    ui->StopBox->setCurrentText("1 bit");
    ui->DataBox->addItems(DataList);
    ui->DataBox->setCurrentText("8 bits");

    connect(this, SIGNAL(BaudRateIndex(int, int, int ,int)), &serialDevice, SLOT(ReadBaudIndex(int, int, int, int)));
}

Dialog::~Dialog() { delete ui; }

void Dialog::on_SearchButton_clicked()
{

    if(serialDevice.available_devices().isEmpty())
        this->addLogs("No available devices...");

    for (int i = 0; i < serialDevice.available_devices().count(); i++)
    {
        this->addLogs("Searching available devics...");
        this->addLogs("Found device: " + serialDevice.available_devices().at(i).portName()
                            + "\t" + serialDevice.available_devices().at(i).description());
        ui->PortsBox->addItem(serialDevice.available_devices().at(i).portName() + "\t" + serialDevice.available_devices().at(i).description());
    }
    serialDevice.UsedPortName = ui->PortsBox->currentText().split("\t").first();
    serialDevice.UsedBaudRate = ui->BaudrateBox->currentText().toInt();
    serialDevice.UsedDataBits = 8;
    serialDevice.UsedParity = 0;
    serialDevice.UsedStopBits = 1;
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
    if (!serialDevice.isOpen())
    {
        if (serialDevice.begin(serialDevice.UsedPortName, serialDevice.UsedBaudRate, serialDevice.UsedDataBits,
                               serialDevice.UsedParity, serialDevice.UsedStopBits))

        {
            this->addLogs("Serial port is opened.");
            ui->ConnCheck->setChecked(true);
            ui->ConnCheck->setStyleSheet("color: green");
        }
        else {
            this->addLogs("Serial port is not opened.");
            ui->ConnCheck->setStyleSheet("color: red");

            ui->ConnCheck->setChecked(false);
        }
    }
    else
    {
        this->addLogs("Serial port is already open.");
        return;
    }
    connect(&serialDevice, SIGNAL(Send(QString&)), this, SLOT(XreadSerial(QString&)));
    connect(this, SIGNAL(SendMsg(QString&)), &serialDevice, SLOT(ReadMsg(QString&)));
    connect(this, SIGNAL(ChangeImuScale(int&,int&)), &serialDevice, SLOT(ReadImuScale(int&,int&)));
}

void Dialog::on_DisconnectButton_clicked()
{
    ui->receivedData->setChecked(false);
    ui->ConnCheck->setStyleSheet("color: red");
    ui->ConnCheck->setChecked(false);

    serialDevice.close();
}

void Dialog::XreadSerial(QString &msg)
{
    _MSG = msg;
    if ((_MSG.length() > 25) && (_MSG.length() < 32)){
        this->addLogs(_MSG);

            values = _MSG.split("/");
            ui->receivedData->animateClick();
            Roll_Val = values[0].toFloat();
            KFRoll_Val = values[1].toFloat();
            Pitch_Val = values[2].toFloat();
            KFPitch_Val = values[3].toFloat();
            Yaw_Val = values[4].toFloat();
            Temp_Val = values[5].toFloat();
            qDebug() << Temp_Val;
            emit changeRoll(values[0]);
            emit changePitch(values[2]);
            emit changeYaw(values[4]);
    }
    drawGraph();
    saveGraph();
    updateImuView();


}


void Dialog::drawGraph()
{

    QElapsedTimer timer;
    timer.restart();
    timer.start();

    double key = timer.msecsSinceReference() / 1000.0;
    //qDebug() << key;
    // calculate two new data points:
    static double lastTime = 0;
    if (key - lastTime >= 0.02) // at most add point every 2 ms

    {
      // add data to lines:
      ui->AccGraph->graph(0)->addData(key, Roll_Val);
      ui->AccGraph->graph(1)->addData(key, KFRoll_Val);

      ui->GyroGraph->graph(0)->addData(key, Pitch_Val);
      ui->GyroGraph->graph(1)->addData(key, KFPitch_Val);

      ui->YawGraph->graph(0)->addData(key, Yaw_Val);
      ui->TempGraph->graph(0)->addData(key, Temp_Val);

    }
    else {
        ui->AccGraph->graph(0)->addData(0, Roll_Val);
        ui->AccGraph->graph(1)->addData(0, KFRoll_Val);

        ui->GyroGraph->graph(0)->addData(0, Pitch_Val);
        ui->GyroGraph->graph(1)->addData(0, KFPitch_Val);

        ui->YawGraph->graph(0)->addData(0, Yaw_Val);
        ui->TempGraph->graph(0)->addData(0, Temp_Val);
    }
    // make timer axis range scroll with the data (at a constant range size of 8):
    ui->AccGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->AccGraph->yAxis->rescale();
    ui->AccGraph->replot();
    ui->GyroGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->GyroGraph->yAxis->rescale();
    ui->GyroGraph->replot();
    ui->YawGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->YawGraph->yAxis->rescale();
    ui->YawGraph->replot();

    ui->TempGraph->xAxis->setRange(key, 4, Qt::AlignRight);
    ui->TempGraph->yAxis->rescale();
    ui->TempGraph->replot();
    lastTime = key;

    timer.restart();


}


void Dialog::updateImuView()
{

    //qDebug() << "JEST";

}

void Dialog::on_SaveImgButton_clicked()
{
    SaveWindow *SaveWin = new SaveWindow();
    connect(SaveWin, SIGNAL(SpeakerFileName(QString&,QString&)), this, SLOT(ListenerFileName(QString&,QString&)));
    connect(SaveWin, SIGNAL(SpeakerGraph(QString&)), this, SLOT(ListenerSaveWindow(QString&)));
    SaveWin->show();
}

void Dialog::saveGraph()
{
    if (Msg == "Roll Graph")
    {
        if (ext == ".jpg") {ui->AccGraph->saveJpg(Filename, 0, 0, 1.0, -1);}
        else if (ext == ".png") {ui->AccGraph->savePng(Filename, 600, 300, 1.0, -1);}
        else if (ext == ".bmp") {ui->AccGraph->saveBmp(Filename, 600, 300,1.0, -1);}
    }
   else if (Msg == "Pitch Graph")
    {
        if (ext == ".jpg") {ui->GyroGraph->saveJpg(Filename, 600, 300, 1.0, -1);}
        else if (ext == ".png") {ui->GyroGraph->savePng(Filename, 600, 300, 1.0, -1);}
        else if (ext == ".bmp") {ui->GyroGraph->saveBmp(Filename, 600, 300, 1.0, -1);}
    }
    else if (Msg == "Yaw Graph")
    {
        if (ext == ".jpg") {ui->YawGraph->saveJpg(Filename, 0, 0, 1.0, -1);}
        else if (ext == ".png") {ui->YawGraph->savePng(Filename, 600, 300, 1.0, -1);}
        else if (ext == ".bmp") {ui->YawGraph->saveBmp(Filename, 600, 300, 1.0, -1);}
    }
}

void Dialog::ListenerFileName(QString &FileName, QString &extended)
{
    Filename = FileName;
    ext = extended;
}

void Dialog::ListenerSaveWindow(QString &msg) {Msg = msg;}

void Dialog::on_saveTxtButton_clicked()
{
    QString SaveTextFile = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text file (*.txt)"));
    if (SaveTextFile.isEmpty()) {QMessageBox::warning(this, tr("Error"), tr("Filename is empty."));}

    QFile f(SaveTextFile);
    if (!f.open(QIODevice::WriteOnly| QIODevice::Text))
    {
        f.errorString();
        return;
    }
    QTextStream stream(&f);
    stream << "Pitch/KF Pitch/Roll/KF Roll/Yaw\n";
    stream <<ui->LogText->toPlainText();
    f.close();
}

void Dialog::on_saveCsvButton_clicked()
{
    QString SaveCsvFile = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Comma-separated file (*.csv)"));
    if (SaveCsvFile.isEmpty()) {QMessageBox::warning(this, tr("Error"), tr("Filename is empty."));}

    QFile f(SaveCsvFile);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        f.errorString();
        return;
    }
    QTextStream stream(&f);
    stream << "Pitch/KF Pitch/Roll/KF Roll/Yaw\n";
    stream << ui->LogText->toPlainText();
    f.close();
}

void Dialog::ConnectionValues(int BaudIndex, int DataIndex, int ParityIndex, int StopIndex)
{
    emit(BaudRateIndex(BaudIndex, DataIndex, ParityIndex, StopIndex));
    ui->receivedData->setChecked(false);
}

void Dialog::on_pushButton_clicked(){ ConnectionValues(BaudIndex, DataIndex, ParityIndex, StopIndex);}

void Dialog::on_DataBox_currentIndexChanged(int index) { DataIndex = index;}

void Dialog::on_StopBox_currentIndexChanged(int index){ StopIndex = index;}

void Dialog::on_ParityBox_currentIndexChanged(int index){ ParityIndex = index;}

void Dialog::on_AccScaleBox_currentIndexChanged(int index){ AccIndex = index;}

void Dialog::on_GyroScaleBox_currentIndexChanged(int index){ GyroIndex = index;}

void Dialog::on_refreshIMU_clicked(){ emit(ChangeImuScale(AccIndex, GyroIndex));}
