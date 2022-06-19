#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QToolButton>
#include <memory>

#include "qt_ext_tab_bar_define.h"

namespace Ui {
class MainWindow;
}

class NcFramelessHelper;
using QtExtTabBarDef::TabState;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private:
  void setupUI();
  bool filterTabBarEvent(QObject *obj, QEvent *event);

 protected slots:
  void OnTabInserted(int index);
  void OnCloseTab(int index);
  void OnAddBtnClicked();
  void OnCloseWnd();
  void OnMinWnd();
  void OnMaxWnd();
  void OnRestoreWnd();
  void OnMaxOrRestore();

 private:
  Ui::MainWindow *ui;
  NcFramelessHelper *frame_less_helper_ = nullptr;
};

#endif  // MAINWINDOW_H
