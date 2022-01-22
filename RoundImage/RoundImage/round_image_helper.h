#ifndef ROUND_IMAGE_HELEPR
#define ROUND_IMAGE_HELEPR

#include "round_shadow_helper.h"

#include <QPixmap>

class RoundImageHelper : public RoundShadowHelper{
 public:
  explicit RoundImageHelper() = default;
  explicit RoundImageHelper(const int shadow_width, const int radius);
  ~RoundImageHelper() = default;
  QPixmap RoundImage(const QPixmap& source_pixmap);
};

#endif  // ROUND_IMAGE_HELEPR
