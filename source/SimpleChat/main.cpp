#include "mainwindow.h"

#include <QApplication>

#include "dbutility.h"

int main(int argc, char *argv[])
{
    DbUtility Ut();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
