#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "QLabel"
#include "QPixmap"

class MessageWidget : public QWidget {
    Q_OBJECT
public:
    MessageWidget(QWidget* parent = nullptr, const QString& text = QString(""));
    ~MessageWidget();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event)      override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    const char* username_;
    QLabel  profile_photo_;
    QLabel  sender_name_;

    void setupDesign(const QString& text);

signals:
    void clicked(const char* username);
};

#endif // MESSAGEWIDGET_H
