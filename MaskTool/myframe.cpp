#include "myframe.h"

MyFrame::MyFrame()
{

}

MyFrame::MyFrame(QWidget *parent, Qt::WindowFlags f) : QFrame(parent)
{
    this->setWindowFlags(f);
}

void MyFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        this->mouse_point = event->globalPosition().toPoint();
        emit mousePressed(Qt::LeftButton);
    }
}

void MyFrame::mouseMoveEvent(QMouseEvent *event)
{
    if ((mouse_point.y() - mapToGlobal(pos()).y()) >= 60)
        return;
    if (event->buttons() & Qt::LeftButton) {
        QPoint new_mouse_point = event->globalPosition().toPoint();
        QPoint offset = new_mouse_point - mouse_point;
        mouse_point = new_mouse_point;
        emit windowChanged(offset);
    }
}
