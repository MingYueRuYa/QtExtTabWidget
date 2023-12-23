#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTabBar>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#include <QProxyStyle>
#include <QStyle>
#include <functional>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>

class QExtTabBar;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool addTab(const QString &icon, const QString &title, QWidget *widget);

protected slots:
  void do_change_index();
 void do_switch_widget(QWidget *widget);

private:
    Ui::Widget *ui = nullptr;
    QExtTabBar* tab_bar_ = nullptr;
};
#endif // WIDGET_H
