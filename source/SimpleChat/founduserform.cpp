#include "founduserform.h"
#include "ui_founduserform.h"

FoundUserForm::FoundUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FoundUserForm)
{
    ui->setupUi(this);

    setupDesign();
}

FoundUserForm::~FoundUserForm()
{
    delete ui;
}

void FoundUserForm::setupDesign() {
    QPixmap pixmap;
    pixmap.load(":../resources/icon/profile_icon.png");
    pixmap = pixmap.scaled(70, 70);

    ui->userPhoto->setFixedSize(100, 75);
    ui->userPhoto->setPixmap(pixmap);

    setFixedHeight(100);
}

void FoundUserForm::setUserInfo(const char* id, const char* username) {
    ui->userInfo->setText(QString("ID: ") + id + "\n"
                        "Username: " + username);
}

void FoundUserForm::enterEvent(QEnterEvent* event) {
    setCursor(Qt::PointingHandCursor);
}

void FoundUserForm::leaveEvent(QEvent* event) {
    setCursor(Qt::ArrowCursor);
}

void FoundUserForm::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}
