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
    QTabWidget *TabMenu;
    QWidget *Scene;
    QWidget *layoutWidget;
    QVBoxLayout *sceneLayout;
    QFrame *switchEngine;
    QLabel *switchEngine_lbl;
    QComboBox *switchEngine_box;
    QFrame *fps;
    QLabel *fps_lbl;
    QLabel *fps_text;
    QFrame *deltaTime;
    QLabel *deltaTime_lbl;
    QLabel *deltaTime_text;
    QFrame *mousePosX;
    QLabel *mousePosX_lbl;
    QLabel *mousePosX_text;
    QFrame *mousePosY;
    QLabel *mousePosY_lbl;
    QLabel *mousePosY_text;
    QFrame *mouseDirX;
    QLabel *mouseDirX_lbl;
    QLabel *mouseDirX_text;
    QFrame *mouseDirY;
    QLabel *mouseDirY_lbl;
    QLabel *mouseDirY_text;
    QFrame *cameraX;
    QLabel *cameraX_lbl;
    QLabel *cameraX_text;
    QFrame *cameraY;
    QLabel *cameraY_lbl;
    QLabel *cameraY_text;
    QFrame *cameraZ;
    QLabel *cameraZ_lbl;
    QLabel *cameraZ_text;
    QWidget *Light;
    QWidget *layoutWidget1;
    QVBoxLayout *lightLayout;
    QFrame *selectedLight;
    QLabel *selectedLight_lbl;
    QComboBox *selectedLight_box;
    QFrame *positionX;
    QLabel *positionX_lbl;
    QDoubleSpinBox *positionX_value;
    QDial *positionX_dial;
    QFrame *positionY;
    QLabel *positionY_lbl;
    QDoubleSpinBox *positionY_value;
    QDial *positionY_dial;
    QFrame *positionZ;
    QLabel *positionZ_lbl;
    QDoubleSpinBox *positionZ_value;
    QDial *positionZ_dial;
    QFrame *attenuationX;
    QLabel *attenuationX_lbl;
    QDoubleSpinBox *attenuationX_value;
    QDial *attenuationX_dial;
    QFrame *attenuationY;
    QLabel *attenuationY_lbl;
    QDoubleSpinBox *attenuationY_value;
    QDial *attenuationY_dial;
    QFrame *attenuationZ;
    QLabel *attenuationZ_lbl;
    QDoubleSpinBox *attenuationZ_value;
    QDial *attenuationZ_dial;
    QFrame *diffuseRed;
    QLabel *diffuseRed_lbl;
    QDoubleSpinBox *diffuseRed_value;
    QDial *diffuseRed_dial;
    QFrame *diffuseGreen;
    QLabel *diffuseGreen_lbl;
    QDoubleSpinBox *diffuseGreen_value;
    QDial *diffuseGreen_dial;
    QFrame *diffuseBlue;
    QLabel *diffuseBlue_lbl;
    QDoubleSpinBox *diffuseBlue_value;
    QDial *diffuseBlue_dial;
    QFrame *specularRed;
    QLabel *specularRed_lbl;
    QDoubleSpinBox *specularRed_value;
    QDial *specularRed_dial;
    QFrame *specularGreen;
    QLabel *specularGreen_lbl;
    QDoubleSpinBox *specularGreen_value;
    QDial *specularGreen_dial;
    QFrame *specularBlue;
    QLabel *specularBlue_lbl;
    QDoubleSpinBox *specularBlue_value;
    QDial *specularBlue_dial;
    QFrame *lightSpecularity;
    QLabel *lightSpecularity_lbl;
    QDoubleSpinBox *lightSpecularity_value;
    QDial *lightSpecularity_dial;
    QWidget *Mesh;
    QWidget *layoutWidget2;
    QVBoxLayout *meshLayout;
    QFrame *selectedMesh;
    QLabel *selectedMesh_lbl;
    QComboBox *selectedMesh_box;
    QFrame *meshSpecularity;
    QLabel *meshSpecularity_lbl;
    QDoubleSpinBox *meshSpecularity_value;
    QDial *meshSpecularity_dial;
    QFrame *backfaceCull;
    QLabel *backfaceCull_lbl;
    QLabel *backfaceCull_text;
    QFrame *transparency;
    QLabel *transparency_lbl;
    QLabel *transparency_text;
    QFrame *diffuseMap;
    QLabel *diffuseMap_lbl;
    QLabel *diffuseMap_text;
    QFrame *specularMap;
    QLabel *specularMap_lbl;
    QLabel *specularMap_text;
    QFrame *normalMap;
    QLabel *normalMap_lbl;
    QLabel *normalMap_text;
    QFrame *shader;
    QLabel *shader_lbl;
    QLabel *shader_text;
    QFrame *showEditor;
    QPushButton *showEditor_btn;
    QWidget *postTab;
    QWidget *layoutWidget3;
    QVBoxLayout *postLayout;
    QFrame *postTexture;
    QLabel *postTexture_lbl;
    QComboBox *postTexture_box;
    QFrame *renderShadows;
    QLabel *renderShadows_lbl;
    QCheckBox *renderShadows_check;

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
        TabMenu = new QTabWidget(Tweaker);
        TabMenu->setObjectName(QStringLiteral("TabMenu"));
        TabMenu->setStyleSheet(QStringLiteral(""));
        TabMenu->setTabPosition(QTabWidget::North);
        TabMenu->setTabShape(QTabWidget::Triangular);
        TabMenu->setMovable(true);
        Scene = new QWidget();
        Scene->setObjectName(QStringLiteral("Scene"));
        Scene->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget = new QWidget(Scene);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 230, 381));
        sceneLayout = new QVBoxLayout(layoutWidget);
        sceneLayout->setSpacing(0);
        sceneLayout->setObjectName(QStringLiteral("sceneLayout"));
        sceneLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        sceneLayout->setContentsMargins(4, 4, 0, 0);
        switchEngine = new QFrame(layoutWidget);
        switchEngine->setObjectName(QStringLiteral("switchEngine"));
        sizePolicy.setHeightForWidth(switchEngine->sizePolicy().hasHeightForWidth());
        switchEngine->setSizePolicy(sizePolicy);
        switchEngine->setMinimumSize(QSize(225, 35));
        switchEngine->setMaximumSize(QSize(225, 35));
        switchEngine->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        switchEngine->setFrameShape(QFrame::Panel);
        switchEngine->setFrameShadow(QFrame::Raised);
        switchEngine_lbl = new QLabel(switchEngine);
        switchEngine_lbl->setObjectName(QStringLiteral("switchEngine_lbl"));
        switchEngine_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(switchEngine_lbl->sizePolicy().hasHeightForWidth());
        switchEngine_lbl->setSizePolicy(sizePolicy);
        switchEngine_lbl->setMinimumSize(QSize(100, 30));
        switchEngine_lbl->setMaximumSize(QSize(100, 30));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        switchEngine_lbl->setFont(font);
        switchEngine_lbl->setMargin(1);
        switchEngine_box = new QComboBox(switchEngine);
        switchEngine_box->setObjectName(QStringLiteral("switchEngine_box"));
        switchEngine_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(switchEngine_box->sizePolicy().hasHeightForWidth());
        switchEngine_box->setSizePolicy(sizePolicy);
        switchEngine_box->setMinimumSize(QSize(115, 22));
        switchEngine_box->setMaximumSize(QSize(115, 22));
        switchEngine_box->setBaseSize(QSize(115, 25));
        switchEngine_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        switchEngine_box->setFrame(false);

        sceneLayout->addWidget(switchEngine);

        fps = new QFrame(layoutWidget);
        fps->setObjectName(QStringLiteral("fps"));
        sizePolicy.setHeightForWidth(fps->sizePolicy().hasHeightForWidth());
        fps->setSizePolicy(sizePolicy);
        fps->setMinimumSize(QSize(225, 35));
        fps->setMaximumSize(QSize(225, 35));
        fps->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        fps->setFrameShape(QFrame::Panel);
        fps->setFrameShadow(QFrame::Raised);
        fps_lbl = new QLabel(fps);
        fps_lbl->setObjectName(QStringLiteral("fps_lbl"));
        fps_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(fps_lbl->sizePolicy().hasHeightForWidth());
        fps_lbl->setSizePolicy(sizePolicy);
        fps_lbl->setMinimumSize(QSize(100, 30));
        fps_lbl->setMaximumSize(QSize(100, 30));
        fps_lbl->setFont(font);
        fps_text = new QLabel(fps);
        fps_text->setObjectName(QStringLiteral("fps_text"));
        fps_text->setGeometry(QRect(106, 5, 115, 25));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fps_text->sizePolicy().hasHeightForWidth());
        fps_text->setSizePolicy(sizePolicy1);
        fps_text->setMinimumSize(QSize(115, 25));
        fps_text->setMaximumSize(QSize(115, 25));
        fps_text->setAutoFillBackground(false);
        fps_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        fps_text->setFrameShape(QFrame::NoFrame);
        fps_text->setFrameShadow(QFrame::Plain);
        fps_text->setLineWidth(0);
        fps_text->setMargin(3);

        sceneLayout->addWidget(fps);

        deltaTime = new QFrame(layoutWidget);
        deltaTime->setObjectName(QStringLiteral("deltaTime"));
        sizePolicy.setHeightForWidth(deltaTime->sizePolicy().hasHeightForWidth());
        deltaTime->setSizePolicy(sizePolicy);
        deltaTime->setMinimumSize(QSize(225, 35));
        deltaTime->setMaximumSize(QSize(225, 35));
        deltaTime->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        deltaTime->setFrameShape(QFrame::Panel);
        deltaTime->setFrameShadow(QFrame::Raised);
        deltaTime_lbl = new QLabel(deltaTime);
        deltaTime_lbl->setObjectName(QStringLiteral("deltaTime_lbl"));
        deltaTime_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(deltaTime_lbl->sizePolicy().hasHeightForWidth());
        deltaTime_lbl->setSizePolicy(sizePolicy);
        deltaTime_lbl->setMinimumSize(QSize(100, 30));
        deltaTime_lbl->setMaximumSize(QSize(100, 30));
        deltaTime_lbl->setFont(font);
        deltaTime_text = new QLabel(deltaTime);
        deltaTime_text->setObjectName(QStringLiteral("deltaTime_text"));
        deltaTime_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(deltaTime_text->sizePolicy().hasHeightForWidth());
        deltaTime_text->setSizePolicy(sizePolicy1);
        deltaTime_text->setMinimumSize(QSize(115, 25));
        deltaTime_text->setMaximumSize(QSize(115, 25));
        deltaTime_text->setAutoFillBackground(false);
        deltaTime_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        deltaTime_text->setFrameShape(QFrame::NoFrame);
        deltaTime_text->setFrameShadow(QFrame::Plain);
        deltaTime_text->setLineWidth(0);
        deltaTime_text->setMargin(3);

        sceneLayout->addWidget(deltaTime);

        mousePosX = new QFrame(layoutWidget);
        mousePosX->setObjectName(QStringLiteral("mousePosX"));
        sizePolicy.setHeightForWidth(mousePosX->sizePolicy().hasHeightForWidth());
        mousePosX->setSizePolicy(sizePolicy);
        mousePosX->setMinimumSize(QSize(225, 35));
        mousePosX->setMaximumSize(QSize(225, 35));
        mousePosX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        mousePosX->setFrameShape(QFrame::Panel);
        mousePosX->setFrameShadow(QFrame::Raised);
        mousePosX_lbl = new QLabel(mousePosX);
        mousePosX_lbl->setObjectName(QStringLiteral("mousePosX_lbl"));
        mousePosX_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(mousePosX_lbl->sizePolicy().hasHeightForWidth());
        mousePosX_lbl->setSizePolicy(sizePolicy);
        mousePosX_lbl->setMinimumSize(QSize(100, 30));
        mousePosX_lbl->setMaximumSize(QSize(100, 30));
        mousePosX_lbl->setFont(font);
        mousePosX_text = new QLabel(mousePosX);
        mousePosX_text->setObjectName(QStringLiteral("mousePosX_text"));
        mousePosX_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(mousePosX_text->sizePolicy().hasHeightForWidth());
        mousePosX_text->setSizePolicy(sizePolicy1);
        mousePosX_text->setMinimumSize(QSize(115, 25));
        mousePosX_text->setMaximumSize(QSize(115, 25));
        mousePosX_text->setAutoFillBackground(false);
        mousePosX_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        mousePosX_text->setFrameShape(QFrame::NoFrame);
        mousePosX_text->setFrameShadow(QFrame::Plain);
        mousePosX_text->setLineWidth(0);
        mousePosX_text->setMargin(3);

        sceneLayout->addWidget(mousePosX);

        mousePosY = new QFrame(layoutWidget);
        mousePosY->setObjectName(QStringLiteral("mousePosY"));
        sizePolicy.setHeightForWidth(mousePosY->sizePolicy().hasHeightForWidth());
        mousePosY->setSizePolicy(sizePolicy);
        mousePosY->setMinimumSize(QSize(225, 35));
        mousePosY->setMaximumSize(QSize(225, 35));
        mousePosY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        mousePosY->setFrameShape(QFrame::Panel);
        mousePosY->setFrameShadow(QFrame::Raised);
        mousePosY_lbl = new QLabel(mousePosY);
        mousePosY_lbl->setObjectName(QStringLiteral("mousePosY_lbl"));
        mousePosY_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(mousePosY_lbl->sizePolicy().hasHeightForWidth());
        mousePosY_lbl->setSizePolicy(sizePolicy);
        mousePosY_lbl->setMinimumSize(QSize(100, 30));
        mousePosY_lbl->setMaximumSize(QSize(100, 30));
        mousePosY_lbl->setFont(font);
        mousePosY_text = new QLabel(mousePosY);
        mousePosY_text->setObjectName(QStringLiteral("mousePosY_text"));
        mousePosY_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(mousePosY_text->sizePolicy().hasHeightForWidth());
        mousePosY_text->setSizePolicy(sizePolicy1);
        mousePosY_text->setMinimumSize(QSize(115, 25));
        mousePosY_text->setMaximumSize(QSize(115, 25));
        mousePosY_text->setAutoFillBackground(false);
        mousePosY_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        mousePosY_text->setFrameShape(QFrame::NoFrame);
        mousePosY_text->setFrameShadow(QFrame::Plain);
        mousePosY_text->setLineWidth(0);
        mousePosY_text->setMargin(3);

        sceneLayout->addWidget(mousePosY);

        mouseDirX = new QFrame(layoutWidget);
        mouseDirX->setObjectName(QStringLiteral("mouseDirX"));
        sizePolicy.setHeightForWidth(mouseDirX->sizePolicy().hasHeightForWidth());
        mouseDirX->setSizePolicy(sizePolicy);
        mouseDirX->setMinimumSize(QSize(225, 35));
        mouseDirX->setMaximumSize(QSize(225, 35));
        mouseDirX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        mouseDirX->setFrameShape(QFrame::Panel);
        mouseDirX->setFrameShadow(QFrame::Raised);
        mouseDirX_lbl = new QLabel(mouseDirX);
        mouseDirX_lbl->setObjectName(QStringLiteral("mouseDirX_lbl"));
        mouseDirX_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(mouseDirX_lbl->sizePolicy().hasHeightForWidth());
        mouseDirX_lbl->setSizePolicy(sizePolicy);
        mouseDirX_lbl->setMinimumSize(QSize(100, 30));
        mouseDirX_lbl->setMaximumSize(QSize(100, 30));
        mouseDirX_lbl->setFont(font);
        mouseDirX_text = new QLabel(mouseDirX);
        mouseDirX_text->setObjectName(QStringLiteral("mouseDirX_text"));
        mouseDirX_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(mouseDirX_text->sizePolicy().hasHeightForWidth());
        mouseDirX_text->setSizePolicy(sizePolicy1);
        mouseDirX_text->setMinimumSize(QSize(115, 25));
        mouseDirX_text->setMaximumSize(QSize(115, 25));
        mouseDirX_text->setAutoFillBackground(false);
        mouseDirX_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        mouseDirX_text->setFrameShape(QFrame::NoFrame);
        mouseDirX_text->setFrameShadow(QFrame::Plain);
        mouseDirX_text->setLineWidth(0);
        mouseDirX_text->setMargin(3);

        sceneLayout->addWidget(mouseDirX);

        mouseDirY = new QFrame(layoutWidget);
        mouseDirY->setObjectName(QStringLiteral("mouseDirY"));
        sizePolicy.setHeightForWidth(mouseDirY->sizePolicy().hasHeightForWidth());
        mouseDirY->setSizePolicy(sizePolicy);
        mouseDirY->setMinimumSize(QSize(225, 35));
        mouseDirY->setMaximumSize(QSize(225, 35));
        mouseDirY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        mouseDirY->setFrameShape(QFrame::Panel);
        mouseDirY->setFrameShadow(QFrame::Raised);
        mouseDirY_lbl = new QLabel(mouseDirY);
        mouseDirY_lbl->setObjectName(QStringLiteral("mouseDirY_lbl"));
        mouseDirY_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(mouseDirY_lbl->sizePolicy().hasHeightForWidth());
        mouseDirY_lbl->setSizePolicy(sizePolicy);
        mouseDirY_lbl->setMinimumSize(QSize(100, 30));
        mouseDirY_lbl->setMaximumSize(QSize(100, 30));
        mouseDirY_lbl->setFont(font);
        mouseDirY_text = new QLabel(mouseDirY);
        mouseDirY_text->setObjectName(QStringLiteral("mouseDirY_text"));
        mouseDirY_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(mouseDirY_text->sizePolicy().hasHeightForWidth());
        mouseDirY_text->setSizePolicy(sizePolicy1);
        mouseDirY_text->setMinimumSize(QSize(115, 25));
        mouseDirY_text->setMaximumSize(QSize(115, 25));
        mouseDirY_text->setAutoFillBackground(false);
        mouseDirY_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        mouseDirY_text->setFrameShape(QFrame::NoFrame);
        mouseDirY_text->setFrameShadow(QFrame::Plain);
        mouseDirY_text->setLineWidth(0);
        mouseDirY_text->setMargin(3);

        sceneLayout->addWidget(mouseDirY);

        cameraX = new QFrame(layoutWidget);
        cameraX->setObjectName(QStringLiteral("cameraX"));
        sizePolicy.setHeightForWidth(cameraX->sizePolicy().hasHeightForWidth());
        cameraX->setSizePolicy(sizePolicy);
        cameraX->setMinimumSize(QSize(225, 35));
        cameraX->setMaximumSize(QSize(225, 35));
        cameraX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraX->setFrameShape(QFrame::Panel);
        cameraX->setFrameShadow(QFrame::Raised);
        cameraX_lbl = new QLabel(cameraX);
        cameraX_lbl->setObjectName(QStringLiteral("cameraX_lbl"));
        cameraX_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraX_lbl->sizePolicy().hasHeightForWidth());
        cameraX_lbl->setSizePolicy(sizePolicy);
        cameraX_lbl->setMinimumSize(QSize(100, 30));
        cameraX_lbl->setMaximumSize(QSize(100, 30));
        cameraX_lbl->setFont(font);
        cameraX_text = new QLabel(cameraX);
        cameraX_text->setObjectName(QStringLiteral("cameraX_text"));
        cameraX_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(cameraX_text->sizePolicy().hasHeightForWidth());
        cameraX_text->setSizePolicy(sizePolicy1);
        cameraX_text->setMinimumSize(QSize(115, 25));
        cameraX_text->setMaximumSize(QSize(115, 25));
        cameraX_text->setAutoFillBackground(false);
        cameraX_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraX_text->setFrameShape(QFrame::NoFrame);
        cameraX_text->setFrameShadow(QFrame::Plain);
        cameraX_text->setLineWidth(0);
        cameraX_text->setMargin(3);

        sceneLayout->addWidget(cameraX);

        cameraY = new QFrame(layoutWidget);
        cameraY->setObjectName(QStringLiteral("cameraY"));
        sizePolicy.setHeightForWidth(cameraY->sizePolicy().hasHeightForWidth());
        cameraY->setSizePolicy(sizePolicy);
        cameraY->setMinimumSize(QSize(225, 35));
        cameraY->setMaximumSize(QSize(225, 35));
        cameraY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraY->setFrameShape(QFrame::Panel);
        cameraY->setFrameShadow(QFrame::Raised);
        cameraY_lbl = new QLabel(cameraY);
        cameraY_lbl->setObjectName(QStringLiteral("cameraY_lbl"));
        cameraY_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraY_lbl->sizePolicy().hasHeightForWidth());
        cameraY_lbl->setSizePolicy(sizePolicy);
        cameraY_lbl->setMinimumSize(QSize(100, 30));
        cameraY_lbl->setMaximumSize(QSize(100, 30));
        cameraY_lbl->setFont(font);
        cameraY_text = new QLabel(cameraY);
        cameraY_text->setObjectName(QStringLiteral("cameraY_text"));
        cameraY_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(cameraY_text->sizePolicy().hasHeightForWidth());
        cameraY_text->setSizePolicy(sizePolicy1);
        cameraY_text->setMinimumSize(QSize(115, 25));
        cameraY_text->setMaximumSize(QSize(115, 25));
        cameraY_text->setAutoFillBackground(false);
        cameraY_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraY_text->setFrameShape(QFrame::NoFrame);
        cameraY_text->setFrameShadow(QFrame::Plain);
        cameraY_text->setLineWidth(0);
        cameraY_text->setMargin(3);

        sceneLayout->addWidget(cameraY);

        cameraZ = new QFrame(layoutWidget);
        cameraZ->setObjectName(QStringLiteral("cameraZ"));
        sizePolicy.setHeightForWidth(cameraZ->sizePolicy().hasHeightForWidth());
        cameraZ->setSizePolicy(sizePolicy);
        cameraZ->setMinimumSize(QSize(225, 35));
        cameraZ->setMaximumSize(QSize(225, 35));
        cameraZ->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraZ->setFrameShape(QFrame::Panel);
        cameraZ->setFrameShadow(QFrame::Raised);
        cameraZ_lbl = new QLabel(cameraZ);
        cameraZ_lbl->setObjectName(QStringLiteral("cameraZ_lbl"));
        cameraZ_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraZ_lbl->sizePolicy().hasHeightForWidth());
        cameraZ_lbl->setSizePolicy(sizePolicy);
        cameraZ_lbl->setMinimumSize(QSize(100, 30));
        cameraZ_lbl->setMaximumSize(QSize(100, 30));
        cameraZ_lbl->setFont(font);
        cameraZ_text = new QLabel(cameraZ);
        cameraZ_text->setObjectName(QStringLiteral("cameraZ_text"));
        cameraZ_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(cameraZ_text->sizePolicy().hasHeightForWidth());
        cameraZ_text->setSizePolicy(sizePolicy1);
        cameraZ_text->setMinimumSize(QSize(115, 25));
        cameraZ_text->setMaximumSize(QSize(115, 25));
        cameraZ_text->setAutoFillBackground(false);
        cameraZ_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraZ_text->setFrameShape(QFrame::NoFrame);
        cameraZ_text->setFrameShadow(QFrame::Plain);
        cameraZ_text->setLineWidth(0);
        cameraZ_text->setMargin(3);

        sceneLayout->addWidget(cameraZ);

        TabMenu->addTab(Scene, QString());
        Light = new QWidget();
        Light->setObjectName(QStringLiteral("Light"));
        Light->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget1 = new QWidget(Light);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 230, 531));
        lightLayout = new QVBoxLayout(layoutWidget1);
        lightLayout->setSpacing(0);
        lightLayout->setObjectName(QStringLiteral("lightLayout"));
        lightLayout->setContentsMargins(4, 4, 0, 0);
        selectedLight = new QFrame(layoutWidget1);
        selectedLight->setObjectName(QStringLiteral("selectedLight"));
        sizePolicy.setHeightForWidth(selectedLight->sizePolicy().hasHeightForWidth());
        selectedLight->setSizePolicy(sizePolicy);
        selectedLight->setMinimumSize(QSize(225, 35));
        selectedLight->setMaximumSize(QSize(225, 35));
        selectedLight->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedLight->setFrameShape(QFrame::Panel);
        selectedLight->setFrameShadow(QFrame::Raised);
        selectedLight_lbl = new QLabel(selectedLight);
        selectedLight_lbl->setObjectName(QStringLiteral("selectedLight_lbl"));
        selectedLight_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedLight_lbl->sizePolicy().hasHeightForWidth());
        selectedLight_lbl->setSizePolicy(sizePolicy);
        selectedLight_lbl->setMinimumSize(QSize(100, 30));
        selectedLight_lbl->setMaximumSize(QSize(100, 30));
        selectedLight_lbl->setFont(font);
        selectedLight_lbl->setMargin(1);
        selectedLight_box = new QComboBox(selectedLight);
        selectedLight_box->setObjectName(QStringLiteral("selectedLight_box"));
        selectedLight_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedLight_box->sizePolicy().hasHeightForWidth());
        selectedLight_box->setSizePolicy(sizePolicy);
        selectedLight_box->setMinimumSize(QSize(115, 22));
        selectedLight_box->setMaximumSize(QSize(115, 22));
        selectedLight_box->setBaseSize(QSize(115, 25));
        selectedLight_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedLight_box->setEditable(false);
        selectedLight_box->setMinimumContentsLength(0);
        selectedLight_box->setFrame(false);

        lightLayout->addWidget(selectedLight);

        positionX = new QFrame(layoutWidget1);
        positionX->setObjectName(QStringLiteral("positionX"));
        positionX->setMinimumSize(QSize(225, 35));
        positionX->setMaximumSize(QSize(225, 35));
        positionX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        positionX->setFrameShape(QFrame::NoFrame);
        positionX->setFrameShadow(QFrame::Plain);
        positionX->setLineWidth(0);
        positionX_lbl = new QLabel(positionX);
        positionX_lbl->setObjectName(QStringLiteral("positionX_lbl"));
        positionX_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(positionX_lbl->sizePolicy().hasHeightForWidth());
        positionX_lbl->setSizePolicy(sizePolicy);
        positionX_lbl->setMinimumSize(QSize(100, 30));
        positionX_lbl->setMaximumSize(QSize(100, 30));
        positionX_lbl->setFont(font);
        positionX_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionX_lbl->setMargin(1);
        positionX_value = new QDoubleSpinBox(positionX);
        positionX_value->setObjectName(QStringLiteral("positionX_value"));
        positionX_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(positionX_value->sizePolicy().hasHeightForWidth());
        positionX_value->setSizePolicy(sizePolicy);
        positionX_value->setMinimumSize(QSize(86, 25));
        positionX_value->setMaximumSize(QSize(86, 25));
        positionX_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        positionX_value->setFrame(false);
        positionX_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        positionX_value->setDecimals(3);
        positionX_value->setMinimum(-9999);
        positionX_value->setMaximum(9999);
        positionX_value->setSingleStep(0.05);
        positionX_dial = new QDial(positionX);
        positionX_dial->setObjectName(QStringLiteral("positionX_dial"));
        positionX_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(positionX_dial->sizePolicy().hasHeightForWidth());
        positionX_dial->setSizePolicy(sizePolicy);
        positionX_dial->setMinimumSize(QSize(30, 30));
        positionX_dial->setMaximumSize(QSize(30, 30));
        positionX_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionX_dial->setMinimum(-10000);
        positionX_dial->setMaximum(10000);

        lightLayout->addWidget(positionX);

        positionY = new QFrame(layoutWidget1);
        positionY->setObjectName(QStringLiteral("positionY"));
        positionY->setMinimumSize(QSize(225, 35));
        positionY->setMaximumSize(QSize(225, 35));
        positionY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        positionY->setFrameShape(QFrame::NoFrame);
        positionY->setFrameShadow(QFrame::Plain);
        positionY->setLineWidth(0);
        positionY_lbl = new QLabel(positionY);
        positionY_lbl->setObjectName(QStringLiteral("positionY_lbl"));
        positionY_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(positionY_lbl->sizePolicy().hasHeightForWidth());
        positionY_lbl->setSizePolicy(sizePolicy);
        positionY_lbl->setMinimumSize(QSize(100, 30));
        positionY_lbl->setMaximumSize(QSize(100, 30));
        positionY_lbl->setFont(font);
        positionY_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionY_lbl->setMargin(1);
        positionY_value = new QDoubleSpinBox(positionY);
        positionY_value->setObjectName(QStringLiteral("positionY_value"));
        positionY_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(positionY_value->sizePolicy().hasHeightForWidth());
        positionY_value->setSizePolicy(sizePolicy);
        positionY_value->setMinimumSize(QSize(86, 25));
        positionY_value->setMaximumSize(QSize(86, 25));
        positionY_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        positionY_value->setFrame(false);
        positionY_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        positionY_value->setDecimals(3);
        positionY_value->setMinimum(-9999);
        positionY_value->setMaximum(9999);
        positionY_value->setSingleStep(0.05);
        positionY_dial = new QDial(positionY);
        positionY_dial->setObjectName(QStringLiteral("positionY_dial"));
        positionY_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(positionY_dial->sizePolicy().hasHeightForWidth());
        positionY_dial->setSizePolicy(sizePolicy);
        positionY_dial->setMinimumSize(QSize(30, 30));
        positionY_dial->setMaximumSize(QSize(30, 30));
        positionY_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionY_dial->setMinimum(-10000);
        positionY_dial->setMaximum(10000);

        lightLayout->addWidget(positionY);

        positionZ = new QFrame(layoutWidget1);
        positionZ->setObjectName(QStringLiteral("positionZ"));
        positionZ->setMinimumSize(QSize(225, 35));
        positionZ->setMaximumSize(QSize(225, 35));
        positionZ->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        positionZ->setFrameShape(QFrame::NoFrame);
        positionZ->setFrameShadow(QFrame::Plain);
        positionZ->setLineWidth(0);
        positionZ_lbl = new QLabel(positionZ);
        positionZ_lbl->setObjectName(QStringLiteral("positionZ_lbl"));
        positionZ_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(positionZ_lbl->sizePolicy().hasHeightForWidth());
        positionZ_lbl->setSizePolicy(sizePolicy);
        positionZ_lbl->setMinimumSize(QSize(100, 30));
        positionZ_lbl->setMaximumSize(QSize(100, 30));
        positionZ_lbl->setFont(font);
        positionZ_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionZ_lbl->setMargin(1);
        positionZ_value = new QDoubleSpinBox(positionZ);
        positionZ_value->setObjectName(QStringLiteral("positionZ_value"));
        positionZ_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(positionZ_value->sizePolicy().hasHeightForWidth());
        positionZ_value->setSizePolicy(sizePolicy);
        positionZ_value->setMinimumSize(QSize(86, 25));
        positionZ_value->setMaximumSize(QSize(86, 25));
        positionZ_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        positionZ_value->setFrame(false);
        positionZ_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        positionZ_value->setDecimals(3);
        positionZ_value->setMinimum(-9999);
        positionZ_value->setMaximum(9999);
        positionZ_value->setSingleStep(0.05);
        positionZ_dial = new QDial(positionZ);
        positionZ_dial->setObjectName(QStringLiteral("positionZ_dial"));
        positionZ_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(positionZ_dial->sizePolicy().hasHeightForWidth());
        positionZ_dial->setSizePolicy(sizePolicy);
        positionZ_dial->setMinimumSize(QSize(30, 30));
        positionZ_dial->setMaximumSize(QSize(30, 30));
        positionZ_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        positionZ_dial->setMinimum(-10000);
        positionZ_dial->setMaximum(10000);

        lightLayout->addWidget(positionZ);

        attenuationX = new QFrame(layoutWidget1);
        attenuationX->setObjectName(QStringLiteral("attenuationX"));
        attenuationX->setMinimumSize(QSize(225, 35));
        attenuationX->setMaximumSize(QSize(225, 35));
        attenuationX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        attenuationX->setFrameShape(QFrame::NoFrame);
        attenuationX->setFrameShadow(QFrame::Plain);
        attenuationX->setLineWidth(0);
        attenuationX_lbl = new QLabel(attenuationX);
        attenuationX_lbl->setObjectName(QStringLiteral("attenuationX_lbl"));
        attenuationX_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(attenuationX_lbl->sizePolicy().hasHeightForWidth());
        attenuationX_lbl->setSizePolicy(sizePolicy);
        attenuationX_lbl->setMinimumSize(QSize(100, 30));
        attenuationX_lbl->setMaximumSize(QSize(100, 30));
        attenuationX_lbl->setFont(font);
        attenuationX_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationX_lbl->setMargin(1);
        attenuationX_value = new QDoubleSpinBox(attenuationX);
        attenuationX_value->setObjectName(QStringLiteral("attenuationX_value"));
        attenuationX_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(attenuationX_value->sizePolicy().hasHeightForWidth());
        attenuationX_value->setSizePolicy(sizePolicy);
        attenuationX_value->setMinimumSize(QSize(86, 25));
        attenuationX_value->setMaximumSize(QSize(86, 25));
        attenuationX_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        attenuationX_value->setFrame(false);
        attenuationX_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        attenuationX_value->setDecimals(3);
        attenuationX_value->setMinimum(-9999);
        attenuationX_value->setMaximum(9999);
        attenuationX_value->setSingleStep(0.05);
        attenuationX_dial = new QDial(attenuationX);
        attenuationX_dial->setObjectName(QStringLiteral("attenuationX_dial"));
        attenuationX_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(attenuationX_dial->sizePolicy().hasHeightForWidth());
        attenuationX_dial->setSizePolicy(sizePolicy);
        attenuationX_dial->setMinimumSize(QSize(30, 30));
        attenuationX_dial->setMaximumSize(QSize(30, 30));
        attenuationX_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationX_dial->setMinimum(-10000);
        attenuationX_dial->setMaximum(10000);

        lightLayout->addWidget(attenuationX);

        attenuationY = new QFrame(layoutWidget1);
        attenuationY->setObjectName(QStringLiteral("attenuationY"));
        attenuationY->setMinimumSize(QSize(225, 35));
        attenuationY->setMaximumSize(QSize(225, 35));
        attenuationY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        attenuationY->setFrameShape(QFrame::NoFrame);
        attenuationY->setFrameShadow(QFrame::Plain);
        attenuationY->setLineWidth(0);
        attenuationY_lbl = new QLabel(attenuationY);
        attenuationY_lbl->setObjectName(QStringLiteral("attenuationY_lbl"));
        attenuationY_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(attenuationY_lbl->sizePolicy().hasHeightForWidth());
        attenuationY_lbl->setSizePolicy(sizePolicy);
        attenuationY_lbl->setMinimumSize(QSize(100, 30));
        attenuationY_lbl->setMaximumSize(QSize(100, 30));
        attenuationY_lbl->setFont(font);
        attenuationY_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationY_lbl->setMargin(1);
        attenuationY_value = new QDoubleSpinBox(attenuationY);
        attenuationY_value->setObjectName(QStringLiteral("attenuationY_value"));
        attenuationY_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(attenuationY_value->sizePolicy().hasHeightForWidth());
        attenuationY_value->setSizePolicy(sizePolicy);
        attenuationY_value->setMinimumSize(QSize(86, 25));
        attenuationY_value->setMaximumSize(QSize(86, 25));
        attenuationY_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        attenuationY_value->setFrame(false);
        attenuationY_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        attenuationY_value->setDecimals(3);
        attenuationY_value->setMinimum(-9999);
        attenuationY_value->setMaximum(9999);
        attenuationY_value->setSingleStep(0.05);
        attenuationY_dial = new QDial(attenuationY);
        attenuationY_dial->setObjectName(QStringLiteral("attenuationY_dial"));
        attenuationY_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(attenuationY_dial->sizePolicy().hasHeightForWidth());
        attenuationY_dial->setSizePolicy(sizePolicy);
        attenuationY_dial->setMinimumSize(QSize(30, 30));
        attenuationY_dial->setMaximumSize(QSize(30, 30));
        attenuationY_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationY_dial->setMinimum(-10000);
        attenuationY_dial->setMaximum(10000);

        lightLayout->addWidget(attenuationY);

        attenuationZ = new QFrame(layoutWidget1);
        attenuationZ->setObjectName(QStringLiteral("attenuationZ"));
        attenuationZ->setMinimumSize(QSize(225, 35));
        attenuationZ->setMaximumSize(QSize(225, 35));
        attenuationZ->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        attenuationZ->setFrameShape(QFrame::NoFrame);
        attenuationZ->setFrameShadow(QFrame::Plain);
        attenuationZ->setLineWidth(0);
        attenuationZ_lbl = new QLabel(attenuationZ);
        attenuationZ_lbl->setObjectName(QStringLiteral("attenuationZ_lbl"));
        attenuationZ_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(attenuationZ_lbl->sizePolicy().hasHeightForWidth());
        attenuationZ_lbl->setSizePolicy(sizePolicy);
        attenuationZ_lbl->setMinimumSize(QSize(100, 30));
        attenuationZ_lbl->setMaximumSize(QSize(100, 30));
        attenuationZ_lbl->setFont(font);
        attenuationZ_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationZ_lbl->setMargin(1);
        attenuationZ_value = new QDoubleSpinBox(attenuationZ);
        attenuationZ_value->setObjectName(QStringLiteral("attenuationZ_value"));
        attenuationZ_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(attenuationZ_value->sizePolicy().hasHeightForWidth());
        attenuationZ_value->setSizePolicy(sizePolicy);
        attenuationZ_value->setMinimumSize(QSize(86, 25));
        attenuationZ_value->setMaximumSize(QSize(86, 25));
        attenuationZ_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        attenuationZ_value->setFrame(false);
        attenuationZ_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        attenuationZ_value->setDecimals(3);
        attenuationZ_value->setMinimum(-9999);
        attenuationZ_value->setMaximum(9999);
        attenuationZ_value->setSingleStep(0.05);
        attenuationZ_dial = new QDial(attenuationZ);
        attenuationZ_dial->setObjectName(QStringLiteral("attenuationZ_dial"));
        attenuationZ_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(attenuationZ_dial->sizePolicy().hasHeightForWidth());
        attenuationZ_dial->setSizePolicy(sizePolicy);
        attenuationZ_dial->setMinimumSize(QSize(30, 30));
        attenuationZ_dial->setMaximumSize(QSize(30, 30));
        attenuationZ_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        attenuationZ_dial->setMinimum(-10000);
        attenuationZ_dial->setMaximum(10000);

        lightLayout->addWidget(attenuationZ);

        diffuseRed = new QFrame(layoutWidget1);
        diffuseRed->setObjectName(QStringLiteral("diffuseRed"));
        diffuseRed->setMinimumSize(QSize(225, 35));
        diffuseRed->setMaximumSize(QSize(225, 35));
        diffuseRed->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        diffuseRed->setFrameShape(QFrame::NoFrame);
        diffuseRed->setFrameShadow(QFrame::Plain);
        diffuseRed->setLineWidth(0);
        diffuseRed_lbl = new QLabel(diffuseRed);
        diffuseRed_lbl->setObjectName(QStringLiteral("diffuseRed_lbl"));
        diffuseRed_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(diffuseRed_lbl->sizePolicy().hasHeightForWidth());
        diffuseRed_lbl->setSizePolicy(sizePolicy);
        diffuseRed_lbl->setMinimumSize(QSize(100, 30));
        diffuseRed_lbl->setMaximumSize(QSize(100, 30));
        diffuseRed_lbl->setFont(font);
        diffuseRed_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseRed_lbl->setMargin(1);
        diffuseRed_value = new QDoubleSpinBox(diffuseRed);
        diffuseRed_value->setObjectName(QStringLiteral("diffuseRed_value"));
        diffuseRed_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(diffuseRed_value->sizePolicy().hasHeightForWidth());
        diffuseRed_value->setSizePolicy(sizePolicy);
        diffuseRed_value->setMinimumSize(QSize(86, 25));
        diffuseRed_value->setMaximumSize(QSize(86, 25));
        diffuseRed_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        diffuseRed_value->setFrame(false);
        diffuseRed_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        diffuseRed_value->setDecimals(3);
        diffuseRed_value->setMinimum(-9999);
        diffuseRed_value->setMaximum(9999);
        diffuseRed_value->setSingleStep(0.05);
        diffuseRed_dial = new QDial(diffuseRed);
        diffuseRed_dial->setObjectName(QStringLiteral("diffuseRed_dial"));
        diffuseRed_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(diffuseRed_dial->sizePolicy().hasHeightForWidth());
        diffuseRed_dial->setSizePolicy(sizePolicy);
        diffuseRed_dial->setMinimumSize(QSize(30, 30));
        diffuseRed_dial->setMaximumSize(QSize(30, 30));
        diffuseRed_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseRed_dial->setMinimum(-10000);
        diffuseRed_dial->setMaximum(10000);

        lightLayout->addWidget(diffuseRed);

        diffuseGreen = new QFrame(layoutWidget1);
        diffuseGreen->setObjectName(QStringLiteral("diffuseGreen"));
        diffuseGreen->setMinimumSize(QSize(225, 35));
        diffuseGreen->setMaximumSize(QSize(225, 35));
        diffuseGreen->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        diffuseGreen->setFrameShape(QFrame::NoFrame);
        diffuseGreen->setFrameShadow(QFrame::Plain);
        diffuseGreen->setLineWidth(0);
        diffuseGreen_lbl = new QLabel(diffuseGreen);
        diffuseGreen_lbl->setObjectName(QStringLiteral("diffuseGreen_lbl"));
        diffuseGreen_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(diffuseGreen_lbl->sizePolicy().hasHeightForWidth());
        diffuseGreen_lbl->setSizePolicy(sizePolicy);
        diffuseGreen_lbl->setMinimumSize(QSize(100, 30));
        diffuseGreen_lbl->setMaximumSize(QSize(100, 30));
        diffuseGreen_lbl->setFont(font);
        diffuseGreen_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseGreen_lbl->setMargin(1);
        diffuseGreen_value = new QDoubleSpinBox(diffuseGreen);
        diffuseGreen_value->setObjectName(QStringLiteral("diffuseGreen_value"));
        diffuseGreen_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(diffuseGreen_value->sizePolicy().hasHeightForWidth());
        diffuseGreen_value->setSizePolicy(sizePolicy);
        diffuseGreen_value->setMinimumSize(QSize(86, 25));
        diffuseGreen_value->setMaximumSize(QSize(86, 25));
        diffuseGreen_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        diffuseGreen_value->setFrame(false);
        diffuseGreen_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        diffuseGreen_value->setDecimals(3);
        diffuseGreen_value->setMinimum(-9999);
        diffuseGreen_value->setMaximum(9999);
        diffuseGreen_value->setSingleStep(0.05);
        diffuseGreen_dial = new QDial(diffuseGreen);
        diffuseGreen_dial->setObjectName(QStringLiteral("diffuseGreen_dial"));
        diffuseGreen_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(diffuseGreen_dial->sizePolicy().hasHeightForWidth());
        diffuseGreen_dial->setSizePolicy(sizePolicy);
        diffuseGreen_dial->setMinimumSize(QSize(30, 30));
        diffuseGreen_dial->setMaximumSize(QSize(30, 30));
        diffuseGreen_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseGreen_dial->setMinimum(-10000);
        diffuseGreen_dial->setMaximum(10000);

        lightLayout->addWidget(diffuseGreen);

        diffuseBlue = new QFrame(layoutWidget1);
        diffuseBlue->setObjectName(QStringLiteral("diffuseBlue"));
        diffuseBlue->setMinimumSize(QSize(225, 35));
        diffuseBlue->setMaximumSize(QSize(225, 35));
        diffuseBlue->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        diffuseBlue->setFrameShape(QFrame::NoFrame);
        diffuseBlue->setFrameShadow(QFrame::Plain);
        diffuseBlue->setLineWidth(0);
        diffuseBlue_lbl = new QLabel(diffuseBlue);
        diffuseBlue_lbl->setObjectName(QStringLiteral("diffuseBlue_lbl"));
        diffuseBlue_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(diffuseBlue_lbl->sizePolicy().hasHeightForWidth());
        diffuseBlue_lbl->setSizePolicy(sizePolicy);
        diffuseBlue_lbl->setMinimumSize(QSize(100, 30));
        diffuseBlue_lbl->setMaximumSize(QSize(100, 30));
        diffuseBlue_lbl->setFont(font);
        diffuseBlue_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseBlue_lbl->setMargin(1);
        diffuseBlue_value = new QDoubleSpinBox(diffuseBlue);
        diffuseBlue_value->setObjectName(QStringLiteral("diffuseBlue_value"));
        diffuseBlue_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(diffuseBlue_value->sizePolicy().hasHeightForWidth());
        diffuseBlue_value->setSizePolicy(sizePolicy);
        diffuseBlue_value->setMinimumSize(QSize(86, 25));
        diffuseBlue_value->setMaximumSize(QSize(86, 25));
        diffuseBlue_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        diffuseBlue_value->setFrame(false);
        diffuseBlue_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        diffuseBlue_value->setDecimals(3);
        diffuseBlue_value->setMinimum(-9999);
        diffuseBlue_value->setMaximum(9999);
        diffuseBlue_value->setSingleStep(0.05);
        diffuseBlue_dial = new QDial(diffuseBlue);
        diffuseBlue_dial->setObjectName(QStringLiteral("diffuseBlue_dial"));
        diffuseBlue_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(diffuseBlue_dial->sizePolicy().hasHeightForWidth());
        diffuseBlue_dial->setSizePolicy(sizePolicy);
        diffuseBlue_dial->setMinimumSize(QSize(30, 30));
        diffuseBlue_dial->setMaximumSize(QSize(30, 30));
        diffuseBlue_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        diffuseBlue_dial->setMinimum(-10000);
        diffuseBlue_dial->setMaximum(10000);

        lightLayout->addWidget(diffuseBlue);

        specularRed = new QFrame(layoutWidget1);
        specularRed->setObjectName(QStringLiteral("specularRed"));
        specularRed->setMinimumSize(QSize(225, 35));
        specularRed->setMaximumSize(QSize(225, 35));
        specularRed->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        specularRed->setFrameShape(QFrame::NoFrame);
        specularRed->setFrameShadow(QFrame::Plain);
        specularRed->setLineWidth(0);
        specularRed_lbl = new QLabel(specularRed);
        specularRed_lbl->setObjectName(QStringLiteral("specularRed_lbl"));
        specularRed_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(specularRed_lbl->sizePolicy().hasHeightForWidth());
        specularRed_lbl->setSizePolicy(sizePolicy);
        specularRed_lbl->setMinimumSize(QSize(100, 30));
        specularRed_lbl->setMaximumSize(QSize(100, 30));
        specularRed_lbl->setFont(font);
        specularRed_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularRed_lbl->setMargin(1);
        specularRed_value = new QDoubleSpinBox(specularRed);
        specularRed_value->setObjectName(QStringLiteral("specularRed_value"));
        specularRed_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(specularRed_value->sizePolicy().hasHeightForWidth());
        specularRed_value->setSizePolicy(sizePolicy);
        specularRed_value->setMinimumSize(QSize(86, 25));
        specularRed_value->setMaximumSize(QSize(86, 25));
        specularRed_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        specularRed_value->setFrame(false);
        specularRed_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        specularRed_value->setDecimals(3);
        specularRed_value->setMinimum(-9999);
        specularRed_value->setMaximum(9999);
        specularRed_value->setSingleStep(0.05);
        specularRed_dial = new QDial(specularRed);
        specularRed_dial->setObjectName(QStringLiteral("specularRed_dial"));
        specularRed_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(specularRed_dial->sizePolicy().hasHeightForWidth());
        specularRed_dial->setSizePolicy(sizePolicy);
        specularRed_dial->setMinimumSize(QSize(30, 30));
        specularRed_dial->setMaximumSize(QSize(30, 30));
        specularRed_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularRed_dial->setMinimum(-10000);
        specularRed_dial->setMaximum(10000);

        lightLayout->addWidget(specularRed);

        specularGreen = new QFrame(layoutWidget1);
        specularGreen->setObjectName(QStringLiteral("specularGreen"));
        specularGreen->setMinimumSize(QSize(225, 35));
        specularGreen->setMaximumSize(QSize(225, 35));
        specularGreen->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        specularGreen->setFrameShape(QFrame::NoFrame);
        specularGreen->setFrameShadow(QFrame::Plain);
        specularGreen->setLineWidth(0);
        specularGreen_lbl = new QLabel(specularGreen);
        specularGreen_lbl->setObjectName(QStringLiteral("specularGreen_lbl"));
        specularGreen_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(specularGreen_lbl->sizePolicy().hasHeightForWidth());
        specularGreen_lbl->setSizePolicy(sizePolicy);
        specularGreen_lbl->setMinimumSize(QSize(100, 30));
        specularGreen_lbl->setMaximumSize(QSize(100, 30));
        specularGreen_lbl->setFont(font);
        specularGreen_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularGreen_lbl->setMargin(1);
        specularGreen_value = new QDoubleSpinBox(specularGreen);
        specularGreen_value->setObjectName(QStringLiteral("specularGreen_value"));
        specularGreen_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(specularGreen_value->sizePolicy().hasHeightForWidth());
        specularGreen_value->setSizePolicy(sizePolicy);
        specularGreen_value->setMinimumSize(QSize(86, 25));
        specularGreen_value->setMaximumSize(QSize(86, 25));
        specularGreen_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        specularGreen_value->setFrame(false);
        specularGreen_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        specularGreen_value->setDecimals(3);
        specularGreen_value->setMinimum(-9999);
        specularGreen_value->setMaximum(9999);
        specularGreen_value->setSingleStep(0.05);
        specularGreen_dial = new QDial(specularGreen);
        specularGreen_dial->setObjectName(QStringLiteral("specularGreen_dial"));
        specularGreen_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(specularGreen_dial->sizePolicy().hasHeightForWidth());
        specularGreen_dial->setSizePolicy(sizePolicy);
        specularGreen_dial->setMinimumSize(QSize(30, 30));
        specularGreen_dial->setMaximumSize(QSize(30, 30));
        specularGreen_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularGreen_dial->setMinimum(-10000);
        specularGreen_dial->setMaximum(10000);
        specularGreen_dial->setOrientation(Qt::Horizontal);

        lightLayout->addWidget(specularGreen);

        specularBlue = new QFrame(layoutWidget1);
        specularBlue->setObjectName(QStringLiteral("specularBlue"));
        specularBlue->setMinimumSize(QSize(225, 35));
        specularBlue->setMaximumSize(QSize(225, 35));
        specularBlue->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        specularBlue->setFrameShape(QFrame::NoFrame);
        specularBlue->setFrameShadow(QFrame::Plain);
        specularBlue->setLineWidth(0);
        specularBlue_lbl = new QLabel(specularBlue);
        specularBlue_lbl->setObjectName(QStringLiteral("specularBlue_lbl"));
        specularBlue_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(specularBlue_lbl->sizePolicy().hasHeightForWidth());
        specularBlue_lbl->setSizePolicy(sizePolicy);
        specularBlue_lbl->setMinimumSize(QSize(100, 30));
        specularBlue_lbl->setMaximumSize(QSize(100, 30));
        specularBlue_lbl->setFont(font);
        specularBlue_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularBlue_lbl->setMargin(1);
        specularBlue_value = new QDoubleSpinBox(specularBlue);
        specularBlue_value->setObjectName(QStringLiteral("specularBlue_value"));
        specularBlue_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(specularBlue_value->sizePolicy().hasHeightForWidth());
        specularBlue_value->setSizePolicy(sizePolicy);
        specularBlue_value->setMinimumSize(QSize(86, 25));
        specularBlue_value->setMaximumSize(QSize(86, 25));
        specularBlue_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        specularBlue_value->setFrame(false);
        specularBlue_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        specularBlue_value->setDecimals(3);
        specularBlue_value->setMinimum(-9999);
        specularBlue_value->setMaximum(9999);
        specularBlue_value->setSingleStep(0.05);
        specularBlue_dial = new QDial(specularBlue);
        specularBlue_dial->setObjectName(QStringLiteral("specularBlue_dial"));
        specularBlue_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(specularBlue_dial->sizePolicy().hasHeightForWidth());
        specularBlue_dial->setSizePolicy(sizePolicy);
        specularBlue_dial->setMinimumSize(QSize(30, 30));
        specularBlue_dial->setMaximumSize(QSize(30, 30));
        specularBlue_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularBlue_dial->setMinimum(-10000);
        specularBlue_dial->setMaximum(10000);

        lightLayout->addWidget(specularBlue);

        lightSpecularity = new QFrame(layoutWidget1);
        lightSpecularity->setObjectName(QStringLiteral("lightSpecularity"));
        lightSpecularity->setMinimumSize(QSize(225, 35));
        lightSpecularity->setMaximumSize(QSize(225, 35));
        lightSpecularity->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        lightSpecularity->setFrameShape(QFrame::NoFrame);
        lightSpecularity->setFrameShadow(QFrame::Plain);
        lightSpecularity->setLineWidth(0);
        lightSpecularity_lbl = new QLabel(lightSpecularity);
        lightSpecularity_lbl->setObjectName(QStringLiteral("lightSpecularity_lbl"));
        lightSpecularity_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(lightSpecularity_lbl->sizePolicy().hasHeightForWidth());
        lightSpecularity_lbl->setSizePolicy(sizePolicy);
        lightSpecularity_lbl->setMinimumSize(QSize(100, 30));
        lightSpecularity_lbl->setMaximumSize(QSize(100, 30));
        lightSpecularity_lbl->setFont(font);
        lightSpecularity_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        lightSpecularity_lbl->setMargin(1);
        lightSpecularity_value = new QDoubleSpinBox(lightSpecularity);
        lightSpecularity_value->setObjectName(QStringLiteral("lightSpecularity_value"));
        lightSpecularity_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(lightSpecularity_value->sizePolicy().hasHeightForWidth());
        lightSpecularity_value->setSizePolicy(sizePolicy);
        lightSpecularity_value->setMinimumSize(QSize(86, 25));
        lightSpecularity_value->setMaximumSize(QSize(86, 25));
        lightSpecularity_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        lightSpecularity_value->setFrame(false);
        lightSpecularity_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        lightSpecularity_value->setDecimals(3);
        lightSpecularity_value->setMinimum(-9999);
        lightSpecularity_value->setMaximum(9999);
        lightSpecularity_value->setSingleStep(0.05);
        lightSpecularity_dial = new QDial(lightSpecularity);
        lightSpecularity_dial->setObjectName(QStringLiteral("lightSpecularity_dial"));
        lightSpecularity_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(lightSpecularity_dial->sizePolicy().hasHeightForWidth());
        lightSpecularity_dial->setSizePolicy(sizePolicy);
        lightSpecularity_dial->setMinimumSize(QSize(30, 30));
        lightSpecularity_dial->setMaximumSize(QSize(30, 30));
        lightSpecularity_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        lightSpecularity_dial->setMinimum(-10000);
        lightSpecularity_dial->setMaximum(10000);

        lightLayout->addWidget(lightSpecularity);

        TabMenu->addTab(Light, QString());
        Mesh = new QWidget();
        Mesh->setObjectName(QStringLiteral("Mesh"));
        Mesh->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget2 = new QWidget(Mesh);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 0, 230, 341));
        meshLayout = new QVBoxLayout(layoutWidget2);
        meshLayout->setSpacing(0);
        meshLayout->setObjectName(QStringLiteral("meshLayout"));
        meshLayout->setContentsMargins(4, 4, 0, 0);
        selectedMesh = new QFrame(layoutWidget2);
        selectedMesh->setObjectName(QStringLiteral("selectedMesh"));
        sizePolicy.setHeightForWidth(selectedMesh->sizePolicy().hasHeightForWidth());
        selectedMesh->setSizePolicy(sizePolicy);
        selectedMesh->setMinimumSize(QSize(225, 35));
        selectedMesh->setMaximumSize(QSize(225, 35));
        selectedMesh->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedMesh->setFrameShape(QFrame::Panel);
        selectedMesh->setFrameShadow(QFrame::Raised);
        selectedMesh_lbl = new QLabel(selectedMesh);
        selectedMesh_lbl->setObjectName(QStringLiteral("selectedMesh_lbl"));
        selectedMesh_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedMesh_lbl->sizePolicy().hasHeightForWidth());
        selectedMesh_lbl->setSizePolicy(sizePolicy);
        selectedMesh_lbl->setMinimumSize(QSize(100, 30));
        selectedMesh_lbl->setMaximumSize(QSize(100, 30));
        selectedMesh_lbl->setFont(font);
        selectedMesh_lbl->setMargin(1);
        selectedMesh_box = new QComboBox(selectedMesh);
        selectedMesh_box->setObjectName(QStringLiteral("selectedMesh_box"));
        selectedMesh_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedMesh_box->sizePolicy().hasHeightForWidth());
        selectedMesh_box->setSizePolicy(sizePolicy);
        selectedMesh_box->setMinimumSize(QSize(115, 22));
        selectedMesh_box->setMaximumSize(QSize(115, 22));
        selectedMesh_box->setBaseSize(QSize(115, 25));
        selectedMesh_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedMesh_box->setFrame(false);

        meshLayout->addWidget(selectedMesh);

        meshSpecularity = new QFrame(layoutWidget2);
        meshSpecularity->setObjectName(QStringLiteral("meshSpecularity"));
        meshSpecularity->setMinimumSize(QSize(225, 35));
        meshSpecularity->setMaximumSize(QSize(225, 35));
        meshSpecularity->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshSpecularity->setFrameShape(QFrame::NoFrame);
        meshSpecularity->setFrameShadow(QFrame::Plain);
        meshSpecularity->setLineWidth(0);
        meshSpecularity_lbl = new QLabel(meshSpecularity);
        meshSpecularity_lbl->setObjectName(QStringLiteral("meshSpecularity_lbl"));
        meshSpecularity_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(meshSpecularity_lbl->sizePolicy().hasHeightForWidth());
        meshSpecularity_lbl->setSizePolicy(sizePolicy);
        meshSpecularity_lbl->setMinimumSize(QSize(100, 30));
        meshSpecularity_lbl->setMaximumSize(QSize(100, 30));
        meshSpecularity_lbl->setFont(font);
        meshSpecularity_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshSpecularity_lbl->setMargin(1);
        meshSpecularity_value = new QDoubleSpinBox(meshSpecularity);
        meshSpecularity_value->setObjectName(QStringLiteral("meshSpecularity_value"));
        meshSpecularity_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshSpecularity_value->sizePolicy().hasHeightForWidth());
        meshSpecularity_value->setSizePolicy(sizePolicy);
        meshSpecularity_value->setMinimumSize(QSize(86, 25));
        meshSpecularity_value->setMaximumSize(QSize(86, 25));
        meshSpecularity_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshSpecularity_value->setFrame(false);
        meshSpecularity_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshSpecularity_value->setDecimals(3);
        meshSpecularity_value->setMinimum(-9999);
        meshSpecularity_value->setMaximum(9999);
        meshSpecularity_value->setSingleStep(0.05);
        meshSpecularity_dial = new QDial(meshSpecularity);
        meshSpecularity_dial->setObjectName(QStringLiteral("meshSpecularity_dial"));
        meshSpecularity_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshSpecularity_dial->sizePolicy().hasHeightForWidth());
        meshSpecularity_dial->setSizePolicy(sizePolicy);
        meshSpecularity_dial->setMinimumSize(QSize(30, 30));
        meshSpecularity_dial->setMaximumSize(QSize(30, 30));
        meshSpecularity_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshSpecularity_dial->setMinimum(-10000);
        meshSpecularity_dial->setMaximum(10000);

        meshLayout->addWidget(meshSpecularity);

        backfaceCull = new QFrame(layoutWidget2);
        backfaceCull->setObjectName(QStringLiteral("backfaceCull"));
        sizePolicy.setHeightForWidth(backfaceCull->sizePolicy().hasHeightForWidth());
        backfaceCull->setSizePolicy(sizePolicy);
        backfaceCull->setMinimumSize(QSize(225, 35));
        backfaceCull->setMaximumSize(QSize(225, 35));
        backfaceCull->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        backfaceCull->setFrameShape(QFrame::Panel);
        backfaceCull->setFrameShadow(QFrame::Raised);
        backfaceCull_lbl = new QLabel(backfaceCull);
        backfaceCull_lbl->setObjectName(QStringLiteral("backfaceCull_lbl"));
        backfaceCull_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(backfaceCull_lbl->sizePolicy().hasHeightForWidth());
        backfaceCull_lbl->setSizePolicy(sizePolicy);
        backfaceCull_lbl->setMinimumSize(QSize(100, 30));
        backfaceCull_lbl->setMaximumSize(QSize(100, 30));
        backfaceCull_lbl->setFont(font);
        backfaceCull_text = new QLabel(backfaceCull);
        backfaceCull_text->setObjectName(QStringLiteral("backfaceCull_text"));
        backfaceCull_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(backfaceCull_text->sizePolicy().hasHeightForWidth());
        backfaceCull_text->setSizePolicy(sizePolicy1);
        backfaceCull_text->setMinimumSize(QSize(115, 25));
        backfaceCull_text->setMaximumSize(QSize(115, 25));
        backfaceCull_text->setAutoFillBackground(false);
        backfaceCull_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        backfaceCull_text->setFrameShape(QFrame::NoFrame);
        backfaceCull_text->setFrameShadow(QFrame::Plain);
        backfaceCull_text->setLineWidth(0);
        backfaceCull_text->setMargin(3);

        meshLayout->addWidget(backfaceCull);

        transparency = new QFrame(layoutWidget2);
        transparency->setObjectName(QStringLiteral("transparency"));
        sizePolicy.setHeightForWidth(transparency->sizePolicy().hasHeightForWidth());
        transparency->setSizePolicy(sizePolicy);
        transparency->setMinimumSize(QSize(225, 35));
        transparency->setMaximumSize(QSize(225, 35));
        transparency->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        transparency->setFrameShape(QFrame::Panel);
        transparency->setFrameShadow(QFrame::Raised);
        transparency_lbl = new QLabel(transparency);
        transparency_lbl->setObjectName(QStringLiteral("transparency_lbl"));
        transparency_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(transparency_lbl->sizePolicy().hasHeightForWidth());
        transparency_lbl->setSizePolicy(sizePolicy);
        transparency_lbl->setMinimumSize(QSize(100, 30));
        transparency_lbl->setMaximumSize(QSize(100, 30));
        transparency_lbl->setFont(font);
        transparency_text = new QLabel(transparency);
        transparency_text->setObjectName(QStringLiteral("transparency_text"));
        transparency_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(transparency_text->sizePolicy().hasHeightForWidth());
        transparency_text->setSizePolicy(sizePolicy1);
        transparency_text->setMinimumSize(QSize(115, 25));
        transparency_text->setMaximumSize(QSize(115, 25));
        transparency_text->setAutoFillBackground(false);
        transparency_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        transparency_text->setFrameShape(QFrame::NoFrame);
        transparency_text->setFrameShadow(QFrame::Plain);
        transparency_text->setLineWidth(0);
        transparency_text->setMargin(3);

        meshLayout->addWidget(transparency);

        diffuseMap = new QFrame(layoutWidget2);
        diffuseMap->setObjectName(QStringLiteral("diffuseMap"));
        sizePolicy.setHeightForWidth(diffuseMap->sizePolicy().hasHeightForWidth());
        diffuseMap->setSizePolicy(sizePolicy);
        diffuseMap->setMinimumSize(QSize(225, 35));
        diffuseMap->setMaximumSize(QSize(225, 35));
        diffuseMap->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        diffuseMap->setFrameShape(QFrame::Panel);
        diffuseMap->setFrameShadow(QFrame::Raised);
        diffuseMap_lbl = new QLabel(diffuseMap);
        diffuseMap_lbl->setObjectName(QStringLiteral("diffuseMap_lbl"));
        diffuseMap_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(diffuseMap_lbl->sizePolicy().hasHeightForWidth());
        diffuseMap_lbl->setSizePolicy(sizePolicy);
        diffuseMap_lbl->setMinimumSize(QSize(100, 30));
        diffuseMap_lbl->setMaximumSize(QSize(100, 30));
        diffuseMap_lbl->setFont(font);
        diffuseMap_text = new QLabel(diffuseMap);
        diffuseMap_text->setObjectName(QStringLiteral("diffuseMap_text"));
        diffuseMap_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(diffuseMap_text->sizePolicy().hasHeightForWidth());
        diffuseMap_text->setSizePolicy(sizePolicy1);
        diffuseMap_text->setMinimumSize(QSize(115, 25));
        diffuseMap_text->setMaximumSize(QSize(115, 25));
        diffuseMap_text->setAutoFillBackground(false);
        diffuseMap_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        diffuseMap_text->setFrameShape(QFrame::NoFrame);
        diffuseMap_text->setFrameShadow(QFrame::Plain);
        diffuseMap_text->setLineWidth(0);
        diffuseMap_text->setMargin(3);

        meshLayout->addWidget(diffuseMap);

        specularMap = new QFrame(layoutWidget2);
        specularMap->setObjectName(QStringLiteral("specularMap"));
        sizePolicy.setHeightForWidth(specularMap->sizePolicy().hasHeightForWidth());
        specularMap->setSizePolicy(sizePolicy);
        specularMap->setMinimumSize(QSize(225, 35));
        specularMap->setMaximumSize(QSize(225, 35));
        specularMap->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        specularMap->setFrameShape(QFrame::Panel);
        specularMap->setFrameShadow(QFrame::Raised);
        specularMap_lbl = new QLabel(specularMap);
        specularMap_lbl->setObjectName(QStringLiteral("specularMap_lbl"));
        specularMap_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(specularMap_lbl->sizePolicy().hasHeightForWidth());
        specularMap_lbl->setSizePolicy(sizePolicy);
        specularMap_lbl->setMinimumSize(QSize(100, 30));
        specularMap_lbl->setMaximumSize(QSize(100, 30));
        specularMap_lbl->setFont(font);
        specularMap_text = new QLabel(specularMap);
        specularMap_text->setObjectName(QStringLiteral("specularMap_text"));
        specularMap_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(specularMap_text->sizePolicy().hasHeightForWidth());
        specularMap_text->setSizePolicy(sizePolicy1);
        specularMap_text->setMinimumSize(QSize(115, 25));
        specularMap_text->setMaximumSize(QSize(115, 25));
        specularMap_text->setAutoFillBackground(false);
        specularMap_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        specularMap_text->setFrameShape(QFrame::NoFrame);
        specularMap_text->setFrameShadow(QFrame::Plain);
        specularMap_text->setLineWidth(0);
        specularMap_text->setMargin(3);

        meshLayout->addWidget(specularMap);

        normalMap = new QFrame(layoutWidget2);
        normalMap->setObjectName(QStringLiteral("normalMap"));
        sizePolicy.setHeightForWidth(normalMap->sizePolicy().hasHeightForWidth());
        normalMap->setSizePolicy(sizePolicy);
        normalMap->setMinimumSize(QSize(225, 35));
        normalMap->setMaximumSize(QSize(225, 35));
        normalMap->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        normalMap->setFrameShape(QFrame::Panel);
        normalMap->setFrameShadow(QFrame::Raised);
        normalMap_lbl = new QLabel(normalMap);
        normalMap_lbl->setObjectName(QStringLiteral("normalMap_lbl"));
        normalMap_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(normalMap_lbl->sizePolicy().hasHeightForWidth());
        normalMap_lbl->setSizePolicy(sizePolicy);
        normalMap_lbl->setMinimumSize(QSize(100, 30));
        normalMap_lbl->setMaximumSize(QSize(100, 30));
        normalMap_lbl->setFont(font);
        normalMap_text = new QLabel(normalMap);
        normalMap_text->setObjectName(QStringLiteral("normalMap_text"));
        normalMap_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(normalMap_text->sizePolicy().hasHeightForWidth());
        normalMap_text->setSizePolicy(sizePolicy1);
        normalMap_text->setMinimumSize(QSize(115, 25));
        normalMap_text->setMaximumSize(QSize(115, 25));
        normalMap_text->setAutoFillBackground(false);
        normalMap_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        normalMap_text->setFrameShape(QFrame::NoFrame);
        normalMap_text->setFrameShadow(QFrame::Plain);
        normalMap_text->setLineWidth(0);
        normalMap_text->setMargin(3);

        meshLayout->addWidget(normalMap);

        shader = new QFrame(layoutWidget2);
        shader->setObjectName(QStringLiteral("shader"));
        sizePolicy.setHeightForWidth(shader->sizePolicy().hasHeightForWidth());
        shader->setSizePolicy(sizePolicy);
        shader->setMinimumSize(QSize(225, 35));
        shader->setMaximumSize(QSize(225, 35));
        shader->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        shader->setFrameShape(QFrame::Panel);
        shader->setFrameShadow(QFrame::Raised);
        shader_lbl = new QLabel(shader);
        shader_lbl->setObjectName(QStringLiteral("shader_lbl"));
        shader_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(shader_lbl->sizePolicy().hasHeightForWidth());
        shader_lbl->setSizePolicy(sizePolicy);
        shader_lbl->setMinimumSize(QSize(100, 30));
        shader_lbl->setMaximumSize(QSize(100, 30));
        shader_lbl->setFont(font);
        shader_text = new QLabel(shader);
        shader_text->setObjectName(QStringLiteral("shader_text"));
        shader_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(shader_text->sizePolicy().hasHeightForWidth());
        shader_text->setSizePolicy(sizePolicy1);
        shader_text->setMinimumSize(QSize(115, 25));
        shader_text->setMaximumSize(QSize(115, 25));
        shader_text->setAutoFillBackground(false);
        shader_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        shader_text->setFrameShape(QFrame::NoFrame);
        shader_text->setFrameShadow(QFrame::Plain);
        shader_text->setLineWidth(0);
        shader_text->setMargin(3);

        meshLayout->addWidget(shader);

        showEditor = new QFrame(layoutWidget2);
        showEditor->setObjectName(QStringLiteral("showEditor"));
        sizePolicy.setHeightForWidth(showEditor->sizePolicy().hasHeightForWidth());
        showEditor->setSizePolicy(sizePolicy);
        showEditor->setMinimumSize(QSize(225, 35));
        showEditor->setMaximumSize(QSize(225, 35));
        showEditor->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        showEditor->setFrameShape(QFrame::Panel);
        showEditor->setFrameShadow(QFrame::Raised);
        showEditor_btn = new QPushButton(showEditor);
        showEditor_btn->setObjectName(QStringLiteral("showEditor_btn"));
        showEditor_btn->setGeometry(QRect(6, 5, 212, 24));
        showEditor_btn->setMinimumSize(QSize(212, 24));
        showEditor_btn->setMaximumSize(QSize(212, 24));
        showEditor_btn->setBaseSize(QSize(220, 0));
        showEditor_btn->setStyleSheet(QLatin1String("QPushButton#showEditor_btn\n"
"{\n"
"background-color: rgb(230, 230, 230);\n"
"border-style: solid;\n"
"border-width: 2px;\n"
"border-bottom-color: rgb(200, 200, 200);\n"
"border-right-color: rgb(200, 200, 200);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton#showEditor_btn:pressed\n"
"{\n"
"background-color: rgb(210, 210, 210);\n"
"border-style: solid;\n"
"border-width: 2px;\n"
"border-top-color: rgb(175, 175, 175);\n"
"border-left-color: rgb(175, 175, 175);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"}"));
        showEditor_btn->setAutoDefault(false);
        showEditor_btn->setDefault(false);
        showEditor_btn->setFlat(false);

        meshLayout->addWidget(showEditor);

        TabMenu->addTab(Mesh, QString());
        postTab = new QWidget();
        postTab->setObjectName(QStringLiteral("postTab"));
        postTab->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget3 = new QWidget(postTab);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(0, 0, 230, 81));
        postLayout = new QVBoxLayout(layoutWidget3);
        postLayout->setSpacing(0);
        postLayout->setObjectName(QStringLiteral("postLayout"));
        postLayout->setContentsMargins(4, 4, 0, 0);
        postTexture = new QFrame(layoutWidget3);
        postTexture->setObjectName(QStringLiteral("postTexture"));
        sizePolicy.setHeightForWidth(postTexture->sizePolicy().hasHeightForWidth());
        postTexture->setSizePolicy(sizePolicy);
        postTexture->setMinimumSize(QSize(225, 35));
        postTexture->setMaximumSize(QSize(225, 35));
        postTexture->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        postTexture->setFrameShape(QFrame::Panel);
        postTexture->setFrameShadow(QFrame::Raised);
        postTexture_lbl = new QLabel(postTexture);
        postTexture_lbl->setObjectName(QStringLiteral("postTexture_lbl"));
        postTexture_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(postTexture_lbl->sizePolicy().hasHeightForWidth());
        postTexture_lbl->setSizePolicy(sizePolicy);
        postTexture_lbl->setMinimumSize(QSize(100, 30));
        postTexture_lbl->setMaximumSize(QSize(100, 30));
        postTexture_lbl->setFont(font);
        postTexture_lbl->setMargin(1);
        postTexture_box = new QComboBox(postTexture);
        postTexture_box->setObjectName(QStringLiteral("postTexture_box"));
        postTexture_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(postTexture_box->sizePolicy().hasHeightForWidth());
        postTexture_box->setSizePolicy(sizePolicy);
        postTexture_box->setMinimumSize(QSize(115, 22));
        postTexture_box->setMaximumSize(QSize(115, 22));
        postTexture_box->setBaseSize(QSize(115, 25));
        postTexture_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        postTexture_box->setFrame(false);

        postLayout->addWidget(postTexture);

        renderShadows = new QFrame(layoutWidget3);
        renderShadows->setObjectName(QStringLiteral("renderShadows"));
        sizePolicy.setHeightForWidth(renderShadows->sizePolicy().hasHeightForWidth());
        renderShadows->setSizePolicy(sizePolicy);
        renderShadows->setMinimumSize(QSize(225, 35));
        renderShadows->setMaximumSize(QSize(225, 35));
        renderShadows->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        renderShadows->setFrameShape(QFrame::Panel);
        renderShadows->setFrameShadow(QFrame::Raised);
        renderShadows_lbl = new QLabel(renderShadows);
        renderShadows_lbl->setObjectName(QStringLiteral("renderShadows_lbl"));
        renderShadows_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(renderShadows_lbl->sizePolicy().hasHeightForWidth());
        renderShadows_lbl->setSizePolicy(sizePolicy);
        renderShadows_lbl->setMinimumSize(QSize(100, 30));
        renderShadows_lbl->setMaximumSize(QSize(100, 30));
        renderShadows_lbl->setFont(font);
        renderShadows_check = new QCheckBox(renderShadows);
        renderShadows_check->setObjectName(QStringLiteral("renderShadows_check"));
        renderShadows_check->setGeometry(QRect(106, 2, 110, 30));
        sizePolicy.setHeightForWidth(renderShadows_check->sizePolicy().hasHeightForWidth());
        renderShadows_check->setSizePolicy(sizePolicy);
        renderShadows_check->setMinimumSize(QSize(110, 30));
        renderShadows_check->setMaximumSize(QSize(110, 30));
        renderShadows_check->setLayoutDirection(Qt::RightToLeft);
        renderShadows_check->setStyleSheet(QStringLiteral(""));

        postLayout->addWidget(renderShadows);

        TabMenu->addTab(postTab, QString());

        verticalLayout_3->addWidget(TabMenu);


        retranslateUi(Tweaker);

        TabMenu->setCurrentIndex(2);
        selectedLight_box->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(Tweaker);
    } // setupUi

    void retranslateUi(QWidget *Tweaker)
    {
        Tweaker->setWindowTitle(QApplication::translate("Tweaker", "Shader Editor", 0));
        switchEngine_lbl->setText(QApplication::translate("Tweaker", "Engine", 0));
        fps_lbl->setText(QApplication::translate("Tweaker", "Frames Per Sec", 0));
        fps_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        deltaTime_lbl->setText(QApplication::translate("Tweaker", "Delta Time", 0));
        deltaTime_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        mousePosX_lbl->setText(QApplication::translate("Tweaker", "Mouse Pos X", 0));
        mousePosX_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        mousePosY_lbl->setText(QApplication::translate("Tweaker", "Mouse Pos Y", 0));
        mousePosY_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        mouseDirX_lbl->setText(QApplication::translate("Tweaker", "Mouse Dir X", 0));
        mouseDirX_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        mouseDirY_lbl->setText(QApplication::translate("Tweaker", "Mouse Dir Y", 0));
        mouseDirY_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        cameraX_lbl->setText(QApplication::translate("Tweaker", "Camera Pos X", 0));
        cameraX_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        cameraY_lbl->setText(QApplication::translate("Tweaker", "Camera Pos Y", 0));
        cameraY_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        cameraZ_lbl->setText(QApplication::translate("Tweaker", "Camera Pos Z", 0));
        cameraZ_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        TabMenu->setTabText(TabMenu->indexOf(Scene), QApplication::translate("Tweaker", "Scene", 0));
        selectedLight_lbl->setText(QApplication::translate("Tweaker", "Selected Light", 0));
        positionX_lbl->setText(QApplication::translate("Tweaker", "Position X", 0));
        positionY_lbl->setText(QApplication::translate("Tweaker", "Position Y", 0));
        positionZ_lbl->setText(QApplication::translate("Tweaker", "Position Z", 0));
        attenuationX_lbl->setText(QApplication::translate("Tweaker", "Attenuation X", 0));
        attenuationY_lbl->setText(QApplication::translate("Tweaker", "Attenuation Y", 0));
        attenuationZ_lbl->setText(QApplication::translate("Tweaker", "Attenuation Z", 0));
        diffuseRed_lbl->setText(QApplication::translate("Tweaker", "Diffuse Red", 0));
        diffuseGreen_lbl->setText(QApplication::translate("Tweaker", "Diffuse Green", 0));
        diffuseBlue_lbl->setText(QApplication::translate("Tweaker", "Diffuse Blue", 0));
        specularRed_lbl->setText(QApplication::translate("Tweaker", "Specular Red", 0));
        specularGreen_lbl->setText(QApplication::translate("Tweaker", "Specular Green", 0));
        specularBlue_lbl->setText(QApplication::translate("Tweaker", "Specular Blue", 0));
        lightSpecularity_lbl->setText(QApplication::translate("Tweaker", "Specularity", 0));
        TabMenu->setTabText(TabMenu->indexOf(Light), QApplication::translate("Tweaker", "Light", 0));
        selectedMesh_lbl->setText(QApplication::translate("Tweaker", "Selected Mesh", 0));
        meshSpecularity_lbl->setText(QApplication::translate("Tweaker", "Specularity", 0));
        backfaceCull_lbl->setText(QApplication::translate("Tweaker", "Backface Cull", 0));
        backfaceCull_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        transparency_lbl->setText(QApplication::translate("Tweaker", "Transparency", 0));
        transparency_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        diffuseMap_lbl->setText(QApplication::translate("Tweaker", "Diffuse Map", 0));
        diffuseMap_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        specularMap_lbl->setText(QApplication::translate("Tweaker", "Specular Map", 0));
        specularMap_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        normalMap_lbl->setText(QApplication::translate("Tweaker", "Normal Map", 0));
        normalMap_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        shader_lbl->setText(QApplication::translate("Tweaker", "Shader", 0));
        shader_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        showEditor_btn->setText(QApplication::translate("Tweaker", "Show Editor", 0));
        TabMenu->setTabText(TabMenu->indexOf(Mesh), QApplication::translate("Tweaker", "Mesh", 0));
        postTexture_lbl->setText(QApplication::translate("Tweaker", "Post Texture", 0));
        renderShadows_lbl->setText(QApplication::translate("Tweaker", "Shadows", 0));
        renderShadows_check->setText(QString());
        TabMenu->setTabText(TabMenu->indexOf(postTab), QApplication::translate("Tweaker", "Post", 0));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
