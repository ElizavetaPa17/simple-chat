#include "chatarea.h"
#include "ui_chatarea.h"

#include "QLabel"
#include "QString"

#include "chatmessagetext.h"

ChatArea::ChatArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatArea)
{
    ui->setupUi(this);
    setupDesign();
}

ChatArea::~ChatArea()
{
    delete ui;
}

void ChatArea::setupDesign() {
    ui->chatLayout->setAlignment(Qt::AlignBottom);
}

void ChatArea::clearArea() {
    static QLayoutItem* item;

    while ((item = ui->chatLayout->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        }

        delete item;
    }
}

void ChatArea::setMessages(std::vector<FetchedMessage>& messages, const char* client_id) {
    clearArea();

    ChatMessageText* label = nullptr;
    QString text;
    for (auto& item : messages) {
        label = new ChatMessageText(item.text.c_str());

        if (item.from_id == client_id) {
            ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 1);
            label->setupDesign(RIGHT_ALIGN);
        } else {
            ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 0);
            label->setupDesign(LEFT_ALIGN);
        }
    }
}
