#include "tab_widget_proxy.h"

#include <functional>

#include "NcFrameLessHelper.h"
#include "draw_helper.h"
#include "qt_ext_tab_widget.h"
#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"
#include "weak_call_back.hpp"

TabWidgetProxy::TabWidgetProxy(QWidget *parent) : QWidget(parent) {
  setupUI();

#ifdef _DEBUG
  QWidget *widget = new QWidget();
  widget->setStyleSheet("background-color:#FF00FF00");
  ext_tab_widget_->addTab2(widget, tr("this is first tab"));
#endif  //_DEBUG
}

TabWidgetProxy::~TabWidgetProxy() { delete frame_less_helper_; }

void TabWidgetProxy::setupUI() {
  this->setWindowFlags(Qt::FramelessWindowHint |
                       Qt::X11BypassWindowManagerHint);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  ext_tab_widget_ = new QtExtTabWidget(this);
  layout->addWidget(ext_tab_widget_);

  // frame_less_helper_ = new NcFramelessHelper();
  // frame_less_helper_->setFilterEventCallBack(
  //     std::bind(&TabWidgetProxy::filterTabBarEvent, this,
  //     std::placeholders::_1,
  //               std::placeholders::_2));
  // frame_less_helper_->activeOnWithChildWidget(this,
  // ext_tab_widget_->tabBar());

  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabInserted(int)), this,
          SLOT(OnTabInserted(int)));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(AddBtnClicked()), this,
          SLOT(OnAddBtnClicked()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabClosed(int)), this,
          SLOT(OnCloseTab(int)));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(closeWnd()), this,
          SLOT(OnCloseWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(minWnd()), this,
          SLOT(OnMinWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(maxWnd()), this,
          SLOT(OnMaxWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(restoreWnd()), this,
          SLOT(OnRestoreWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabBarDoubleClicked()), this,
          SLOT(OnMaxOrRestore()));
  tab_bar_bk_image_.load(":/images/default_100_precent/caption_bg.png");
  tab_bk_normal_image_.load(":/images/default_100_precent/tab.png");
  tab_bk_hover_image_.load(":/images/default_100_precent/tab_hover.png");
  tab_bk_selected_image_.load(":/images/default_100_precent/tab_check.png");

  left_arrow_pixmap_list_.push_back(
      QPixmap(":/images/default_100_precent/left_arrow_normal.png"));
  left_arrow_pixmap_list_.push_back(
      QPixmap(":/images/default_100_precent/left_arrow_disabled.png"));

  right_arrow_pixmap_list_.push_back(
      QPixmap(":/images/default_100_precent/right_arrow_normal.png"));
  right_arrow_pixmap_list_.push_back(
      QPixmap(":/images/default_100_precent/right_arrow_disabled.png"));

  ext_tab_widget_->initProperty();
  ext_tab_widget_->setPaintAddBtnFunc(
      std::bind(&TabWidgetProxy::paintTabAddBtn, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
  ext_tab_widget_->setPaintTabFunc(
      std::bind(&TabWidgetProxy::paintTab, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3,
                std::placeholders::_4, std::placeholders::_5));
  ext_tab_widget_->setPaintTabBarFunc(std::bind(&TabWidgetProxy::paintTabBar,
                                                this, std::placeholders::_1,
                                                std::placeholders::_2));
  ext_tab_widget_->setPaintScrollBtnFunc(std::bind(
      &TabWidgetProxy::paintScrollBtn, this, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  ext_tab_widget_->setCalcIconBtnRectFunc(
      std::bind(&TabWidgetProxy::calcIconRect, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
}

bool TabWidgetProxy::filterTabBarEvent(QObject *obj, QEvent *event) {
  // Only click on the tab bar non-tab area can be dragged
  if (event->type() != QEvent::MouseButtonPress) return false;

  QMouseEvent *mouse_event = dynamic_cast<QMouseEvent *>(event);
  // （只有在拖拽区域不在tab rect区域 || 只有一个tab标签时才可以拖动）
  if (isMaximized()) {
    return true;
  } else if (ext_tab_widget_->tabBar2() == obj &&
             (ext_tab_widget_->pointInTabRectIndex(mouse_event->pos()) < 0 ||
              ext_tab_widget_->tabBar2()->tabCount() == 1)) {
    return false;
  } else {
    return true;
  }
}

QTabBar *TabWidgetProxy::tabBar() const { return ext_tab_widget_->tabBar(); }

void TabWidgetProxy::OnTabInserted(int index) {
  // QPushButton *button = new QPushButton();
  // button->setFixedSize(this->iconSize());
  // button->setStyleSheet("border-image:
  // url(:/images/x-capture-options.png);"); ui->tabWidget->setTabButton2(index,
  // QTabBar::LeftSide, button);

  if (isHasParent()) emit signal_.tabInserted(index);
  QPushButton *button = new QPushButton();
  button->setStyleSheet(
      "QPushButton{border-image: url(:/images/close.png)}"
      "QPushButton:hover{border-image: url(:/images/close_hover.png)}");
  ext_tab_widget_->setTabButton2(index, QTabBar::RightSide, button);
  ext_tab_widget_->setTabIcon(index, QIcon(":/images/x-capture-options.png"));
}

void TabWidgetProxy::OnCloseTab(int index) {
  // todo something
  if (isHasParent()) emit signal_.tabClosed(index);
}

void TabWidgetProxy::OnAddBtnClicked() {
  QWidget *widget = new QWidget();
  widget->setStyleSheet("background-color:#FFFFFF00");
  ext_tab_widget_->addTab2(widget, tr("this is first tab"));
}

void TabWidgetProxy::OnCloseWnd() {
  if (isHasParent())
    emit signal_.closeWnd();
  else
    this->close();
}

void TabWidgetProxy::OnMinWnd() {
  if (isHasParent())
    emit signal_.minWnd();
  else
    this->showMinimized();
}

void TabWidgetProxy::OnMaxWnd() {
  if (isHasParent())
    emit signal_.maxWnd();
  else {
    this->showMaximized();
    ext_tab_widget_->currentWidget()->setFocus();
  }
  ext_tab_widget_->setMaxRestoreBtnVisible(false, true);
}

void TabWidgetProxy::OnRestoreWnd() {
  if (isHasParent())
    emit signal_.restoreWnd();
  else {
    this->showNormal();
  }
  ext_tab_widget_->setMaxRestoreBtnVisible(true, false);
}

void TabWidgetProxy::OnMaxOrRestore() {
  if (isHasParent())
    emit signal_.tabBarDoubleClicked();
  else {
    if (isMaximized()) {
      this->showNormal();
      ext_tab_widget_->setMaxRestoreBtnVisible(true, false);
    } else {
      this->showMaximized();
      ext_tab_widget_->setMaxRestoreBtnVisible(false, true);
    }
  }
}

void TabWidgetProxy::paintTabAddBtn(QPainter &painter, const QRect &rect,
                                    TabState tab_state) {
  QColor color = (TabState::hover_ == tab_state) ? QColor(255, 255, 255, 50)
                                                 : Qt::transparent;
  painter.save();
  QRect draw_rect = QRect(QPoint(0, 0), QSize(20, 20));
  draw_rect.moveCenter(rect.center());
  DrawCircle::Draw(&painter, draw_rect, color);
  DrawCharacter::DrawPlus(&painter, draw_rect, Qt::white);
  painter.restore();
}

void TabWidgetProxy::paintTab(QPainter &painter, const QRect &draw_rect,
                              TabState tab_state, const QString &text,
                              const QIcon &icon) {
  painter.save();
  painter.setPen(Qt::NoPen);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  if (TabState::selected_ == tab_state) {
    if (!tab_bk_selected_image_.isNull())
      painter.drawPixmap(draw_rect, tab_bk_selected_image_);
  } else if (TabState::hover_ == tab_state) {
    if (!tab_bk_hover_image_.isNull())
      painter.drawPixmap(draw_rect, tab_bk_hover_image_);
  } else if (TabState::normal_ == tab_state) {
    if (!tab_bk_normal_image_.isNull())
      painter.drawPixmap(draw_rect, tab_bk_normal_image_);
  }
  QRect real_rect = draw_rect;
  real_rect -= QMargins(35, 0, 30, 0);
  painter.setPen(Qt::white);
  QString draw_text =
      fontMetrics().elidedText(text, Qt::ElideRight, real_rect.width(), 0);
  painter.drawText(real_rect, Qt::AlignLeft | Qt::AlignVCenter, draw_text);
  painter.restore();
  // draw tab icon
  if (!icon.isNull()) {
    painter.save();
    painter.setPen(Qt::NoPen);
    QList<QSize> actual_size = icon.availableSizes();
    QSize icon_size = actual_size[0];
    QPoint icon_point = QPoint(draw_rect.left() + 5,
                               (draw_rect.height() - icon_size.height()) / 2);
    QRect icon_rect(icon_point, icon_size);
    painter.drawPixmap(icon_rect, icon.pixmap(icon_size));
    painter.restore();
  }
}

void TabWidgetProxy::paintTabBar(QPainter &painter, const QRect &rect) {
  if (tab_bar_bk_image_.isNull()) return;
  painter.save();
  painter.drawPixmap(rect, tab_bar_bk_image_);
  painter.restore();
}

void TabWidgetProxy::paintScrollBtn(bool left, QPainter &painter,
                                    const QRect &rect,
                                    const QToolButton *tool_btn) {
  QBrush rect_brush = Qt::transparent;
  if (tool_btn->underMouse()) {
    rect_brush = QColor(214, 214, 214, 100);
  }

  QRect draw_rect = QRect(0, 0, 20, 20);
  draw_rect.moveCenter(rect.center());
  RoundShadowHelper round_helper;
  round_helper.FillRoundShadow(&painter, draw_rect, rect_brush.color(), 4);

  draw_rect = QRect(0, 0, 16, 16);
  draw_rect.moveCenter(rect.center());
  painter.save();
  if (!tool_btn->isEnabled()) {
    // disabled
    painter.drawPixmap(draw_rect, left ? left_arrow_pixmap_list_[1]
                                       : right_arrow_pixmap_list_[1]);
  } else {
    // normal
    painter.drawPixmap(draw_rect, left ? left_arrow_pixmap_list_[0]
                                       : right_arrow_pixmap_list_[0]);
  }

  painter.restore();
}

QRect TabWidgetProxy::calcIconRect(bool left_button, const QSize &icon_size,
                                   const QRect &btn_rect) {
  QPoint center_pos = QPoint(0, 0);
  QRect button_rect = QRect(0, 0, 0, 0);
  const int icon_padding = 8;
  if (left_button) {
    center_pos =
        QPoint(icon_padding + icon_size.width() / 2 + btn_rect.x(),
               btn_rect.y() + (btn_rect.height() - icon_size.height()) / 2 +
                   icon_size.height() / 2);
  } else {
    center_pos = QPoint(
        btn_rect.x() + btn_rect.width() - icon_padding - icon_size.width() / 2,
        btn_rect.y() + (btn_rect.height() - icon_size.height()) / 2 +
            icon_size.height() / 2);
  }
  button_rect = QRect(QPoint(0, 0), icon_size);
  button_rect.moveCenter(center_pos);
  return button_rect;
}

bool TabWidgetProxy::isHasParent() const { return nullptr != parentWidget(); }
