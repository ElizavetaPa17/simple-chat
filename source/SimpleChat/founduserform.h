#ifndef FOUNDUSERFORM_H
#define FOUNDUSERFORM_H

#include <QWidget>
#include "constants.h"

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
    char dest_id_[ID_BUFFER_SIZE]{};
    char dest_username_[USRNM_BUFFER_SIZE]{};

    void setupDesign();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event)      override;

    void mousePressEvent(QMouseEvent* event) override;

signals:
    void clicked(const char* id, const char* username);
};

#endif // FOUNDUSERFORM_H
