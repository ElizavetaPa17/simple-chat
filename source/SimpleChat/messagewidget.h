#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "QLabel"
#include "QPixmap"

class MessageWidget : public QWidget {
    Q_OBJECT
public:
    MessageWidget(const QString& dest_id, const QString& dest_username, const QString& date, QWidget* parent = nullptr);
    ~MessageWidget();

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event)      override;

    void mousePressEvent(QMouseEvent* event) override;

private:
    QString sender_id_;
    QString sender_name_;

    QLabel  profile_photo_;
    QLabel  profile_name_;
    //QLabel  date_;

    void setupDesign(const QString& dest_id, const QString& dest_username);

signals:
    void clicked(const char* dest_id, const char* dest_username);
};

#endif // MESSAGEWIDGET_H
