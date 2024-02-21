#ifndef WIDGET_H
#define WIDGET_H

#include <QTabBar>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QProxyStyle>
#include <QStyle>
#include <functional>

class QExtTabBar;

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

  bool addTab(const QString &icon, const QString &title, QWidget *widget);
  bool insert_tab(int index, QWidget *widget, const QString &title);
  bool remove_tab(int index, QWidget *widget);
  bool forward_tab();

 protected slots:
  void do_change_index();
  void do_switch_widget(QWidget *widget);
  void on_btn_add_clicked();
  void do_remove_tab(int index, QWidget *widget);

private:
  void update_tabbar_width();

 private:
  Ui::Widget *ui = nullptr;
  QExtTabBar *tab_bar_ = nullptr;
};
#endif  // WIDGET_H
