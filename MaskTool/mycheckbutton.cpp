#include "mycheckbutton.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

MyCheckButton::MyCheckButton()
{
    setFixedSize(80 , 80);
}

MyCheckButton::MyCheckButton(uint32_t bit, QString& name,  bool checked, QWidget *parent) : QPushButton(parent),bit(bit)
{
    initStyle();
    setText(name);
    setChecked(checked);
}

MyCheckButton::MyCheckButton(const MyCheckButton &check_button)
{
    *this = check_button;
}

MyCheckButton::~MyCheckButton()
{
}

void MyCheckButton::initStyle()
{
    setCheckable(true);
    setFixedSize(80 , 80);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(QString("QPushButton{"
                          "color: white;"
                          "border-image: url(:/svg/resources/round_red.svg) 80% no-repeat;"
                          "background-color: rgba(0,0,0,0);"
                          "border-style: none;"
                          "border-radius: 15px;}"
                          "QPushButton:hover{background-color: rgba(0,0,0,0.3);}"
                          "QPushButton:pressed{background-color: rgba(0,0,0,0.6);}"
                          "QPushButton:checked{border-image: url(:/svg/resources/round_green.svg) 80% no-repeat;}"));
    show();
}

void MyCheckButton::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu rightMenu(this);
    rightMenu.setWindowFlags(Qt::FramelessWindowHint | rightMenu.windowFlags() | Qt::NoDropShadowWindowHint);
    //rightMenu.setAttribute(Qt::WA_TranslucentBackground);
    rightMenu.setStyleSheet("QMenu{color:white; font: 25 10pt '微软雅黑 Light'; background-color: rgb(31, 35, 42);}"
                            "QMenu::item {background-color: transparent; padding: 1 5 1 5;}"
                            "QMenu::item:selected {background-color: rgba(80, 80, 80, 0.8); border-style: none; border-radius:4px}");
    QAction* act = rightMenu.addAction(QIcon(":/svg/resources/delete.svg"), "delete");
    rightMenu.move(event->globalPos());
    connect(act, &QAction::triggered, [=]() {
       emit removeButton(bit);
    });
    rightMenu.exec();
}

void MyCheckButton::setBit(uint32_t bit)
{
    this->bit = bit;
}

uint32_t MyCheckButton::getBit() const
{
    return bit;
}

MyCheckButton &MyCheckButton::operator=(const MyCheckButton &check_button)
{
    setBit(check_button.getBit());
    setStyleSheet(check_button.styleSheet());
    setText(check_button.text());
    setFixedSize(check_button.size());
    setCursor(check_button.cursor());
    setCheckable(check_button.isCheckable());
    return *this;
}
