#include "customview.h"

CustomView::CustomView(QWidget *parent) : QGraphicsView(parent) {}

void CustomView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.1;

    if (event->delta() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1 / scaleFactor, 1 / scaleFactor);
    }
}
