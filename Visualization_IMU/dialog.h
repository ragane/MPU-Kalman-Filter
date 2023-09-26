#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <src/qcustomplot.h>
#include <QSerialPort>
#include <QVector>
#include <string>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void setupGraph();
    void setupAccScaleBox();
    void setupGyroScaleBox();
    void setupConnectionBox();
    void drawGraph();

    //*TODO set ability to change the work of MPU
    //void setAccScale(mpu6050_range_t);
    //void setGyroScale(int index);


    void on_SearchButton_clicked();
    void on_ConnectButton_clicked();
    void on_DisconnectButton_clicked();
    void addLogs(QString msg);
    void readSerial();

private:
    Ui::Dialog *ui;

    QSerialPort * device;
    QCustomPlot * AccGraph;
    QCustomPlot * GyroGraph;
    QCustomPlot * YawGraph;
    QComboBox * AccScaleBox;
    QComboBox * GyroScaleBox;
    QComboBox * BaudrateBox;
    QComboBox * ParityBox;
    QComboBox * FlowBox;
    QComboBox * StopBox;

    QStringList baudrateList = {"300", "1200", "2400", "4800", "9600", "14400", "19200",
                                 "28800", "38400", "57600", "115200"};
    QStringList parityList = {"No parity", "Even Parity", "Odd Parity"};
    QStringList flowList = {"Flow control", "No flow control"};
    QStringList stopList = {"1 bit", "2 bits"};

    double Roll_Val, Pitch_Val, Yaw_Val;
    double KFRoll_Val, KFPitch_Val, KFYaw_Val;


    //Yaw_Val, Kal_Roll, Kal_Pitch;
};
#endif // DIALOG_H
