#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , main_window_ui_(new Ui::MainWindow)
    , authent_widget_(new AuthentificationWidget(this))
{
    setupDesign();
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
}
