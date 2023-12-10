#ifndef MESSAGESAREA_H
#define MESSAGESAREA_H

#include "QScrollArea"
#include "QVBoxLayout"

class MessagesArea : public QScrollArea {
    Q_OBJECT
public:
    MessagesArea(QWidget* parent = nullptr);
    ~MessagesArea();

    void addMessage(QString text);

private:
    QVBoxLayout* vrtclLayout;

private slots:
    void sltOpenChat();

signals:
    void sgnlOpenChat();
};

#endif // MESSAGESAREA_H
