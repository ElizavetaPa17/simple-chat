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

    void handleSuccessAuthentification(int auth_type);
    void handleFailedAuthentification();

private slots:
    void sltSendAuthInfo(const char* username, const char* password, int auth_type);
    void sltSwitchPage();
    void sltOpenChat(const char* id, const char* username);
    void sltFindUser(const char* username);
    void sltSendMessage(const char* text);
};
#endif // MAINWINDOW_H
