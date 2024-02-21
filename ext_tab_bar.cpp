#include "ext_tab_bar.h"

#include <QApplication>
#include <QButtonGroup>
#include <QDrag>
#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>

#include "const.h"
#include "cus_button.h"
#include "ext_tab_bar_style.h"
#include "widget.h"

QExtTabBar::QExtTabBar(QWidget* parent) : QTabBar(parent) {
  this->setAcceptDrops(true);
  this->setMouseTracking(true);
  this->setIconSize(QSize(0, 0));
  this->setStyle(new QExtTabBarStyle(this));
  group_ = new QButtonGroup(this);
}

QExtTabBar::~QExtTabBar() {}

QSize QExtTabBar::tabSizeHint(int index) const {
  return QSize(kTAB_BUTTON_WIDTH, kTAB_BUTTON_HEIGHT);
}

bool QExtTabBar::add_tab2(const QString& icon, const QString& title,
                          QWidget* widget) {
  this->addTab("");
  CusBtn* cus_btn = new CusBtn(this);
  cus_btn->setText(title);
  cus_btn->setCheckable(true);
  cus_btn->setFixedWidth(kTAB_BUTTON_WIDTH);
  this->group_->addButton(cus_btn);
  this->setTabButton(count() - 1, QTabBar::LeftSide, cus_btn);
  connect(cus_btn, SIGNAL(btn_clicked()), this, SLOT(tab_clicked_changed()));
  connect(cus_btn, SIGNAL(btn_closed()), this, SLOT(tab_closed()));
  // this->btn_widget_map_[cus_btn] = widget;
  this->btn_widget_map_.push_back({cus_btn, widget});
  return true;
}

bool QExtTabBar::remove_tab2(int index, QWidget *widget) {
  this->removeTab(index);
  CusBtn* btn = this->get_btn(widget);
  delete btn;
  btn = nullptr;
  return true;
}

void QExtTabBar::mouse_release_event(QMouseEvent* event) {
  this->mouseReleaseEvent(event);
}

void QExtTabBar::tab_mouse_move_event(QObject* obj, QMouseEvent* event) {
  CusBtn* btn = dynamic_cast<CusBtn*>(obj);
  if (btn == nullptr) {
    return;
  }
  // 将子窗口坐标转为父窗口
  QPoint pos = btn->mapToParent(event->pos());
  // 高度超过标签栏高度 或 标签数量=1 时，开启拖拽
  if (dragging_ && (event->buttons() & Qt::LeftButton) &&
      (count() >= 1 || !contentsRect().contains(pos))) {
    bool containts = false;
    int index = 0;
    for (int i = 0; i < count(); ++i) {
      if (tabRect(i).contains(pos)) {
        index = i;
        containts = true;
        break;
      }
    }

    // 拖拽到外面来了
    if (count() >= 0 && containts) {
      start_drag(index);
    }
  }
}

void QExtTabBar::tab_clicked_changed() {
  CusBtn* cur_btn = (CusBtn*)(sender());
  QWidget *widget = this->get_widget(cur_btn);
  if (nullptr == widget) {
    return; 
  }
  emit this->switch_widget(widget);
}

void QExtTabBar::tab_closed() {
  CusBtn* btn = (CusBtn*)sender();
  QWidget* widget = nullptr;
  if ((widget = this->get_widget(btn)) == nullptr) {
    return;
  }
  int index = this->get_btn_index(btn);
  if (index < 0) {
    return;
  }
  emit remove_tab(index, widget);
}

bool QExtTabBar::eventFilter(QObject* obj, QEvent* event) {
  if (event->type() == QEvent::MouseButtonPress) {
    this->mousePressEvent((QMouseEvent*)event);
  } else if (event->type() == QEvent::MouseButtonRelease) {
    this->mouseReleaseEvent((QMouseEvent*)event);
  } else if (event->type() == QEvent::MouseMove) {
    this->tab_mouse_move_event(obj, (QMouseEvent*)event);
  }
  return QTabBar::eventFilter(obj, event);
}

void QExtTabBar::dragEnterEvent(QDragEnterEvent* event) {
  const QMimeData* mime = event->mimeData();
  // Tab拖拽
  QPoint pos = event->pos();
  if (mime->hasFormat(DRAGABLE_TAB_WIDGET_MIME_KEY)) {
    event->accept();
  } else {
    event->ignore();
  }
}
void QExtTabBar::dragLeaveEvent(QDragLeaveEvent* event) {}
void QExtTabBar::dragMoveEvent(QDragMoveEvent* event) {}
void QExtTabBar::dropEvent(QDropEvent* event) {
  if (merge_tab(event)) {
    event->accept();
  }
}

void QExtTabBar::mouseReleaseEvent(QMouseEvent* e) {
  QTabBar::mouseReleaseEvent(e);
  if (dragging_ && e->button() == Qt::LeftButton) {
    dragging_ = false;
  }
}

int QExtTabBar::pointInTabRectIndex(const QPoint& point) {
  for (int i = 0; i < count(); ++i) {
    QRect rect = tabRect(i);
    if (!rect.contains(point))
      continue;
    return i;
  }
  return -1;
}

void QExtTabBar::mousePressEvent(QMouseEvent* event) {
  QTabBar::mousePressEvent(event);

  int index = pointInTabRectIndex(event->pos());
  if (event->button() == Qt::LeftButton && count() >= 0) {
    dragging_ = true;
    press_pos_ = event->pos();
  }
}

void QExtTabBar::mouseMoveEvent(QMouseEvent* event) {
  QTabBar::mouseMoveEvent(event);
}

void QExtTabBar::start_drag(int index) {
  bool is_main_one = (count() == 1);
  if (is_main_one)
    return;

  drag_state_ = DragState::Unknown;
  QPoint dragging_point_delta_ =
      QCursor::pos() - (parent() == nullptr ? this->mapToGlobal(this->pos())
                                            : mapToGlobal(QPoint(0, 0)));
  dragging_point_delta_ = QPoint(0, 0);

  QPixmap pixmap(this->size());
  pixmap.fill(Qt::transparent);
  this->render(&pixmap);
  QRect pixmap_rect = this->tabRect(index);
  pixmap = pixmap.copy(pixmap_rect);

  TabDragData* data = new TabDragData();
  data->tab_bar = this;
  data->label = this->tabText(index);

  QMimeData* mime = new QMimeData;
  mime->setData(DRAGABLE_TAB_WIDGET_MIME_KEY,
                QString::number(reinterpret_cast<int>(data)).toUtf8());
  QDrag* drag = new QDrag(this);
  drag->setMimeData(mime);
  drag->setPixmap(pixmap);
  drag->setHotSpot(dragging_point_delta_);

  // if (!isInMain()) this->hide();

  connect(drag, &QDrag::destroyed, this, [=](QObject*) {
    // 判断有没有被合并到窗口
    if (drag_state_ == DragState::NoChanged || drag_state_ == DragState::Merged)
      return;

    // 没有合并到其他窗口
    // if (tab_bar_->tabCount() == 1 && !isInMain()) {
    //}
    // else {
    //  // 多个标签拖出，创建新窗口
    //  createDraggedNewWindow();
    //}
    this->move(QCursor::pos());
    this->show();
  });

  // exec 操作会一直阻塞后面的代码，除非使用多线程或者信号槽
  drag->exec();
}

void QExtTabBar::send_tab_bar_hover(const QPoint& pos) {
  QHoverEvent* hover_event =
      new QHoverEvent(QEvent::HoverLeave, QPoint(-1, -1), pos);
  QApplication::postEvent(this, hover_event);
}

bool QExtTabBar::merge_tab(QDropEvent* event) {
  int insert_index = this->pointInTabRectIndex(event->pos());

  const QMimeData* mime = event->mimeData();
  TabDragData* data = reinterpret_cast<TabDragData*>(
      mime->data(DRAGABLE_TAB_WIDGET_MIME_KEY).toInt());
  QTabBar* tab_bar = data->tab_bar;
  if (data->tab_bar == this) {
    drag_state_ = DragState::NoChanged;
    if (insert_index == currentIndex())
      return false;
    removeTab(currentIndex());

    QWidget *widget = this->get_widget((CusBtn *)data->btn);
    if (nullptr == widget) {
      return false; 
    }
    // TODO: 暂且不处理拖拽合并操作
    insert_tab(insert_index, widget, data->label);
    setCurrentIndex(insert_index);
    return true;
  }

  setCurrentIndex(insert_index);
  this->setFocus();
  return true;
}

void QExtTabBar::insert_tab(int index, QWidget* widget, const QString& title) {
  Widget* parent = (Widget*)this->parentWidget();
  parent->insert_tab(index, widget, title);
}

QWidget* QExtTabBar::get_widget(CusBtn* btn) {
  for (auto &itr : this->btn_widget_map_) {
    if (itr.first != btn) {
      continue;
    }
    return itr.second;
  }
  return nullptr;
}

int QExtTabBar::get_btn_index(CusBtn* btn) {
  QPoint pos = btn->mapToParent(QPoint(1, 1));
  return this->pointInTabRectIndex(pos);
}

CusBtn* QExtTabBar::get_btn(QWidget* widget) {
  for (auto &itr: this->btn_widget_map_) {
    if (widget != itr.second ) {
      continue; 
    }
    return itr.first;
  }

  return nullptr;
}

CusBtn* QExtTabBar::get_btn_by_index(int index) {
  if (index < 0 || index >= this->count() || index >= this->btn_widget_map_.size()) {
    return nullptr; 
  }
  
  return this->btn_widget_map_[index].first;
}
