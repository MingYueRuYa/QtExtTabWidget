#include "widget.h"
#include "ui_widget.h"
#include "cus_button.h"

#include <QLabel>
#include <QTabBar>
#include <QPushButton>
#include <QButtonGroup>
#include <QStyleOption>


QRect QExtTabBarStyle::subElementRect(SubElement element,
                                      const QStyleOption *option,
                                      const QWidget *widget) const {
     if (SE_TabBarTabLeftButton == element ) {
          const QStyleOptionTab *tab_option = qstyleoption_cast<const QStyleOptionTab *>(option);
          QRect rect = tab_option->rect;
          rect.setWidth(150);
          rect.setHeight(30);
          return rect;
     }
     else
     {
        return QProxyStyle::subElementRect(element, option, widget);
     }

}

QTabBar *g_bar = nullptr;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    //  "QPushButton{"
    //      "border: 2px solid #8f8f91;"
    //      "border-radius: 6px;"
    //      "background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,"
    //                                        "stop : 0 #f6f7fa, stop: 1 #dadbde);"
    //      "min-width: 80px;"
    //"}"
    QString style = "QPushButton:hover{background-color:yellow;}"
      "QPushButton:pressed{"
      "background-color: #FF00FF00;"
      "}"
      "QPushButton:checked{"
      "background-color: #FFFFFF00;"
      "}";
    
    QExtTabBar *bar = new QExtTabBar(this);
    bar->setStyle(new QExtTabBarStyle(bar));
    bar->addTab("");
    bar->addTab("");
    bar->addTab("");

    QPushButton *label = new QPushButton(bar);
    label->setCheckable(true);
    connect(label, SIGNAL(clicked()), this, SLOT(test()));
    label->setFixedSize(QSize(150,30));
    label->setText("this is label");
    label->setStyleSheet(style);
    QPushButton *label2 = new QPushButton(bar);
    connect(label2, SIGNAL(clicked()), this, SLOT(test2()));
    label2->setCheckable(true);
    label2->setFixedSize(QSize(150,30));
    label2->setText("this is second label");
    label2->setStyleSheet(style);

    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(label);
    group->addButton(label2);

    bar->setTabButton(0, QTabBar::LeftSide, label);
    bar->setTabButton(1, QTabBar::LeftSide, label2);

    CusBtn* cus_btn = new CusBtn(bar);
    cus_btn->setCheckable(true);
    cus_btn->setStyleSheet(style);
    cus_btn->setFixedWidth(150);
    group->addButton(cus_btn);
    bar->setTabButton(2, QTabBar::LeftSide, cus_btn);
    connect(cus_btn, SIGNAL(change_index()), this, SLOT(do_change_index()));

    bar->setStyleSheet("background-color:red;padding:0 0 0 0;");
    bar->setContentsMargins(0, 0, 0, 0);
    ui->widget_tabbar_container->layout()->addWidget(bar);
    ui->widget_tabbar_container->setFixedWidth(450);
    bar->setFixedHeight(ui->widget_tabbar_container->height());
    g_bar = bar;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::test()
{
    QSize size02 = ((QPushButton*)sender())->size();
    QSize size = g_bar->size();
    QSize size_1 = ui->widget_tabbar_container->size();
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::test2()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::do_change_index()
{
    ui->stackedWidget->setCurrentIndex(2);
}
