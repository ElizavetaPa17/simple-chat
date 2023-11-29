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

private:
    Ui::AuthentificationForm* authent_form_ui_;

    void setupConnection();
    void setupDesign();

private slots:
    void slotSwitchPage(QWidget* child);
};

#endif // AUTHENTIFICATIONWIDGET_H
