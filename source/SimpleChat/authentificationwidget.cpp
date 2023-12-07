#include "authentificationwidget.h"

#include "QTimer"
#include "constants.h"

AuthentificationWidget::AuthentificationWidget(QWidget* parent)
    : QWidget(parent)
    , authent_form_ui_(new Ui::AuthentificationForm)
{
    setupDesign();
    setupConnection();
}

AuthentificationWidget::~AuthentificationWidget() {
    delete authent_form_ui_;
}

void AuthentificationWidget::setupConnection() {
    connect(authent_form_ui_->registerLabel, &ClickableLabel::clicked, this, &AuthentificationWidget::slotSwitchPage);
    connect(authent_form_ui_->loginLabel,   &ClickableLabel::clicked, this, &AuthentificationWidget::slotSwitchPage);

    connect(authent_form_ui_->loginButton,   &QPushButton::clicked, this, &AuthentificationWidget::slotSendLoginInfo);
    connect(authent_form_ui_->registerButton, &QPushButton::clicked, this, &AuthentificationWidget::slotSendRegistrInfo);
}

void AuthentificationWidget::setupDesign() {
    setAttribute(Qt::WA_StyledBackground);
    authent_form_ui_->setupUi(this);

    QPixmap pixmap;
    pixmap.load(":../resources/icon/label.png");
    authent_form_ui_->logoLoginLabel->setPixmap(pixmap);
    authent_form_ui_->logoRegisterLabel->setPixmap(pixmap);

    authent_form_ui_->authLoginErrorLabel->hide();
    authent_form_ui_->authRegisterErrorLabel->hide();
}

void AuthentificationWidget::slotSwitchPage(QWidget* child) {
    if (child == authent_form_ui_->loginLabel) {
        authent_form_ui_->stackedAuthWidget->setCurrentIndex(0);
    } else {
        authent_form_ui_->stackedAuthWidget->setCurrentIndex(1);
    }
}

void AuthentificationWidget::slotSendLoginInfo() {
    if (authent_form_ui_->usrnameLoginLineedit->text().isEmpty() ||
        authent_form_ui_->passLoginLineedit->text().isEmpty())
    {
        authent_form_ui_->authLoginErrorLabel->setText("Error. All fields must be filled in.");
        authent_form_ui_->authLoginErrorLabel->show();
        QTimer::singleShot(3000, authent_form_ui_->authLoginErrorLabel, &QLabel::hide);
        return;
    }

    emit sgnlSendAuthInfo(authent_form_ui_->usrnameLoginLineedit->text().toUtf8().toStdString().c_str(),
                          authent_form_ui_->passLoginLineedit->text().toUtf8().toStdString().c_str(), LOGIN);
}

void AuthentificationWidget::slotSendRegistrInfo() {
    if (authent_form_ui_->confpassRegisterLineedit->text().isEmpty() ||
        authent_form_ui_->passRegisterLineedit->text().isEmpty()     ||
        authent_form_ui_->usrnameRegisterLineedit->text().isEmpty())
    {
        authent_form_ui_->authRegisterErrorLabel->setText("Error. All fields must be filled in.");
        authent_form_ui_->authRegisterErrorLabel->show();
        QTimer::singleShot(3000, authent_form_ui_->authRegisterErrorLabel, &QLabel::hide);
        return;
    }

    if (authent_form_ui_->confpassRegisterLineedit->text() != authent_form_ui_->passRegisterLineedit->text()) {
        authent_form_ui_->authRegisterErrorLabel->setText("Error. Passwords don't match.");
        authent_form_ui_->authRegisterErrorLabel->show();
        QTimer::singleShot(3000, authent_form_ui_->authRegisterErrorLabel, &QLabel::hide);
        return;
    }

    emit sgnlSendAuthInfo(authent_form_ui_->usrnameRegisterLineedit->text().toUtf8().toStdString().c_str(),
                          authent_form_ui_->passRegisterLineedit->text().toUtf8().toStdString().c_str(), RGSTR);
}
