#include "messagewidget.h"

#include "QHBoxLayout"
#include "QEnterEvent"

MessageWidget::MessageWidget(const QString& dest_id, const QString& dest_username, const QString& date, QWidget* parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setupDesign(dest_id, dest_username);

    sender_id_ = dest_id;
    sender_name_ = dest_username;
}

MessageWidget::~MessageWidget() {
}

void MessageWidget::setupDesign(const QString& dest_id, const QString& dest_username) {
    profile_name_.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    profile_name_.setStyleSheet(".QLabel { font: 700 12pt \"ori1Uni\"; }");
    profile_name_.setText("ID: " + dest_id + "\nUsername: " + dest_username);

    QPixmap pixmap;
    pixmap.load(":../resources/icon/profile_icon.png");
    pixmap = pixmap.scaled(70, 70);
    profile_photo_.setPixmap(pixmap);
    profile_photo_.setFixedSize(71, 71);

    QHBoxLayout* hzLayout = new QHBoxLayout;
    hzLayout->addWidget(&profile_photo_);
    hzLayout->addWidget(&profile_name_);
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
    emit clicked(sender_id_.toStdString().c_str(), sender_name_.toStdString().c_str());
}
