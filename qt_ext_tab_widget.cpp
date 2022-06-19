#include "qt_ext_tab_widget.h"

#include <QPainter>

#include "NcFrameLessHelper.h"
#include "button_widget.h"

#define DRAGABLE_TAB_WINDOW_MIME_KEY "DRAGABLE_TAB_WINDOW_MIME_KEY"
#define DRAGABLE_TAB_WIDGET_MIME_KEY "DRAGABLE_TAB_WIGET_MIME_KEY"
#define DRAGABLE_TAB_LABEL_MIME_KEY "DRAGABLE_TAB_LABEL_MIME_KEY"
#define DRAGABLE_TAB_ICON_MIME_KEY "DRAGABLE_TAB_ICON_MIME_KEY"
#define WIN_FRAME_LEFE_OFFSET 8

const int kBUTTON_CONTAINER_WIDTH = 100;
QtExtTabWidget::DragState QtExtTabWidget::drag_state_ = DragState::Unknown;
QString QtExtTabWidget::dragging_label_ = "";
QIcon QtExtTabWidget::dragging_icon_ = QIcon();

QtExtTabWidget::QtExtTabWidget(QWidget *parent) { setupUI(); }

void QtExtTabWidget::initProperty(bool useScrollButtons, bool isMain) {
  this->setMovable(false);
  this->setMouseTracking(true);
  this->setAcceptDrops(true);
  this->setDocumentMode(true);
  // notice: this is for custom style
  this->setStyleSheet("background-color:transparent;");
  this->setUsesScrollButtons(useScrollButtons);
  this->is_main_ = isMain;
}

void QtExtTabWidget::addTab2(QWidget *widget, const QString &label) {
  insertTab2(-1, widget, label);
}

void QtExtTabWidget::insertTab2(int index, QWidget *widget,
                                const QString &label) {
  int tab_index = this->insertTab(index, widget, label);
  this->setCurrentIndex(tab_index);
}

void QtExtTabWidget::setTabButton2(int index, QTabBar::ButtonPosition btn_pos,
                                   QWidget *btn) {
  tab_bar_->setTabButton2(index, btn_pos, btn);
}

void QtExtTabWidget::setMaxRestoreBtnVisible(bool max_visible,
                                             bool restore_visible) {
  button_widget_->setMaxRestoreBtnVisible(max_visible, restore_visible);
}

int QtExtTabWidget::pointInTabRectIndex(const QPoint &point) {
  return tab_bar_->pointInTabRectIndex(point);
}

void QtExtTabWidget::setPaintAddBtnFunc(QtExtTabBar::PaintAddButtonFunc func) {
  if (nullptr != tab_bar_) tab_bar_->setPaintAddBtnFunc(func);
}

void QtExtTabWidget::setPaintTabFunc(QtExtTabBar::PaintTabFunc func) {
  if (nullptr != tab_bar_) tab_bar_->setPaintTabFunc(func);
}

void QtExtTabWidget::setPaintTabBarFunc(QtExtTabBar::PaintTabBarFunc func) {
  if (nullptr != tab_bar_) tab_bar_->setPaintTabBarFunc(func);
}

void QtExtTabWidget::setPaintScrollBtnFunc(
    QtExtTabBar::PaintScrollBtnFunc func) {
  if (nullptr != tab_bar_) tab_bar_->setPaintScrollBtnFunc(func);
}

void QtExtTabWidget::setCalcIconBtnRectFunc(
    QtExtTabBar::CalcIconBtnRectFunc func) {
  if (nullptr != tab_bar_) tab_bar_->setCalcIconBtnRectFunc(func);
}

QSize QtExtTabWidget::tabSize() const {
  if (nullptr != tab_bar_)
    return tab_bar_->tabSize();
  else
    return QSize(-1, -1);
}

void QtExtTabWidget::setTabSize(const QSize &tab_size) {
  if (nullptr == tab_bar_) return;
  if (tab_bar_->tabSize() == tab_size) return;
  tab_bar_->setTabSize(tab_size);
}

bool QtExtTabWidget::tabAddButton() const {
  if (nullptr == tab_bar_)
    return false;
  else
    return tab_bar_->tabAddButton();
}

void QtExtTabWidget::setTabAddButton(bool draw_add) {
  if (nullptr == tab_bar_) return;
  if (tab_bar_->tabAddButton() == draw_add) return;
  tab_bar_->setTabAddButton(draw_add);
}

int QtExtTabWidget::tabAddButtonWidth() const {
  if (nullptr == tab_bar_)
    return -1;
  else
    return tab_bar_->tabAddButtonWidth();
}

void QtExtTabWidget::setTabAddButtonWidth(int width) {
  if (nullptr == tab_bar_) return;
  if (tab_bar_->tabAddButtonWidth() == width) return;
  tab_bar_->setTabAddButtonWidth(width);
}

QtExtTabBar *QtExtTabWidget::tabBar2() const { return tab_bar_; }

const TabWidgetSignal &QtExtTabWidget::getSignal() const { return signal_; }

void QtExtTabWidget::paintEvent(QPaintEvent *ev) { QTabWidget::paintEvent(ev); }

void QtExtTabWidget::tabInserted(int index) { emit signal_.tabInserted(index); }

void QtExtTabWidget::resizeEvent(QResizeEvent *ev) {
  button_widget_->setFixedSize(
      QSize(kBUTTON_CONTAINER_WIDTH, tab_bar_->tabSize().height()));
  int tab_bar_width = this->width() - button_widget_->width();
  button_widget_->move(QPoint(tab_bar_width, 0));
  tab_bar_->setFixedWidth(tab_bar_width);
  QTabWidget::resizeEvent(ev);
}

void QtExtTabWidget::dragEnterEvent(QDragEnterEvent *event) {
  const QMimeData *mime = event->mimeData();
  // Tab拖拽
  QPoint pos = event->pos();
  if (mime->hasFormat(DRAGABLE_TAB_WIDGET_MIME_KEY)) {
    event->accept();
  } else {
    event->ignore();
  }
}

void QtExtTabWidget::dragMoveEvent(QDragMoveEvent *event) {
  const QMimeData *mime = event->mimeData();
  QPoint pos = event->pos();

  if (!is_main_) {
    event->ignore();
  } else if (count() == 0) {
    event->accept();
  } else if (mime->hasFormat(DRAGABLE_TAB_WIDGET_MIME_KEY) &&
             tab_bar_->geometry().contains(pos)) {  // 整行拖拽
    if (tab_bar_->geometry().contains(pos)) {
      int index = tab_bar_->pointInTabRectIndex(pos);
      event->accept();
      QHoverEvent *hover_event =
          new QHoverEvent(QEvent::HoverEnter, pos, QPointF(0, 0));
      QApplication::postEvent(tab_bar_, hover_event);
    }
  } else {
    event->ignore();
    QHoverEvent *event =
        new QHoverEvent(QEvent::HoverLeave, QPoint(-1, -1), pos);
    QApplication::postEvent(tab_bar_, event);
  }
}

void QtExtTabWidget::dropEvent(QDropEvent *event) {
  if (OnMergeDroppedLabel(event)) event->accept();

  QHoverEvent *hover_event =
      new QHoverEvent(QEvent::HoverLeave, QPoint(-1, -1), event->pos());
  QApplication::postEvent(tab_bar_, hover_event);
  this->show();

  return QTabWidget::dropEvent(event);
}

void QtExtTabWidget::dragLeaveEvent(QDragLeaveEvent *event) {
  QHoverEvent *hover_event =
      new QHoverEvent(QEvent::HoverLeave, QPoint(-1, -1), QPoint(0, 0));
  QApplication::postEvent(tab_bar_, hover_event);
  this->show();
  QTabWidget::dragLeaveEvent(event);
}

void QtExtTabWidget::setTabBar2(QTabBar *tab_bar) {
  tab_bar_ = dynamic_cast<QtExtTabBar *>(tab_bar);
  this->setTabBar(tab_bar_);
}

void QtExtTabWidget::OnCloseTab(int index) {
  this->removeTab(index);
  emit signal_.tabClosed(index);
  if (this->currentIndex() == index && index - 1 > 0)
    this->setCurrentIndex(index - 1);
  deleteIfEmptyWindow();
}

void QtExtTabWidget::OnTabBarDoubleClicked(int index) {
  if (-1 != index) {
    OnCloseTab(index);
  } else {
    if (nullptr == parentWidget()) {
      isMaximized() ? OnRestoreWnd() : OnMaxWnd();
    } else {
      emit signal_.tabBarDoubleClicked();
    }
  }
}

void QtExtTabWidget::OnStartDrag(int index) {
  bool is_main_one = (tab_bar_->tabCount() == 1 && isInMain());
  if (is_main_one) return;

  drag_state_ = DragState::Unknown;
  dragging_index_ = index;
  dragging_widget_ = this->widget(index);
  dragging_point_delta_ =
      QCursor::pos() - (parent() == nullptr ? this->mapToGlobal(this->pos())
                                            : mapToGlobal(QPoint(0, 0)));
  dragging_point_delta_ = QPoint(0, 0);

  QPixmap pixmap(tab_bar_->size());
  pixmap.fill(Qt::transparent);
  tab_bar_->render(&pixmap);
  QRect pixmap_rect = tab_bar_->tabRect(index);
  pixmap = pixmap.copy(pixmap_rect);

  QMimeData *mime = new QMimeData;
  mime->setData(DRAGABLE_TAB_WINDOW_MIME_KEY,
                QString::number(reinterpret_cast<int>(this)).toUtf8());
  mime->setData(
      DRAGABLE_TAB_WIDGET_MIME_KEY,
      QString::number(reinterpret_cast<int>(dragging_widget_)).toUtf8());
  mime->setData(DRAGABLE_TAB_LABEL_MIME_KEY,
                tab_bar_->tabText(index).toLocal8Bit());
  dragging_label_ = tab_bar_->tabText(index);
  dragging_icon_ = tab_bar_->tabIcon(index);
  QDrag *drag = new QDrag(this);
  drag->setMimeData(mime);
  drag->setPixmap(pixmap);
  drag->setHotSpot(dragging_point_delta_);

  if (!isInMain()) this->hide();

  connect(drag, &QDrag::destroyed, this, [=](QObject *) {
    // 判断有没有被合并到窗口
    if (drag_state_ == DragState::NoChanged || drag_state_ == DragState::Merged)
      return;

    // 没有合并到其他窗口
    if (tab_bar_->tabCount() == 1 && !isInMain()) {
    } else {
      // 多个标签拖出，创建新窗口
      createDraggedNewWindow();
    }
    this->move(QCursor::pos());
    this->show();
  });

  // exec 操作会一直阻塞后面的代码，除非使用多线程或者信号槽
  drag->exec();
}

void QtExtTabWidget::OnCloseWnd() {
  if (nullptr == this->parentWidget())
    close();
  else
    emit signal_.closeWnd();
}

void QtExtTabWidget::OnMinWnd() {
  if (nullptr == this->parentWidget())
    this->showMinimized();
  else
    emit signal_.minWnd();
}

void QtExtTabWidget::OnMaxWnd() {
  if (nullptr == this->parentWidget()) {
    this->showMaximized();
    this->setMaxRestoreBtnVisible(false, true);
    this->currentWidget()->setFocus();
  } else
    emit signal_.maxWnd();
}

void QtExtTabWidget::OnRestoreWnd() {
  if (nullptr == this->parentWidget()) {
    this->showNormal();
    this->setMaxRestoreBtnVisible(true, false);
  } else {
    emit signal_.restoreWnd();
  }
}

bool QtExtTabWidget::OnMergeDroppedLabel(QDropEvent *event) {
  int insert_index = -1;
  for (int i = tab_bar_->count() - 1; i >= 0; i--) {
    if (tab_bar_->tabRect(i).contains(event->pos())) {
      insert_index = i;
      break;
    }
  }

  const QMimeData *mime = event->mimeData();
  QtExtTabWidget *window = reinterpret_cast<QtExtTabWidget *>(
      mime->data(DRAGABLE_TAB_WINDOW_MIME_KEY).toInt());
  QWidget *widget = reinterpret_cast<QWidget *>(
      mime->data(DRAGABLE_TAB_WIDGET_MIME_KEY).toInt());
  QString label =
      QString::fromLocal8Bit(mime->data(DRAGABLE_TAB_LABEL_MIME_KEY));
  QIcon icon = dragging_icon_;
  if (window == this) {
    drag_state_ = DragState::NoChanged;
    if (insert_index == currentIndex()) return false;
    removeTab(currentIndex());
    insertTab(insert_index, widget, label);
    setCurrentIndex(insert_index);
    return false;
  }

  drag_state_ = DragState::Merged;
  // 移除旧的
  window->removeTab(window->currentIndex());

  // 插入新的
  if (icon.isNull()) {
    if (insert_index >= count())
      addTab(widget, label);
    else
      insertTab(insert_index, widget, label);
  } else {
    if (insert_index >= count())
      addTab(widget, icon, label);
    else
      insertTab(insert_index, widget, icon, label);
  }

  setCurrentIndex(insert_index);
  this->setFocus();
  QTimer::singleShot(0, this, [=] { widget->setFocus(); });
  return true;
}

void QtExtTabWidget::setupUI() {
  if (nullptr == (tab_bar_ = dynamic_cast<QtExtTabBar *>(tabBar()))) {
    tab_bar_ = new QtExtTabBar(this);
    setTabBar(tab_bar_);
  }
  connect(&(tab_bar_->getSignal()), SIGNAL(addBtnClicked()), &signal_,
          SIGNAL(addBtnClicked()));
  connect(&(tab_bar_->getSignal()), SIGNAL(closeTab(int)), this,
          SLOT(OnCloseTab(int)));

  button_widget_ = new ButtonWidget(this);
  connect(button_widget_, SIGNAL(closeClicked()), this, SLOT(OnCloseWnd()));
  connect(button_widget_, SIGNAL(minClicked()), this, SLOT(OnMinWnd()));
  connect(button_widget_, SIGNAL(maxClicked()), this, SLOT(OnMaxWnd()));
  connect(button_widget_, SIGNAL(restoreClicked()), this, SLOT(OnRestoreWnd()));

  connect(this, SIGNAL(tabBarDoubleClicked(int)), this,
          SLOT(OnTabBarDoubleClicked(int)));
  connect(&(tab_bar_->getSignal()), SIGNAL(signalStartDrag(int)), this,
          SLOT(OnStartDrag(int)));
  connect(&(tab_bar_->getSignal()), SIGNAL(signalEndDrag()), this,
          SLOT(createDraggedNewWindow()));
}

bool QtExtTabWidget::isInMain() { return parentWidget() != nullptr; }

QtExtTabWidget *QtExtTabWidget::createDraggedNewWindow() {
  int titlebar_height = style()->pixelMetric(QStyle::PM_TitleBarHeight);

  QtExtTabWidget *window = newTabWidget();
  window->setAttribute(Qt::WA_DeleteOnClose, true);
  window->resize(QSize(800, 600));
  window->move(QCursor::pos() - dragging_point_delta_ -
               QPoint(WIN_FRAME_LEFE_OFFSET, titlebar_height));
  window->show();
  QString label = tab_bar_->tabText(dragging_index_);
  QIcon icon = tab_bar_->tabIcon(dragging_index_);
  QWidget *button = tab_bar_->tabButton(dragging_index_, QTabBar::RightSide);
  tab_bar_->setTabButton(dragging_index_, QTabBar::RightSide, nullptr);
  removeTab(dragging_index_);
  if (!icon.isNull())
    window->addTab(dragging_widget_, icon, label);
  else
    window->addTab(dragging_widget_, label);
  if (button != nullptr) {
    window->setTabButton2(0, QTabBar::RightSide, button);
  }
  if (!is_main_ && count() == 0)
    deleteLater();
  window->raise();
  window->setFocus();
  dragging_widget_->setFocus();
  QTimer::singleShot(0, dragging_widget_, [=] {
    window->setFocus();
  });
  return window;
}

void QtExtTabWidget::deleteIfEmptyWindow() {
  if (!is_main_ && count() == 0) deleteLater();
}

QtExtTabWidget *QtExtTabWidget::newTabWidget() {
  QtExtTabWidget *widget = new QtExtTabWidget();
  NcFramelessHelper *frame_less_helper_ = new NcFramelessHelper();
  widget->setDocumentMode(true);
  widget->setWindowFlags(widget->windowFlags() | Qt::FramelessWindowHint);
  QtExtTabBar *new_tab_bar = dynamic_cast<QtExtTabBar *>(widget->tabBar());
  new_tab_bar->copyOtherTabBar(tab_bar_);
  new_tab_bar->setTabAddButton(false);
  frame_less_helper_->activeOnWithChildWidget(widget, new_tab_bar);
  widget->initProperty(true, false);
  return widget;
}