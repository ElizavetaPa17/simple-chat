#include "mainwindow.h"

#include <QApplication>

#include "chatserver.h"

int main(int argc, char *argv[])
{
    ChatServer chat_server_;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
