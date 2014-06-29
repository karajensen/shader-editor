/********************************************************************************
** Form generated from reading UI file 'tweaker.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWEAKER_H
#define UI_TWEAKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tweaker
{
public:
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *sceneTab;
    QFrame *tweaker_button;
    QHBoxLayout *tweaker_lbl_2;
    QPushButton *tweaker_button_btn;
    QFrame *tweaker_combo;
    QHBoxLayout *tweaker_lbl_3;
    QLabel *tweaker_combo_lbl;
    QComboBox *tweaker_combo_box;
    QFrame *tweaker_spin;
    QHBoxLayout *tweaker_value;
    QLabel *tweaker_spin_lbl;
    QDoubleSpinBox *tweaker_spin_value;
    QDial *tweaker_spin_dial;
    QFrame *tweaker_label;
    QHBoxLayout *tweaker_lbl;
    QLabel *tweaker_label_lbl;
    QLabel *tweaker_label_val;
    QFrame *tweaker_check;
    QHBoxLayout *tweaker_tick;
    QLabel *tweaker_check_lbl;
    QCheckBox *tweaker_check_box;
    QWidget *lightTab;
    QWidget *shaderTab;

    void setupUi(QWidget *Tweaker)
    {
        if (Tweaker->objectName().isEmpty())
            Tweaker->setObjectName(QStringLiteral("Tweaker"));
        Tweaker->resize(250, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tweaker->sizePolicy().hasHeightForWidth());
        Tweaker->setSizePolicy(sizePolicy);
        Tweaker->setMinimumSize(QSize(250, 600));
        Tweaker->setMaximumSize(QSize(250, 600));
        verticalLayout_3 = new QVBoxLayout(Tweaker);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 4, 4);
        tabWidget = new QTabWidget(Tweaker);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setStyleSheet(QStringLiteral(""));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setMovable(true);
        sceneTab = new QWidget();
        sceneTab->setObjectName(QStringLiteral("sceneTab"));
        sceneTab->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tweaker_button = new QFrame(sceneTab);
        tweaker_button->setObjectName(QStringLiteral("tweaker_button"));
        tweaker_button->setGeometry(QRect(5, 130, 225, 35));
        sizePolicy.setHeightForWidth(tweaker_button->sizePolicy().hasHeightForWidth());
        tweaker_button->setSizePolicy(sizePolicy);
        tweaker_button->setMinimumSize(QSize(225, 35));
        tweaker_button->setMaximumSize(QSize(225, 35));
        tweaker_button->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 245);"));
        tweaker_button->setFrameShape(QFrame::Panel);
        tweaker_button->setFrameShadow(QFrame::Raised);
        tweaker_lbl_2 = new QHBoxLayout(tweaker_button);
        tweaker_lbl_2->setSpacing(2);
        tweaker_lbl_2->setObjectName(QStringLiteral("tweaker_lbl_2"));
        tweaker_lbl_2->setContentsMargins(2, 0, 2, 0);
        tweaker_button_btn = new QPushButton(tweaker_button);
        tweaker_button_btn->setObjectName(QStringLiteral("tweaker_button_btn"));
        tweaker_button_btn->setMinimumSize(QSize(212, 24));
        tweaker_button_btn->setMaximumSize(QSize(212, 24));
        tweaker_button_btn->setBaseSize(QSize(220, 0));
        tweaker_button_btn->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tweaker_button_btn->setAutoDefault(false);
        tweaker_button_btn->setDefault(false);
        tweaker_button_btn->setFlat(false);

        tweaker_lbl_2->addWidget(tweaker_button_btn);

        tweaker_combo = new QFrame(sceneTab);
        tweaker_combo->setObjectName(QStringLiteral("tweaker_combo"));
        tweaker_combo->setGeometry(QRect(5, 170, 225, 35));
        sizePolicy.setHeightForWidth(tweaker_combo->sizePolicy().hasHeightForWidth());
        tweaker_combo->setSizePolicy(sizePolicy);
        tweaker_combo->setMinimumSize(QSize(225, 35));
        tweaker_combo->setMaximumSize(QSize(225, 35));
        tweaker_combo->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 245);"));
        tweaker_combo->setFrameShape(QFrame::Panel);
        tweaker_combo->setFrameShadow(QFrame::Raised);
        tweaker_lbl_3 = new QHBoxLayout(tweaker_combo);
        tweaker_lbl_3->setSpacing(2);
        tweaker_lbl_3->setObjectName(QStringLiteral("tweaker_lbl_3"));
        tweaker_lbl_3->setContentsMargins(2, 0, 4, 0);
        tweaker_combo_lbl = new QLabel(tweaker_combo);
        tweaker_combo_lbl->setObjectName(QStringLiteral("tweaker_combo_lbl"));
        sizePolicy.setHeightForWidth(tweaker_combo_lbl->sizePolicy().hasHeightForWidth());
        tweaker_combo_lbl->setSizePolicy(sizePolicy);
        tweaker_combo_lbl->setMinimumSize(QSize(100, 30));
        tweaker_combo_lbl->setMaximumSize(QSize(100, 30));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        tweaker_combo_lbl->setFont(font);

        tweaker_lbl_3->addWidget(tweaker_combo_lbl);

        tweaker_combo_box = new QComboBox(tweaker_combo);
        tweaker_combo_box->setObjectName(QStringLiteral("tweaker_combo_box"));
        sizePolicy.setHeightForWidth(tweaker_combo_box->sizePolicy().hasHeightForWidth());
        tweaker_combo_box->setSizePolicy(sizePolicy);
        tweaker_combo_box->setMinimumSize(QSize(115, 22));
        tweaker_combo_box->setMaximumSize(QSize(115, 22));
        tweaker_combo_box->setBaseSize(QSize(115, 25));
        tweaker_combo_box->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));

        tweaker_lbl_3->addWidget(tweaker_combo_box);

        tweaker_spin = new QFrame(sceneTab);
        tweaker_spin->setObjectName(QStringLiteral("tweaker_spin"));
        tweaker_spin->setGeometry(QRect(5, 10, 225, 35));
        tweaker_spin->setMinimumSize(QSize(225, 35));
        tweaker_spin->setMaximumSize(QSize(225, 35));
        tweaker_spin->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 245);"));
        tweaker_spin->setFrameShape(QFrame::Panel);
        tweaker_spin->setFrameShadow(QFrame::Raised);
        tweaker_value = new QHBoxLayout(tweaker_spin);
        tweaker_value->setSpacing(2);
        tweaker_value->setObjectName(QStringLiteral("tweaker_value"));
        tweaker_value->setContentsMargins(2, 0, 2, 0);
        tweaker_spin_lbl = new QLabel(tweaker_spin);
        tweaker_spin_lbl->setObjectName(QStringLiteral("tweaker_spin_lbl"));
        sizePolicy.setHeightForWidth(tweaker_spin_lbl->sizePolicy().hasHeightForWidth());
        tweaker_spin_lbl->setSizePolicy(sizePolicy);
        tweaker_spin_lbl->setMinimumSize(QSize(100, 30));
        tweaker_spin_lbl->setMaximumSize(QSize(100, 30));
        tweaker_spin_lbl->setFont(font);

        tweaker_value->addWidget(tweaker_spin_lbl);

        tweaker_spin_value = new QDoubleSpinBox(tweaker_spin);
        tweaker_spin_value->setObjectName(QStringLiteral("tweaker_spin_value"));
        sizePolicy.setHeightForWidth(tweaker_spin_value->sizePolicy().hasHeightForWidth());
        tweaker_spin_value->setSizePolicy(sizePolicy);
        tweaker_spin_value->setMinimumSize(QSize(86, 25));
        tweaker_spin_value->setMaximumSize(QSize(86, 25));
        tweaker_spin_value->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tweaker_spin_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        tweaker_spin_value->setDecimals(3);
        tweaker_spin_value->setMinimum(-9999);
        tweaker_spin_value->setMaximum(9999);

        tweaker_value->addWidget(tweaker_spin_value);

        tweaker_spin_dial = new QDial(tweaker_spin);
        tweaker_spin_dial->setObjectName(QStringLiteral("tweaker_spin_dial"));
        sizePolicy.setHeightForWidth(tweaker_spin_dial->sizePolicy().hasHeightForWidth());
        tweaker_spin_dial->setSizePolicy(sizePolicy);
        tweaker_spin_dial->setMinimumSize(QSize(30, 30));
        tweaker_spin_dial->setMaximumSize(QSize(30, 30));

        tweaker_value->addWidget(tweaker_spin_dial);

        tweaker_label = new QFrame(sceneTab);
        tweaker_label->setObjectName(QStringLiteral("tweaker_label"));
        tweaker_label->setGeometry(QRect(5, 90, 225, 35));
        sizePolicy.setHeightForWidth(tweaker_label->sizePolicy().hasHeightForWidth());
        tweaker_label->setSizePolicy(sizePolicy);
        tweaker_label->setMinimumSize(QSize(225, 35));
        tweaker_label->setMaximumSize(QSize(225, 35));
        tweaker_label->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 245);"));
        tweaker_label->setFrameShape(QFrame::Panel);
        tweaker_label->setFrameShadow(QFrame::Raised);
        tweaker_lbl = new QHBoxLayout(tweaker_label);
        tweaker_lbl->setSpacing(2);
        tweaker_lbl->setObjectName(QStringLiteral("tweaker_lbl"));
        tweaker_lbl->setContentsMargins(2, 0, 2, 0);
        tweaker_label_lbl = new QLabel(tweaker_label);
        tweaker_label_lbl->setObjectName(QStringLiteral("tweaker_label_lbl"));
        sizePolicy.setHeightForWidth(tweaker_label_lbl->sizePolicy().hasHeightForWidth());
        tweaker_label_lbl->setSizePolicy(sizePolicy);
        tweaker_label_lbl->setMinimumSize(QSize(100, 30));
        tweaker_label_lbl->setMaximumSize(QSize(100, 30));
        tweaker_label_lbl->setFont(font);

        tweaker_lbl->addWidget(tweaker_label_lbl);

        tweaker_label_val = new QLabel(tweaker_label);
        tweaker_label_val->setObjectName(QStringLiteral("tweaker_label_val"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tweaker_label_val->sizePolicy().hasHeightForWidth());
        tweaker_label_val->setSizePolicy(sizePolicy1);
        tweaker_label_val->setMinimumSize(QSize(115, 25));
        tweaker_label_val->setMaximumSize(QSize(115, 25));
        tweaker_label_val->setAutoFillBackground(false);
        tweaker_label_val->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tweaker_label_val->setFrameShape(QFrame::Panel);
        tweaker_label_val->setFrameShadow(QFrame::Sunken);
        tweaker_label_val->setLineWidth(1);
        tweaker_label_val->setMargin(3);

        tweaker_lbl->addWidget(tweaker_label_val);

        tweaker_check = new QFrame(sceneTab);
        tweaker_check->setObjectName(QStringLiteral("tweaker_check"));
        tweaker_check->setGeometry(QRect(5, 50, 225, 35));
        sizePolicy.setHeightForWidth(tweaker_check->sizePolicy().hasHeightForWidth());
        tweaker_check->setSizePolicy(sizePolicy);
        tweaker_check->setMinimumSize(QSize(225, 35));
        tweaker_check->setMaximumSize(QSize(225, 35));
        tweaker_check->setStyleSheet(QStringLiteral("background-color: rgb(245, 245, 245);"));
        tweaker_check->setFrameShape(QFrame::Panel);
        tweaker_check->setFrameShadow(QFrame::Raised);
        tweaker_tick = new QHBoxLayout(tweaker_check);
        tweaker_tick->setSpacing(2);
        tweaker_tick->setObjectName(QStringLiteral("tweaker_tick"));
        tweaker_tick->setContentsMargins(2, 0, 6, 0);
        tweaker_check_lbl = new QLabel(tweaker_check);
        tweaker_check_lbl->setObjectName(QStringLiteral("tweaker_check_lbl"));
        sizePolicy.setHeightForWidth(tweaker_check_lbl->sizePolicy().hasHeightForWidth());
        tweaker_check_lbl->setSizePolicy(sizePolicy);
        tweaker_check_lbl->setMinimumSize(QSize(100, 30));
        tweaker_check_lbl->setMaximumSize(QSize(100, 30));
        tweaker_check_lbl->setFont(font);

        tweaker_tick->addWidget(tweaker_check_lbl);

        tweaker_check_box = new QCheckBox(tweaker_check);
        tweaker_check_box->setObjectName(QStringLiteral("tweaker_check_box"));
        sizePolicy.setHeightForWidth(tweaker_check_box->sizePolicy().hasHeightForWidth());
        tweaker_check_box->setSizePolicy(sizePolicy);
        tweaker_check_box->setMinimumSize(QSize(110, 30));
        tweaker_check_box->setMaximumSize(QSize(110, 30));
        tweaker_check_box->setLayoutDirection(Qt::RightToLeft);

        tweaker_tick->addWidget(tweaker_check_box);

        tabWidget->addTab(sceneTab, QString());
        lightTab = new QWidget();
        lightTab->setObjectName(QStringLiteral("lightTab"));
        lightTab->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tabWidget->addTab(lightTab, QString());
        shaderTab = new QWidget();
        shaderTab->setObjectName(QStringLiteral("shaderTab"));
        shaderTab->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        tabWidget->addTab(shaderTab, QString());

        verticalLayout_3->addWidget(tabWidget);


        retranslateUi(Tweaker);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Tweaker);
    } // setupUi

    void retranslateUi(QWidget *Tweaker)
    {
        Tweaker->setWindowTitle(QApplication::translate("Tweaker", "Shader Editor", 0));
        tweaker_button_btn->setText(QApplication::translate("Tweaker", "PushButton", 0));
        tweaker_combo_lbl->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        tweaker_spin_lbl->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        tweaker_label_lbl->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        tweaker_label_val->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        tweaker_check_lbl->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        tweaker_check_box->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(sceneTab), QApplication::translate("Tweaker", "Scene", 0));
        tabWidget->setTabText(tabWidget->indexOf(lightTab), QApplication::translate("Tweaker", "Light", 0));
        tabWidget->setTabText(tabWidget->indexOf(shaderTab), QApplication::translate("Tweaker", "Shader", 0));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
