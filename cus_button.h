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
//   void setWidget(QWidget *widget);
//   const QWidget* getWidget() const;

protected slots:
  void on_btn_clicked_clicked();

signals:
  void btn_clicked();

private:
  Ui::Ui_CusBtn *ui;
//   QWidget* widget_ = nullptr;
};

