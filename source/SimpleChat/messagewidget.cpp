#include "messagewidget.h"

#include "QHBoxLayout"
#include "QEnterEvent"

MessageWidget::MessageWidget(const QString& id, const QString& username, const QString& date, QWidget* parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setupDesign(id);
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
    emit clicked("test id", "test name");
}
