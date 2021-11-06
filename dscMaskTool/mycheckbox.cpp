#include "mycheckbox.h"

MyCheckBox::MyCheckBox(QWidget *parent)
{
    this->setParent(parent);
    this->setCheckable(true);
    initStyle();
}

MyCheckBox::MyCheckBox(const MyCheckBox &checkBox)
{
    *this = checkBox;
}

MyCheckBox::MyCheckBox(const QString &text, bool checkedState, QWidget *parent)
{
    this->setCheckable(true);
    this->setText(text);
    this->setParent(parent);
    this->setChecked(checkedState);
    initStyle();
}

MyCheckBox::~MyCheckBox()
{

}

MyCheckBox &MyCheckBox::operator=(const MyCheckBox &checkBox)
{
    this->setCheckable(checkBox.isCheckable());
    this->setText(checkBox.text());
    this->setChecked(checkBox.isChecked());
    this->setStyleSheet(checkBox.styleSheet());
    this->setFixedSize(checkBox.size());
    this->setFont(checkBox.font());
    this->setPalette(checkBox.palette());
    return *this;
}

void MyCheckBox::initStyle()
{
    setFixedSize(QSize(80, 80));
    QFont font("dengxian", 10);
    font.setBold(true);
    setFont(font);
    QPalette pal;
    pal.setColor(QPalette::ButtonText, QColor("#ffffff"));
    setPalette(pal);
    setStyleSheet("QPushButton{border-image: url(:/icon/indicatorUnCheck.png);}"
                  "QPushButton:checked{border-image: url(:/icon/indicatorChecked.png);}"
                  "QPushButton:hover{border-image: url(:/icon/hoverUnCheck.png);border:none;}"
                  "QPushButton:hover:checked{border-image: url(:/icon/hoverChecked.png);border:none;}");

}
