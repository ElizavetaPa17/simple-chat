#include "messagewidget.h"

#include "QHBoxLayout"
#include "QEnterEvent"

MessageWidget::MessageWidget(QWidget* parent, const QString& text)
    : QWidget(parent)
{
    setMouseTracking(true);
    setupDesign(text);
}

MessageWidget::~MessageWidget() {
}

void MessageWidget::setupDesign(const QString& text) {
    sender_name_.setText(text);

    QPixmap pixmap;
    pixmap.load(":../resources/icon/profile_icon.png");
    pixmap = pixmap.scaled(70, 70);
    profile_photo_.setPixmap(pixmap);

    QHBoxLayout* hzLayout = new QHBoxLayout;
    hzLayout->addWidget(&profile_photo_);
    hzLayout->addWidget(&sender_name_);
    setLayout(hzLayout);

    setFixedHeight(100);
}

void MessageWidget::enterEvent(QEnterEvent* event) {
    setCursor(Qt::PointingHandCursor);
}

void MessageWidget::leaveEvent(QEvent* event) {
    setCursor(Qt::ArrowCursor);
}

void MessageWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
