#pragma once
#include <QWidget>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class Ui_CusBtn; }
QT_END_NAMESPACE

class CusBtn :
    public QPushButton 
{
  Q_OBJECT

public:
  explicit CusBtn(QWidget *parent = nullptr);
  ~CusBtn();


protected slots:
  void on_btn_clicked_clicked();
  void clicked();

signals:
  void change_index();

private:
  Ui::Ui_CusBtn *ui;
};

