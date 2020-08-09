#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    /*QFile style(":/Style.css");
#ifdef Q_OS_ANDROID
    style.setFileName(":/StyleAndroid.css");
#endif
    if (style.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(style.readAll());
        style.close();
    }*/
    MainWindow w;
    w.setObjectName("obj_MainWindow");
    w.setFixedSize(360,640);
    w.show();
    return a.exec();
}
