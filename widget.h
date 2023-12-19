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

#define DRAGABLE_TAB_WIDGET_MIME_KEY "DRAGABLE_TAB_WIGET_MIME_KEY"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected slots:
  void do_change_index();

private:
    Ui::Widget *ui = nullptr;
    QExtTabBar* tab_bar_ = nullptr;
};
#endif // WIDGET_H
