/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_ffne_CSV_Datei;
    QAction *actionDaten_entfernen;
    QAction *actionExport;
    QAction *action_Hilfe;
    QAction *actionSpeichern;
    QAction *actionOffnen;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter_2;
    QCustomPlot *plot;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBoxLegende;
    QSpinBox *spinBoxIdealHum;
    QPushButton *pushFoto;
    QPushButton *plotUebertragenFlach;
    QPushButton *plotUebertragen;
    QToolButton *toolButtonBold;
    QToolButton *toolButtonUnterstrichen;
    QToolButton *toolButtonItalic;
    QFontComboBox *fontComboBox;
    QComboBox *comboSize;
    QSpacerItem *horizontalSpacer_2;
    QTextEdit *textEdit;
    QTableView *tableView;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuDatei;
    QMenu *Hilfe;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1050, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        action_ffne_CSV_Datei = new QAction(MainWindow);
        action_ffne_CSV_Datei->setObjectName(QString::fromUtf8("action_ffne_CSV_Datei"));
        actionDaten_entfernen = new QAction(MainWindow);
        actionDaten_entfernen->setObjectName(QString::fromUtf8("actionDaten_entfernen"));
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName(QString::fromUtf8("actionExport"));
        action_Hilfe = new QAction(MainWindow);
        action_Hilfe->setObjectName(QString::fromUtf8("action_Hilfe"));
        actionSpeichern = new QAction(MainWindow);
        actionSpeichern->setObjectName(QString::fromUtf8("actionSpeichern"));
        actionOffnen = new QAction(MainWindow);
        actionOffnen->setObjectName(QString::fromUtf8("actionOffnen"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        plot = new QCustomPlot(splitter_2);
        plot->setObjectName(QString::fromUtf8("plot"));
        plot->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy1);
        plot->setMinimumSize(QSize(0, 0));
        splitter_2->addWidget(plot);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        checkBoxLegende = new QCheckBox(layoutWidget);
        checkBoxLegende->setObjectName(QString::fromUtf8("checkBoxLegende"));
        checkBoxLegende->setChecked(true);

        horizontalLayout->addWidget(checkBoxLegende);

        spinBoxIdealHum = new QSpinBox(layoutWidget);
        spinBoxIdealHum->setObjectName(QString::fromUtf8("spinBoxIdealHum"));
        spinBoxIdealHum->setMinimum(40);
        spinBoxIdealHum->setMaximum(65);
        spinBoxIdealHum->setValue(50);

        horizontalLayout->addWidget(spinBoxIdealHum);

        pushFoto = new QPushButton(layoutWidget);
        pushFoto->setObjectName(QString::fromUtf8("pushFoto"));

        horizontalLayout->addWidget(pushFoto);

        plotUebertragenFlach = new QPushButton(layoutWidget);
        plotUebertragenFlach->setObjectName(QString::fromUtf8("plotUebertragenFlach"));

        horizontalLayout->addWidget(plotUebertragenFlach);

        plotUebertragen = new QPushButton(layoutWidget);
        plotUebertragen->setObjectName(QString::fromUtf8("plotUebertragen"));

        horizontalLayout->addWidget(plotUebertragen);

        toolButtonBold = new QToolButton(layoutWidget);
        toolButtonBold->setObjectName(QString::fromUtf8("toolButtonBold"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        toolButtonBold->setFont(font);
        toolButtonBold->setCheckable(true);

        horizontalLayout->addWidget(toolButtonBold);

        toolButtonUnterstrichen = new QToolButton(layoutWidget);
        toolButtonUnterstrichen->setObjectName(QString::fromUtf8("toolButtonUnterstrichen"));
        QFont font1;
        font1.setUnderline(true);
        toolButtonUnterstrichen->setFont(font1);
        toolButtonUnterstrichen->setCheckable(true);

        horizontalLayout->addWidget(toolButtonUnterstrichen);

        toolButtonItalic = new QToolButton(layoutWidget);
        toolButtonItalic->setObjectName(QString::fromUtf8("toolButtonItalic"));
        QFont font2;
        font2.setItalic(true);
        toolButtonItalic->setFont(font2);
        toolButtonItalic->setCheckable(true);

        horizontalLayout->addWidget(toolButtonItalic);

        fontComboBox = new QFontComboBox(layoutWidget);
        fontComboBox->setObjectName(QString::fromUtf8("fontComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fontComboBox->sizePolicy().hasHeightForWidth());
        fontComboBox->setSizePolicy(sizePolicy2);
        fontComboBox->setEditable(false);

        horizontalLayout->addWidget(fontComboBox);

        comboSize = new QComboBox(layoutWidget);
        comboSize->setObjectName(QString::fromUtf8("comboSize"));

        horizontalLayout->addWidget(comboSize);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QTextEdit(layoutWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setUndoRedoEnabled(true);
        textEdit->setLineWrapMode(QTextEdit::FixedPixelWidth);
        textEdit->setLineWrapColumnOrWidth(630);
        textEdit->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextEditable|Qt::TextEditorInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(textEdit);

        splitter->addWidget(layoutWidget);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        sizePolicy.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy);
        tableView->setMaximumSize(QSize(16777215, 16777215));
        splitter->addWidget(tableView);
        splitter_2->addWidget(splitter);

        verticalLayout_2->addWidget(splitter_2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1050, 30));
        menuDatei = new QMenu(menuBar);
        menuDatei->setObjectName(QString::fromUtf8("menuDatei"));
        Hilfe = new QMenu(menuBar);
        Hilfe->setObjectName(QString::fromUtf8("Hilfe"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuDatei->menuAction());
        menuBar->addAction(Hilfe->menuAction());
        menuDatei->addAction(action_ffne_CSV_Datei);
        menuDatei->addAction(actionDaten_entfernen);
        menuDatei->addAction(actionOffnen);
        menuDatei->addAction(actionSpeichern);
        menuDatei->addAction(actionExport);
        Hilfe->addAction(action_Hilfe);

        retranslateUi(MainWindow);
        QObject::connect(action_ffne_CSV_Datei, SIGNAL(triggered()), MainWindow, SLOT(addDataFromFile()));
        QObject::connect(actionDaten_entfernen, SIGNAL(triggered()), MainWindow, SLOT(clearCSVList()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SSC Hygro-Thermometer Datenansicht", 0, QApplication::UnicodeUTF8));
        action_ffne_CSV_Datei->setText(QApplication::translate("MainWindow", "&Daten aus CSV-Datei hinzuf\303\274gen", 0, QApplication::UnicodeUTF8));
        actionDaten_entfernen->setText(QApplication::translate("MainWindow", "Daten &entfernen", 0, QApplication::UnicodeUTF8));
        actionExport->setText(QApplication::translate("MainWindow", "E&xport PDF", 0, QApplication::UnicodeUTF8));
        action_Hilfe->setText(QApplication::translate("MainWindow", "&Hilfe", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Hilfe->setToolTip(QApplication::translate("MainWindow", "Hilfe", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSpeichern->setText(QApplication::translate("MainWindow", "&Speichern", 0, QApplication::UnicodeUTF8));
        actionOffnen->setText(QApplication::translate("MainWindow", "&\303\226ffnen", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBoxLegende->setToolTip(QApplication::translate("MainWindow", "Plot Legende anzeigen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBoxLegende->setText(QApplication::translate("MainWindow", "Legende", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        spinBoxIdealHum->setToolTip(QApplication::translate("MainWindow", "Setze idealen Luftfeuchtigkeitswert", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushFoto->setToolTip(QApplication::translate("MainWindow", "Speichere ein Foto vom aktuellen Plot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushFoto->setText(QApplication::translate("MainWindow", "Foto", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        plotUebertragenFlach->setToolTip(QApplication::translate("MainWindow", "Plot mit 300px H\303\266he einf\303\274gen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        plotUebertragenFlach->setText(QApplication::translate("MainWindow", "Plot einf\303\274gen (flach)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        plotUebertragen->setToolTip(QApplication::translate("MainWindow", "Plot mit 600px H\303\266he einf\303\274gen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        plotUebertragen->setText(QApplication::translate("MainWindow", "Plot einf\303\274gen (hoch)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButtonBold->setToolTip(QApplication::translate("MainWindow", "Fett", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButtonBold->setText(QApplication::translate("MainWindow", "F", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButtonUnterstrichen->setToolTip(QApplication::translate("MainWindow", "Unterstrichen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButtonUnterstrichen->setText(QApplication::translate("MainWindow", "U", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButtonItalic->setToolTip(QApplication::translate("MainWindow", "Kursiv", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButtonItalic->setText(QApplication::translate("MainWindow", "K", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fontComboBox->setToolTip(QApplication::translate("MainWindow", "Schrift", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        comboSize->setToolTip(QApplication::translate("MainWindow", "Schriftgr\303\266\303\237e", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuDatei->setTitle(QApplication::translate("MainWindow", "Da&tei", 0, QApplication::UnicodeUTF8));
        Hilfe->setTitle(QApplication::translate("MainWindow", "Hi&lfe", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
