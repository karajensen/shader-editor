/********************************************************************************
** Form generated from reading UI file 'tweaker.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWEAKER_H
#define UI_TWEAKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
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
    QFrame *postImage;
    QLabel *postImage_lbl;
    QComboBox *postImage_box;
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
    QSpacerItem *_sceneSpacer;
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
    QSpacerItem *_lightSpacer;
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
    QFrame *meshAmbience;
    QLabel *meshAmbience_lbl;
    QDoubleSpinBox *meshAmbience_value;
    QDial *meshAmbience_dial;
    QFrame *meshBump;
    QLabel *meshBump_lbl;
    QDoubleSpinBox *meshBump_value;
    QDial *meshBump_dial;
    QFrame *meshGlow;
    QLabel *meshGlow_lbl;
    QDoubleSpinBox *meshGlow_value;
    QDial *meshGlow_dial;
    QFrame *shader;
    QLabel *shader_lbl;
    QLabel *shader_text;
    QFrame *selectedParticles;
    QLabel *selectedParticles_lbl;
    QComboBox *selectedParticles_box;
    QFrame *particleCount;
    QLabel *particleCount_lbl;
    QLabel *particleCount_text;
    QFrame *particleSize;
    QLabel *particleSize_lbl;
    QDoubleSpinBox *particleSize_value;
    QDial *particleSize_dial;
    QFrame *particleSpeed;
    QLabel *particleSpeed_lbl;
    QDoubleSpinBox *particleSpeed_value;
    QDial *particleSpeed_dial;
    QFrame *particleColor;
    QDoubleSpinBox *particleColor_value;
    QDial *particleColor_dial;
    QComboBox *particleColor_box;
    QFrame *particleDirection;
    QDoubleSpinBox *particleDirection_value;
    QDial *particleDirection_dial;
    QComboBox *particleDirection_box;
    QSpacerItem *_meshSpacer;
    QWidget *Post;
    QWidget *layoutWidget3;
    QVBoxLayout *postLayout;
    QFrame *minRange;
    QDoubleSpinBox *minRange_value;
    QDial *minRange_dial;
    QComboBox *minRange_box;
    QFrame *maxRange;
    QDoubleSpinBox *maxRange_value;
    QDial *maxRange_dial;
    QComboBox *maxRange_box;
    QFrame *contrast;
    QLabel *contrast_lbl;
    QDoubleSpinBox *contrast_value;
    QDial *contrast_dial;
    QFrame *brightness;
    QLabel *brightness_lbl;
    QDoubleSpinBox *brightness_value;
    QDial *brightness_dial;
    QFrame *blurring;
    QLabel *blurring_lbl;
    QDoubleSpinBox *blurring_value;
    QDial *blurring_dial;
    QFrame *blurStep;
    QLabel *blurStep_lbl;
    QDoubleSpinBox *blurStep_value;
    QDial *blurStep_dial;
    QFrame *glowIntensity;
    QLabel *glowIntensity_lbl;
    QDoubleSpinBox *glowIntensity_value;
    QDial *glowIntensity_dial;
    QFrame *depthNear;
    QLabel *depthNear_lbl;
    QDoubleSpinBox *depthNear_value;
    QDial *depthNear_dial;
    QFrame *depthFar;
    QLabel *depthFar_lbl;
    QDoubleSpinBox *depthFar_value;
    QDial *depthFar_dial;
    QFrame *dofDistance;
    QLabel *dofDistance_lbl;
    QDoubleSpinBox *dofDistance_value;
    QDial *dofDistance_dial;
    QSpacerItem *_postSpacer;

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
        Tweaker->setMaximumSize(QSize(300, 600));
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
        layoutWidget->setGeometry(QRect(0, 0, 230, 561));
        sceneLayout = new QVBoxLayout(layoutWidget);
        sceneLayout->setSpacing(2);
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
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
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

        postImage = new QFrame(layoutWidget);
        postImage->setObjectName(QStringLiteral("postImage"));
        sizePolicy.setHeightForWidth(postImage->sizePolicy().hasHeightForWidth());
        postImage->setSizePolicy(sizePolicy);
        postImage->setMinimumSize(QSize(225, 35));
        postImage->setMaximumSize(QSize(225, 35));
        postImage->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        postImage->setFrameShape(QFrame::Panel);
        postImage->setFrameShadow(QFrame::Raised);
        postImage_lbl = new QLabel(postImage);
        postImage_lbl->setObjectName(QStringLiteral("postImage_lbl"));
        postImage_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(postImage_lbl->sizePolicy().hasHeightForWidth());
        postImage_lbl->setSizePolicy(sizePolicy);
        postImage_lbl->setMinimumSize(QSize(100, 30));
        postImage_lbl->setMaximumSize(QSize(100, 30));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        postImage_lbl->setFont(font1);
        postImage_lbl->setMargin(1);
        postImage_box = new QComboBox(postImage);
        postImage_box->setObjectName(QStringLiteral("postImage_box"));
        postImage_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(postImage_box->sizePolicy().hasHeightForWidth());
        postImage_box->setSizePolicy(sizePolicy);
        postImage_box->setMinimumSize(QSize(115, 22));
        postImage_box->setMaximumSize(QSize(115, 22));
        postImage_box->setBaseSize(QSize(115, 25));
        postImage_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        postImage_box->setFrame(false);

        sceneLayout->addWidget(postImage);

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
        fps_lbl->setFont(font1);
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
        deltaTime_lbl->setFont(font1);
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
        mousePosX_lbl->setFont(font1);
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
        mousePosY_lbl->setFont(font1);
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
        mouseDirX_lbl->setFont(font1);
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
        mouseDirY_lbl->setFont(font1);
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

        _sceneSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sceneLayout->addItem(_sceneSpacer);

        TabMenu->addTab(Scene, QString());
        Light = new QWidget();
        Light->setObjectName(QStringLiteral("Light"));
        Light->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget1 = new QWidget(Light);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 230, 561));
        lightLayout = new QVBoxLayout(layoutWidget1);
        lightLayout->setSpacing(2);
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
        positionX_lbl->setFont(font1);
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
        positionY_lbl->setFont(font1);
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
        positionZ_lbl->setFont(font1);
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
        attenuationX_lbl->setFont(font1);
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
        attenuationY_lbl->setFont(font1);
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
        attenuationZ_lbl->setFont(font1);
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
        diffuseRed_lbl->setFont(font1);
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
        diffuseGreen_lbl->setFont(font1);
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
        diffuseBlue_lbl->setFont(font1);
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
        specularRed_lbl->setFont(font1);
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
        specularGreen_lbl->setFont(font1);
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
        specularBlue_lbl->setFont(font1);
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
        lightSpecularity_lbl->setFont(font1);
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

        _lightSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        lightLayout->addItem(_lightSpacer);

        TabMenu->addTab(Light, QString());
        Mesh = new QWidget();
        Mesh->setObjectName(QStringLiteral("Mesh"));
        Mesh->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget2 = new QWidget(Mesh);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 0, 230, 561));
        meshLayout = new QVBoxLayout(layoutWidget2);
        meshLayout->setSpacing(2);
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
        meshSpecularity_lbl->setFont(font1);
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

        meshAmbience = new QFrame(layoutWidget2);
        meshAmbience->setObjectName(QStringLiteral("meshAmbience"));
        meshAmbience->setMinimumSize(QSize(225, 35));
        meshAmbience->setMaximumSize(QSize(225, 35));
        meshAmbience->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshAmbience->setFrameShape(QFrame::NoFrame);
        meshAmbience->setFrameShadow(QFrame::Plain);
        meshAmbience->setLineWidth(0);
        meshAmbience_lbl = new QLabel(meshAmbience);
        meshAmbience_lbl->setObjectName(QStringLiteral("meshAmbience_lbl"));
        meshAmbience_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(meshAmbience_lbl->sizePolicy().hasHeightForWidth());
        meshAmbience_lbl->setSizePolicy(sizePolicy);
        meshAmbience_lbl->setMinimumSize(QSize(100, 30));
        meshAmbience_lbl->setMaximumSize(QSize(100, 30));
        meshAmbience_lbl->setFont(font1);
        meshAmbience_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshAmbience_lbl->setMargin(1);
        meshAmbience_value = new QDoubleSpinBox(meshAmbience);
        meshAmbience_value->setObjectName(QStringLiteral("meshAmbience_value"));
        meshAmbience_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshAmbience_value->sizePolicy().hasHeightForWidth());
        meshAmbience_value->setSizePolicy(sizePolicy);
        meshAmbience_value->setMinimumSize(QSize(86, 25));
        meshAmbience_value->setMaximumSize(QSize(86, 25));
        meshAmbience_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshAmbience_value->setFrame(false);
        meshAmbience_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshAmbience_value->setDecimals(3);
        meshAmbience_value->setMinimum(-9999);
        meshAmbience_value->setMaximum(9999);
        meshAmbience_value->setSingleStep(0.05);
        meshAmbience_dial = new QDial(meshAmbience);
        meshAmbience_dial->setObjectName(QStringLiteral("meshAmbience_dial"));
        meshAmbience_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshAmbience_dial->sizePolicy().hasHeightForWidth());
        meshAmbience_dial->setSizePolicy(sizePolicy);
        meshAmbience_dial->setMinimumSize(QSize(30, 30));
        meshAmbience_dial->setMaximumSize(QSize(30, 30));
        meshAmbience_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshAmbience_dial->setMinimum(-10000);
        meshAmbience_dial->setMaximum(10000);

        meshLayout->addWidget(meshAmbience);

        meshBump = new QFrame(layoutWidget2);
        meshBump->setObjectName(QStringLiteral("meshBump"));
        meshBump->setMinimumSize(QSize(225, 35));
        meshBump->setMaximumSize(QSize(225, 35));
        meshBump->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshBump->setFrameShape(QFrame::NoFrame);
        meshBump->setFrameShadow(QFrame::Plain);
        meshBump->setLineWidth(0);
        meshBump_lbl = new QLabel(meshBump);
        meshBump_lbl->setObjectName(QStringLiteral("meshBump_lbl"));
        meshBump_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(meshBump_lbl->sizePolicy().hasHeightForWidth());
        meshBump_lbl->setSizePolicy(sizePolicy);
        meshBump_lbl->setMinimumSize(QSize(100, 30));
        meshBump_lbl->setMaximumSize(QSize(100, 30));
        meshBump_lbl->setFont(font1);
        meshBump_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshBump_lbl->setMargin(1);
        meshBump_value = new QDoubleSpinBox(meshBump);
        meshBump_value->setObjectName(QStringLiteral("meshBump_value"));
        meshBump_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshBump_value->sizePolicy().hasHeightForWidth());
        meshBump_value->setSizePolicy(sizePolicy);
        meshBump_value->setMinimumSize(QSize(86, 25));
        meshBump_value->setMaximumSize(QSize(86, 25));
        meshBump_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshBump_value->setFrame(false);
        meshBump_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshBump_value->setDecimals(3);
        meshBump_value->setMinimum(-9999);
        meshBump_value->setMaximum(9999);
        meshBump_value->setSingleStep(0.05);
        meshBump_dial = new QDial(meshBump);
        meshBump_dial->setObjectName(QStringLiteral("meshBump_dial"));
        meshBump_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshBump_dial->sizePolicy().hasHeightForWidth());
        meshBump_dial->setSizePolicy(sizePolicy);
        meshBump_dial->setMinimumSize(QSize(30, 30));
        meshBump_dial->setMaximumSize(QSize(30, 30));
        meshBump_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshBump_dial->setMinimum(-10000);
        meshBump_dial->setMaximum(10000);

        meshLayout->addWidget(meshBump);

        meshGlow = new QFrame(layoutWidget2);
        meshGlow->setObjectName(QStringLiteral("meshGlow"));
        meshGlow->setMinimumSize(QSize(225, 35));
        meshGlow->setMaximumSize(QSize(225, 35));
        meshGlow->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshGlow->setFrameShape(QFrame::NoFrame);
        meshGlow->setFrameShadow(QFrame::Plain);
        meshGlow->setLineWidth(0);
        meshGlow_lbl = new QLabel(meshGlow);
        meshGlow_lbl->setObjectName(QStringLiteral("meshGlow_lbl"));
        meshGlow_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(meshGlow_lbl->sizePolicy().hasHeightForWidth());
        meshGlow_lbl->setSizePolicy(sizePolicy);
        meshGlow_lbl->setMinimumSize(QSize(100, 30));
        meshGlow_lbl->setMaximumSize(QSize(100, 30));
        meshGlow_lbl->setFont(font1);
        meshGlow_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshGlow_lbl->setMargin(1);
        meshGlow_value = new QDoubleSpinBox(meshGlow);
        meshGlow_value->setObjectName(QStringLiteral("meshGlow_value"));
        meshGlow_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshGlow_value->sizePolicy().hasHeightForWidth());
        meshGlow_value->setSizePolicy(sizePolicy);
        meshGlow_value->setMinimumSize(QSize(86, 25));
        meshGlow_value->setMaximumSize(QSize(86, 25));
        meshGlow_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshGlow_value->setFrame(false);
        meshGlow_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshGlow_value->setDecimals(3);
        meshGlow_value->setMinimum(-9999);
        meshGlow_value->setMaximum(9999);
        meshGlow_value->setSingleStep(0.05);
        meshGlow_dial = new QDial(meshGlow);
        meshGlow_dial->setObjectName(QStringLiteral("meshGlow_dial"));
        meshGlow_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshGlow_dial->sizePolicy().hasHeightForWidth());
        meshGlow_dial->setSizePolicy(sizePolicy);
        meshGlow_dial->setMinimumSize(QSize(30, 30));
        meshGlow_dial->setMaximumSize(QSize(30, 30));
        meshGlow_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshGlow_dial->setMinimum(-10000);
        meshGlow_dial->setMaximum(10000);

        meshLayout->addWidget(meshGlow);

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
        shader_lbl->setFont(font1);
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

        selectedParticles = new QFrame(layoutWidget2);
        selectedParticles->setObjectName(QStringLiteral("selectedParticles"));
        sizePolicy.setHeightForWidth(selectedParticles->sizePolicy().hasHeightForWidth());
        selectedParticles->setSizePolicy(sizePolicy);
        selectedParticles->setMinimumSize(QSize(225, 35));
        selectedParticles->setMaximumSize(QSize(225, 35));
        selectedParticles->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedParticles->setFrameShape(QFrame::Panel);
        selectedParticles->setFrameShadow(QFrame::Raised);
        selectedParticles_lbl = new QLabel(selectedParticles);
        selectedParticles_lbl->setObjectName(QStringLiteral("selectedParticles_lbl"));
        selectedParticles_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedParticles_lbl->sizePolicy().hasHeightForWidth());
        selectedParticles_lbl->setSizePolicy(sizePolicy);
        selectedParticles_lbl->setMinimumSize(QSize(100, 30));
        selectedParticles_lbl->setMaximumSize(QSize(100, 30));
        selectedParticles_lbl->setFont(font);
        selectedParticles_lbl->setMargin(1);
        selectedParticles_box = new QComboBox(selectedParticles);
        selectedParticles_box->setObjectName(QStringLiteral("selectedParticles_box"));
        selectedParticles_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedParticles_box->sizePolicy().hasHeightForWidth());
        selectedParticles_box->setSizePolicy(sizePolicy);
        selectedParticles_box->setMinimumSize(QSize(115, 22));
        selectedParticles_box->setMaximumSize(QSize(115, 22));
        selectedParticles_box->setBaseSize(QSize(115, 25));
        selectedParticles_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedParticles_box->setFrame(false);

        meshLayout->addWidget(selectedParticles);

        particleCount = new QFrame(layoutWidget2);
        particleCount->setObjectName(QStringLiteral("particleCount"));
        sizePolicy.setHeightForWidth(particleCount->sizePolicy().hasHeightForWidth());
        particleCount->setSizePolicy(sizePolicy);
        particleCount->setMinimumSize(QSize(225, 35));
        particleCount->setMaximumSize(QSize(225, 35));
        particleCount->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleCount->setFrameShape(QFrame::Panel);
        particleCount->setFrameShadow(QFrame::Raised);
        particleCount_lbl = new QLabel(particleCount);
        particleCount_lbl->setObjectName(QStringLiteral("particleCount_lbl"));
        particleCount_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(particleCount_lbl->sizePolicy().hasHeightForWidth());
        particleCount_lbl->setSizePolicy(sizePolicy);
        particleCount_lbl->setMinimumSize(QSize(100, 30));
        particleCount_lbl->setMaximumSize(QSize(100, 30));
        particleCount_lbl->setFont(font1);
        particleCount_text = new QLabel(particleCount);
        particleCount_text->setObjectName(QStringLiteral("particleCount_text"));
        particleCount_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(particleCount_text->sizePolicy().hasHeightForWidth());
        particleCount_text->setSizePolicy(sizePolicy1);
        particleCount_text->setMinimumSize(QSize(115, 25));
        particleCount_text->setMaximumSize(QSize(115, 25));
        particleCount_text->setAutoFillBackground(false);
        particleCount_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        particleCount_text->setFrameShape(QFrame::NoFrame);
        particleCount_text->setFrameShadow(QFrame::Plain);
        particleCount_text->setLineWidth(0);
        particleCount_text->setMargin(3);

        meshLayout->addWidget(particleCount);

        particleSize = new QFrame(layoutWidget2);
        particleSize->setObjectName(QStringLiteral("particleSize"));
        particleSize->setMinimumSize(QSize(225, 35));
        particleSize->setMaximumSize(QSize(225, 35));
        particleSize->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleSize->setFrameShape(QFrame::NoFrame);
        particleSize->setFrameShadow(QFrame::Plain);
        particleSize->setLineWidth(0);
        particleSize_lbl = new QLabel(particleSize);
        particleSize_lbl->setObjectName(QStringLiteral("particleSize_lbl"));
        particleSize_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(particleSize_lbl->sizePolicy().hasHeightForWidth());
        particleSize_lbl->setSizePolicy(sizePolicy);
        particleSize_lbl->setMinimumSize(QSize(100, 30));
        particleSize_lbl->setMaximumSize(QSize(100, 30));
        particleSize_lbl->setFont(font1);
        particleSize_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleSize_lbl->setMargin(1);
        particleSize_value = new QDoubleSpinBox(particleSize);
        particleSize_value->setObjectName(QStringLiteral("particleSize_value"));
        particleSize_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(particleSize_value->sizePolicy().hasHeightForWidth());
        particleSize_value->setSizePolicy(sizePolicy);
        particleSize_value->setMinimumSize(QSize(86, 25));
        particleSize_value->setMaximumSize(QSize(86, 25));
        particleSize_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleSize_value->setFrame(false);
        particleSize_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        particleSize_value->setDecimals(3);
        particleSize_value->setMinimum(-9999);
        particleSize_value->setMaximum(9999);
        particleSize_value->setSingleStep(0.05);
        particleSize_dial = new QDial(particleSize);
        particleSize_dial->setObjectName(QStringLiteral("particleSize_dial"));
        particleSize_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(particleSize_dial->sizePolicy().hasHeightForWidth());
        particleSize_dial->setSizePolicy(sizePolicy);
        particleSize_dial->setMinimumSize(QSize(30, 30));
        particleSize_dial->setMaximumSize(QSize(30, 30));
        particleSize_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleSize_dial->setMinimum(-10000);
        particleSize_dial->setMaximum(10000);

        meshLayout->addWidget(particleSize);

        particleSpeed = new QFrame(layoutWidget2);
        particleSpeed->setObjectName(QStringLiteral("particleSpeed"));
        particleSpeed->setMinimumSize(QSize(225, 35));
        particleSpeed->setMaximumSize(QSize(225, 35));
        particleSpeed->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleSpeed->setFrameShape(QFrame::NoFrame);
        particleSpeed->setFrameShadow(QFrame::Plain);
        particleSpeed->setLineWidth(0);
        particleSpeed_lbl = new QLabel(particleSpeed);
        particleSpeed_lbl->setObjectName(QStringLiteral("particleSpeed_lbl"));
        particleSpeed_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(particleSpeed_lbl->sizePolicy().hasHeightForWidth());
        particleSpeed_lbl->setSizePolicy(sizePolicy);
        particleSpeed_lbl->setMinimumSize(QSize(100, 30));
        particleSpeed_lbl->setMaximumSize(QSize(100, 30));
        particleSpeed_lbl->setFont(font1);
        particleSpeed_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleSpeed_lbl->setMargin(1);
        particleSpeed_value = new QDoubleSpinBox(particleSpeed);
        particleSpeed_value->setObjectName(QStringLiteral("particleSpeed_value"));
        particleSpeed_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(particleSpeed_value->sizePolicy().hasHeightForWidth());
        particleSpeed_value->setSizePolicy(sizePolicy);
        particleSpeed_value->setMinimumSize(QSize(86, 25));
        particleSpeed_value->setMaximumSize(QSize(86, 25));
        particleSpeed_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleSpeed_value->setFrame(false);
        particleSpeed_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        particleSpeed_value->setDecimals(3);
        particleSpeed_value->setMinimum(-9999);
        particleSpeed_value->setMaximum(9999);
        particleSpeed_value->setSingleStep(0.05);
        particleSpeed_dial = new QDial(particleSpeed);
        particleSpeed_dial->setObjectName(QStringLiteral("particleSpeed_dial"));
        particleSpeed_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(particleSpeed_dial->sizePolicy().hasHeightForWidth());
        particleSpeed_dial->setSizePolicy(sizePolicy);
        particleSpeed_dial->setMinimumSize(QSize(30, 30));
        particleSpeed_dial->setMaximumSize(QSize(30, 30));
        particleSpeed_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleSpeed_dial->setMinimum(-10000);
        particleSpeed_dial->setMaximum(10000);

        meshLayout->addWidget(particleSpeed);

        particleColor = new QFrame(layoutWidget2);
        particleColor->setObjectName(QStringLiteral("particleColor"));
        particleColor->setMinimumSize(QSize(225, 35));
        particleColor->setMaximumSize(QSize(225, 35));
        particleColor->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleColor->setFrameShape(QFrame::NoFrame);
        particleColor->setFrameShadow(QFrame::Plain);
        particleColor->setLineWidth(0);
        particleColor_value = new QDoubleSpinBox(particleColor);
        particleColor_value->setObjectName(QStringLiteral("particleColor_value"));
        particleColor_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(particleColor_value->sizePolicy().hasHeightForWidth());
        particleColor_value->setSizePolicy(sizePolicy);
        particleColor_value->setMinimumSize(QSize(86, 25));
        particleColor_value->setMaximumSize(QSize(86, 25));
        particleColor_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleColor_value->setFrame(false);
        particleColor_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        particleColor_value->setDecimals(3);
        particleColor_value->setMinimum(-9999);
        particleColor_value->setMaximum(9999);
        particleColor_value->setSingleStep(0.05);
        particleColor_dial = new QDial(particleColor);
        particleColor_dial->setObjectName(QStringLiteral("particleColor_dial"));
        particleColor_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(particleColor_dial->sizePolicy().hasHeightForWidth());
        particleColor_dial->setSizePolicy(sizePolicy);
        particleColor_dial->setMinimumSize(QSize(30, 30));
        particleColor_dial->setMaximumSize(QSize(30, 30));
        particleColor_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleColor_dial->setMinimum(-10000);
        particleColor_dial->setMaximum(10000);
        particleColor_box = new QComboBox(particleColor);
        particleColor_box->setObjectName(QStringLiteral("particleColor_box"));
        particleColor_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(particleColor_box->sizePolicy().hasHeightForWidth());
        particleColor_box->setSizePolicy(sizePolicy);
        particleColor_box->setMinimumSize(QSize(95, 22));
        particleColor_box->setMaximumSize(QSize(90, 22));
        particleColor_box->setBaseSize(QSize(90, 25));
        particleColor_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleColor_box->setFrame(false);

        meshLayout->addWidget(particleColor);

        particleDirection = new QFrame(layoutWidget2);
        particleDirection->setObjectName(QStringLiteral("particleDirection"));
        particleDirection->setMinimumSize(QSize(225, 35));
        particleDirection->setMaximumSize(QSize(225, 35));
        particleDirection->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleDirection->setFrameShape(QFrame::NoFrame);
        particleDirection->setFrameShadow(QFrame::Plain);
        particleDirection->setLineWidth(0);
        particleDirection_value = new QDoubleSpinBox(particleDirection);
        particleDirection_value->setObjectName(QStringLiteral("particleDirection_value"));
        particleDirection_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(particleDirection_value->sizePolicy().hasHeightForWidth());
        particleDirection_value->setSizePolicy(sizePolicy);
        particleDirection_value->setMinimumSize(QSize(86, 25));
        particleDirection_value->setMaximumSize(QSize(86, 25));
        particleDirection_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleDirection_value->setFrame(false);
        particleDirection_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        particleDirection_value->setDecimals(3);
        particleDirection_value->setMinimum(-9999);
        particleDirection_value->setMaximum(9999);
        particleDirection_value->setSingleStep(0.05);
        particleDirection_dial = new QDial(particleDirection);
        particleDirection_dial->setObjectName(QStringLiteral("particleDirection_dial"));
        particleDirection_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(particleDirection_dial->sizePolicy().hasHeightForWidth());
        particleDirection_dial->setSizePolicy(sizePolicy);
        particleDirection_dial->setMinimumSize(QSize(30, 30));
        particleDirection_dial->setMaximumSize(QSize(30, 30));
        particleDirection_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleDirection_dial->setMinimum(-10000);
        particleDirection_dial->setMaximum(10000);
        particleDirection_box = new QComboBox(particleDirection);
        particleDirection_box->setObjectName(QStringLiteral("particleDirection_box"));
        particleDirection_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(particleDirection_box->sizePolicy().hasHeightForWidth());
        particleDirection_box->setSizePolicy(sizePolicy);
        particleDirection_box->setMinimumSize(QSize(95, 22));
        particleDirection_box->setMaximumSize(QSize(90, 22));
        particleDirection_box->setBaseSize(QSize(90, 25));
        particleDirection_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleDirection_box->setFrame(false);

        meshLayout->addWidget(particleDirection);

        _meshSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        meshLayout->addItem(_meshSpacer);

        TabMenu->addTab(Mesh, QString());
        Post = new QWidget();
        Post->setObjectName(QStringLiteral("Post"));
        Post->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget3 = new QWidget(Post);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(0, 0, 230, 561));
        postLayout = new QVBoxLayout(layoutWidget3);
        postLayout->setSpacing(2);
        postLayout->setObjectName(QStringLiteral("postLayout"));
        postLayout->setContentsMargins(4, 4, 0, 0);
        minRange = new QFrame(layoutWidget3);
        minRange->setObjectName(QStringLiteral("minRange"));
        minRange->setMinimumSize(QSize(225, 35));
        minRange->setMaximumSize(QSize(225, 35));
        minRange->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        minRange->setFrameShape(QFrame::NoFrame);
        minRange->setFrameShadow(QFrame::Plain);
        minRange->setLineWidth(0);
        minRange_value = new QDoubleSpinBox(minRange);
        minRange_value->setObjectName(QStringLiteral("minRange_value"));
        minRange_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(minRange_value->sizePolicy().hasHeightForWidth());
        minRange_value->setSizePolicy(sizePolicy);
        minRange_value->setMinimumSize(QSize(86, 25));
        minRange_value->setMaximumSize(QSize(86, 25));
        minRange_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        minRange_value->setFrame(false);
        minRange_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        minRange_value->setDecimals(3);
        minRange_value->setMinimum(-9999);
        minRange_value->setMaximum(9999);
        minRange_value->setSingleStep(0.05);
        minRange_dial = new QDial(minRange);
        minRange_dial->setObjectName(QStringLiteral("minRange_dial"));
        minRange_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(minRange_dial->sizePolicy().hasHeightForWidth());
        minRange_dial->setSizePolicy(sizePolicy);
        minRange_dial->setMinimumSize(QSize(30, 30));
        minRange_dial->setMaximumSize(QSize(30, 30));
        minRange_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        minRange_dial->setMinimum(-10000);
        minRange_dial->setMaximum(10000);
        minRange_box = new QComboBox(minRange);
        minRange_box->setObjectName(QStringLiteral("minRange_box"));
        minRange_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(minRange_box->sizePolicy().hasHeightForWidth());
        minRange_box->setSizePolicy(sizePolicy);
        minRange_box->setMinimumSize(QSize(95, 22));
        minRange_box->setMaximumSize(QSize(90, 22));
        minRange_box->setBaseSize(QSize(90, 25));
        minRange_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        minRange_box->setFrame(false);

        postLayout->addWidget(minRange);

        maxRange = new QFrame(layoutWidget3);
        maxRange->setObjectName(QStringLiteral("maxRange"));
        maxRange->setMinimumSize(QSize(225, 35));
        maxRange->setMaximumSize(QSize(225, 35));
        maxRange->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        maxRange->setFrameShape(QFrame::NoFrame);
        maxRange->setFrameShadow(QFrame::Plain);
        maxRange->setLineWidth(0);
        maxRange_value = new QDoubleSpinBox(maxRange);
        maxRange_value->setObjectName(QStringLiteral("maxRange_value"));
        maxRange_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(maxRange_value->sizePolicy().hasHeightForWidth());
        maxRange_value->setSizePolicy(sizePolicy);
        maxRange_value->setMinimumSize(QSize(86, 25));
        maxRange_value->setMaximumSize(QSize(86, 25));
        maxRange_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        maxRange_value->setFrame(false);
        maxRange_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        maxRange_value->setDecimals(3);
        maxRange_value->setMinimum(-9999);
        maxRange_value->setMaximum(9999);
        maxRange_value->setSingleStep(0.05);
        maxRange_dial = new QDial(maxRange);
        maxRange_dial->setObjectName(QStringLiteral("maxRange_dial"));
        maxRange_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(maxRange_dial->sizePolicy().hasHeightForWidth());
        maxRange_dial->setSizePolicy(sizePolicy);
        maxRange_dial->setMinimumSize(QSize(30, 30));
        maxRange_dial->setMaximumSize(QSize(30, 30));
        maxRange_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        maxRange_dial->setMinimum(-10000);
        maxRange_dial->setMaximum(10000);
        maxRange_box = new QComboBox(maxRange);
        maxRange_box->setObjectName(QStringLiteral("maxRange_box"));
        maxRange_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(maxRange_box->sizePolicy().hasHeightForWidth());
        maxRange_box->setSizePolicy(sizePolicy);
        maxRange_box->setMinimumSize(QSize(95, 22));
        maxRange_box->setMaximumSize(QSize(90, 22));
        maxRange_box->setBaseSize(QSize(90, 25));
        maxRange_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        maxRange_box->setFrame(false);

        postLayout->addWidget(maxRange);

        contrast = new QFrame(layoutWidget3);
        contrast->setObjectName(QStringLiteral("contrast"));
        contrast->setMinimumSize(QSize(225, 35));
        contrast->setMaximumSize(QSize(225, 35));
        contrast->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        contrast->setFrameShape(QFrame::NoFrame);
        contrast->setFrameShadow(QFrame::Plain);
        contrast->setLineWidth(0);
        contrast_lbl = new QLabel(contrast);
        contrast_lbl->setObjectName(QStringLiteral("contrast_lbl"));
        contrast_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(contrast_lbl->sizePolicy().hasHeightForWidth());
        contrast_lbl->setSizePolicy(sizePolicy);
        contrast_lbl->setMinimumSize(QSize(100, 30));
        contrast_lbl->setMaximumSize(QSize(100, 30));
        contrast_lbl->setFont(font1);
        contrast_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        contrast_lbl->setMargin(1);
        contrast_value = new QDoubleSpinBox(contrast);
        contrast_value->setObjectName(QStringLiteral("contrast_value"));
        contrast_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(contrast_value->sizePolicy().hasHeightForWidth());
        contrast_value->setSizePolicy(sizePolicy);
        contrast_value->setMinimumSize(QSize(86, 25));
        contrast_value->setMaximumSize(QSize(86, 25));
        contrast_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        contrast_value->setFrame(false);
        contrast_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        contrast_value->setDecimals(3);
        contrast_value->setMinimum(-9999);
        contrast_value->setMaximum(9999);
        contrast_value->setSingleStep(0.05);
        contrast_dial = new QDial(contrast);
        contrast_dial->setObjectName(QStringLiteral("contrast_dial"));
        contrast_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(contrast_dial->sizePolicy().hasHeightForWidth());
        contrast_dial->setSizePolicy(sizePolicy);
        contrast_dial->setMinimumSize(QSize(30, 30));
        contrast_dial->setMaximumSize(QSize(30, 30));
        contrast_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        contrast_dial->setMinimum(-10000);
        contrast_dial->setMaximum(10000);

        postLayout->addWidget(contrast);

        brightness = new QFrame(layoutWidget3);
        brightness->setObjectName(QStringLiteral("brightness"));
        brightness->setMinimumSize(QSize(225, 35));
        brightness->setMaximumSize(QSize(225, 35));
        brightness->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        brightness->setFrameShape(QFrame::NoFrame);
        brightness->setFrameShadow(QFrame::Plain);
        brightness->setLineWidth(0);
        brightness_lbl = new QLabel(brightness);
        brightness_lbl->setObjectName(QStringLiteral("brightness_lbl"));
        brightness_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(brightness_lbl->sizePolicy().hasHeightForWidth());
        brightness_lbl->setSizePolicy(sizePolicy);
        brightness_lbl->setMinimumSize(QSize(100, 30));
        brightness_lbl->setMaximumSize(QSize(100, 30));
        brightness_lbl->setFont(font1);
        brightness_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        brightness_lbl->setMargin(1);
        brightness_value = new QDoubleSpinBox(brightness);
        brightness_value->setObjectName(QStringLiteral("brightness_value"));
        brightness_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(brightness_value->sizePolicy().hasHeightForWidth());
        brightness_value->setSizePolicy(sizePolicy);
        brightness_value->setMinimumSize(QSize(86, 25));
        brightness_value->setMaximumSize(QSize(86, 25));
        brightness_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        brightness_value->setFrame(false);
        brightness_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        brightness_value->setDecimals(3);
        brightness_value->setMinimum(-9999);
        brightness_value->setMaximum(9999);
        brightness_value->setSingleStep(0.05);
        brightness_dial = new QDial(brightness);
        brightness_dial->setObjectName(QStringLiteral("brightness_dial"));
        brightness_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(brightness_dial->sizePolicy().hasHeightForWidth());
        brightness_dial->setSizePolicy(sizePolicy);
        brightness_dial->setMinimumSize(QSize(30, 30));
        brightness_dial->setMaximumSize(QSize(30, 30));
        brightness_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        brightness_dial->setMinimum(-10000);
        brightness_dial->setMaximum(10000);

        postLayout->addWidget(brightness);

        blurring = new QFrame(layoutWidget3);
        blurring->setObjectName(QStringLiteral("blurring"));
        blurring->setMinimumSize(QSize(225, 35));
        blurring->setMaximumSize(QSize(225, 35));
        blurring->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        blurring->setFrameShape(QFrame::NoFrame);
        blurring->setFrameShadow(QFrame::Plain);
        blurring->setLineWidth(0);
        blurring_lbl = new QLabel(blurring);
        blurring_lbl->setObjectName(QStringLiteral("blurring_lbl"));
        blurring_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(blurring_lbl->sizePolicy().hasHeightForWidth());
        blurring_lbl->setSizePolicy(sizePolicy);
        blurring_lbl->setMinimumSize(QSize(100, 30));
        blurring_lbl->setMaximumSize(QSize(100, 30));
        blurring_lbl->setFont(font1);
        blurring_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        blurring_lbl->setMargin(1);
        blurring_value = new QDoubleSpinBox(blurring);
        blurring_value->setObjectName(QStringLiteral("blurring_value"));
        blurring_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(blurring_value->sizePolicy().hasHeightForWidth());
        blurring_value->setSizePolicy(sizePolicy);
        blurring_value->setMinimumSize(QSize(86, 25));
        blurring_value->setMaximumSize(QSize(86, 25));
        blurring_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        blurring_value->setFrame(false);
        blurring_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        blurring_value->setDecimals(3);
        blurring_value->setMinimum(-9999);
        blurring_value->setMaximum(9999);
        blurring_value->setSingleStep(0.05);
        blurring_dial = new QDial(blurring);
        blurring_dial->setObjectName(QStringLiteral("blurring_dial"));
        blurring_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(blurring_dial->sizePolicy().hasHeightForWidth());
        blurring_dial->setSizePolicy(sizePolicy);
        blurring_dial->setMinimumSize(QSize(30, 30));
        blurring_dial->setMaximumSize(QSize(30, 30));
        blurring_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        blurring_dial->setMinimum(-10000);
        blurring_dial->setMaximum(10000);

        postLayout->addWidget(blurring);

        blurStep = new QFrame(layoutWidget3);
        blurStep->setObjectName(QStringLiteral("blurStep"));
        blurStep->setMinimumSize(QSize(225, 35));
        blurStep->setMaximumSize(QSize(225, 35));
        blurStep->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        blurStep->setFrameShape(QFrame::NoFrame);
        blurStep->setFrameShadow(QFrame::Plain);
        blurStep->setLineWidth(0);
        blurStep_lbl = new QLabel(blurStep);
        blurStep_lbl->setObjectName(QStringLiteral("blurStep_lbl"));
        blurStep_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(blurStep_lbl->sizePolicy().hasHeightForWidth());
        blurStep_lbl->setSizePolicy(sizePolicy);
        blurStep_lbl->setMinimumSize(QSize(100, 30));
        blurStep_lbl->setMaximumSize(QSize(100, 30));
        blurStep_lbl->setFont(font1);
        blurStep_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        blurStep_lbl->setMargin(1);
        blurStep_value = new QDoubleSpinBox(blurStep);
        blurStep_value->setObjectName(QStringLiteral("blurStep_value"));
        blurStep_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(blurStep_value->sizePolicy().hasHeightForWidth());
        blurStep_value->setSizePolicy(sizePolicy);
        blurStep_value->setMinimumSize(QSize(86, 25));
        blurStep_value->setMaximumSize(QSize(86, 25));
        blurStep_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        blurStep_value->setFrame(false);
        blurStep_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        blurStep_value->setDecimals(3);
        blurStep_value->setMinimum(-9999);
        blurStep_value->setMaximum(9999);
        blurStep_value->setSingleStep(0.05);
        blurStep_dial = new QDial(blurStep);
        blurStep_dial->setObjectName(QStringLiteral("blurStep_dial"));
        blurStep_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(blurStep_dial->sizePolicy().hasHeightForWidth());
        blurStep_dial->setSizePolicy(sizePolicy);
        blurStep_dial->setMinimumSize(QSize(30, 30));
        blurStep_dial->setMaximumSize(QSize(30, 30));
        blurStep_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        blurStep_dial->setMinimum(-10000);
        blurStep_dial->setMaximum(10000);

        postLayout->addWidget(blurStep);

        glowIntensity = new QFrame(layoutWidget3);
        glowIntensity->setObjectName(QStringLiteral("glowIntensity"));
        glowIntensity->setMinimumSize(QSize(225, 35));
        glowIntensity->setMaximumSize(QSize(225, 35));
        glowIntensity->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        glowIntensity->setFrameShape(QFrame::NoFrame);
        glowIntensity->setFrameShadow(QFrame::Plain);
        glowIntensity->setLineWidth(0);
        glowIntensity_lbl = new QLabel(glowIntensity);
        glowIntensity_lbl->setObjectName(QStringLiteral("glowIntensity_lbl"));
        glowIntensity_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(glowIntensity_lbl->sizePolicy().hasHeightForWidth());
        glowIntensity_lbl->setSizePolicy(sizePolicy);
        glowIntensity_lbl->setMinimumSize(QSize(100, 30));
        glowIntensity_lbl->setMaximumSize(QSize(100, 30));
        glowIntensity_lbl->setFont(font1);
        glowIntensity_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        glowIntensity_lbl->setMargin(1);
        glowIntensity_value = new QDoubleSpinBox(glowIntensity);
        glowIntensity_value->setObjectName(QStringLiteral("glowIntensity_value"));
        glowIntensity_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(glowIntensity_value->sizePolicy().hasHeightForWidth());
        glowIntensity_value->setSizePolicy(sizePolicy);
        glowIntensity_value->setMinimumSize(QSize(86, 25));
        glowIntensity_value->setMaximumSize(QSize(86, 25));
        glowIntensity_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        glowIntensity_value->setFrame(false);
        glowIntensity_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        glowIntensity_value->setDecimals(3);
        glowIntensity_value->setMinimum(-9999);
        glowIntensity_value->setMaximum(9999);
        glowIntensity_value->setSingleStep(0.05);
        glowIntensity_dial = new QDial(glowIntensity);
        glowIntensity_dial->setObjectName(QStringLiteral("glowIntensity_dial"));
        glowIntensity_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(glowIntensity_dial->sizePolicy().hasHeightForWidth());
        glowIntensity_dial->setSizePolicy(sizePolicy);
        glowIntensity_dial->setMinimumSize(QSize(30, 30));
        glowIntensity_dial->setMaximumSize(QSize(30, 30));
        glowIntensity_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        glowIntensity_dial->setMinimum(-10000);
        glowIntensity_dial->setMaximum(10000);

        postLayout->addWidget(glowIntensity);

        depthNear = new QFrame(layoutWidget3);
        depthNear->setObjectName(QStringLiteral("depthNear"));
        depthNear->setMinimumSize(QSize(225, 35));
        depthNear->setMaximumSize(QSize(225, 35));
        depthNear->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        depthNear->setFrameShape(QFrame::NoFrame);
        depthNear->setFrameShadow(QFrame::Plain);
        depthNear->setLineWidth(0);
        depthNear_lbl = new QLabel(depthNear);
        depthNear_lbl->setObjectName(QStringLiteral("depthNear_lbl"));
        depthNear_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(depthNear_lbl->sizePolicy().hasHeightForWidth());
        depthNear_lbl->setSizePolicy(sizePolicy);
        depthNear_lbl->setMinimumSize(QSize(100, 30));
        depthNear_lbl->setMaximumSize(QSize(100, 30));
        depthNear_lbl->setFont(font1);
        depthNear_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        depthNear_lbl->setMargin(1);
        depthNear_value = new QDoubleSpinBox(depthNear);
        depthNear_value->setObjectName(QStringLiteral("depthNear_value"));
        depthNear_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(depthNear_value->sizePolicy().hasHeightForWidth());
        depthNear_value->setSizePolicy(sizePolicy);
        depthNear_value->setMinimumSize(QSize(86, 25));
        depthNear_value->setMaximumSize(QSize(86, 25));
        depthNear_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        depthNear_value->setFrame(false);
        depthNear_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        depthNear_value->setDecimals(3);
        depthNear_value->setMinimum(-9999);
        depthNear_value->setMaximum(9999);
        depthNear_value->setSingleStep(0.05);
        depthNear_dial = new QDial(depthNear);
        depthNear_dial->setObjectName(QStringLiteral("depthNear_dial"));
        depthNear_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(depthNear_dial->sizePolicy().hasHeightForWidth());
        depthNear_dial->setSizePolicy(sizePolicy);
        depthNear_dial->setMinimumSize(QSize(30, 30));
        depthNear_dial->setMaximumSize(QSize(30, 30));
        depthNear_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        depthNear_dial->setMinimum(-10000);
        depthNear_dial->setMaximum(10000);

        postLayout->addWidget(depthNear);

        depthFar = new QFrame(layoutWidget3);
        depthFar->setObjectName(QStringLiteral("depthFar"));
        depthFar->setMinimumSize(QSize(225, 35));
        depthFar->setMaximumSize(QSize(225, 35));
        depthFar->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        depthFar->setFrameShape(QFrame::NoFrame);
        depthFar->setFrameShadow(QFrame::Plain);
        depthFar->setLineWidth(0);
        depthFar_lbl = new QLabel(depthFar);
        depthFar_lbl->setObjectName(QStringLiteral("depthFar_lbl"));
        depthFar_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(depthFar_lbl->sizePolicy().hasHeightForWidth());
        depthFar_lbl->setSizePolicy(sizePolicy);
        depthFar_lbl->setMinimumSize(QSize(100, 30));
        depthFar_lbl->setMaximumSize(QSize(100, 30));
        depthFar_lbl->setFont(font1);
        depthFar_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        depthFar_lbl->setMargin(1);
        depthFar_value = new QDoubleSpinBox(depthFar);
        depthFar_value->setObjectName(QStringLiteral("depthFar_value"));
        depthFar_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(depthFar_value->sizePolicy().hasHeightForWidth());
        depthFar_value->setSizePolicy(sizePolicy);
        depthFar_value->setMinimumSize(QSize(86, 25));
        depthFar_value->setMaximumSize(QSize(86, 25));
        depthFar_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        depthFar_value->setFrame(false);
        depthFar_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        depthFar_value->setDecimals(3);
        depthFar_value->setMinimum(-9999);
        depthFar_value->setMaximum(9999);
        depthFar_value->setSingleStep(0.05);
        depthFar_dial = new QDial(depthFar);
        depthFar_dial->setObjectName(QStringLiteral("depthFar_dial"));
        depthFar_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(depthFar_dial->sizePolicy().hasHeightForWidth());
        depthFar_dial->setSizePolicy(sizePolicy);
        depthFar_dial->setMinimumSize(QSize(30, 30));
        depthFar_dial->setMaximumSize(QSize(30, 30));
        depthFar_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        depthFar_dial->setMinimum(-10000);
        depthFar_dial->setMaximum(10000);

        postLayout->addWidget(depthFar);

        dofDistance = new QFrame(layoutWidget3);
        dofDistance->setObjectName(QStringLiteral("dofDistance"));
        dofDistance->setMinimumSize(QSize(225, 35));
        dofDistance->setMaximumSize(QSize(225, 35));
        dofDistance->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        dofDistance->setFrameShape(QFrame::NoFrame);
        dofDistance->setFrameShadow(QFrame::Plain);
        dofDistance->setLineWidth(0);
        dofDistance_lbl = new QLabel(dofDistance);
        dofDistance_lbl->setObjectName(QStringLiteral("dofDistance_lbl"));
        dofDistance_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(dofDistance_lbl->sizePolicy().hasHeightForWidth());
        dofDistance_lbl->setSizePolicy(sizePolicy);
        dofDistance_lbl->setMinimumSize(QSize(100, 30));
        dofDistance_lbl->setMaximumSize(QSize(100, 30));
        dofDistance_lbl->setFont(font1);
        dofDistance_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        dofDistance_lbl->setMargin(1);
        dofDistance_value = new QDoubleSpinBox(dofDistance);
        dofDistance_value->setObjectName(QStringLiteral("dofDistance_value"));
        dofDistance_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(dofDistance_value->sizePolicy().hasHeightForWidth());
        dofDistance_value->setSizePolicy(sizePolicy);
        dofDistance_value->setMinimumSize(QSize(86, 25));
        dofDistance_value->setMaximumSize(QSize(86, 25));
        dofDistance_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        dofDistance_value->setFrame(false);
        dofDistance_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        dofDistance_value->setDecimals(3);
        dofDistance_value->setMinimum(-9999);
        dofDistance_value->setMaximum(9999);
        dofDistance_value->setSingleStep(0.05);
        dofDistance_dial = new QDial(dofDistance);
        dofDistance_dial->setObjectName(QStringLiteral("dofDistance_dial"));
        dofDistance_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(dofDistance_dial->sizePolicy().hasHeightForWidth());
        dofDistance_dial->setSizePolicy(sizePolicy);
        dofDistance_dial->setMinimumSize(QSize(30, 30));
        dofDistance_dial->setMaximumSize(QSize(30, 30));
        dofDistance_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        dofDistance_dial->setMinimum(-10000);
        dofDistance_dial->setMaximum(10000);

        postLayout->addWidget(dofDistance);

        _postSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        postLayout->addItem(_postSpacer);

        TabMenu->addTab(Post, QString());

        verticalLayout_3->addWidget(TabMenu);


        retranslateUi(Tweaker);

        TabMenu->setCurrentIndex(3);
        selectedLight_box->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(Tweaker);
    } // setupUi

    void retranslateUi(QWidget *Tweaker)
    {
        Tweaker->setWindowTitle(QApplication::translate("Tweaker", "Shader Editor", 0));
        switchEngine_lbl->setText(QApplication::translate("Tweaker", "Engine", 0));
        postImage_lbl->setText(QApplication::translate("Tweaker", "Rendering", 0));
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
        TabMenu->setTabText(TabMenu->indexOf(Scene), QApplication::translate("Tweaker", "Scene", 0));
        selectedLight_lbl->setText(QApplication::translate("Tweaker", "Light", 0));
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
        selectedMesh_lbl->setText(QApplication::translate("Tweaker", "Mesh", 0));
        meshSpecularity_lbl->setText(QApplication::translate("Tweaker", "Specularity", 0));
        meshAmbience_lbl->setText(QApplication::translate("Tweaker", "Ambience", 0));
        meshBump_lbl->setText(QApplication::translate("Tweaker", "Bump", 0));
        meshGlow_lbl->setText(QApplication::translate("Tweaker", "Glow", 0));
        shader_lbl->setText(QApplication::translate("Tweaker", "Shader", 0));
        shader_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        selectedParticles_lbl->setText(QApplication::translate("Tweaker", "Emitter", 0));
        particleCount_lbl->setText(QApplication::translate("Tweaker", "Count", 0));
        particleCount_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        particleSize_lbl->setText(QApplication::translate("Tweaker", "Size", 0));
        particleSpeed_lbl->setText(QApplication::translate("Tweaker", "Speed", 0));
        TabMenu->setTabText(TabMenu->indexOf(Mesh), QApplication::translate("Tweaker", "Mesh", 0));
        contrast_lbl->setText(QApplication::translate("Tweaker", "Contrast", 0));
        brightness_lbl->setText(QApplication::translate("Tweaker", "Brightness", 0));
        blurring_lbl->setText(QApplication::translate("Tweaker", "Blur Amount", 0));
        blurStep_lbl->setText(QApplication::translate("Tweaker", "Blur Step", 0));
        glowIntensity_lbl->setText(QApplication::translate("Tweaker", "Glow Intensity", 0));
        depthNear_lbl->setText(QApplication::translate("Tweaker", "Depth Near", 0));
        depthFar_lbl->setText(QApplication::translate("Tweaker", "Depth Far", 0));
        dofDistance_lbl->setText(QApplication::translate("Tweaker", "Depth of Field", 0));
        TabMenu->setTabText(TabMenu->indexOf(Post), QApplication::translate("Tweaker", "Post", 0));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
