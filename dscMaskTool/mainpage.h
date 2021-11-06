#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QPoint>
#include <QColor>
#include <QLabel>
#include <QVector>
#include <QRect>
#include <QPixmap>
#include <QImage>
#include "mycheckbox.h"


class MainPage : public QWidget
{
    Q_OBJECT
public:
    MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void setDarkMode(bool mode);
    void initCheckBoxVector();
    void initCheckBoxLayout();
    void updateDscMask();

private:
    QPixmap pixBackground;
    QPoint dragPos;
    QLabel* title;
    QLabel* dscMaskLabel;
    QPushButton* close;
    QPushButton* minimum;
    QPushButton* mode;

    QColor titleBarBg;
    QColor windowBg;
    QColor windowFontFg;

    /* check box */
    QVector<MyCheckBox> checkBox;
    /* dsc_mask */
    uint32_t dscMask;
 public:

};
#endif // MAINPAGE_H
