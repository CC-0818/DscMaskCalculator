#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QSize>

class MyCheckBox : public QPushButton
{
public:
    MyCheckBox(QWidget* parent = nullptr);
    MyCheckBox(const MyCheckBox &checkBox);
    MyCheckBox(const QString &text, bool checkedState = true, QWidget* parent = nullptr);
    ~MyCheckBox();
    MyCheckBox & operator=(const MyCheckBox &checkBox);

private:
    void initStyle();
};

#endif // MYCHECKBOX_H
