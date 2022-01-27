#ifndef MYCHECKBUTTON_H
#define MYCHECKBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>

class MyCheckButton : public QPushButton
{
    Q_OBJECT
public:
    MyCheckButton();
    explicit MyCheckButton(uint32_t bit, QString& name, bool checked = false, QWidget* parent = nullptr);
    MyCheckButton(const MyCheckButton& check_button);
    virtual ~MyCheckButton();
private:
    void initStyle();
public:
    void contextMenuEvent(QContextMenuEvent *event);
public:
    void setBit(uint32_t bit);
    uint32_t getBit() const;
    MyCheckButton& operator=(const MyCheckButton& check_button);

signals:
    void removeButton(uint32_t bit);
private:
    uint32_t bit;
};

#endif // MYCHECKBUTTON_H
