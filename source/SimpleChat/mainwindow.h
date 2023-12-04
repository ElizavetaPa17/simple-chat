#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "authentificationwidget.h"
#include "chatclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ChatClient client_;

private:
    Ui::MainWindow *main_window_ui_;
    AuthentificationWidget* authent_widget_;

    void setupDesign();
    void setupConnection();

private slots:
    void sltSendAuthInfo(const char* username, const char* password, int auth_type);
};
#endif // MAINWINDOW_H
