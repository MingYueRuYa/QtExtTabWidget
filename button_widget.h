#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include <QWidget>

namespace Ui {
class ButtonWidget;
}

class ButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidget(QWidget *parent = 0);
    ~ButtonWidget();

    void setMaxRestoreBtnVisible(bool max_visible, bool restore_visible);

Q_SIGNALS:
    void closeClicked();
    void minClicked();
    void maxClicked();
    void restoreClicked();

protected Q_SLOTS:
    void on_btn_close_clicked();
    void on_btn_min_clicked();
    void on_btn_max_clicked();
    void on_btn_restore_clicked();

private:
    void setupUI();

private:
    Ui::ButtonWidget *ui;
    bool is_maxed_ = false;
};

#endif // BUTTON_WIDGET_H
