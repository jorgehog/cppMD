/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Exit;
    QWidget *centralWidget;
    QLabel *label_2;
    QComboBox *meshComboBox;
    QPushButton *pushButton;
    QLabel *label_3;
    QListWidget *EventList;
    QLabel *label_6;
    QListWidget *listWidget;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableWidget *tableWidget;
    QComboBox *eventComboBox;
    QLabel *label;
    QWidget *tab_2;
    QFormLayout *formLayout;
    QLabel *label_5;
    QLineEdit *lineEdit;
    QLabel *label_4;
    QTableWidget *tableWidget_mesh;
    QGraphicsView *graphicsView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuUberGUI;
    QMenu *menuOptions;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(547, 821);
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(279, 9, 81, 16));
        meshComboBox = new QComboBox(centralWidget);
        meshComboBox->setObjectName(QStringLiteral("meshComboBox"));
        meshComboBox->setGeometry(QRect(410, 9, 79, 23));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(279, 261, 80, 23));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(279, 38, 110, 16));
        EventList = new QListWidget(centralWidget);
        EventList->setObjectName(QStringLiteral("EventList"));
        EventList->setGeometry(QRect(279, 59, 256, 85));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(279, 150, 114, 16));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(279, 171, 256, 84));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(166, 261, 80, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(410, 261, 80, 23));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(9, 30, 241, 231));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(10, 50, 221, 191));
        tableWidget->setShowGrid(true);
        tableWidget->setCornerButtonEnabled(true);
        tableWidget->setRowCount(6);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);
        eventComboBox = new QComboBox(tab);
        eventComboBox->setObjectName(QStringLiteral("eventComboBox"));
        eventComboBox->setGeometry(QRect(110, 10, 131, 23));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 80, 16));
        tabWidget->addTab(tab, QString());
        eventComboBox->raise();
        tableWidget->raise();
        label->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        formLayout = new QFormLayout(tab_2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_5);

        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(1, QFormLayout::SpanningRole, label_4);

        tableWidget_mesh = new QTableWidget(tab_2);
        if (tableWidget_mesh->columnCount() < 2)
            tableWidget_mesh->setColumnCount(2);
        if (tableWidget_mesh->rowCount() < 3)
            tableWidget_mesh->setRowCount(3);
        tableWidget_mesh->setObjectName(QStringLiteral("tableWidget_mesh"));
        tableWidget_mesh->setRowCount(3);
        tableWidget_mesh->setColumnCount(2);

        formLayout->setWidget(2, QFormLayout::SpanningRole, tableWidget_mesh);

        tabWidget->addTab(tab_2, QString());
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(10, 300, 531, 461));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 547, 20));
        menuUberGUI = new QMenu(menuBar);
        menuUberGUI->setObjectName(QStringLiteral("menuUberGUI"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuUberGUI->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuUberGUI->addSeparator();
        menuUberGUI->addAction(action_Exit);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action_Exit->setText(QApplication::translate("MainWindow", "Exit", 0));
        label_2->setText(QApplication::translate("MainWindow", "Current Mesh", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Remove", 0));
        label_3->setText(QApplication::translate("MainWindow", "Associated Events", 0));
        label_6->setText(QApplication::translate("MainWindow", "Associated Meshes", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Add", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Run", 0));
        label->setText(QApplication::translate("MainWindow", "Select Event:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Events", 0));
        label_5->setText(QApplication::translate("MainWindow", "Name", 0));
        label_4->setText(QApplication::translate("MainWindow", "Topology", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Meshes", 0));
        menuUberGUI->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
