#include "mainwindow.h"

#include <functional>

#include "NcFrameLessHelper.h"
#include "draw_helper.h"
#include "round_shadow_helper.h"
#include "ui_mainwindow.h"
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
      std::bind(&TabWidgetProxy::filterTabBarEvent, ui->tabWidgetProxy, std::placeholders::_1,
                std::placeholders::_2));
  frame_less_helper_->activeOnWithChildWidget(this, ui->tabWidgetProxy->tabBar());

  // connect(ui->tabWidget, SIGNAL(TabInserted(int)), this,
  //         SLOT(OnTabInserted(int)));
  // connect(ui->tabWidget, SIGNAL(AddBtnClicked()), this,
  //         SLOT(OnAddBtnClicked()));
  // connect(ui->tabWidget, SIGNAL(TabClosed(int)), this,
  // SLOT(OnCloseTab(int))); connect(ui->tabWidget, SIGNAL(closeWnd()), this,
  // SLOT(OnCloseWnd())); connect(ui->tabWidget, SIGNAL(minWnd()), this,
  // SLOT(OnMinWnd())); connect(ui->tabWidget, SIGNAL(maxWnd()), this,
  // SLOT(OnMaxWnd())); connect(ui->tabWidget, SIGNAL(restoreWnd()), this,
  // SLOT(OnRestoreWnd())); connect(ui->tabWidget,
  // SIGNAL(tabBarDoubleClicked()), this,
  //         SLOT(OnMaxOrRestore()));
}

bool MainWindow::filterTabBarEvent(QObject *obj, QEvent *event) {
  // Only click on the tab bar non-tab area can be dragged
  if (event->type() != QEvent::MouseButtonPress) return false;

  // QMouseEvent *mouse_event = dynamic_cast<QMouseEvent *>(event);
  // // （只有在拖拽区域不在tab rect区域 || 只有一个tab标签时才可以拖动）
  // if (isMaximized()) {
  //   return true;
  // } else if (ui->tabWidget->tabBar2() == obj &&
  //            (ui->tabWidget->pointInTabRectIndex(mouse_event->pos()) < 0 ||
  //             ui->tabWidget->tabBar2()->tabCount() == 1)) {
  //   return false;
  // } else {
  //   return true;
  // }
  return false;
}

void MainWindow::OnTabInserted(int index) {
  // QPushButton *button = new QPushButton();
  // button->setFixedSize(this->iconSize());
  // button->setStyleSheet("border-image:
  // url(:/images/x-capture-options.png);"); ui->tabWidget->setTabButton2(index,
  // QTabBar::LeftSide, button);

  // QPushButton *button = new QPushButton();
  // button->setStyleSheet(
  //     "QPushButton{border-image: url(:/images/close.png)}"
  //     "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
  // ui->tabWidget->setTabButton2(index, QTabBar::RightSide, button);
  // ui->tabWidget->setTabIcon(index, QIcon(":/images/x-capture-options.png"));
}

void MainWindow::OnCloseTab(int index) {
  // todo something
}

void MainWindow::OnAddBtnClicked() {
  // QWidget *widget = new QWidget();
  // widget->setStyleSheet("background-color:#FFFFFF00");
  // ui->tabWidget->addTab2(widget, tr("this is first tab"));
}

void MainWindow::OnCloseWnd() { this->close(); }

void MainWindow::OnMinWnd() { this->showMinimized(); }

void MainWindow::OnMaxWnd() {
  this->showMaximized();
  // ui->tabWidget->setMaxRestoreBtnVisible(false, true);
  // ui->tabWidget->currentWidget()->setFocus();
}

void MainWindow::OnRestoreWnd() {
  this->showNormal();
  // ui->tabWidget->setMaxRestoreBtnVisible(true, false);
}

void MainWindow::OnMaxOrRestore() {
  if (isMaximized()) {
    this->showNormal();
    // ui->tabWidget->setMaxRestoreBtnVisible(true, false);
  } else {
    this->showMaximized();
    // ui->tabWidget->setMaxRestoreBtnVisible(false, true);
  }
}
