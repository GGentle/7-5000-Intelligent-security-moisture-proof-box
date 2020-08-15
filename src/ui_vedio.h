/********************************************************************************
** Form generated from reading ui file 'vedio.ui'
**
** Created: Thu Mar 17 17:30:38 2016
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VEDIO_H
#define UI_VEDIO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vedio
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *vedio)
    {
    if (vedio->objectName().isEmpty())
        vedio->setObjectName(QString::fromUtf8("vedio"));
    vedio->resize(399, 195);
    centralWidget = new QWidget(vedio);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setMargin(11);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    label = new QLabel(centralWidget);
    label->setObjectName(QString::fromUtf8("label"));

    verticalLayout->addWidget(label);

    pushButton = new QPushButton(centralWidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));

    verticalLayout->addWidget(pushButton);


    horizontalLayout->addLayout(verticalLayout);

    vedio->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(vedio);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 399, 24));
    vedio->setMenuBar(menuBar);
    mainToolBar = new QToolBar(vedio);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    vedio->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(vedio);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    vedio->setStatusBar(statusBar);

    retranslateUi(vedio);

    QMetaObject::connectSlotsByName(vedio);
    } // setupUi

    void retranslateUi(QMainWindow *vedio)
    {
    vedio->setWindowTitle(QApplication::translate("vedio", "vedio", 0, QApplication::UnicodeUTF8));
    label->setText(QString());
    pushButton->setText(QApplication::translate("vedio", "PushButton", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(vedio);
    } // retranslateUi

};

namespace Ui {
    class vedio: public Ui_vedio {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEDIO_H
