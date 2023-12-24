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
            ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 1, 1, 1);
            label->setupDesign(RIGHT_ALIGN);

            if (ui->chatLayout->rowCount() == 2) {
                insertEmptyLabel(RIGHT_ALIGN);
            }

        } else {
            ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 0, 1, 1);
            label->setupDesign(LEFT_ALIGN);

            if (ui->chatLayout->rowCount() == 2) {
                insertEmptyLabel(LEFT_ALIGN);
            }
        }
    }
}

void ChatArea::insertEmptyLabel(int align_type) {
    QLabel* empty_label = new QLabel("");
    empty_label->setFixedWidth(FIXED_MSG_WIDTH+50);

    if (align_type == RIGHT_ALIGN) {
        ui->chatLayout->addWidget(empty_label, 0, 0, 1, 1);
    } else {
        ui->chatLayout->addWidget(empty_label, 0, 1, 1, 1);
    }
}

void ChatArea::addMessage(const QString& text, bool is_outgoing) {
    ChatMessageText* label = new ChatMessageText(text.toStdString().c_str());

    if (is_outgoing) {
        ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 1, 1, 1);
        label->setupDesign(RIGHT_ALIGN);

        if (ui->chatLayout->rowCount() == 2) {
            insertEmptyLabel(RIGHT_ALIGN);
        }
    } else {
        ui->chatLayout->addWidget(label, ui->chatLayout->rowCount(), 0, 1, 1);
        label->setupDesign(LEFT_ALIGN);

        if (ui->chatLayout->rowCount() == 2) {
            insertEmptyLabel(LEFT_ALIGN);
        }
    }
}
