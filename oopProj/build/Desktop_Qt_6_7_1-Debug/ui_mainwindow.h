/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QWidget *navbar_2;
    QVBoxLayout *verticalLayout;
    QWidget *navbar_1;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *signup;
    QPushButton *login;
    QLabel *heading;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1920, 1080);
        MainWindow->setStyleSheet(QString::fromUtf8("/* Navbar Styling */\n"
"#navbar_2 {\n"
"    background-color: #2c3e50;  /* Dark background */\n"
"    padding: 10px;  /* Add some padding */\n"
"}\n"
"\n"
"QLabel#heading {\n"
"    color: #ecf0f1;  /* Light color for the title text */\n"
"    font-size: 24px;\n"
"    font-weight: bold;\n"
"    margin-left: 20px;  /* Padding to move title from left */\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #3498db;  /* Light blue background for buttons */\n"
"    color: #fff;  /* White text */\n"
"    border: none;\n"
"    padding: 8px 15px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #2980b9;  /* Darker blue on hover */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1f6393;  /* Even darker blue when pressed */\n"
"}\n"
"\n"
"QPushButton#signup {\n"
"    margin-left: 10px;  /* Space between Login and Register */\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 120, 1331, 601));
        navbar_2 = new QWidget(centralwidget);
        navbar_2->setObjectName("navbar_2");
        navbar_2->setGeometry(QRect(-1, -11, 1920, 72));
        verticalLayout = new QVBoxLayout(navbar_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        navbar_1 = new QWidget(navbar_2);
        navbar_1->setObjectName("navbar_1");
        gridLayout = new QGridLayout(navbar_1);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(662, 28, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        signup = new QPushButton(navbar_1);
        signup->setObjectName("signup");
        QFont font;
        font.setBold(true);
        signup->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/user-plus-solid.svg"), QSize(), QIcon::Normal, QIcon::Off);
        signup->setIcon(icon);
        signup->setIconSize(QSize(14, 14));

        gridLayout->addWidget(signup, 0, 3, 1, 1);

        login = new QPushButton(navbar_1);
        login->setObjectName("login");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        login->setFont(font1);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/right-to-bracket-solid.svg"), QSize(), QIcon::Normal, QIcon::Off);
        login->setIcon(icon1);
        login->setIconSize(QSize(11, 14));

        gridLayout->addWidget(login, 0, 2, 1, 1, Qt::AlignRight);

        heading = new QLabel(navbar_1);
        heading->setObjectName("heading");
        heading->setFont(font);
        heading->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(heading, 0, 0, 1, 1);


        verticalLayout->addWidget(navbar_1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        signup->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
        login->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        heading->setText(QCoreApplication::translate("MainWindow", "Popular Movies", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
