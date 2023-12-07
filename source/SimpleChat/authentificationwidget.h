#ifndef AUTHENTIFICATIONWIDGET_H
#define AUTHENTIFICATIONWIDGET_H

#include <QWidget>
#include "./ui_authentificationform.h"

class AuthentificationWidget : public QWidget
{
    Q_OBJECT
public:
    AuthentificationWidget(QWidget* parent = nullptr);
    ~AuthentificationWidget();

    void handleFailedAuthentification();

private:
    Ui::AuthentificationForm* authent_form_ui_;

    void setupConnection();
    void setupDesign();

private slots:
    void slotSwitchPage(QWidget* child);
    void slotSendLoginInfo();
    void slotSendRegistrInfo();

signals:
    void sgnlSendAuthInfo(const char* username, const char* password, bool auth_type);
};

#endif // AUTHENTIFICATIONWIDGET_H
