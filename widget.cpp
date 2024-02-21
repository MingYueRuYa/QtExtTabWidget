#include "widget.h"

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QTabBar>

#include "const.h"
#include "cus_button.h"
#include "ext_tab_bar.h"
#include "ext_tab_bar_style.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  tab_bar_ = new QExtTabBar(this);
  tab_bar_->setContentsMargins(0, 0, 0, 0);
  ui->widget_tabbar_container->layout()->addWidget(tab_bar_);
  tab_bar_->setFixedHeight(ui->widget_tabbar_container->height());
  connect(tab_bar_, SIGNAL(switch_widget(QWidget*)), this,
          SLOT(do_switch_widget(QWidget*)));
  connect(tab_bar_, SIGNAL(remove_tab(int, QWidget*)), this,
          SLOT(do_remove_tab(int, QWidget*)));
  QWidget* widget = new QWidget(this);
  widget->setStyleSheet("background-color:#FF00FF00");
  addTab("", "this is test tab 1", widget);
  widget = new QWidget(this);
  widget->setStyleSheet("background-color:#FF00FFFF");
  addTab("", "this is test tab 2", widget);
  this->update_tabbar_width();
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

void Widget::on_btn_add_clicked() {
  QWidget* widget = new QWidget(this);
  widget->setStyleSheet("background-color:#FF0FFFF0");
  this->addTab("", "for add button", widget);
  this->update_tabbar_width();
}

void Widget::do_remove_tab(int index, QWidget* widget) {
  tab_bar_->remove_tab2(index, widget);
  delete widget;
  widget = nullptr;
  this->update_tabbar_width();
}

void Widget::update_tabbar_width() {
  ui->widget_tabbar_container->setFixedWidth(tab_bar_->count() *
                                             kTAB_BUTTON_WIDTH);
}

bool Widget::addTab(const QString& icon, const QString& title,
                    QWidget* widget) {
  if (nullptr == widget) {
    return false;
  }

  ui->stackedWidget->addWidget(widget);
  tab_bar_->add_tab2(icon, title, widget);

  return true;
}

bool Widget::insert_tab(int index, QWidget* widget, const QString& title) {
  tab_bar_->insertTab(index, title);
  ui->stackedWidget->setCurrentWidget(widget);
  return true;
}

bool Widget::remove_tab(int index, QWidget* widget) {
  tab_bar_->remove_tab2(index, widget);
  delete widget;
  widget = nullptr;
  return true;
}

bool Widget::forward_tab() {
  return false;
}
