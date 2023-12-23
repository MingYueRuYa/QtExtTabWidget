#include "cus_button.h"
#include "ui_cus_button.h"

#include <QTabBar>


CusBtn::CusBtn(QWidget* parent):QPushButton(parent), ui(new Ui::Ui_CusBtn)
{
  ui->setupUi(this);
  installEventFilter(parent);
  connect(this, SIGNAL(clicked()), this, SLOT(on_btn_clicked_clicked()));
}

CusBtn::~CusBtn()
{
}

// void CusBtn::setWidget(QWidget* widget)
// {
//   this->widget_ = widget;
// }

// const QWidget* CusBtn::getWidget() const
// {
//   return this->widget_;
// }

void CusBtn::on_btn_clicked_clicked()
{
  emit btn_clicked();
}

