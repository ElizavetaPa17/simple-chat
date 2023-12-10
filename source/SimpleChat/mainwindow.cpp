#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QMessageBox"
#include "QListWidgetItem"

#include "messagewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , main_window_ui_(new Ui::MainWindow)
    , authent_widget_(new AuthentificationWidget(this))
{
    setupDesign();
    setupConnection();
}

MainWindow::~MainWindow() {
    delete main_window_ui_;
    QLayoutItem* child;;
    while ((child = main_window_ui_->verticalLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}

void MainWindow::setupDesign() {
    main_window_ui_->setupUi(this);

    main_window_ui_->verticalLayout->addWidget(authent_widget_);
    main_window_ui_->stackedMainWidget->setCurrentIndex(0);
    main_window_ui_->chatStackedWidget->setCurrentIndex(1);

    QPixmap pixmap;
    pixmap.load(":../resources/icon/logo.png");
    main_window_ui_->mainWindowLogoLabel->setPixmap(pixmap);
    main_window_ui_->welcomeLogoLabel->setPixmap(pixmap);
}

void MainWindow::setupConnection() {
    connect(authent_widget_, &AuthentificationWidget::sgnlSendAuthInfo, this, &MainWindow::sltSendAuthInfo);
    connect(main_window_ui_->continueButton, &QPushButton::clicked,     this, &MainWindow::sltSwitchPage);
    connect(main_window_ui_->messagesArea, &MessagesArea::sgnlOpenChat, this, &MainWindow::sltOpenChat);
    connect(main_window_ui_->searchWidget, &SearchForm::sgnlFindUser,   this, &MainWindow::sltFindUser);
}

void MainWindow::handleSuccessAuthentification(int auth_type) {
    if (auth_type == LOGIN) {
        main_window_ui_->welcomeTextLabel->setText(QString("Nice to see you again, ") +
                                                            client_.getClientUsername() + "!");
    } else { /* auth_type = RGSTR*/
        main_window_ui_->welcomeTextLabel->setText(QString("Thank for registration, ") +
                                                            client_.getClientUsername() + "!");
    }

    main_window_ui_->stackedMainWidget->setCurrentIndex(2);
}

void MainWindow::handleFailedAuthentification() {
    authent_widget_->handleFailedAuthentification();
}

void MainWindow::sltSendAuthInfo(const char* username, const char* password, int auth_type) {
    if (client_.authorizeUser(username, password, auth_type)) {
        handleSuccessAuthentification(auth_type);
    } else {
        handleFailedAuthentification();
    }
}

void MainWindow::sltSwitchPage() {
    main_window_ui_->stackedMainWidget->setCurrentIndex(1);
}

void MainWindow::sltOpenChat() {
    main_window_ui_->chatStackedWidget->setCurrentIndex(0);
}

void MainWindow::sltFindUser(const char *username) {
    client_.findUser(username);
}
