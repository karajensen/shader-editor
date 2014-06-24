/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *menuHorizontalLayout;
    QLabel *label;
    QPushButton *resetBtn;
    QPushButton *compileBtn;
    QSplitter *splitter;
    QTextEdit *shaderTextBox;
    QTextEdit *assemblyTextBox;

    void setupUi(QWidget *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(400, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Gui->sizePolicy().hasHeightForWidth());
        Gui->setSizePolicy(sizePolicy);
        Gui->setMinimumSize(QSize(400, 600));
        horizontalLayoutWidget = new QWidget(Gui);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 381, 30));
        menuHorizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        menuHorizontalLayout->setObjectName(QStringLiteral("menuHorizontalLayout"));
        menuHorizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        menuHorizontalLayout->addWidget(label);

        resetBtn = new QPushButton(horizontalLayoutWidget);
        resetBtn->setObjectName(QStringLiteral("resetBtn"));
        resetBtn->setMaximumSize(QSize(80, 28));

        menuHorizontalLayout->addWidget(resetBtn);

        compileBtn = new QPushButton(horizontalLayoutWidget);
        compileBtn->setObjectName(QStringLiteral("compileBtn"));
        compileBtn->setMaximumSize(QSize(80, 28));
        compileBtn->setBaseSize(QSize(0, 0));

        menuHorizontalLayout->addWidget(compileBtn);

        splitter = new QSplitter(Gui);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(11, 48, 381, 541));
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        shaderTextBox = new QTextEdit(splitter);
        shaderTextBox->setObjectName(QStringLiteral("shaderTextBox"));
        sizePolicy.setHeightForWidth(shaderTextBox->sizePolicy().hasHeightForWidth());
        shaderTextBox->setSizePolicy(sizePolicy);
        splitter->addWidget(shaderTextBox);
        assemblyTextBox = new QTextEdit(splitter);
        assemblyTextBox->setObjectName(QStringLiteral("assemblyTextBox"));
        sizePolicy.setHeightForWidth(assemblyTextBox->sizePolicy().hasHeightForWidth());
        assemblyTextBox->setSizePolicy(sizePolicy);
        splitter->addWidget(assemblyTextBox);

        retranslateUi(Gui);

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QWidget *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "Shader Editor", 0));
        label->setText(QApplication::translate("Gui", "TextLabel", 0));
        resetBtn->setText(QApplication::translate("Gui", "Reset", 0));
        compileBtn->setText(QApplication::translate("Gui", "Compile", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
