#include "dialog.h"
#include "savewindow.h"
#include <serialdevice.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    SaveWindow SaveWin;

    QObject::connect(&SaveWin, SIGNAL(SpeakerFileName(QString&,QString&)), &w, SLOT(ListenerFileName(QString&,QString&)));
    QObject::connect(&SaveWin, SIGNAL(SpeakerGraph(QString&)), &w, SLOT(ListenerSaveWindow(QString&)));
    w.show();

    return a.exec();
}
