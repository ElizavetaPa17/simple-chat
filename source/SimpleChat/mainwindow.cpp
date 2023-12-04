#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

void MainWindow::setupConnection() {
    connect(authent_widget_, &AuthentificationWidget::sgnlSendAuthInfo, this, &MainWindow::sltSendAuthInfo);
}

void MainWindow::sltSendAuthInfo(const char* username, const char* password, int auth_type) {
    client_.sendAuthInfo(username, password, auth_type);
}
