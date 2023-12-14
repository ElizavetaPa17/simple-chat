#ifndef MESSAGESAREA_H
#define MESSAGESAREA_H

#include "QScrollArea"
#include "QVBoxLayout"

#include "constants.h"

class MessagesArea : public QScrollArea {
    Q_OBJECT
public:
    MessagesArea(QWidget* parent = nullptr);
    ~MessagesArea();

    void displayMessages();
    void displayFoundUser(const char* id, const char* username);
    void displayNotFoundUser();

private:
    void clearArea();

private slots:
    void sltOpenChat(const char* dest_id, const char* dest_username);

signals:
    void sgnlOpenChat(const char* dest_id, const char* dest_username);
};

#endif // MESSAGESAREA_H
