#ifndef COMMON_UI_ENUMS_H
#define COMMON_UI_ENUMS_H

#include <QObject>

/*! \brief CommonUIEnums
    \author liushixiong (635672377@qq.com)
    \version 0.01
    \date 2022-1-11 13:39:50

    专为插件构造的enum
    须以这种方式构造enum否则在designer属性栏中看不到选择项

*/
class CommonUIEnums : public QObject {
  Q_OBJECT
  Q_ENUMS(DrawType)
  Q_ENUMS(Position9)
  Q_ENUMS(WidgetSizePolicy)
  Q_ENUMS(ImageSizePolicy)

 public:
  CommonUIEnums() = default;
  ~CommonUIEnums() = default;

 public:
  enum DrawType { DrawNone = 0, Circle, Rectangle };

  // 九宫格位置
  enum Position9 {
    PosNone = 0,
    TopLeft = 1,
    TopMiddle,
    TopRight,

    RightMiddle,
    RightBottom,

    BottomMiddle,
    BottomLeft,

    LeftMiddle,
    HVCenter
  };

  enum WidgetSizePolicy {
    WidgetSizeNone = 0,  // 无动作，让布局自动调整

    // size不可变
    FixSize,                  // m_widgetSize_为准
    UseImageSize,             // 保持和图片大小一致
    FixWidthWithImageRatio,   // 宽度用户固定，并以Image ratio为准
    FixHeightWithImageRatio,  // 高度用户固定，并以Image ratio为准

    // 可变，比例不变
    ScaleHeightByLayoutWithRatio,  // 由布局器管理大小，width由布局器决定,
                                   // 然后再根据设置的比例计算大小
    ScaleWidthByLayoutWithRatio,  // 由布局器管理大小，height由布局器决定，然后再根据设置的比例计算大小

    // 可变，随机比例
    WidgetSizeDragDrop  // 用户在无布局的情况下自行拖拽
  };

  enum ImageSizePolicy {
    ImageSizeNone = 0,
    // 不可变
    FixImageSize,  // 图片大小固定，用户可设置以m_imageSize_为准
    SelfSize,      // 图片大小固定，使用图片大小

    // 可变：确定比例
    AdjustifyImageSize,  // 保持比例拉伸

    // 可变：不确定比例
    Stretch,  // 双向拉伸
  };
};

using UIPosition9 = CommonUIEnums::Position9;
using UIWidgetSizePolicy = CommonUIEnums::WidgetSizePolicy;
using UIImageSizePolicy = CommonUIEnums::ImageSizePolicy;
using UIDrawType = CommonUIEnums::DrawType;

#endif  // COMMON_UI_ENUMS_H
