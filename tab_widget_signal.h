#ifndef TAB_WIDGET_SIGNAL_H
#define TAB_WIDGET_SIGNAL_H

#include <QObject>

class TabWidgetSignal : public QObject {
  Q_OBJECT

 public:
  TabWidgetSignal() {}
  ~TabWidgetSignal() {}

 signals:
  void addBtnClicked();
  void tabInserted(int index);
  void tabClosed(int index);
  void closeWnd();
  void minWnd();
  void maxWnd();
  void restoreWnd();
  void tabBarDoubleClicked();
};

#endif  // TAB_WIDGET_SIGNAL_H
