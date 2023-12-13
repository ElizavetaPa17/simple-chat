#include "messagesarea.h"

#include "messagewidget.h"
#include "founduserform.h"

#include "QFrame"

MessagesArea::MessagesArea(QWidget* parent)
    : QScrollArea(parent)
{
}

MessagesArea::~MessagesArea() {
}

void MessagesArea::clearArea() {
    static QLayoutItem* item;

    while ((item = widget()->layout()->takeAt(0))) {
        if (item->widget()) {
            delete item->widget();
        }

        delete item;
    }
}

void MessagesArea::displayMessages() {
    static MessageWidget* wdgt;

    clearArea();
    widget()->layout()->setAlignment(Qt::AlignTop);

    wdgt = new MessageWidget(this, "HI!");
    connect(wdgt, &MessageWidget::clicked, this, &MessagesArea::sltOpenChat);

    widget()->layout()->addWidget(wdgt);
}

void MessagesArea::displayFoundUser(const char* id, const char* username) {
    static FoundUserForm* wdgt;

    clearArea();
    widget()->layout()->setAlignment(Qt::AlignTop);

    wdgt = new FoundUserForm;
    wdgt->setUserInfo(id, username);
    widget()->layout()->addWidget(wdgt);
    connect(wdgt, &FoundUserForm::clicked, this, &MessagesArea::sltOpenChat);

    QFrame* frame = new QFrame();
    frame->setFrameShape(QFrame::HLine);
    widget()->layout()->addWidget(frame);
}

void MessagesArea::displayNotFoundUser() {
    static QLabel* label_not_found;

    clearArea();
    widget()->layout()->setAlignment(Qt::AlignCenter);

    label_not_found = new QLabel("Such a user wasn't found.");
    label_not_found->setStyleSheet("color: rgb(255, 255, 255);"
                                   "font: 700 11pt \"ori1Uni\";");

    widget()->layout()->addWidget(label_not_found);
}

void MessagesArea::sltOpenChat(const char* username) {
    emit sgnlOpenChat(username);
}
