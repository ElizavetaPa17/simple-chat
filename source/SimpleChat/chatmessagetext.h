#ifndef CHATMESSAGETEXT_H
#define CHATMESSAGETEXT_H

#include <QTextEdit>

enum DesignAlignment {
    LEFT_ALIGN,
    RIGHT_ALIGN
};

class ChatMessageText : public QTextEdit
{
public:
    explicit ChatMessageText(QWidget *parent = nullptr);
    explicit ChatMessageText(const QString& text, QWidget *parent = nullptr);

    ~ChatMessageText();

    void setupDesign(int align_type);
};

#endif // CHATMESSAGETEXT_H
