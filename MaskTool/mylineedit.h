#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include <QEvent>
#include <QPushButton>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
private:
    enum AnimationActiveType{Expand = 0, Narrow};
public:
    explicit MyLineEdit(QWidget* parent = nullptr);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private:
    void initPropertyAnimation(AnimationActiveType type);
public slots:
    void lostFocus(Qt::MouseButton button);

private:
    bool expand_state;
    QPropertyAnimation* m_animation;
    QPushButton* m_search_button;
};

#endif // MYLINEEDIT_H
