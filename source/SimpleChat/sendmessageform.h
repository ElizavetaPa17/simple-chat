#ifndef SENDMESSAGEFORM_H
#define SENDMESSAGEFORM_H

#include <QWidget>
#include "constants.h"

namespace Ui {
class SendMessageForm;
}

class SendMessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit SendMessageForm(QWidget *parent = nullptr);
    ~SendMessageForm();

private:
    Ui::SendMessageForm *ui;

signals:
    void sgnlSendMessage(const char* text);

private slots:
    void sltSendMessage();
};

#endif // SENDMESSAGEFORM_H
