/********************************************************************************
** Form generated from reading UI file 'cus_button.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUS_BUTTON_H
#define UI_CUS_BUTTON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ui_CusBtn
{
public:
    QGridLayout *gridLayout;
    QWidget *icon_container;
    QGridLayout *gridLayout_3;
    QPushButton *btn_icon;
    QWidget *btn_container;
    QGridLayout *gridLayout_2;
    QPushButton *btn_close;

    void setupUi(QPushButton *Ui_CusBtn)
    {
        if (Ui_CusBtn->objectName().isEmpty())
            Ui_CusBtn->setObjectName(QString::fromUtf8("Ui_CusBtn"));
        Ui_CusBtn->resize(150, 35);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Ui_CusBtn->sizePolicy().hasHeightForWidth());
        Ui_CusBtn->setSizePolicy(sizePolicy);
        Ui_CusBtn->setStyleSheet(QString::fromUtf8("QPushButton#Ui_CusBtn {\n"
"background-color:#FF00FFFF;\n"
"border:1px solid yellow;\n"
"}\n"
"\n"
"QPushButton#Ui_CusBtn:hover {\n"
"background-color:#FF0000FF;\n"
"}\n"
"\n"
"QPushButton#Ui_CusBtn:pressed {\n"
"background-color:#FFFF00FF;\n"
"}\n"
"\n"
""));
        gridLayout = new QGridLayout(Ui_CusBtn);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        icon_container = new QWidget(Ui_CusBtn);
        icon_container->setObjectName(QString::fromUtf8("icon_container"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(icon_container->sizePolicy().hasHeightForWidth());
        icon_container->setSizePolicy(sizePolicy1);
        icon_container->setMinimumSize(QSize(30, 0));
        icon_container->setMaximumSize(QSize(40, 30));
        gridLayout_3 = new QGridLayout(icon_container);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(10, 0, 0, 0);
        btn_icon = new QPushButton(icon_container);
        btn_icon->setObjectName(QString::fromUtf8("btn_icon"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btn_icon->sizePolicy().hasHeightForWidth());
        btn_icon->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(btn_icon, 0, 0, 1, 1);


        gridLayout->addWidget(icon_container, 0, 0, 1, 1, Qt::AlignLeft);

        btn_container = new QWidget(Ui_CusBtn);
        btn_container->setObjectName(QString::fromUtf8("btn_container"));
        sizePolicy1.setHeightForWidth(btn_container->sizePolicy().hasHeightForWidth());
        btn_container->setSizePolicy(sizePolicy1);
        btn_container->setMinimumSize(QSize(30, 0));
        gridLayout_2 = new QGridLayout(btn_container);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        btn_close = new QPushButton(btn_container);
        btn_close->setObjectName(QString::fromUtf8("btn_close"));
        sizePolicy.setHeightForWidth(btn_close->sizePolicy().hasHeightForWidth());
        btn_close->setSizePolicy(sizePolicy);
        btn_close->setMinimumSize(QSize(20, 20));
        btn_close->setMaximumSize(QSize(20, 20));

        gridLayout_2->addWidget(btn_close, 0, 0, 1, 1);


        gridLayout->addWidget(btn_container, 0, 0, 1, 1, Qt::AlignRight);

        btn_container->raise();
        icon_container->raise();

        retranslateUi(Ui_CusBtn);

        QMetaObject::connectSlotsByName(Ui_CusBtn);
    } // setupUi

    void retranslateUi(QPushButton *Ui_CusBtn)
    {
        Ui_CusBtn->setWindowTitle(QCoreApplication::translate("Ui_CusBtn", "Form", nullptr));
        btn_icon->setText(QCoreApplication::translate("Ui_CusBtn", "icon", nullptr));
        btn_close->setText(QCoreApplication::translate("Ui_CusBtn", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Ui_CusBtn: public Ui_Ui_CusBtn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUS_BUTTON_H
