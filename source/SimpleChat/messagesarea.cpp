#include "messagesarea.h"

#include "messagewidget.h"

MessagesArea::MessagesArea(QWidget* parent)
    : QScrollArea(parent), vrtclLayout(nullptr)
{
    vrtclLayout = new QVBoxLayout(this);
}

MessagesArea::~MessagesArea() {
    QLayoutItem* item;
    QWidget*     widget;

    while (item = layout()->takeAt(0)) {
        if ((widget = item->widget()) != nullptr) {
            delete widget;
        }

        delete item;
    }

    delete vrtclLayout;
}

void MessagesArea::addMessage(QString text) {
    static MessageWidget* wdgt;

    wdgt = new MessageWidget(this, text);
    connect(wdgt, &MessageWidget::clicked, this, &MessagesArea::sltOpenChat);

    vrtclLayout->addWidget(wdgt);
}

void MessagesArea::sltOpenChat() {
    emit sgnlOpenChat();
}
