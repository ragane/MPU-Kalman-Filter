#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtQuickWidgets/QQuickWidget>
#include <src/qcustomplot.h>
#include <QSerialPort>
#include <QVector>
#include <string>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStatusBar>
#include <QElapsedTimer>
#include <serialdevice.h>
#include <savewindow.h>
#include <QDateTime>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickView>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
public:

    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    int BaudIndex = 1, DataIndex = 3, StopIndex = 0, ParityIndex = 0, FlowIndex = 1;
    int GyroIndex, AccIndex;
    QString RefreshVal;
    QQuickView *qmlView = new QQuickView();

public slots:

    void ListenerSaveWindow(QString &msg);
    void ListenerFileName(QString &Filename, QString &extended);

signals:
    void changeRoll(QString str);
    void changePitch(QString str);
    void changeYaw(QString str);
    void SendMsg(QString &msg);
    void ChangeImuScale(int &AccScale, int &GyroScale);
    void BaudRateIndex(int BaudIndex, int DataIndex, int ParityIndex, int StopIndex);



private slots:

    void setupGraph();
    void setupAccScaleBox();
    void setupGyroScaleBox();
    void setupConnectionBox();
    void drawGraph();
    void updateImuView();
    void on_SearchButton_clicked();
    void on_ConnectButton_clicked();
    void on_DisconnectButton_clicked();
    void addLogs(QString msg);
    void XreadSerial(QString& ImuValues);
    void on_SaveImgButton_clicked();
    void saveGraph();
    void on_saveTxtButton_clicked();
    void on_saveCsvButton_clicked();
    void on_pushButton_clicked();
    void on_DataBox_currentIndexChanged(int index);
    void on_StopBox_currentIndexChanged(int index);
    void on_ParityBox_currentIndexChanged(int index);
    void on_refreshIMU_clicked();
    void on_AccScaleBox_currentIndexChanged(int index);
    void on_GyroScaleBox_currentIndexChanged(int index);


private:

    Ui::Dialog *ui;
    SaveWindow *SaveWin;
    SerialDevice serialDevice;

    QCustomPlot * AccGraph;
    QCustomPlot * GyroGraph;
    QCustomPlot * YawGraph;
    QComboBox * AccScaleBox;
    QComboBox * GyroScaleBox;
    QComboBox * BaudrateBox;
    QComboBox * ParityBox;
    QComboBox * FlowBox;
    QComboBox * StopBox;
    QStringList baudrateList = {"9600", "115200"};
    QStringList DataList = {"6 bits", "7 bits", "8 bits"};
    QStringList ParityList = {"No parity", "Even Parity", "Odd Parity"};
    QStringList StopList = {"1 bit", "2 bits"};
    QStringList values;
    float Roll_Val, Pitch_Val, Yaw_Val;
    float KFRoll_Val, KFPitch_Val, KFYaw_Val;
    float Temp_Val;
    void ConnectionValues(int BaudIndex, int DataIndex, int StopIndex, int ParityIndex);
    QString _MSG;
    QString Msg, Filename, ext;
    QVBoxLayout * ImuVisulizator;
    QQuickWidget *Rectangle;

};
#endif // DIALOG_H
