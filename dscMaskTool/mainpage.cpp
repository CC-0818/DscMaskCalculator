#include "mainpage.h"
#include <QFont>
#include <QSize>
#include <QPalette>
#include <QPainter>
#include <QLabel>
#include <QPen>
#include <QMouseEvent>
#include <QIcon>

static const QSize WINDOW_SIZE(800, 600);
static const QSize TITLE_BAR_SIZE(800, 30);
static const QSize BUTTON_SIZE(TITLE_BAR_SIZE.height(), TITLE_BAR_SIZE.height());
static const QString ICON_FILE(":/icon/Calculator.ico");
static const QSize DSC_LABEL_SIZE(200, 100);

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
{
    this->setFont(QFont("dengxian" ,10, true));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(WINDOW_SIZE);
    this->setMouseTracking(true);
    QIcon icon(ICON_FILE);
    this->setWindowIcon(icon);
    //this->setWindowOpacity(0.9);

    QFont font("dengxian", 12);
    title = new QLabel("Dsc Mask Calculator ", this);
    title->setFont(font);
    dscMaskLabel = new QLabel("", this);
    dscMaskLabel->setGeometry((150 - DSC_LABEL_SIZE.width()/2), 110, DSC_LABEL_SIZE.width(), DSC_LABEL_SIZE.height());
    dscMaskLabel->setFont(QFont("dengxian", 60));
    dscMaskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    dscMaskLabel->setAlignment(Qt::AlignCenter);
    dscMaskLabel->setStyleSheet("color: #00c25f;");

    title->move(TITLE_BAR_SIZE.height() + 5, (TITLE_BAR_SIZE.height() - 12)/2);
    minimum = new QPushButton("_", this);
    close   = new QPushButton("X", this);
    mode    = new QPushButton("", this);
    mode->setCheckable(true);
    connect(minimum, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(close, &QPushButton::clicked, this, &QWidget::close);
    connect(mode, &QPushButton::toggled, [=](bool checked){setDarkMode(checked);});
    font.setBold(true);
    close->setFont(font);
    minimum->setFont(font);
    close->setGeometry(width()-BUTTON_SIZE.width(), (TITLE_BAR_SIZE.height()-BUTTON_SIZE.height())/2, BUTTON_SIZE.width(), BUTTON_SIZE.height());
    minimum->setGeometry(width()-BUTTON_SIZE.width()*2, (TITLE_BAR_SIZE.height()-BUTTON_SIZE.height())/2, BUTTON_SIZE.width(), BUTTON_SIZE.height());
    mode->setGeometry(width()-BUTTON_SIZE.width()*3, (TITLE_BAR_SIZE.height()-BUTTON_SIZE.height())/2, BUTTON_SIZE.width(), BUTTON_SIZE.height());

    close->setStyleSheet("QPushButton{border:none;}"
                         "QPushButton:hover{background-color: rgb(255, 0, 0);border:none;color:rgb(255, 255, 255);}");
    minimum->setStyleSheet("QPushButton{border:none;}"
                           "QPushButton:hover{background-color: rgba(192, 192, 192, 70);border:none;}");
    initCheckBoxVector();
    initCheckBoxLayout();
    /* default dark mode */
    mode->setChecked(true);
    setDarkMode(false);
    setDarkMode(true);
}

MainPage::~MainPage()
{
}

void MainPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)


    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), pixBackground);
}

void MainPage::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        dragPos = event->globalPos();
        event->accept();
    }
}

void MainPage::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton) {
        dragPos = event->globalPos();
        event->accept();
    }
}

void MainPage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && event->pos().y() < TITLE_BAR_SIZE.height()) {
        QPoint offset = event->globalPos() - dragPos;
        move(this->pos() + offset);
        dragPos = event->globalPos();
        event->accept();
    }
}

void MainPage::setDarkMode(bool mode)
{
    if(mode == true) {
        titleBarBg = QColor("#2e2f30");
        windowBg = QColor("#404142");
        windowFontFg = QColor("#FFFFFF");

        this->mode->setStyleSheet("QPushButton{border-image: url(:/icon/Light.png);}"
                            "QPushButton:hover{background-color: rgba(192, 192, 192, 70);border:none;}");
    }else {
        titleBarBg = QColor("#FFFFFF");
        windowBg = QColor("#EFEFEF");
        windowFontFg = QColor("#000000");

        this->mode->setStyleSheet("QPushButton{border-image: url(:/icon/Dark.png);}"
                            "QPushButton:hover{background-color: rgba(192, 192, 192, 70);border:none;}");
    }
    /* reset color */
    QPalette pal;
    pal.setColor(QPalette::WindowText, windowFontFg);
    title->setPalette(pal);
    pal.setColor(QPalette::ButtonText, windowFontFg);
    close->setPalette(pal);
    minimum->setPalette(pal);
    for(QVector<MyCheckBox>::iterator it = checkBox.begin(); it != checkBox.end(); it++)
         it->setPalette(pal);

    /* draw background */
    pixBackground = QPixmap(width(), height());
    pixBackground.fill(Qt::white);
    QPainter cachePainter(&pixBackground);
    cachePainter.setRenderHint(QPainter::Antialiasing);
    cachePainter.fillRect(0, 0, this->width(), this->height(), windowBg);
    cachePainter.fillRect(0, 0, this->width(), TITLE_BAR_SIZE.height(), titleBarBg);
    QPixmap titleIcon;
    titleIcon.load(ICON_FILE);
    titleIcon.scaled(QSize(20, 20), Qt::KeepAspectRatio);
    cachePainter.drawPixmap((TITLE_BAR_SIZE.height()-20)/2, (TITLE_BAR_SIZE.height()-20)/2, 20, 20, titleIcon);
    QPen pen;
    pen.setColor(QColor("#FF6781"));
    pen.setWidth(2);
    cachePainter.setPen(pen);
    /* draw line round */
    QPoint rotateCenter(150, 110 + DSC_LABEL_SIZE.height()/2);
    QPoint lineStart(100, 0);
    cachePainter.translate(rotateCenter);
    for(int i = 0; i < 60; i++) {
        cachePainter.rotate(6);
        cachePainter.drawLine(lineStart, lineStart + QPoint(10, 0));
    }
    /* update UI */
    update();
}

void MainPage::initCheckBoxVector()
{
    checkBox.append(MyCheckBox("UIM"));
    checkBox.append(MyCheckBox("PWR \nBank"));
    checkBox.append(MyCheckBox("SD \nCard"));
    checkBox.append(MyCheckBox("Wifi"));
    checkBox.append(MyCheckBox("CD \nRom", false));
    checkBox.append(MyCheckBox("USSD"));
    checkBox.append(MyCheckBox("SMS"));
    checkBox.append(MyCheckBox("Phone \nBook"));
    checkBox.append(MyCheckBox("Wifi \nExtender", false));
    checkBox.append(MyCheckBox("WDS", false));
    checkBox.append(MyCheckBox("WPS"));
    checkBox.append(MyCheckBox("Local \nUpdate"));
    checkBox.append(MyCheckBox("UPNP", false));
    checkBox.append(MyCheckBox("Port \nForward", false));
    checkBox.append(MyCheckBox("DMZ", false));
    checkBox.append(MyCheckBox("DDNS", false));
}

void MainPage::initCheckBoxLayout()
{
    const QPoint margin(80, 350);
    const int columns = 6;
    const QPoint offset((width()-margin.x()*2)/(columns - 1), 100);
    QPoint startDot(margin);
    int i = 0;
    for(QVector<MyCheckBox>::iterator it = checkBox.begin(); it != checkBox.end(); it++) {
        it->setParent(this);
        it->move(startDot - QPoint(it->width()/2, it->height()/2));
        connect(it, &QPushButton::toggled, [=](){updateDscMask();});
        if(i >= columns - 1) {
            startDot = QPoint(margin.x(), startDot.y() + offset.y());
            i = 0;
        }else {
            startDot.setX(startDot.x() + offset.x());
            i++;
        }
    }
    updateDscMask();
}

void MainPage::updateDscMask()
{
    int i = 0;
    dscMask = 0;
    for(QVector<MyCheckBox>::iterator it = checkBox.begin(); it != checkBox.end(); it++) {
        if(it->isChecked() == true)
            dscMask |= 1 << i;
        i++;
    }
    dscMaskLabel->setText(QString::number(dscMask,10));
    update();
}



