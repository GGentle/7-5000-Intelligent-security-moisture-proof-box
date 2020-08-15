/********************************************************************************
** Form generated from reading ui file 'vediorcv.ui'
**
** Created: Sat Sep 19 21:44:43 2015
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VEDIORCV_H
#define UI_VEDIORCV_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vediorcv
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *vediorcv)
    {
    if (vediorcv->objectName().isEmpty())
        vediorcv->setObjectName(QString::fromUtf8("vediorcv"));
    vediorcv->resize(426, 325);
    centralWidget = new QWidget(vediorcv);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    label = new QLabel(centralWidget);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(40, 20, 341, 241));
    vediorcv->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(vediorcv);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 426, 24));
    vediorcv->setMenuBar(menuBar);
    mainToolBar = new QToolBar(vediorcv);
    mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    vediorcv->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(vediorcv);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    vediorcv->setStatusBar(statusBar);

    retranslateUi(vediorcv);

    QMetaObject::connectSlotsByName(vediorcv);
    } // setupUi

    void retranslateUi(QMainWindow *vediorcv)
    {
    vediorcv->setWindowTitle(QApplication::translate("vediorcv", "vediorcv", 0, QApplication::UnicodeUTF8));
    label->setText(QString());
    Q_UNUSED(vediorcv);
    } // retranslateUi

};

namespace Ui {
    class vediorcv: public Ui_vediorcv {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEDIORCV_H
