#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <src/qcustomplot.h>

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

private:
    Ui::Dialog *ui;

};
#endif // DIALOG_H
