#include "authentificationwidget.h"

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
    connect(authent_form_ui_->loginLabel, &ClickableLabel::clicked, this, &AuthentificationWidget::slotSwitchPage);
}

void AuthentificationWidget::setupDesign() {
    setAttribute(Qt::WA_StyledBackground);
    authent_form_ui_->setupUi(this);

    QPixmap pixmap;
    pixmap.load(":../resources/icon/label.png");
    authent_form_ui_->logoLoginLabel->setPixmap(pixmap);
    authent_form_ui_->logoRegistrLabel->setPixmap(pixmap);
}

void AuthentificationWidget::slotSwitchPage(QWidget* child) {
    if (child == authent_form_ui_->loginLabel) {
        authent_form_ui_->stackedAuthWidget->setCurrentIndex(0);
    } else {
        authent_form_ui_->stackedAuthWidget->setCurrentIndex(1);
    }
}
