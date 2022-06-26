#include <QFont>
#include "draw_helper.h"
#include "round_shadow_helper.h"
#include "tab_bar_draw_helper.h"

TabBarDrawHelper::TabBarDrawHelper() {
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
}

TabBarDrawHelper::~TabBarDrawHelper() {}

void TabBarDrawHelper::paintTabAddBtn(QPainter &painter, const QRect &rect,
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

void TabBarDrawHelper::paintTab(QPainter &painter, const QRect &draw_rect,
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
      painter.fontMetrics().elidedText(text, Qt::ElideRight, real_rect.width(), 0);
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

void TabBarDrawHelper::paintTabBar(QPainter &painter, const QRect &rect) {
  if (tab_bar_bk_image_.isNull()) return;
  painter.save();
  painter.drawPixmap(rect, tab_bar_bk_image_);
  painter.restore();
}

void TabBarDrawHelper::paintScrollBtn(bool left, QPainter &painter,
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

QRect TabBarDrawHelper::calcIconBtnRect(bool left_button,
                                        const QSize &icon_size,
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
