#include "widget.h"
#include "ui_widget.h"
#include "cus_button.h"
#include "ext_tab_bar.h"
#include "ext_tab_bar_style.h"

#include <QLabel>
#include <QTabBar>
#include <QPushButton>
#include <QButtonGroup>

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  tab_bar_ = new QExtTabBar(this);
  tab_bar_->setStyleSheet("background-color:red;");
  tab_bar_->setContentsMargins(0, 0, 0, 0);
  ui->widget_tabbar_container->layout()->addWidget(tab_bar_);
  tab_bar_->setFixedHeight(ui->widget_tabbar_container->height());
  connect(tab_bar_, SIGNAL(switch_widget(QWidget *)), this, SLOT(do_switch_widget(QWidget *)));
  addTab("", "this is test tab 1", new QWidget(this));
  addTab("", "this is test tab 2", new QWidget(this));
  ui->widget_tabbar_container->setFixedWidth(300);
}

Widget::~Widget() {
  delete ui;
}

void Widget::do_change_index() {
  ui->stackedWidget->setCurrentIndex(2);
}

void Widget::do_switch_widget(QWidget* widget) {
  ui->stackedWidget->setCurrentWidget(widget);
}

bool Widget::addTab(const QString& icon, const QString& title,
                    QWidget* widget) {
  if (nullptr == widget) {
    return false;
  }

  ui->stackedWidget->addWidget(widget);
  tab_bar_->addTab2(icon, title, widget);

  return true;
}
