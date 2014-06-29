/********************************************************************************
** Form generated from reading UI file 'editor.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITOR_H
#define UI_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Editor
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *shaderNameLbl;
    QPushButton *previousBtn;
    QPushButton *compileBtn;
    QPushButton *nextBtn;
    QSplitter *splitter;
    QTextEdit *shaderTextBox;
    QTextEdit *assemblyTextBox;

    void setupUi(QWidget *Editor)
    {
        if (Editor->objectName().isEmpty())
            Editor->setObjectName(QStringLiteral("Editor"));
        Editor->resize(350, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Editor->sizePolicy().hasHeightForWidth());
        Editor->setSizePolicy(sizePolicy);
        Editor->setMinimumSize(QSize(350, 600));
        verticalLayout = new QVBoxLayout(Editor);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(5);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, -1, -1, -1);
        shaderNameLbl = new QLabel(Editor);
        shaderNameLbl->setObjectName(QStringLiteral("shaderNameLbl"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(shaderNameLbl->sizePolicy().hasHeightForWidth());
        shaderNameLbl->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(shaderNameLbl, 0, 0, 1, 1);

        previousBtn = new QPushButton(Editor);
        previousBtn->setObjectName(QStringLiteral("previousBtn"));
        previousBtn->setMaximumSize(QSize(30, 28));

        gridLayout->addWidget(previousBtn, 0, 1, 1, 1);

        compileBtn = new QPushButton(Editor);
        compileBtn->setObjectName(QStringLiteral("compileBtn"));
        compileBtn->setMaximumSize(QSize(70, 28));
        compileBtn->setBaseSize(QSize(0, 0));

        gridLayout->addWidget(compileBtn, 0, 3, 1, 1);

        nextBtn = new QPushButton(Editor);
        nextBtn->setObjectName(QStringLiteral("nextBtn"));
        nextBtn->setMaximumSize(QSize(30, 28));

        gridLayout->addWidget(nextBtn, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        splitter = new QSplitter(Editor);
        splitter->setObjectName(QStringLiteral("splitter"));
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(5);
        shaderTextBox = new QTextEdit(splitter);
        shaderTextBox->setObjectName(QStringLiteral("shaderTextBox"));
        sizePolicy.setHeightForWidth(shaderTextBox->sizePolicy().hasHeightForWidth());
        shaderTextBox->setSizePolicy(sizePolicy);
        shaderTextBox->setLineWrapMode(QTextEdit::NoWrap);
        splitter->addWidget(shaderTextBox);
        assemblyTextBox = new QTextEdit(splitter);
        assemblyTextBox->setObjectName(QStringLiteral("assemblyTextBox"));
        sizePolicy.setHeightForWidth(assemblyTextBox->sizePolicy().hasHeightForWidth());
        assemblyTextBox->setSizePolicy(sizePolicy);
        assemblyTextBox->setLineWrapMode(QTextEdit::NoWrap);
        splitter->addWidget(assemblyTextBox);

        verticalLayout->addWidget(splitter);


        retranslateUi(Editor);

        QMetaObject::connectSlotsByName(Editor);
    } // setupUi

    void retranslateUi(QWidget *Editor)
    {
        Editor->setWindowTitle(QApplication::translate("Editor", "Shader Editor", 0));
        shaderNameLbl->setText(QApplication::translate("Editor", "Shader Name", 0));
        previousBtn->setText(QApplication::translate("Editor", "<", 0));
        compileBtn->setText(QApplication::translate("Editor", "Compile", 0));
        nextBtn->setText(QApplication::translate("Editor", ">", 0));
    } // retranslateUi

};

namespace Ui {
    class Editor: public Ui_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITOR_H
