#include <QtGui/QApplication>				
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("CP-1251");
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    a.setApplicationName("Client");
    a.setApplicationVersion("v0.1");
    MainWindow w;
    w.show();
    
    return a.exec();
}
