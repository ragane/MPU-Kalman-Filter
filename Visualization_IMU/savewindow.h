#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFileSystemModel>
#include <QTreeWidgetItem>

namespace Ui {
class SaveWindow;
}

class SaveWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SaveWindow(QWidget *parent = 0);
    ~SaveWindow();

    void CreateSaveTree();
    QFileSystemModel model;

    QString msg;
    QString FileName;
    QString extended;

signals:

    void SpeakerFileName(QString &FileName, QString &extended);
    void SpeakerGraph(QString &msg);

private slots:

    void on_RollButton_clicked();
    void on_PitchButton_clicked();
    void on_YawButton_clicked();
    void on_buttonBox_accepted();
    void on_SaveTree_clicked(const QModelIndex &index);
    void on_SaveTree_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_ExtendBox_currentTextChanged(const QString &arg1);
    void on_FilenameLine_textEdited(const QString &arg1);

private:

    Ui::SaveWindow *ui;
    QString filename;
    QString path;

};

#endif // SAVEWINDOW_H
