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

class QToolButton;
class QTabBar;

#define DRAGABLE_TAB_WIDGET_MIME_KEY "DRAGABLE_TAB_WIGET_MIME_KEY"

class QExtTabBarStyle : public QProxyStyle {
 public:
    QExtTabBarStyle(QTabBar *bar, QStyle *parent = Q_NULLPTR):_bar(bar){}
    ~QExtTabBarStyle(){}


  virtual QRect subElementRect(SubElement element, const QStyleOption *option,
                               const QWidget *widget) const Q_DECL_OVERRIDE;

private:
  QTabBar* _bar;
};

class QExtTabBar : public QTabBar
{
public:
  QExtTabBar(QWidget* parent):QTabBar(parent) {
    this->setAcceptDrops(true);
    //this->setMovable(true);
    this->setMouseTracking(true);
  }
  ~QExtTabBar() {}

  QSize QExtTabBar::tabSizeHint(int index) const {
    return QSize(150, 30);
  }

  void mouse_release_event(QMouseEvent* event)
  {
    this->mouseReleaseEvent(event);
  }

protected:

protected:
  bool eventFilter(QObject* obj, QEvent* event) override
  {
    if (event->type() == QEvent::MouseButtonPress)
    {
      this->mousePressEvent((QMouseEvent*)event);
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
      this->mouseReleaseEvent((QMouseEvent*)event);
    }
    else if (event->type() == QEvent::MouseMove)
    {
      this->mouseMoveEvent((QMouseEvent*)event);
    }
    return QTabBar::eventFilter(obj, event);
  }

  virtual void dragEnterEvent(QDragEnterEvent* event)
  {
    const QMimeData* mime = event->mimeData();
    // Tab拖拽
    QPoint pos = event->pos();
    if (mime->hasFormat(DRAGABLE_TAB_WIDGET_MIME_KEY)) {
      event->accept();
    }
    else {
      event->ignore();
    }
  }
  virtual void dragLeaveEvent(QDragLeaveEvent* event)
  {

  }
  virtual void dragMoveEvent(QDragMoveEvent* event)
  {

  }
  virtual void dropEvent(QDropEvent* event)
  {

    }


  void mouseReleaseEvent(QMouseEvent* e) {
    QTabBar::mouseReleaseEvent(e);
    //  if (dragging_ && e->button() == Qt::LeftButton &&
    //      !contentsRect().contains(e->pos())) {
    if (dragging_ && e->button() == Qt::LeftButton) {
      dragging_ = false;
    }
    //emit signal_.mouseRelease(e);
  }

  int pointInTabRectIndex(const QPoint& point) {
    for (int i = 0; i < count(); ++i) {
      QRect rect = tabRect(i);
      if (!rect.contains(point)) continue;
      return i;
    }
    return -1;
  }


  void mousePressEvent(QMouseEvent* event) {
    QTabBar::mousePressEvent(event);

    int index = pointInTabRectIndex(event->pos());
    //if (index == count() - 1 && tab_add_button_) {
    //  emit signal_.addBtnClicked();
    //  return;
    //}

    if (event->button() == Qt::LeftButton && count() >= 0) {
      dragging_ = true;
      press_pos_ = event->pos();
    }
  }

  void mouseMoveEvent(QMouseEvent* event) {
    QTabBar::mouseMoveEvent(event);

    // 高度超过标签栏高度 或 标签数量=1 时，开启拖拽
    if (dragging_ && (event->buttons() & Qt::LeftButton) &&
      (count() >= 1 || !contentsRect().contains(event->pos()))) {
      int index = this->currentIndex();
      if (index == -1 || (index == count())) return;

      bool containts = false;
      for (int i = 0; i < count(); ++i) {
        if (tabRect(index).contains(event->pos())) {
          containts = true;
          break;
        }
      }

      // 拖拽到外面来了
      // if (count() == 1 || containts) {
      if (count() >= 0 && containts) {
        // emit signal_.signalStartDrag(index);
        start_drag(index);
      }
    }
  }

  void start_drag(int index)
  {
    bool is_main_one = (count() == 1);
    if (is_main_one) return;

    //drag_state_ = DragState::Unknown;
    //dragging_index_ = index;
    //dragging_widget_ = this->widget(index);
    QPoint dragging_point_delta_ =
      QCursor::pos() - (parent() == nullptr ? this->mapToGlobal(this->pos())
        : mapToGlobal(QPoint(0, 0)));
    dragging_point_delta_ = QPoint(0, 0);

    QPixmap pixmap(this->size());
    pixmap.fill(Qt::transparent);
    this->render(&pixmap);
    QRect pixmap_rect = this->tabRect(index);
    pixmap = pixmap.copy(pixmap_rect);

    QMimeData* mime = new QMimeData;
    //mime->setData(DRAGABLE_TAB_WINDOW_MIME_KEY,
    //  QString::number(reinterpret_cast<int>(this)).toUtf8());
    //mime->setData(
    //  DRAGABLE_TAB_WIDGET_MIME_KEY,
    //  QString::number(reinterpret_cast<int>(dragging_widget_)).toUtf8());
    //mime->setData(DRAGABLE_TAB_LABEL_MIME_KEY,
    //  tab_bar_->tabText(index).toLocal8Bit());
    //dragging_label_ = tab_bar_->tabText(index);
    //dragging_icon_ = tab_bar_->tabIcon(index);
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mime);
    drag->setPixmap(pixmap);
    drag->setHotSpot(dragging_point_delta_);

    //if (!isInMain()) this->hide();

    //connect(drag, &QDrag::destroyed, this, [=](QObject*) {
    //  // 判断有没有被合并到窗口
    //  if (drag_state_ == DragState::NoChanged || drag_state_ == DragState::Merged)
    //    return;

    //  // 没有合并到其他窗口
    //  if (tab_bar_->tabCount() == 1 && !isInMain()) {
    //  }
    //  else {
    //    // 多个标签拖出，创建新窗口
    //    createDraggedNewWindow();
    //  }
    //  this->move(QCursor::pos());
    //  this->show();
    //  });

    // exec 操作会一直阻塞后面的代码，除非使用多线程或者信号槽
    drag->exec();

  }

private:
  bool dragging_ = false;
  QPoint press_pos_ = QPoint(0, 0);
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected slots:
  void test();
  void test2();
  void do_change_index();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
