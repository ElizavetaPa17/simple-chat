#include "sendmessageform.h"
#include "ui_sendmessageform.h"

#include "QMessageBox"

#include "constants.h"

SendMessageForm::SendMessageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendMessageForm) {
    ui->setupUi(this);
    connect(ui->sendButton, &QPushButton::clicked, this, &SendMessageForm::sltSendMessage);
}

SendMessageForm::~SendMessageForm() {
    delete ui;
}

void SendMessageForm::sltSendMessage() {
    if (ui->messageText->toPlainText().size() > MAX_MSG_SIZE) {
        QMessageBox::warning(nullptr, "Max message length", "You've recieved max message length.",
                             QMessageBox::Ok);
        return;
    } else {
        emit sgnlSendMessage(ui->messageText->toPlainText().toUtf8().toStdString().c_str());
    }
}
