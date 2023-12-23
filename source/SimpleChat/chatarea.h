#ifndef CHATAREA_H
#define CHATAREA_H

#include <QWidget>
#include <vector>

#include "constants.h"

namespace Ui {
class ChatArea;
}

class ChatArea : public QWidget
{
    Q_OBJECT

public:
    explicit ChatArea(QWidget *parent = nullptr);
    ~ChatArea();

    void setMessages(std::vector<FetchedMessage>& messages, const char* client_id);

private:
    Ui::ChatArea *ui;

    void setupDesign();
    void clearArea();
};

#endif // CHATAREA_H
