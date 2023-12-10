#include "clickablelabel.h"

#include "QMouseEvent"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags flags)
    : QLabel(parent, flags)
{
    setMouseTracking(true);
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    QLabel::mousePressEvent(event);
    event->accept();

    emit clicked(this);
}
