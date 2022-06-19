#pragma once

#include <QtCore/qglobal.h>

// 定义基本类型的属性
#define UI_PROPERTY_B(TYPE, name, write, def, call) \
  Q_PROPERTY(TYPE name READ name WRITE write)       \
 private:                                           \
  TYPE name##_ = def;                               \
                                                    \
 public:                                            \
  TYPE name() const { return name##_; }             \
  void write(TYPE value) {                          \
    if (name##_ != value) {                         \
      name##_ = value;                              \
      call;                                         \
    }                                               \
  }

// 定义枚举类型的属性
#define UI_PROPERTY_E(TYPE, name, write, def, call) \
  Q_ENUMS(TYPE)                                     \
  Q_PROPERTY(TYPE name READ name WRITE write)       \
 private:                                           \
  TYPE name##_ = def;                           \
                                                    \
 public:                                            \
  TYPE name() const { return name##_; }         \
  void write(TYPE value) {                          \
    if (name##_ != value) {                     \
      name##_ = value;                          \
      call;                                         \
    }                                               \
  }

#define HOVER_TAB_CONTAINER QColor(243, 244, 246)
#define NORMAL_TAB_CONTAINER QColor(222, 225, 230)
#define CIRCLE_BG_COLOR QColor(220, 220, 221)
#define X_CHARACTER_COLOR QColor(60, 64, 67)
#define NORMAL_TAB_COLOR QColor(240, 240, 240)
