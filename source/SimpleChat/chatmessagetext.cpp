#include "chatmessagetext.h"

#include <QFontMetrics>

const int FIXED_WIDTH = 500;
const int CHARS_IN_LINE = 52;

ChatMessageText::ChatMessageText(QWidget *parent)
    : QTextEdit(parent)
{
}

ChatMessageText::ChatMessageText(const QString& text, QWidget *parent)
    : QTextEdit(text, parent)
{
}

ChatMessageText::~ChatMessageText() {
}

void ChatMessageText::setupDesign(int align_type) {
    if (align_type == LEFT_ALIGN) {
        setStyleSheet(".QTextEdit {"
                                  "   background-color: rgb(0, 22, 100); "
                                  "   border-top-left-radius: 10px; "
                                  "   border-top-right-radius: 10px; "
                                  "   border-bottom-right-radius: 10px; "
                                  "   font: 700 11pt \"ori1Uni\"; "
                                  " }");
    } else {
        setStyleSheet(".QTextEdit {"
                                  "   background-color: rgb(0, 22, 100); "
                                  "   border-top-left-radius: 10px; "
                                  "   border-top-right-radius: 10px; "
                                  "   border-bottom-left-radius: 10px; "
                                  "   font: 700 11pt \"ori1Uni\"; "
                                  " }");
    }

    QFontMetrics font_metrics(font());
    size_t font_width = font_metrics.size(0, toPlainText()).width();
    size_t font_height = font_metrics.height();

    double chars_in_line = toPlainText().size() / (static_cast<double>(font_width) / FIXED_WIDTH);
    int line_number = toPlainText().size() / chars_in_line + 0.5;
    qDebug() << "size:" << toPlainText().size();
    qDebug() << "chars line:" << chars_in_line;
    qDebug() << "font width" << font_width;
    qDebug() << "line number:" << line_number;
    setFixedHeight(font_height * (line_number+2));

    setReadOnly(true);
}
