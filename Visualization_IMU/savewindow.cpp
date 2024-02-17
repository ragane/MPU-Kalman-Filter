#include "savewindow.h"
#include "ui_savewindow.h"

SaveWindow::SaveWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Save Graph File");
    CreateSaveTree();
}
SaveWindow::~SaveWindow()
{
    emit accepted();
    delete ui;
}

void SaveWindow::CreateSaveTree()
{
    model.setRootPath(QDir::currentPath());
    ui->SaveTree->setModel(&model);
    ui->SaveTree->setRootIndex(model.index("/home/ragane"));
    ui->SaveTree->setWindowTitle(QObject::tr("Dir View"));

    ui->ExtendBox->addItem(".png");
    ui->ExtendBox->addItem(".bmp");
    ui->ExtendBox->addItem(".jpg");
}

void SaveWindow::on_RollButton_clicked() { msg = "Roll Graph";}

void SaveWindow::on_PitchButton_clicked() {msg ="Pitch Graph";}

void SaveWindow::on_YawButton_clicked() { msg = "Yaw Graph";}

void SaveWindow::on_SaveTree_clicked(const QModelIndex &index) {path = model.filePath(index);}

void SaveWindow::on_SaveTree_doubleClicked(const QModelIndex &index) {ui->SaveTree->setRootIndex(index);}

void SaveWindow::on_FilenameLine_textEdited(const QString &Filename) {filename = Filename;}

void SaveWindow::on_buttonBox_accepted()
{
    QString FileName = path + "/" + filename + extended;
    qDebug() << FileName;

    emit SpeakerGraph(msg);
    emit SpeakerFileName(FileName, extended);
}


void SaveWindow::on_pushButton_clicked()
{
    QModelIndex currentRoot = ui->SaveTree->rootIndex();
    ui->SaveTree->setRootIndex(currentRoot.parent());
}

void SaveWindow::on_ExtendBox_currentIndexChanged(const QString &extension)
{
    extended = (extension == ".png") ? ".png" : (extension ==".jpg") ? ".jpg" : ".bmp";
}
