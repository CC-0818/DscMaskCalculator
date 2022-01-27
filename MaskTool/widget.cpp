#include "widget.h"
#include "./ui_widget.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QScrollBar>
#include <QRect>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QByteArray>
#include <QPair>
#include <QJsonArray>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initConfigFile();
    initAddButton();
    initWidgetProperty();
    initConnect();
    initMaskList();
    showMaskButton();
}

Widget::~Widget()
{
    delete ui;
    if (m_button_list != nullptr) {
        delete m_button_list;
    }
}

void Widget::initWidgetProperty()
{
    this->setWindowTitle(tr("Mask Tool"));
    /* remove window border */
    this->setWindowFlags(Qt::FramelessWindowHint);
    /* set background transparent complete */
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void Widget::initConnect()
{
    connect(ui->m_frame, &MyFrame::windowChanged, this, &Widget::updateWidgetPosition);
    connect(ui->m_frame, &MyFrame::mousePressed, ui->m_lineEdit, &MyLineEdit::lostFocus);
    connect(ui->m_lineEdit, &MyLineEdit::editingFinished, this, &Widget::updateMaskData);
    connect(add_button, &QPushButton::clicked, [=]() {addNewCheckButton();});
    connect(ui->m_save_button, &QPushButton::clicked, [=]() {updateConfigFile();});
    connect(ui->m_init_button, &QPushButton::clicked, [=]() {defaultConfig();});
}

void Widget::initMaskList()
{
    m_button_list = new QList<MyCheckButton*>;
    uint32_t bit(0);
    for (QPair<QString, bool> var : list) {
        MyCheckButton* button = new MyCheckButton(bit++, var.first, var.second, ui->m_scroll_area_widget);
        connect(button, &MyCheckButton::toggled, this, &Widget::updateLineShow);
        connect(button, &MyCheckButton::removeButton, this, &Widget::removeCheckButton);
        m_button_list->push_back(button);
    }
    updateLineShow();
}

void Widget::showMaskButton()
{
    const uint8_t margin(15);
    ui->m_scroll_area_widget->resize(550, 598);
    QSize curtain_size(ui->m_scroll_area_widget->size());
    uint8_t column(4);

    const QSize bt_size(MyCheckButton().size());
    const uint32_t spacing = (curtain_size.width() - (margin * 2) - bt_size.width()*column) / (column - 1);
    if(m_button_list->isEmpty())
        return;
    QPoint move_to(margin, margin);
    foreach (MyCheckButton* var, *m_button_list) {
        var->move(move_to);
        move_to += QPoint(spacing + bt_size.width(), 0);
        if (move_to.x() >= curtain_size.width()) {
            move_to.setX(margin);
            move_to.setY(move_to.y() + spacing + bt_size.height());
            if(move_to.y() >= curtain_size.height() - bt_size.height()) {
                curtain_size.setHeight(move_to.y() + bt_size.height() + margin);
                ui->m_scroll_area_widget->resize(curtain_size);
            }
        }
    }
    add_button->move(move_to);
}

void Widget::initAddButton()
{
    add_button = new QPushButton("", ui->m_scroll_area_widget);
    add_button->setFixedSize(80, 80);
    add_button->setStyleSheet("QPushButton{background-color: transparent;"
                              "background-image: url(:/svg/resources/add.svg);"
                              "background-repeat: no-repeat;"
                              "background-position: center;"
                              "border-style: none;"
                              "border-radius: 15px;}"
                              "QPushButton:hover{background-color: rgba(46, 45, 44, 0.5);}"
                              "QPushButton:pressed{background-color: rgba(46, 45, 44, 0.7);}");
}

void Widget::addNewCheckButton()
{
    QInputDialog input_dialog;
    input_dialog.resize(QSize(300, 200));
    input_dialog.setFont(QFont("微软雅黑 Light", 16));
    input_dialog.setLabelText("Input mask name:");
    input_dialog.setWindowFlags(Qt::FramelessWindowHint | input_dialog.windowFlags());
    //input_dialog.setAttribute(Qt::WA_TranslucentBackground);
    input_dialog.setStyleSheet("QInputDialog{color: white; background-color: rgb(31, 35, 42);}"
                               "QLabel{color: white;}"
                               "QPushButton{color: white; background-color: rgba(125, 125, 125, 0.3); border-style: solid; border-radius:4px; padding: 1 6 1 6;}"
                               "QPushButton:hover{background-color: rgba(125, 125, 125, 0.5);}"
                               "QPushButton:pressed{background-color: rgba(125, 125, 125, 0.8);}"
                               "QLineEdit{color: black; background-color: rgba(125, 125, 125, 0.8);border-style: none;border-radius: 5px;}");
    if (input_dialog.exec()) {
        QString button_name = input_dialog.textValue().replace("\\n", "\n");
        uint32_t bit(0);
        if (!m_button_list->isEmpty())
            bit = m_button_list->constLast()->getBit() + 1;
        MyCheckButton* button = new MyCheckButton(bit, button_name, false, ui->m_scroll_area_widget);
        connect(button, &MyCheckButton::toggled, this, &Widget::updateLineShow);
        connect(button, &MyCheckButton::removeButton, this, &Widget::removeCheckButton);
        m_button_list->push_back(button);
        showMaskButton();
        updateLineShow();
    }
}

void Widget::initConfigFile()
{
    m_config_file = QString("./config.json");
    QFile f(m_config_file);
    f.open(QFile::ReadWrite);
    QByteArray read = f.readAll();
    f.close();
    if (read.isEmpty()) {
        return;
    }
    QJsonParseError parse_error;
    QJsonDocument doc(QJsonDocument::fromJson(read, &parse_error));
    if (parse_error.error != QJsonParseError::NoError) {
        qDebug() << "parse error";
        return;
    }

    QJsonObject obj = doc.object();
    if (obj.isEmpty())
        return;
    for (int i = 0; i < obj.size(); i++) {
//        qDebug() << obj.find(QString::number(i)).value().toArray().first().toString() << obj.find(QString::number(i)).value().toArray().last().toBool();
        list.push_back(QPair<QString,bool>(obj.find(QString::number(i)).value().toArray().first().toString(),obj.find(QString::number(i)).value().toArray().last().toBool()));
    }

}

void Widget::updateConfigFile()
{
    QJsonObject obj;
    list.clear();
    foreach (MyCheckButton* var, *m_button_list) {
        QJsonArray arr;
        arr.push_back(var->text());
        qDebug() << var->text()<< "" << var->isChecked();
        arr.push_back(var->isChecked());
        obj.insert(QString::number(var->getBit()), arr);
        list.push_back(QPair<QString,bool>(var->text(), var->isChecked()));
    }

    QJsonDocument doc(obj);
    QByteArray write = doc.toJson();
    QFile f(m_config_file);
    f.open(QFile::WriteOnly);
    f.write(write);
    f.close();
}

void Widget::defaultConfig()
{
    foreach (MyCheckButton* var, *m_button_list) {
        var->hide();
        m_button_list->removeOne(var);
    }
    int bit(0);
    for (QPair<QString,bool> var : list) {
            MyCheckButton* button = new MyCheckButton(bit++, var.first, var.second, ui->m_scroll_area_widget);
            connect(button, &MyCheckButton::toggled, this, &Widget::updateLineShow);
            connect(button, &MyCheckButton::removeButton, this, &Widget::removeCheckButton);
            m_button_list->push_back(button);
    }
    updateLineShow();
    showMaskButton();
}

void Widget::updateWidgetPosition(QPoint &point)
{
    this->move(pos().x() + point.x(), pos().y() + point.y());
}

void Widget::updateMaskData()
{
    if (ui->m_lineEdit->text().isEmpty())
        return;
    if (!ui->m_lineEdit->text().contains(QRegularExpression("^\\d+"))) {
        QMessageBox box;
        box.setText("Error input");
        box.exec();
        ui->m_lineEdit->clear();
        return;
    }
    uint32_t mask = ui->m_lineEdit->text().toUInt();
    foreach (MyCheckButton* var, *m_button_list) {
        bool checked = (1 << var->getBit()) & mask;
        var->setChecked(checked);
    }
}

void Widget::updateLineShow()
{
    if (m_button_list->isEmpty())
        return;
    uint32_t mask(0);
    foreach (MyCheckButton* var, *m_button_list) {
        if (var->isChecked())
            mask |=  (1 << var->getBit());
    }

    ui->m_lineEdit->setText(QString().number(mask));
}

void Widget::removeCheckButton(uint32_t bit)
{
    bool left_move(false);
    foreach (MyCheckButton* var, *m_button_list) {
        if (var->getBit() == bit) {
            m_button_list->removeOne(var);
            left_move = true;
            var->hide();
            continue;
        }
        if (left_move) {
            var->setBit(var->getBit() - 1);
        }
    }
    showMaskButton();
    updateLineShow();
}

