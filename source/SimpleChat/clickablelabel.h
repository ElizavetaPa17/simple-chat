#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include "QLabel"

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    // index is a page ID
    void clicked(QWidget* child);
};

#endif // CLICKEDLABEL_H
