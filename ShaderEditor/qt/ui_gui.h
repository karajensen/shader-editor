/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHADEREDITORGUI_H
#define UI_SHADEREDITORGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3_2;
    QVBoxLayout *vboxLayout;
    QLabel *label_2_2_2;
    QLabel *outputWidget;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *vboxLayout1;
    QLabel *label_2;
    QSpinBox *inputSpinBox2;
    QLabel *label_3;
    QVBoxLayout *vboxLayout2;
    QLabel *label;
    QSpinBox *inputSpinBox1;

    void setupUi(QWidget *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
        Gui->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(Gui);
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(9, 9, 9, 9);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 6, 1, 1);

        label_3_2 = new QLabel(Gui);
        label_3_2->setObjectName(QStringLiteral("label_3_2"));
        label_3_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3_2, 0, 4, 1, 1);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        vboxLayout->setContentsMargins(1, 1, 1, 1);
        label_2_2_2 = new QLabel(Gui);
        label_2_2_2->setObjectName(QStringLiteral("label_2_2_2"));

        vboxLayout->addWidget(label_2_2_2);

        outputWidget = new QLabel(Gui);
        outputWidget->setObjectName(QStringLiteral("outputWidget"));
        outputWidget->setFrameShape(QFrame::Box);
        outputWidget->setFrameShadow(QFrame::Sunken);
        outputWidget->setAlignment(Qt::AlignAbsolute|Qt::AlignBaseline|Qt::AlignBottom|Qt::AlignCenter|Qt::AlignHCenter|Qt::AlignHorizontal_Mask|Qt::AlignJustify|Qt::AlignLeading|Qt::AlignLeft|Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing|Qt::AlignVCenter|Qt::AlignVertical_Mask);

        vboxLayout->addWidget(outputWidget);


        gridLayout->addLayout(vboxLayout, 0, 5, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 2, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QStringLiteral("vboxLayout1"));
        vboxLayout1->setContentsMargins(1, 1, 1, 1);
        label_2 = new QLabel(Gui);
        label_2->setObjectName(QStringLiteral("label_2"));

        vboxLayout1->addWidget(label_2);

        inputSpinBox2 = new QSpinBox(Gui);
        inputSpinBox2->setObjectName(QStringLiteral("inputSpinBox2"));

        vboxLayout1->addWidget(inputSpinBox2);


        gridLayout->addLayout(vboxLayout1, 0, 3, 1, 1);

        label_3 = new QLabel(Gui);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName(QStringLiteral("vboxLayout2"));
        vboxLayout2->setContentsMargins(1, 1, 1, 1);
        label = new QLabel(Gui);
        label->setObjectName(QStringLiteral("label"));

        vboxLayout2->addWidget(label);

        inputSpinBox1 = new QSpinBox(Gui);
        inputSpinBox1->setObjectName(QStringLiteral("inputSpinBox1"));

        vboxLayout2->addWidget(inputSpinBox1);


        gridLayout->addLayout(vboxLayout2, 0, 0, 1, 1);


        retranslateUi(Gui);
        QObject::connect(label_3_2, SIGNAL(destroyed()), Gui, SLOT(close()));

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QWidget *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "Shader Editor", 0));
        label_3_2->setText(QApplication::translate("Gui", "=", 0));
        label_2_2_2->setText(QApplication::translate("Gui", "Output", 0));
        outputWidget->setText(QApplication::translate("Gui", "0", 0));
        label_2->setText(QApplication::translate("Gui", "Input 2", 0));
        label_3->setText(QApplication::translate("Gui", "+", 0));
        label->setText(QApplication::translate("Gui", "Input 1", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHADEREDITORGUI_H
