#ifndef MESSAGESAREA_H
#define MESSAGESAREA_H

#include "QScrollArea"
#include "QVBoxLayout"

class MessagesArea : public QScrollArea {
    Q_OBJECT
public:
    MessagesArea(QWidget* parent = nullptr);
    ~MessagesArea();

    //void addMessage(QString text);
    void displayMessages();
    void displayFoundUser(const char* id, const char* username);
    void displayNotFoundUser();

private:
    void clearArea();

private slots:
    void sltOpenChat();

signals:
    void sgnlOpenChat();
};

#endif // MESSAGESAREA_H