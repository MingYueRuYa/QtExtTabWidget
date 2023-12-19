#include "widget.h"
#include "ui_widget.h"
#include "cus_button.h"
#include "ext_tab_bar.h"
#include "ext_tab_bar_style.h"

#include <QLabel>
#include <QTabBar>
#include <QPushButton>
#include <QButtonGroup>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    QString style = "QPushButton:hover{background-color:yellow;}"
      "QPushButton:pressed{"
      "background-color: #FF00FF00;"
      "}"
      "QPushButton:checked{"
      "background-color: #FFFFFF00;"
      "}";
    
    tab_bar_ = new QExtTabBar(this);
    tab_bar_->setStyle(new QExtTabBarStyle(tab_bar_));
    tab_bar_->addTab("");
    tab_bar_->addTab("");
    tab_bar_->addTab("");

    QPushButton* label = new QPushButton(tab_bar_);
    label->setCheckable(true);
    connect(label, SIGNAL(clicked()), this, SLOT(test()));
    label->setFixedSize(QSize(150,30));
    label->setText("this is label");
    label->setStyleSheet(style);
    QPushButton *label2 = new QPushButton(tab_bar_);
    connect(label2, SIGNAL(clicked()), this, SLOT(test2()));
    label2->setCheckable(true);
    label2->setFixedSize(QSize(150,30));
    label2->setText("this is second label");
    label2->setStyleSheet(style);

    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(label);
    group->addButton(label2);

    tab_bar_->setTabButton(0, QTabBar::LeftSide, label);
    tab_bar_->setTabButton(1, QTabBar::LeftSide, label2);

    CusBtn* cus_btn = new CusBtn(tab_bar_);
    cus_btn->setCheckable(true);
    cus_btn->setStyleSheet(style);
    cus_btn->setFixedWidth(150);
    group->addButton(cus_btn);
    tab_bar_->setTabButton(2, QTabBar::LeftSide, cus_btn);
    connect(cus_btn, SIGNAL(change_index()), this, SLOT(do_change_index()));

    tab_bar_->setStyleSheet("background-color:red;padding:0 0 0 0;");
    tab_bar_->setContentsMargins(0, 0, 0, 0);
    ui->widget_tabbar_container->layout()->addWidget(tab_bar_);
    ui->widget_tabbar_container->setFixedWidth(450);
    tab_bar_->setFixedHeight(ui->widget_tabbar_container->height());
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
