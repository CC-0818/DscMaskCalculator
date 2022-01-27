#include "mylineedit.h"
#include "QRect"
#include <QCoreApplication>
#include <QTextCursor>
#include <QFocusEvent>
#include <QString>
#include <QIcon>
#include <QSize>
#include <QImage>
#include <QCursor>

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{
    m_animation = new QPropertyAnimation(this, "geometry", this);
    expand_state = false;
    m_search_button = new QPushButton(QIcon(":/svg/resources/circle_cyan.svg") ,"", this);
    m_search_button->setIconSize(QSize(30, 30));
    m_search_button->setCursor(QCursor(Qt::PointingHandCursor));
    m_search_button->hide();
    m_search_button->setFixedSize(40 ,40);
    m_search_button->move(350, 10);
    m_search_button->setMouseTracking(true);
    m_search_button->setStyleSheet("QPushButton{background-color: transparent;"
                                   "border-radius: 20px;}"
                                   "QPushButton:hover{background-color: rgba(80, 80, 80, 0.5);}"
                                   "QPushButton:pressed{background-color: rgba(120, 120, 120, 0.5);}");
    connect(m_search_button, &QPushButton::clicked, [=]() {
        emit editingFinished();
    });
}

void MyLineEdit::enterEvent(QEnterEvent *event)
{
    setCursor(QCursor(Qt::IBeamCursor)); //这句话很奇怪,如果不重新set,光标会消失
    QLineEdit::enterEvent(event);
    if (expand_state)
        return;
    if (m_animation->state() == QPropertyAnimation::Running)
        m_animation->stop();
    initPropertyAnimation(Expand);
    m_animation->start();
    expand_state = true;
}

void MyLineEdit::leaveEvent(QEvent *event)
{
    QLineEdit::leaveEvent(event);
    if (hasFocus() || m_search_button->hasFocus())
        return;
    if (!text().isEmpty())
        return;
    if (m_animation->state() == QPropertyAnimation::Running)
        m_animation->stop();
    initPropertyAnimation(Narrow);
    m_animation->start();
    expand_state = false;
    m_search_button->hide();
}

void MyLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event); //can't remove this,
    m_search_button->show();
}

void MyLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    if (m_search_button->hasFocus())
        return;
    if (!text().isEmpty())
        return;
    this->setCursor(Qt::BlankCursor);
    if (m_animation->state() == QPropertyAnimation::Running)
        m_animation->stop();
    initPropertyAnimation(Narrow);
    m_animation->start();
    expand_state = false;
    m_search_button->hide();
}

void MyLineEdit::initPropertyAnimation(AnimationActiveType type)
{
    QRect start, end;
    switch (type) {
        case Expand:
            m_animation->setDuration(150);
            start = QRect(300 - width()/2, height(), width(), height());
            end = QRect(100, height(), 400, height());
            m_animation->setEasingCurve(QEasingCurve::InQuad);
            break;
        case Narrow:
            m_animation->setDuration(150);
            start = QRect(300 - width()/2, height(), width(), height());
            end = QRect(200, height(), 200, height());
            m_animation->setEasingCurve(QEasingCurve::InQuad);
            break;
        default:
            QCoreApplication::instance()->exit(EXIT_FAILURE);
            break;
    }
    m_animation->setStartValue(start);
    m_animation->setEndValue(end);
}

void MyLineEdit::lostFocus(Qt::MouseButton button)
{
   QFocusEvent event(QFocusEvent::FocusOut);
   QCoreApplication::sendEvent(this, &event);
}
