#include "cus_button.h"
#include "ui_cus_button.h"

#include <QTabBar>


CusBtn::CusBtn(QWidget* parent):QPushButton(parent), ui(new Ui::Ui_CusBtn)
{
  ui->setupUi(this);
  installEventFilter(parent);
  connect(this, SIGNAL(clicked()), this, SLOT(clicked()));
}

CusBtn::~CusBtn()
{

}

void CusBtn::on_btn_clicked_clicked()
{

  emit change_index();

}

void CusBtn::clicked()
{
  emit change_index();
}

