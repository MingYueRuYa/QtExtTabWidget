/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_tabbar;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_tabbar_container;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_btn_container;
    QStackedWidget *stackedWidget;
    QWidget *page_3;
    QWidget *page;
    QWidget *page_2;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QWidget *tab;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(899, 600);
        Widget->setStyleSheet(QString::fromUtf8("border:1px solid red;\n"
"background-color: rgb(0, 85, 127);"));
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 30);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_tabbar = new QWidget(Widget);
        widget_tabbar->setObjectName(QString::fromUtf8("widget_tabbar"));
        widget_tabbar->setMinimumSize(QSize(0, 30));
        widget_tabbar->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);"));
        horizontalLayout_2 = new QHBoxLayout(widget_tabbar);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_tabbar_container = new QWidget(widget_tabbar);
        widget_tabbar_container->setObjectName(QString::fromUtf8("widget_tabbar_container"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_tabbar_container->sizePolicy().hasHeightForWidth());
        widget_tabbar_container->setSizePolicy(sizePolicy);
        widget_tabbar_container->setStyleSheet(QString::fromUtf8("border-width: 0px;\n"
"\n"
"background-color: rgb(255, 255, 0);"));
        horizontalLayout_3 = new QHBoxLayout(widget_tabbar_container);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);

        horizontalLayout_2->addWidget(widget_tabbar_container);

        pushButton = new QPushButton(widget_tabbar);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(26, 26));
        pushButton->setMaximumSize(QSize(26, 26));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(302, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        horizontalLayout->addWidget(widget_tabbar);

        widget_btn_container = new QWidget(Widget);
        widget_btn_container->setObjectName(QString::fromUtf8("widget_btn_container"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_btn_container->sizePolicy().hasHeightForWidth());
        widget_btn_container->setSizePolicy(sizePolicy2);
        widget_btn_container->setMinimumSize(QSize(150, 0));
        widget_btn_container->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(widget_btn_container);

        horizontalLayout->setStretch(0, 8);
        horizontalLayout->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout);

        stackedWidget = new QStackedWidget(Widget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        stackedWidget->addWidget(page_3);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 85, 255);"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 0);"));
        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);

        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setAcceptDrops(true);
        tabWidget->setMovable(true);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Widget", "Tab 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Widget", "Tab 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
