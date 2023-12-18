#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "QLabel"
#include "QPixmap"

class MessageWidget : public QWidget {
    Q_OBJECT
public:
    MessageWidget(const QString& id, const QString& username, const QString& date, QWidget* parent = nullptr);
    ~MessageWidget();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event)      override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    const char* sender_id_;
    QLabel  profile_photo_;
    QLabel  sender_name_;
    QLabel  date_;

    void setupDesign(const QString& text);
    void fetchID(const QString& text);

signals:
    void clicked(const char* id, const char* username);
};

#endif // MESSAGEWIDGET_H
