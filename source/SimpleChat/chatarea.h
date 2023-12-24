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
    void addMessage(const QString& text, bool is_outgoing);

private:
    Ui::ChatArea *ui;

    void setupDesign();
    void clearArea();
    void insertEmptyLabel(int align_type);
};

#endif // CHATAREA_H
