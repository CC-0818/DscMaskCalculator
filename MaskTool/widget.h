#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QList>
#include <QPair>
#include "mycheckbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    void initWidgetProperty();
    void initConnect();
    void initMaskList();
    void showMaskButton();
    void initAddButton();
    void addNewCheckButton();
    void initConfigFile();
    void updateConfigFile();
    void defaultConfig();
public slots:
    void updateWidgetPosition(QPoint& point);
    void updateMaskData();
    void updateLineShow();
    void removeCheckButton(uint32_t bit);
private:
    Ui::Widget *ui;
    QPropertyAnimation* m_button_ai;
    QList<MyCheckButton*>* m_button_list;
    QPushButton* add_button;
    QString m_config_file;
    QList<QPair<QString,bool>> list;
};
#endif // WIDGET_H
