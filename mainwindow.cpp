#include "mainwindow.h"

#include <functional>
#include <QPushButton>

#include "NcFrameLessHelper.h"
#include "draw_helper.h"
#include "round_shadow_helper.h"
#include "ui_mainwindow.h"
#include "tab_bar_draw_helper.h"
#include "weak_call_back.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setupUI();
}

MainWindow::~MainWindow() {
  delete ui;
  delete frame_less_helper_;
}

void MainWindow::setupUI() {
  this->setWindowFlags(Qt::FramelessWindowHint |
                       Qt::X11BypassWindowManagerHint);
  frame_less_helper_ = new NcFramelessHelper();
  frame_less_helper_->activateOn(this);
  frame_less_helper_->setFilterEventCallBack(
      std::bind(&TabWidgetProxy::filterTabBarEvent, ui->tabWidgetProxy,
                std::placeholders::_1, std::placeholders::_2));
  frame_less_helper_->activeOnWithChildWidget(this,
                                              ui->tabWidgetProxy->tabBar());

  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(tabInserted(int)), this,
          SLOT(OnTabInserted(int)));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(addBtnClicked()), this,
          SLOT(OnAddBtnClicked()));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(tabClosed(int)), this,
          SLOT(OnCloseTab(int)));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(closeWnd()), this,
          SLOT(OnCloseWnd()));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(minWnd()), this,
          SLOT(OnMinWnd()));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(maxWnd()), this,
          SLOT(OnMaxWnd()));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(restoreWnd()), this,
          SLOT(OnRestoreWnd()));
  connect(&(ui->tabWidgetProxy->getSignal()), SIGNAL(tabBarDoubleClicked()),
          this, SLOT(OnMaxOrRestore()));

  ui->tabWidgetProxy->updateDrawHelp(new TabBarDrawHelper());

#ifdef _DEBUG
  QWidget *widget = new QWidget();
  widget->setStyleSheet("background-color:#FF00FF00");
  ui->tabWidgetProxy->addTab2(widget, tr("this is first tab tab tab tab tab"));
#endif  //_DEBUG

}

void MainWindow::OnTabInserted(int index) {
  QPushButton *button = new QPushButton();
  button->setStyleSheet(
      "QPushButton{border-image: url(:/images/close.png)}"
      "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
  ui->tabWidgetProxy->setTabButton2(index, QTabBar::RightSide, button);
  ui->tabWidgetProxy->setTabIcon(index, QIcon(":/images/x-capture-options.png"));
}

void MainWindow::OnCloseTab(int index) {
  // todo something
}

void MainWindow::OnAddBtnClicked() {
  QWidget *widget = new QWidget();
  widget->setStyleSheet("background-color:#FFFFFF00");
  ui->tabWidgetProxy->addTab2(widget, tr("this is first tab"));
}

void MainWindow::OnCloseWnd() { this->close(); }

void MainWindow::OnMinWnd() {}

void MainWindow::OnMaxWnd() {}

void MainWindow::OnRestoreWnd() {}

void MainWindow::OnMaxOrRestore() {}
