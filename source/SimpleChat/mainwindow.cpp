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
    connect(authent_widget_, &AuthentificationWidget::sgnlSendAuthInfo,            this, &MainWindow::sltSendAuthInfo);
    connect(main_window_ui_->continueButton,    &QPushButton::clicked,             this, &MainWindow::sltSwitchPage);
    connect(main_window_ui_->messagesArea,      &MessagesArea::sgnlOpenChat,       this, &MainWindow::sltOpenChat);
    connect(main_window_ui_->searchWidget,      &SearchForm::sgnlFindUser,         this, &MainWindow::sltFindUser);
    connect(main_window_ui_->sendMessageWidget, &SendMessageForm::sgnlSendMessage, this, &MainWindow::sltSendMessage);
    connect(&timer_new_senders_,                &QTimer::timeout,                  this, &MainWindow::handleNewSenders);
}

void MainWindow::handleSuccessAuthentification(int auth_type) {
    if (auth_type == LOGIN) {
        main_window_ui_->welcomeTextLabel->setText(QString("Nice to see you again, ") +
                                                            client_.getClientInfo().username + "!");
    } else { /* auth_type = RGSTR*/
        main_window_ui_->welcomeTextLabel->setText(QString("Thank for registration, ") +
                                                            client_.getClientInfo().username + "!");
    }

    main_window_ui_->stackedMainWidget->setCurrentIndex(2);
}

void MainWindow::handleFailedAuthentification() {
    authent_widget_->handleFailedAuthentification();
}

void MainWindow::handleNewSenders() {
    static std::vector<UserInfo> new_senders;

    new_senders = client_.getAllSendersInfo(true);
    for (auto& item : new_senders) {
        qDebug() << item.id;
    }

    main_window_ui_->messagesArea->addNewMessage(new_senders);
}

void MainWindow::sltSendAuthInfo(const char* username, const char* password, int auth_type) {
    if (client_.authorizeUser(username, password, auth_type)) {
        handleSuccessAuthentification(auth_type);
        std::vector<UserInfo> senders_id = client_.getAllSendersInfo(false);

        main_window_ui_->messagesArea->resetMessages(senders_id);
        timer_new_senders_.start(1500);
    } else {
        handleFailedAuthentification();
    }
}

void MainWindow::sltSwitchPage() {
    main_window_ui_->stackedMainWidget->setCurrentIndex(1);
}

void MainWindow::sltOpenChat(const char* id, const char* username) {
    client_.findUser(id, false);

    main_window_ui_->chatStackedWidget->setCurrentIndex(0);
    main_window_ui_->chatUserUsername->setText("Chat with " + QString(username));

    std::vector<FetchedMessage> messages = client_.getAllSenderMessages();
    main_window_ui_->chatArea->setMessages(messages, client_.getClientInfo().id);
}

void MainWindow::sltFindUser(const char *username) {
    if (username == nullptr) {
        std::vector<UserInfo> senders_id = client_.getAllSendersInfo(false);
        main_window_ui_->messagesArea->resetMessages(senders_id);
        return;
    } else if (!strcmp(username, client_.getClientInfo().username)) {
        main_window_ui_->messagesArea->displayNotFoundUser();
        return;
    }

    if (client_.findUser(username, true)) {
        main_window_ui_->messagesArea->displayFoundUser(client_.getFoundUser()->id, client_.getFoundUser()->username);
    } else {
        main_window_ui_->messagesArea->displayNotFoundUser();
    }
}

void MainWindow::sltSendMessage(const char* text) {
    client_.sendMessage(text);
    main_window_ui_->chatArea->addMessage(text, true);
}
