#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QMessageBox"

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

    QPixmap pixmap;
    pixmap.load(":../resources/icon/logo.png");
    main_window_ui_->mainWindowLogoLabel->setPixmap(pixmap);
    main_window_ui_->welcomeLogoLabel->setPixmap(pixmap);
}

void MainWindow::setupConnection() {
    connect(authent_widget_, &AuthentificationWidget::sgnlSendAuthInfo, this, &MainWindow::sltSendAuthInfo);
    connect(main_window_ui_->continueButton, &QPushButton::clicked,     this, &MainWindow::sltSwitchPage);
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
