#include "dialog.h"
#include "ui_dialog.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QTimer>
#include <QScreen>

#include <src/qcustomplot.h>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)

{
    ui->setupUi(this);
    Dialog::setupGraph();
}


void Dialog::setupGraph()
{
    ui->AccGraph->addGraph();
    ui->AccGraph->graph(0)->setPen(QPen(Qt::blue));

    ui->AccGraph->addGraph();
    ui->AccGraph->graph(1)->setPen(QPen(Qt::green));

    ui->AccGraph->addGraph();
    ui->AccGraph->graph(2)->setPen(QPen(Qt::red));

    ui->AccGraph->xAxis->setLabel("Time [s]");
    ui->AccGraph->yAxis->setLabel("Acceleration [g]");

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(0)->setPen(QPen(Qt::blue));

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(1)->setPen(QPen(Qt::green));

    ui->GyroGraph->addGraph();
    ui->GyroGraph->graph(2)->setPen(QPen(Qt::red));

    ui->GyroGraph->xAxis->setLabel("Time [s]");
    ui->GyroGraph->yAxis->setLabel("Gyraction [°/s]");


}


Dialog::~Dialog()
{
    delete ui;
}


