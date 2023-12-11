#ifndef FOUNDUSERFORM_H
#define FOUNDUSERFORM_H

#include <QWidget>

namespace Ui {
class FoundUserForm;
}

class FoundUserForm : public QWidget
{
    Q_OBJECT

public:
    explicit FoundUserForm(QWidget *parent = nullptr);
    ~FoundUserForm();

    void setUserInfo(const char* id, const char* username);    

private:
    Ui::FoundUserForm *ui;
    void setupDesign();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event)      override;

    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked();
};

#endif // FOUNDUSERFORM_H
