#include "button_widget.h"
#include "ui_button_widget.h"

#include <QApplication>

ButtonWidget::ButtonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonWidget)
{
    ui->setupUi(this);
    setupUI();
}

ButtonWidget::~ButtonWidget()
{
    delete ui;
}

void ButtonWidget::setMaxRestoreBtnVisible(bool max_visible, bool restore_visible)
{
    ui->btn_max->setVisible(max_visible);
    ui->btn_restore->setVisible(restore_visible);
}

void ButtonWidget::setupUI()
{
    QStringList image_path;
    image_path << ":/images/default_100_precent/restore.png" 
                <<":/images/default_100_precent/restore_hover.png" 
                << ":/images/default_100_precent/restore_pressed.png"
                << "";
    ui->btn_restore->setImagePath(image_path);
    image_path.clear();
    image_path << ":/images/default_100_precent/max.png" 
                <<":/images/default_100_precent/max_hover.png" 
                << ":/images/default_100_precent/max_pressed.png"
                << "";
    ui->btn_max->setImagePath(image_path);
    ui->btn_restore->setVisible(false);
}

void ButtonWidget::on_btn_close_clicked()
{
    emit closeClicked();
}

void ButtonWidget::on_btn_min_clicked()
{
    emit minClicked();
}

void ButtonWidget::on_btn_max_clicked()
{
    emit maxClicked();
}

void ButtonWidget::on_btn_restore_clicked()
{
    emit restoreClicked();
}
