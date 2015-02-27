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
#include <QtWidgets/QPushButton>
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
    QFrame *timer;
    QLabel *timer_lbl;
    QLabel *timer_text;
    QFrame *cameraX;
    QLabel *cameraX_lbl;
    QDoubleSpinBox *cameraX_value;
    QDial *cameraX_dial;
    QFrame *cameraY;
    QLabel *cameraY_lbl;
    QDoubleSpinBox *cameraY_value;
    QDial *cameraY_dial;
    QFrame *cameraZ;
    QLabel *cameraZ_lbl;
    QDoubleSpinBox *cameraZ_value;
    QDial *cameraZ_dial;
    QFrame *cameraPitch;
    QLabel *cameraPitch_lbl;
    QDoubleSpinBox *cameraPitch_value;
    QDial *cameraPitch_dial;
    QFrame *cameraYaw;
    QLabel *cameraYaw_lbl;
    QDoubleSpinBox *cameraYaw_value;
    QDial *cameraYaw_dial;
    QFrame *cameraRoll;
    QLabel *cameraRoll_lbl;
    QDoubleSpinBox *cameraRoll_value;
    QDial *cameraRoll_dial;
    QFrame *reloadScene;
    QPushButton *reloadScene_btn;
    QFrame *saveScene;
    QPushButton *saveScene_btn;
    QFrame *savePost;
    QPushButton *savePost_btn;
    QSpacerItem *_sceneSpacer;
    QWidget *Light;
    QWidget *layoutWidget1;
    QVBoxLayout *lightLayout;
    QFrame *lightDiagnostics;
    QPushButton *lightDiagnostics_btn;
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
    QFrame *specularity;
    QLabel *specularity_lbl;
    QDoubleSpinBox *specularity_value;
    QDial *specularity_dial;
    QFrame *light_diffuse;
    QDoubleSpinBox *light_diffuse_value;
    QDial *light_diffuse_dial;
    QComboBox *light_diffuse_box;
    QFrame *light_specular;
    QDoubleSpinBox *light_specular_value;
    QDial *light_specular_dial;
    QComboBox *light_specular_box;
    QSpacerItem *_lightSpacer;
    QWidget *Mesh;
    QWidget *layoutWidget2;
    QVBoxLayout *meshLayout;
    QFrame *selectedMesh;
    QLabel *selectedMesh_lbl;
    QComboBox *selectedMesh_box;
    QFrame *shader;
    QLabel *shader_lbl;
    QLabel *shader_text;
    QFrame *instances;
    QLabel *instances_lbl;
    QLabel *instances_text;
    QFrame *meshAttributes;
    QDoubleSpinBox *meshAttributes_value;
    QDial *meshAttributes_dial;
    QComboBox *meshAttributes_box;
    QSpacerItem *_meshSpacer_2;
    QFrame *selectedWater;
    QLabel *selectedWater_lbl;
    QComboBox *selectedWater_box;
    QFrame *waveNumber;
    QLabel *waveNumber_lbl;
    QDoubleSpinBox *waveNumber_value;
    QDial *waveNumber_dial;
    QFrame *wave;
    QDoubleSpinBox *wave_value;
    QDial *wave_dial;
    QComboBox *wave_box;
    QFrame *water;
    QDoubleSpinBox *water_value;
    QDial *water_dial;
    QComboBox *water_box;
    QSpacerItem *_meshSpacer_3;
    QFrame *selectedParticles;
    QLabel *selectedParticles_lbl;
    QComboBox *selectedParticles_box;
    QFrame *particleAmount;
    QLabel *particleAmount_lbl;
    QDoubleSpinBox *particleAmount_value;
    QDial *particleAmount_dial;
    QFrame *emitter;
    QDoubleSpinBox *emitter_value;
    QDial *emitter_dial;
    QComboBox *emitter_box;
    QFrame *emitterColour;
    QDoubleSpinBox *emitterColour_value;
    QDial *emitterColour_dial;
    QComboBox *emitterColour_box;
    QFrame *emitterMinMax;
    QDoubleSpinBox *emitterMinMax_value;
    QDial *emitterMinMax_dial;
    QComboBox *emitterMinMax_box;
    QFrame *pauseEmission;
    QPushButton *pauseEmission_btn;
    QSpacerItem *_meshSpacer;
    QWidget *Post;
    QWidget *layoutWidget3;
    QVBoxLayout *postLayout;
    QFrame *depthNear;
    QLabel *depthNear_lbl;
    QDoubleSpinBox *depthNear_value;
    QDial *depthNear_dial;
    QFrame *depthFar;
    QLabel *depthFar_lbl;
    QDoubleSpinBox *depthFar_value;
    QDial *depthFar_dial;
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
    QFrame *dofDistance;
    QLabel *dofDistance_lbl;
    QDoubleSpinBox *dofDistance_value;
    QDial *dofDistance_dial;
    QFrame *dofFade;
    QLabel *dofFade_lbl;
    QDoubleSpinBox *dofFade_value;
    QDial *dofFade_dial;
    QFrame *contrast;
    QLabel *contrast_lbl;
    QDoubleSpinBox *contrast_value;
    QDial *contrast_dial;
    QFrame *saturation;
    QLabel *saturation_lbl;
    QDoubleSpinBox *saturation_value;
    QDial *saturation_dial;
    QFrame *maxRange;
    QDoubleSpinBox *maxRange_value;
    QDial *maxRange_dial;
    QComboBox *maxRange_box;
    QFrame *minRange;
    QDoubleSpinBox *minRange_value;
    QDial *minRange_dial;
    QComboBox *minRange_box;
    QFrame *fog;
    QDoubleSpinBox *fog_value;
    QDial *fog_dial;
    QComboBox *fog_box;
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

        timer = new QFrame(layoutWidget);
        timer->setObjectName(QStringLiteral("timer"));
        sizePolicy.setHeightForWidth(timer->sizePolicy().hasHeightForWidth());
        timer->setSizePolicy(sizePolicy);
        timer->setMinimumSize(QSize(225, 35));
        timer->setMaximumSize(QSize(225, 35));
        timer->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        timer->setFrameShape(QFrame::Panel);
        timer->setFrameShadow(QFrame::Raised);
        timer_lbl = new QLabel(timer);
        timer_lbl->setObjectName(QStringLiteral("timer_lbl"));
        timer_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(timer_lbl->sizePolicy().hasHeightForWidth());
        timer_lbl->setSizePolicy(sizePolicy);
        timer_lbl->setMinimumSize(QSize(100, 30));
        timer_lbl->setMaximumSize(QSize(100, 30));
        timer_lbl->setFont(font1);
        timer_text = new QLabel(timer);
        timer_text->setObjectName(QStringLiteral("timer_text"));
        timer_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(timer_text->sizePolicy().hasHeightForWidth());
        timer_text->setSizePolicy(sizePolicy1);
        timer_text->setMinimumSize(QSize(115, 25));
        timer_text->setMaximumSize(QSize(115, 25));
        timer_text->setAutoFillBackground(false);
        timer_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        timer_text->setFrameShape(QFrame::NoFrame);
        timer_text->setFrameShadow(QFrame::Plain);
        timer_text->setLineWidth(0);
        timer_text->setMargin(3);

        sceneLayout->addWidget(timer);

        cameraX = new QFrame(layoutWidget);
        cameraX->setObjectName(QStringLiteral("cameraX"));
        cameraX->setMinimumSize(QSize(225, 35));
        cameraX->setMaximumSize(QSize(225, 35));
        cameraX->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraX->setFrameShape(QFrame::NoFrame);
        cameraX->setFrameShadow(QFrame::Plain);
        cameraX->setLineWidth(0);
        cameraX_lbl = new QLabel(cameraX);
        cameraX_lbl->setObjectName(QStringLiteral("cameraX_lbl"));
        cameraX_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraX_lbl->sizePolicy().hasHeightForWidth());
        cameraX_lbl->setSizePolicy(sizePolicy);
        cameraX_lbl->setMinimumSize(QSize(100, 30));
        cameraX_lbl->setMaximumSize(QSize(100, 30));
        cameraX_lbl->setFont(font1);
        cameraX_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraX_lbl->setMargin(1);
        cameraX_value = new QDoubleSpinBox(cameraX);
        cameraX_value->setObjectName(QStringLiteral("cameraX_value"));
        cameraX_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraX_value->sizePolicy().hasHeightForWidth());
        cameraX_value->setSizePolicy(sizePolicy);
        cameraX_value->setMinimumSize(QSize(86, 25));
        cameraX_value->setMaximumSize(QSize(86, 25));
        cameraX_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraX_value->setFrame(false);
        cameraX_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraX_value->setDecimals(3);
        cameraX_value->setMinimum(-9999);
        cameraX_value->setMaximum(9999);
        cameraX_value->setSingleStep(0.05);
        cameraX_dial = new QDial(cameraX);
        cameraX_dial->setObjectName(QStringLiteral("cameraX_dial"));
        cameraX_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraX_dial->sizePolicy().hasHeightForWidth());
        cameraX_dial->setSizePolicy(sizePolicy);
        cameraX_dial->setMinimumSize(QSize(30, 30));
        cameraX_dial->setMaximumSize(QSize(30, 30));
        cameraX_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraX_dial->setMinimum(-10000);
        cameraX_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraX);

        cameraY = new QFrame(layoutWidget);
        cameraY->setObjectName(QStringLiteral("cameraY"));
        cameraY->setMinimumSize(QSize(225, 35));
        cameraY->setMaximumSize(QSize(225, 35));
        cameraY->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraY->setFrameShape(QFrame::NoFrame);
        cameraY->setFrameShadow(QFrame::Plain);
        cameraY->setLineWidth(0);
        cameraY_lbl = new QLabel(cameraY);
        cameraY_lbl->setObjectName(QStringLiteral("cameraY_lbl"));
        cameraY_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraY_lbl->sizePolicy().hasHeightForWidth());
        cameraY_lbl->setSizePolicy(sizePolicy);
        cameraY_lbl->setMinimumSize(QSize(100, 30));
        cameraY_lbl->setMaximumSize(QSize(100, 30));
        cameraY_lbl->setFont(font1);
        cameraY_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraY_lbl->setMargin(1);
        cameraY_value = new QDoubleSpinBox(cameraY);
        cameraY_value->setObjectName(QStringLiteral("cameraY_value"));
        cameraY_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraY_value->sizePolicy().hasHeightForWidth());
        cameraY_value->setSizePolicy(sizePolicy);
        cameraY_value->setMinimumSize(QSize(86, 25));
        cameraY_value->setMaximumSize(QSize(86, 25));
        cameraY_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraY_value->setFrame(false);
        cameraY_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraY_value->setDecimals(3);
        cameraY_value->setMinimum(-9999);
        cameraY_value->setMaximum(9999);
        cameraY_value->setSingleStep(0.05);
        cameraY_dial = new QDial(cameraY);
        cameraY_dial->setObjectName(QStringLiteral("cameraY_dial"));
        cameraY_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraY_dial->sizePolicy().hasHeightForWidth());
        cameraY_dial->setSizePolicy(sizePolicy);
        cameraY_dial->setMinimumSize(QSize(30, 30));
        cameraY_dial->setMaximumSize(QSize(30, 30));
        cameraY_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraY_dial->setMinimum(-10000);
        cameraY_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraY);

        cameraZ = new QFrame(layoutWidget);
        cameraZ->setObjectName(QStringLiteral("cameraZ"));
        cameraZ->setMinimumSize(QSize(225, 35));
        cameraZ->setMaximumSize(QSize(225, 35));
        cameraZ->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraZ->setFrameShape(QFrame::NoFrame);
        cameraZ->setFrameShadow(QFrame::Plain);
        cameraZ->setLineWidth(0);
        cameraZ_lbl = new QLabel(cameraZ);
        cameraZ_lbl->setObjectName(QStringLiteral("cameraZ_lbl"));
        cameraZ_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraZ_lbl->sizePolicy().hasHeightForWidth());
        cameraZ_lbl->setSizePolicy(sizePolicy);
        cameraZ_lbl->setMinimumSize(QSize(100, 30));
        cameraZ_lbl->setMaximumSize(QSize(100, 30));
        cameraZ_lbl->setFont(font1);
        cameraZ_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraZ_lbl->setMargin(1);
        cameraZ_value = new QDoubleSpinBox(cameraZ);
        cameraZ_value->setObjectName(QStringLiteral("cameraZ_value"));
        cameraZ_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraZ_value->sizePolicy().hasHeightForWidth());
        cameraZ_value->setSizePolicy(sizePolicy);
        cameraZ_value->setMinimumSize(QSize(86, 25));
        cameraZ_value->setMaximumSize(QSize(86, 25));
        cameraZ_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraZ_value->setFrame(false);
        cameraZ_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraZ_value->setDecimals(3);
        cameraZ_value->setMinimum(-9999);
        cameraZ_value->setMaximum(9999);
        cameraZ_value->setSingleStep(0.05);
        cameraZ_dial = new QDial(cameraZ);
        cameraZ_dial->setObjectName(QStringLiteral("cameraZ_dial"));
        cameraZ_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraZ_dial->sizePolicy().hasHeightForWidth());
        cameraZ_dial->setSizePolicy(sizePolicy);
        cameraZ_dial->setMinimumSize(QSize(30, 30));
        cameraZ_dial->setMaximumSize(QSize(30, 30));
        cameraZ_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraZ_dial->setMinimum(-10000);
        cameraZ_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraZ);

        cameraPitch = new QFrame(layoutWidget);
        cameraPitch->setObjectName(QStringLiteral("cameraPitch"));
        cameraPitch->setMinimumSize(QSize(225, 35));
        cameraPitch->setMaximumSize(QSize(225, 35));
        cameraPitch->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraPitch->setFrameShape(QFrame::NoFrame);
        cameraPitch->setFrameShadow(QFrame::Plain);
        cameraPitch->setLineWidth(0);
        cameraPitch_lbl = new QLabel(cameraPitch);
        cameraPitch_lbl->setObjectName(QStringLiteral("cameraPitch_lbl"));
        cameraPitch_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraPitch_lbl->sizePolicy().hasHeightForWidth());
        cameraPitch_lbl->setSizePolicy(sizePolicy);
        cameraPitch_lbl->setMinimumSize(QSize(100, 30));
        cameraPitch_lbl->setMaximumSize(QSize(100, 30));
        cameraPitch_lbl->setFont(font1);
        cameraPitch_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraPitch_lbl->setMargin(1);
        cameraPitch_value = new QDoubleSpinBox(cameraPitch);
        cameraPitch_value->setObjectName(QStringLiteral("cameraPitch_value"));
        cameraPitch_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraPitch_value->sizePolicy().hasHeightForWidth());
        cameraPitch_value->setSizePolicy(sizePolicy);
        cameraPitch_value->setMinimumSize(QSize(86, 25));
        cameraPitch_value->setMaximumSize(QSize(86, 25));
        cameraPitch_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraPitch_value->setFrame(false);
        cameraPitch_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraPitch_value->setDecimals(3);
        cameraPitch_value->setMinimum(-9999);
        cameraPitch_value->setMaximum(9999);
        cameraPitch_value->setSingleStep(0.05);
        cameraPitch_dial = new QDial(cameraPitch);
        cameraPitch_dial->setObjectName(QStringLiteral("cameraPitch_dial"));
        cameraPitch_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraPitch_dial->sizePolicy().hasHeightForWidth());
        cameraPitch_dial->setSizePolicy(sizePolicy);
        cameraPitch_dial->setMinimumSize(QSize(30, 30));
        cameraPitch_dial->setMaximumSize(QSize(30, 30));
        cameraPitch_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraPitch_dial->setMinimum(-10000);
        cameraPitch_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraPitch);

        cameraYaw = new QFrame(layoutWidget);
        cameraYaw->setObjectName(QStringLiteral("cameraYaw"));
        cameraYaw->setMinimumSize(QSize(225, 35));
        cameraYaw->setMaximumSize(QSize(225, 35));
        cameraYaw->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraYaw->setFrameShape(QFrame::NoFrame);
        cameraYaw->setFrameShadow(QFrame::Plain);
        cameraYaw->setLineWidth(0);
        cameraYaw_lbl = new QLabel(cameraYaw);
        cameraYaw_lbl->setObjectName(QStringLiteral("cameraYaw_lbl"));
        cameraYaw_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraYaw_lbl->sizePolicy().hasHeightForWidth());
        cameraYaw_lbl->setSizePolicy(sizePolicy);
        cameraYaw_lbl->setMinimumSize(QSize(100, 30));
        cameraYaw_lbl->setMaximumSize(QSize(100, 30));
        cameraYaw_lbl->setFont(font1);
        cameraYaw_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraYaw_lbl->setMargin(1);
        cameraYaw_value = new QDoubleSpinBox(cameraYaw);
        cameraYaw_value->setObjectName(QStringLiteral("cameraYaw_value"));
        cameraYaw_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraYaw_value->sizePolicy().hasHeightForWidth());
        cameraYaw_value->setSizePolicy(sizePolicy);
        cameraYaw_value->setMinimumSize(QSize(86, 25));
        cameraYaw_value->setMaximumSize(QSize(86, 25));
        cameraYaw_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraYaw_value->setFrame(false);
        cameraYaw_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraYaw_value->setDecimals(3);
        cameraYaw_value->setMinimum(-9999);
        cameraYaw_value->setMaximum(9999);
        cameraYaw_value->setSingleStep(0.05);
        cameraYaw_dial = new QDial(cameraYaw);
        cameraYaw_dial->setObjectName(QStringLiteral("cameraYaw_dial"));
        cameraYaw_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraYaw_dial->sizePolicy().hasHeightForWidth());
        cameraYaw_dial->setSizePolicy(sizePolicy);
        cameraYaw_dial->setMinimumSize(QSize(30, 30));
        cameraYaw_dial->setMaximumSize(QSize(30, 30));
        cameraYaw_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraYaw_dial->setMinimum(-10000);
        cameraYaw_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraYaw);

        cameraRoll = new QFrame(layoutWidget);
        cameraRoll->setObjectName(QStringLiteral("cameraRoll"));
        cameraRoll->setMinimumSize(QSize(225, 35));
        cameraRoll->setMaximumSize(QSize(225, 35));
        cameraRoll->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraRoll->setFrameShape(QFrame::NoFrame);
        cameraRoll->setFrameShadow(QFrame::Plain);
        cameraRoll->setLineWidth(0);
        cameraRoll_lbl = new QLabel(cameraRoll);
        cameraRoll_lbl->setObjectName(QStringLiteral("cameraRoll_lbl"));
        cameraRoll_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraRoll_lbl->sizePolicy().hasHeightForWidth());
        cameraRoll_lbl->setSizePolicy(sizePolicy);
        cameraRoll_lbl->setMinimumSize(QSize(100, 30));
        cameraRoll_lbl->setMaximumSize(QSize(100, 30));
        cameraRoll_lbl->setFont(font1);
        cameraRoll_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraRoll_lbl->setMargin(1);
        cameraRoll_value = new QDoubleSpinBox(cameraRoll);
        cameraRoll_value->setObjectName(QStringLiteral("cameraRoll_value"));
        cameraRoll_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraRoll_value->sizePolicy().hasHeightForWidth());
        cameraRoll_value->setSizePolicy(sizePolicy);
        cameraRoll_value->setMinimumSize(QSize(86, 25));
        cameraRoll_value->setMaximumSize(QSize(86, 25));
        cameraRoll_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraRoll_value->setFrame(false);
        cameraRoll_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraRoll_value->setDecimals(3);
        cameraRoll_value->setMinimum(-9999);
        cameraRoll_value->setMaximum(9999);
        cameraRoll_value->setSingleStep(0.05);
        cameraRoll_dial = new QDial(cameraRoll);
        cameraRoll_dial->setObjectName(QStringLiteral("cameraRoll_dial"));
        cameraRoll_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraRoll_dial->sizePolicy().hasHeightForWidth());
        cameraRoll_dial->setSizePolicy(sizePolicy);
        cameraRoll_dial->setMinimumSize(QSize(30, 30));
        cameraRoll_dial->setMaximumSize(QSize(30, 30));
        cameraRoll_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        cameraRoll_dial->setMinimum(-10000);
        cameraRoll_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraRoll);

        reloadScene = new QFrame(layoutWidget);
        reloadScene->setObjectName(QStringLiteral("reloadScene"));
        sizePolicy.setHeightForWidth(reloadScene->sizePolicy().hasHeightForWidth());
        reloadScene->setSizePolicy(sizePolicy);
        reloadScene->setMinimumSize(QSize(225, 35));
        reloadScene->setMaximumSize(QSize(225, 35));
        reloadScene->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadScene->setFrameShape(QFrame::Panel);
        reloadScene->setFrameShadow(QFrame::Raised);
        reloadScene_btn = new QPushButton(reloadScene);
        reloadScene_btn->setObjectName(QStringLiteral("reloadScene_btn"));
        reloadScene_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadScene_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        reloadScene_btn->setDefault(false);

        sceneLayout->addWidget(reloadScene);

        saveScene = new QFrame(layoutWidget);
        saveScene->setObjectName(QStringLiteral("saveScene"));
        sizePolicy.setHeightForWidth(saveScene->sizePolicy().hasHeightForWidth());
        saveScene->setSizePolicy(sizePolicy);
        saveScene->setMinimumSize(QSize(225, 35));
        saveScene->setMaximumSize(QSize(225, 35));
        saveScene->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        saveScene->setFrameShape(QFrame::Panel);
        saveScene->setFrameShadow(QFrame::Raised);
        saveScene_btn = new QPushButton(saveScene);
        saveScene_btn->setObjectName(QStringLiteral("saveScene_btn"));
        saveScene_btn->setGeometry(QRect(5, 4, 216, 25));
        saveScene_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        saveScene_btn->setDefault(false);

        sceneLayout->addWidget(saveScene);

        savePost = new QFrame(layoutWidget);
        savePost->setObjectName(QStringLiteral("savePost"));
        sizePolicy.setHeightForWidth(savePost->sizePolicy().hasHeightForWidth());
        savePost->setSizePolicy(sizePolicy);
        savePost->setMinimumSize(QSize(225, 35));
        savePost->setMaximumSize(QSize(225, 35));
        savePost->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        savePost->setFrameShape(QFrame::Panel);
        savePost->setFrameShadow(QFrame::Raised);
        savePost_btn = new QPushButton(savePost);
        savePost_btn->setObjectName(QStringLiteral("savePost_btn"));
        savePost_btn->setGeometry(QRect(5, 4, 216, 25));
        savePost_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        sceneLayout->addWidget(savePost);

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
        lightDiagnostics = new QFrame(layoutWidget1);
        lightDiagnostics->setObjectName(QStringLiteral("lightDiagnostics"));
        sizePolicy.setHeightForWidth(lightDiagnostics->sizePolicy().hasHeightForWidth());
        lightDiagnostics->setSizePolicy(sizePolicy);
        lightDiagnostics->setMinimumSize(QSize(225, 35));
        lightDiagnostics->setMaximumSize(QSize(225, 35));
        lightDiagnostics->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        lightDiagnostics->setFrameShape(QFrame::Panel);
        lightDiagnostics->setFrameShadow(QFrame::Raised);
        lightDiagnostics_btn = new QPushButton(lightDiagnostics);
        lightDiagnostics_btn->setObjectName(QStringLiteral("lightDiagnostics_btn"));
        lightDiagnostics_btn->setGeometry(QRect(5, 4, 216, 25));
        lightDiagnostics_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        lightDiagnostics_btn->setDefault(false);

        lightLayout->addWidget(lightDiagnostics);

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

        specularity = new QFrame(layoutWidget1);
        specularity->setObjectName(QStringLiteral("specularity"));
        specularity->setMinimumSize(QSize(225, 35));
        specularity->setMaximumSize(QSize(225, 35));
        specularity->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        specularity->setFrameShape(QFrame::NoFrame);
        specularity->setFrameShadow(QFrame::Plain);
        specularity->setLineWidth(0);
        specularity_lbl = new QLabel(specularity);
        specularity_lbl->setObjectName(QStringLiteral("specularity_lbl"));
        specularity_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(specularity_lbl->sizePolicy().hasHeightForWidth());
        specularity_lbl->setSizePolicy(sizePolicy);
        specularity_lbl->setMinimumSize(QSize(100, 30));
        specularity_lbl->setMaximumSize(QSize(100, 30));
        specularity_lbl->setFont(font1);
        specularity_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularity_lbl->setMargin(1);
        specularity_value = new QDoubleSpinBox(specularity);
        specularity_value->setObjectName(QStringLiteral("specularity_value"));
        specularity_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(specularity_value->sizePolicy().hasHeightForWidth());
        specularity_value->setSizePolicy(sizePolicy);
        specularity_value->setMinimumSize(QSize(86, 25));
        specularity_value->setMaximumSize(QSize(86, 25));
        specularity_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        specularity_value->setFrame(false);
        specularity_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        specularity_value->setDecimals(3);
        specularity_value->setMinimum(-9999);
        specularity_value->setMaximum(9999);
        specularity_value->setSingleStep(0.05);
        specularity_dial = new QDial(specularity);
        specularity_dial->setObjectName(QStringLiteral("specularity_dial"));
        specularity_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(specularity_dial->sizePolicy().hasHeightForWidth());
        specularity_dial->setSizePolicy(sizePolicy);
        specularity_dial->setMinimumSize(QSize(30, 30));
        specularity_dial->setMaximumSize(QSize(30, 30));
        specularity_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        specularity_dial->setMinimum(-10000);
        specularity_dial->setMaximum(10000);

        lightLayout->addWidget(specularity);

        light_diffuse = new QFrame(layoutWidget1);
        light_diffuse->setObjectName(QStringLiteral("light_diffuse"));
        light_diffuse->setMinimumSize(QSize(225, 35));
        light_diffuse->setMaximumSize(QSize(225, 35));
        light_diffuse->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_diffuse->setFrameShape(QFrame::NoFrame);
        light_diffuse->setFrameShadow(QFrame::Plain);
        light_diffuse->setLineWidth(0);
        light_diffuse_value = new QDoubleSpinBox(light_diffuse);
        light_diffuse_value->setObjectName(QStringLiteral("light_diffuse_value"));
        light_diffuse_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_diffuse_value->sizePolicy().hasHeightForWidth());
        light_diffuse_value->setSizePolicy(sizePolicy);
        light_diffuse_value->setMinimumSize(QSize(86, 25));
        light_diffuse_value->setMaximumSize(QSize(86, 25));
        light_diffuse_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_diffuse_value->setFrame(false);
        light_diffuse_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_diffuse_value->setDecimals(3);
        light_diffuse_value->setMinimum(-9999);
        light_diffuse_value->setMaximum(9999);
        light_diffuse_value->setSingleStep(0.05);
        light_diffuse_dial = new QDial(light_diffuse);
        light_diffuse_dial->setObjectName(QStringLiteral("light_diffuse_dial"));
        light_diffuse_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_diffuse_dial->sizePolicy().hasHeightForWidth());
        light_diffuse_dial->setSizePolicy(sizePolicy);
        light_diffuse_dial->setMinimumSize(QSize(30, 30));
        light_diffuse_dial->setMaximumSize(QSize(30, 30));
        light_diffuse_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_diffuse_dial->setMinimum(-10000);
        light_diffuse_dial->setMaximum(10000);
        light_diffuse_box = new QComboBox(light_diffuse);
        light_diffuse_box->setObjectName(QStringLiteral("light_diffuse_box"));
        light_diffuse_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_diffuse_box->sizePolicy().hasHeightForWidth());
        light_diffuse_box->setSizePolicy(sizePolicy);
        light_diffuse_box->setMinimumSize(QSize(95, 22));
        light_diffuse_box->setMaximumSize(QSize(90, 22));
        light_diffuse_box->setBaseSize(QSize(90, 25));
        light_diffuse_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_diffuse_box->setMaxVisibleItems(30);
        light_diffuse_box->setFrame(false);

        lightLayout->addWidget(light_diffuse);

        light_specular = new QFrame(layoutWidget1);
        light_specular->setObjectName(QStringLiteral("light_specular"));
        light_specular->setMinimumSize(QSize(225, 35));
        light_specular->setMaximumSize(QSize(225, 35));
        light_specular->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_specular->setFrameShape(QFrame::NoFrame);
        light_specular->setFrameShadow(QFrame::Plain);
        light_specular->setLineWidth(0);
        light_specular_value = new QDoubleSpinBox(light_specular);
        light_specular_value->setObjectName(QStringLiteral("light_specular_value"));
        light_specular_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_specular_value->sizePolicy().hasHeightForWidth());
        light_specular_value->setSizePolicy(sizePolicy);
        light_specular_value->setMinimumSize(QSize(86, 25));
        light_specular_value->setMaximumSize(QSize(86, 25));
        light_specular_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_specular_value->setFrame(false);
        light_specular_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_specular_value->setDecimals(3);
        light_specular_value->setMinimum(-9999);
        light_specular_value->setMaximum(9999);
        light_specular_value->setSingleStep(0.05);
        light_specular_dial = new QDial(light_specular);
        light_specular_dial->setObjectName(QStringLiteral("light_specular_dial"));
        light_specular_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_specular_dial->sizePolicy().hasHeightForWidth());
        light_specular_dial->setSizePolicy(sizePolicy);
        light_specular_dial->setMinimumSize(QSize(30, 30));
        light_specular_dial->setMaximumSize(QSize(30, 30));
        light_specular_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_specular_dial->setMinimum(-10000);
        light_specular_dial->setMaximum(10000);
        light_specular_box = new QComboBox(light_specular);
        light_specular_box->setObjectName(QStringLiteral("light_specular_box"));
        light_specular_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_specular_box->sizePolicy().hasHeightForWidth());
        light_specular_box->setSizePolicy(sizePolicy);
        light_specular_box->setMinimumSize(QSize(95, 22));
        light_specular_box->setMaximumSize(QSize(90, 22));
        light_specular_box->setBaseSize(QSize(90, 25));
        light_specular_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_specular_box->setMaxVisibleItems(30);
        light_specular_box->setFrame(false);

        lightLayout->addWidget(light_specular);

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

        instances = new QFrame(layoutWidget2);
        instances->setObjectName(QStringLiteral("instances"));
        sizePolicy.setHeightForWidth(instances->sizePolicy().hasHeightForWidth());
        instances->setSizePolicy(sizePolicy);
        instances->setMinimumSize(QSize(225, 35));
        instances->setMaximumSize(QSize(225, 35));
        instances->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        instances->setFrameShape(QFrame::Panel);
        instances->setFrameShadow(QFrame::Raised);
        instances_lbl = new QLabel(instances);
        instances_lbl->setObjectName(QStringLiteral("instances_lbl"));
        instances_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(instances_lbl->sizePolicy().hasHeightForWidth());
        instances_lbl->setSizePolicy(sizePolicy);
        instances_lbl->setMinimumSize(QSize(100, 30));
        instances_lbl->setMaximumSize(QSize(100, 30));
        instances_lbl->setFont(font1);
        instances_text = new QLabel(instances);
        instances_text->setObjectName(QStringLiteral("instances_text"));
        instances_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(instances_text->sizePolicy().hasHeightForWidth());
        instances_text->setSizePolicy(sizePolicy1);
        instances_text->setMinimumSize(QSize(115, 25));
        instances_text->setMaximumSize(QSize(115, 25));
        instances_text->setAutoFillBackground(false);
        instances_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        instances_text->setFrameShape(QFrame::NoFrame);
        instances_text->setFrameShadow(QFrame::Plain);
        instances_text->setLineWidth(0);
        instances_text->setMargin(3);

        meshLayout->addWidget(instances);

        meshAttributes = new QFrame(layoutWidget2);
        meshAttributes->setObjectName(QStringLiteral("meshAttributes"));
        meshAttributes->setMinimumSize(QSize(225, 35));
        meshAttributes->setMaximumSize(QSize(225, 35));
        meshAttributes->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshAttributes->setFrameShape(QFrame::NoFrame);
        meshAttributes->setFrameShadow(QFrame::Plain);
        meshAttributes->setLineWidth(0);
        meshAttributes_value = new QDoubleSpinBox(meshAttributes);
        meshAttributes_value->setObjectName(QStringLiteral("meshAttributes_value"));
        meshAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshAttributes_value->sizePolicy().hasHeightForWidth());
        meshAttributes_value->setSizePolicy(sizePolicy);
        meshAttributes_value->setMinimumSize(QSize(86, 25));
        meshAttributes_value->setMaximumSize(QSize(86, 25));
        meshAttributes_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshAttributes_value->setFrame(false);
        meshAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshAttributes_value->setDecimals(3);
        meshAttributes_value->setMinimum(-9999);
        meshAttributes_value->setMaximum(9999);
        meshAttributes_value->setSingleStep(0.05);
        meshAttributes_dial = new QDial(meshAttributes);
        meshAttributes_dial->setObjectName(QStringLiteral("meshAttributes_dial"));
        meshAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshAttributes_dial->sizePolicy().hasHeightForWidth());
        meshAttributes_dial->setSizePolicy(sizePolicy);
        meshAttributes_dial->setMinimumSize(QSize(30, 30));
        meshAttributes_dial->setMaximumSize(QSize(30, 30));
        meshAttributes_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        meshAttributes_dial->setMinimum(-10000);
        meshAttributes_dial->setMaximum(10000);
        meshAttributes_box = new QComboBox(meshAttributes);
        meshAttributes_box->setObjectName(QStringLiteral("meshAttributes_box"));
        meshAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(meshAttributes_box->sizePolicy().hasHeightForWidth());
        meshAttributes_box->setSizePolicy(sizePolicy);
        meshAttributes_box->setMinimumSize(QSize(95, 22));
        meshAttributes_box->setMaximumSize(QSize(90, 22));
        meshAttributes_box->setBaseSize(QSize(90, 25));
        meshAttributes_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshAttributes_box->setFrame(false);

        meshLayout->addWidget(meshAttributes);

        _meshSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        meshLayout->addItem(_meshSpacer_2);

        selectedWater = new QFrame(layoutWidget2);
        selectedWater->setObjectName(QStringLiteral("selectedWater"));
        sizePolicy.setHeightForWidth(selectedWater->sizePolicy().hasHeightForWidth());
        selectedWater->setSizePolicy(sizePolicy);
        selectedWater->setMinimumSize(QSize(225, 35));
        selectedWater->setMaximumSize(QSize(225, 35));
        selectedWater->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedWater->setFrameShape(QFrame::Panel);
        selectedWater->setFrameShadow(QFrame::Raised);
        selectedWater_lbl = new QLabel(selectedWater);
        selectedWater_lbl->setObjectName(QStringLiteral("selectedWater_lbl"));
        selectedWater_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedWater_lbl->sizePolicy().hasHeightForWidth());
        selectedWater_lbl->setSizePolicy(sizePolicy);
        selectedWater_lbl->setMinimumSize(QSize(100, 30));
        selectedWater_lbl->setMaximumSize(QSize(100, 30));
        selectedWater_lbl->setFont(font);
        selectedWater_lbl->setMargin(1);
        selectedWater_box = new QComboBox(selectedWater);
        selectedWater_box->setObjectName(QStringLiteral("selectedWater_box"));
        selectedWater_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedWater_box->sizePolicy().hasHeightForWidth());
        selectedWater_box->setSizePolicy(sizePolicy);
        selectedWater_box->setMinimumSize(QSize(115, 22));
        selectedWater_box->setMaximumSize(QSize(115, 22));
        selectedWater_box->setBaseSize(QSize(115, 25));
        selectedWater_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedWater_box->setFrame(false);

        meshLayout->addWidget(selectedWater);

        waveNumber = new QFrame(layoutWidget2);
        waveNumber->setObjectName(QStringLiteral("waveNumber"));
        waveNumber->setMinimumSize(QSize(225, 35));
        waveNumber->setMaximumSize(QSize(225, 35));
        waveNumber->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        waveNumber->setFrameShape(QFrame::NoFrame);
        waveNumber->setFrameShadow(QFrame::Plain);
        waveNumber->setLineWidth(0);
        waveNumber_lbl = new QLabel(waveNumber);
        waveNumber_lbl->setObjectName(QStringLiteral("waveNumber_lbl"));
        waveNumber_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(waveNumber_lbl->sizePolicy().hasHeightForWidth());
        waveNumber_lbl->setSizePolicy(sizePolicy);
        waveNumber_lbl->setMinimumSize(QSize(100, 30));
        waveNumber_lbl->setMaximumSize(QSize(100, 30));
        waveNumber_lbl->setFont(font1);
        waveNumber_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        waveNumber_lbl->setMargin(1);
        waveNumber_value = new QDoubleSpinBox(waveNumber);
        waveNumber_value->setObjectName(QStringLiteral("waveNumber_value"));
        waveNumber_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(waveNumber_value->sizePolicy().hasHeightForWidth());
        waveNumber_value->setSizePolicy(sizePolicy);
        waveNumber_value->setMinimumSize(QSize(86, 25));
        waveNumber_value->setMaximumSize(QSize(86, 25));
        waveNumber_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        waveNumber_value->setFrame(false);
        waveNumber_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        waveNumber_value->setDecimals(0);
        waveNumber_value->setMinimum(-9999);
        waveNumber_value->setMaximum(9999);
        waveNumber_value->setSingleStep(0.05);
        waveNumber_dial = new QDial(waveNumber);
        waveNumber_dial->setObjectName(QStringLiteral("waveNumber_dial"));
        waveNumber_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(waveNumber_dial->sizePolicy().hasHeightForWidth());
        waveNumber_dial->setSizePolicy(sizePolicy);
        waveNumber_dial->setMinimumSize(QSize(30, 30));
        waveNumber_dial->setMaximumSize(QSize(30, 30));
        waveNumber_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        waveNumber_dial->setMinimum(-10000);
        waveNumber_dial->setMaximum(10000);

        meshLayout->addWidget(waveNumber);

        wave = new QFrame(layoutWidget2);
        wave->setObjectName(QStringLiteral("wave"));
        wave->setMinimumSize(QSize(225, 35));
        wave->setMaximumSize(QSize(225, 35));
        wave->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        wave->setFrameShape(QFrame::NoFrame);
        wave->setFrameShadow(QFrame::Plain);
        wave->setLineWidth(0);
        wave_value = new QDoubleSpinBox(wave);
        wave_value->setObjectName(QStringLiteral("wave_value"));
        wave_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(wave_value->sizePolicy().hasHeightForWidth());
        wave_value->setSizePolicy(sizePolicy);
        wave_value->setMinimumSize(QSize(86, 25));
        wave_value->setMaximumSize(QSize(86, 25));
        wave_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        wave_value->setFrame(false);
        wave_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        wave_value->setDecimals(3);
        wave_value->setMinimum(-9999);
        wave_value->setMaximum(9999);
        wave_value->setSingleStep(0.05);
        wave_dial = new QDial(wave);
        wave_dial->setObjectName(QStringLiteral("wave_dial"));
        wave_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(wave_dial->sizePolicy().hasHeightForWidth());
        wave_dial->setSizePolicy(sizePolicy);
        wave_dial->setMinimumSize(QSize(30, 30));
        wave_dial->setMaximumSize(QSize(30, 30));
        wave_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        wave_dial->setMinimum(-10000);
        wave_dial->setMaximum(10000);
        wave_box = new QComboBox(wave);
        wave_box->setObjectName(QStringLiteral("wave_box"));
        wave_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(wave_box->sizePolicy().hasHeightForWidth());
        wave_box->setSizePolicy(sizePolicy);
        wave_box->setMinimumSize(QSize(95, 22));
        wave_box->setMaximumSize(QSize(90, 22));
        wave_box->setBaseSize(QSize(90, 25));
        wave_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        wave_box->setFrame(false);

        meshLayout->addWidget(wave);

        water = new QFrame(layoutWidget2);
        water->setObjectName(QStringLiteral("water"));
        water->setMinimumSize(QSize(225, 35));
        water->setMaximumSize(QSize(225, 35));
        water->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        water->setFrameShape(QFrame::NoFrame);
        water->setFrameShadow(QFrame::Plain);
        water->setLineWidth(0);
        water_value = new QDoubleSpinBox(water);
        water_value->setObjectName(QStringLiteral("water_value"));
        water_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(water_value->sizePolicy().hasHeightForWidth());
        water_value->setSizePolicy(sizePolicy);
        water_value->setMinimumSize(QSize(86, 25));
        water_value->setMaximumSize(QSize(86, 25));
        water_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        water_value->setFrame(false);
        water_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        water_value->setDecimals(3);
        water_value->setMinimum(-9999);
        water_value->setMaximum(9999);
        water_value->setSingleStep(0.05);
        water_dial = new QDial(water);
        water_dial->setObjectName(QStringLiteral("water_dial"));
        water_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(water_dial->sizePolicy().hasHeightForWidth());
        water_dial->setSizePolicy(sizePolicy);
        water_dial->setMinimumSize(QSize(30, 30));
        water_dial->setMaximumSize(QSize(30, 30));
        water_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        water_dial->setMinimum(-10000);
        water_dial->setMaximum(10000);
        water_box = new QComboBox(water);
        water_box->setObjectName(QStringLiteral("water_box"));
        water_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(water_box->sizePolicy().hasHeightForWidth());
        water_box->setSizePolicy(sizePolicy);
        water_box->setMinimumSize(QSize(95, 22));
        water_box->setMaximumSize(QSize(90, 22));
        water_box->setBaseSize(QSize(90, 25));
        water_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        water_box->setMaxVisibleItems(30);
        water_box->setFrame(false);

        meshLayout->addWidget(water);

        _meshSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        meshLayout->addItem(_meshSpacer_3);

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

        particleAmount = new QFrame(layoutWidget2);
        particleAmount->setObjectName(QStringLiteral("particleAmount"));
        particleAmount->setMinimumSize(QSize(225, 35));
        particleAmount->setMaximumSize(QSize(225, 35));
        particleAmount->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        particleAmount->setFrameShape(QFrame::NoFrame);
        particleAmount->setFrameShadow(QFrame::Plain);
        particleAmount->setLineWidth(0);
        particleAmount_lbl = new QLabel(particleAmount);
        particleAmount_lbl->setObjectName(QStringLiteral("particleAmount_lbl"));
        particleAmount_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(particleAmount_lbl->sizePolicy().hasHeightForWidth());
        particleAmount_lbl->setSizePolicy(sizePolicy);
        particleAmount_lbl->setMinimumSize(QSize(100, 30));
        particleAmount_lbl->setMaximumSize(QSize(100, 30));
        particleAmount_lbl->setFont(font1);
        particleAmount_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleAmount_lbl->setMargin(1);
        particleAmount_value = new QDoubleSpinBox(particleAmount);
        particleAmount_value->setObjectName(QStringLiteral("particleAmount_value"));
        particleAmount_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(particleAmount_value->sizePolicy().hasHeightForWidth());
        particleAmount_value->setSizePolicy(sizePolicy);
        particleAmount_value->setMinimumSize(QSize(86, 25));
        particleAmount_value->setMaximumSize(QSize(86, 25));
        particleAmount_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        particleAmount_value->setFrame(false);
        particleAmount_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        particleAmount_value->setDecimals(0);
        particleAmount_value->setMinimum(-9999);
        particleAmount_value->setMaximum(9999);
        particleAmount_value->setSingleStep(1);
        particleAmount_dial = new QDial(particleAmount);
        particleAmount_dial->setObjectName(QStringLiteral("particleAmount_dial"));
        particleAmount_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(particleAmount_dial->sizePolicy().hasHeightForWidth());
        particleAmount_dial->setSizePolicy(sizePolicy);
        particleAmount_dial->setMinimumSize(QSize(30, 30));
        particleAmount_dial->setMaximumSize(QSize(30, 30));
        particleAmount_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        particleAmount_dial->setMinimum(-10000);
        particleAmount_dial->setMaximum(10000);

        meshLayout->addWidget(particleAmount);

        emitter = new QFrame(layoutWidget2);
        emitter->setObjectName(QStringLiteral("emitter"));
        emitter->setMinimumSize(QSize(225, 35));
        emitter->setMaximumSize(QSize(225, 35));
        emitter->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        emitter->setFrameShape(QFrame::NoFrame);
        emitter->setFrameShadow(QFrame::Plain);
        emitter->setLineWidth(0);
        emitter_value = new QDoubleSpinBox(emitter);
        emitter_value->setObjectName(QStringLiteral("emitter_value"));
        emitter_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(emitter_value->sizePolicy().hasHeightForWidth());
        emitter_value->setSizePolicy(sizePolicy);
        emitter_value->setMinimumSize(QSize(86, 25));
        emitter_value->setMaximumSize(QSize(86, 25));
        emitter_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitter_value->setFrame(false);
        emitter_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        emitter_value->setDecimals(3);
        emitter_value->setMinimum(-9999);
        emitter_value->setMaximum(9999);
        emitter_value->setSingleStep(0.05);
        emitter_dial = new QDial(emitter);
        emitter_dial->setObjectName(QStringLiteral("emitter_dial"));
        emitter_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(emitter_dial->sizePolicy().hasHeightForWidth());
        emitter_dial->setSizePolicy(sizePolicy);
        emitter_dial->setMinimumSize(QSize(30, 30));
        emitter_dial->setMaximumSize(QSize(30, 30));
        emitter_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        emitter_dial->setMinimum(-10000);
        emitter_dial->setMaximum(10000);
        emitter_box = new QComboBox(emitter);
        emitter_box->setObjectName(QStringLiteral("emitter_box"));
        emitter_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(emitter_box->sizePolicy().hasHeightForWidth());
        emitter_box->setSizePolicy(sizePolicy);
        emitter_box->setMinimumSize(QSize(95, 22));
        emitter_box->setMaximumSize(QSize(90, 22));
        emitter_box->setBaseSize(QSize(90, 25));
        emitter_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitter_box->setMaxVisibleItems(20);
        emitter_box->setFrame(false);

        meshLayout->addWidget(emitter);

        emitterColour = new QFrame(layoutWidget2);
        emitterColour->setObjectName(QStringLiteral("emitterColour"));
        emitterColour->setMinimumSize(QSize(225, 35));
        emitterColour->setMaximumSize(QSize(225, 35));
        emitterColour->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        emitterColour->setFrameShape(QFrame::NoFrame);
        emitterColour->setFrameShadow(QFrame::Plain);
        emitterColour->setLineWidth(0);
        emitterColour_value = new QDoubleSpinBox(emitterColour);
        emitterColour_value->setObjectName(QStringLiteral("emitterColour_value"));
        emitterColour_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(emitterColour_value->sizePolicy().hasHeightForWidth());
        emitterColour_value->setSizePolicy(sizePolicy);
        emitterColour_value->setMinimumSize(QSize(86, 25));
        emitterColour_value->setMaximumSize(QSize(86, 25));
        emitterColour_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterColour_value->setFrame(false);
        emitterColour_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        emitterColour_value->setDecimals(3);
        emitterColour_value->setMinimum(-9999);
        emitterColour_value->setMaximum(9999);
        emitterColour_value->setSingleStep(0.05);
        emitterColour_dial = new QDial(emitterColour);
        emitterColour_dial->setObjectName(QStringLiteral("emitterColour_dial"));
        emitterColour_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(emitterColour_dial->sizePolicy().hasHeightForWidth());
        emitterColour_dial->setSizePolicy(sizePolicy);
        emitterColour_dial->setMinimumSize(QSize(30, 30));
        emitterColour_dial->setMaximumSize(QSize(30, 30));
        emitterColour_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        emitterColour_dial->setMinimum(-10000);
        emitterColour_dial->setMaximum(10000);
        emitterColour_box = new QComboBox(emitterColour);
        emitterColour_box->setObjectName(QStringLiteral("emitterColour_box"));
        emitterColour_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(emitterColour_box->sizePolicy().hasHeightForWidth());
        emitterColour_box->setSizePolicy(sizePolicy);
        emitterColour_box->setMinimumSize(QSize(95, 22));
        emitterColour_box->setMaximumSize(QSize(90, 22));
        emitterColour_box->setBaseSize(QSize(90, 25));
        emitterColour_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterColour_box->setMaxVisibleItems(20);
        emitterColour_box->setFrame(false);

        meshLayout->addWidget(emitterColour);

        emitterMinMax = new QFrame(layoutWidget2);
        emitterMinMax->setObjectName(QStringLiteral("emitterMinMax"));
        emitterMinMax->setMinimumSize(QSize(225, 35));
        emitterMinMax->setMaximumSize(QSize(225, 35));
        emitterMinMax->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        emitterMinMax->setFrameShape(QFrame::NoFrame);
        emitterMinMax->setFrameShadow(QFrame::Plain);
        emitterMinMax->setLineWidth(0);
        emitterMinMax_value = new QDoubleSpinBox(emitterMinMax);
        emitterMinMax_value->setObjectName(QStringLiteral("emitterMinMax_value"));
        emitterMinMax_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(emitterMinMax_value->sizePolicy().hasHeightForWidth());
        emitterMinMax_value->setSizePolicy(sizePolicy);
        emitterMinMax_value->setMinimumSize(QSize(86, 25));
        emitterMinMax_value->setMaximumSize(QSize(86, 25));
        emitterMinMax_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterMinMax_value->setFrame(false);
        emitterMinMax_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        emitterMinMax_value->setDecimals(3);
        emitterMinMax_value->setMinimum(-9999);
        emitterMinMax_value->setMaximum(9999);
        emitterMinMax_value->setSingleStep(0.05);
        emitterMinMax_dial = new QDial(emitterMinMax);
        emitterMinMax_dial->setObjectName(QStringLiteral("emitterMinMax_dial"));
        emitterMinMax_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(emitterMinMax_dial->sizePolicy().hasHeightForWidth());
        emitterMinMax_dial->setSizePolicy(sizePolicy);
        emitterMinMax_dial->setMinimumSize(QSize(30, 30));
        emitterMinMax_dial->setMaximumSize(QSize(30, 30));
        emitterMinMax_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        emitterMinMax_dial->setMinimum(-10000);
        emitterMinMax_dial->setMaximum(10000);
        emitterMinMax_box = new QComboBox(emitterMinMax);
        emitterMinMax_box->setObjectName(QStringLiteral("emitterMinMax_box"));
        emitterMinMax_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(emitterMinMax_box->sizePolicy().hasHeightForWidth());
        emitterMinMax_box->setSizePolicy(sizePolicy);
        emitterMinMax_box->setMinimumSize(QSize(95, 22));
        emitterMinMax_box->setMaximumSize(QSize(90, 22));
        emitterMinMax_box->setBaseSize(QSize(90, 25));
        emitterMinMax_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterMinMax_box->setFrame(false);

        meshLayout->addWidget(emitterMinMax);

        pauseEmission = new QFrame(layoutWidget2);
        pauseEmission->setObjectName(QStringLiteral("pauseEmission"));
        sizePolicy.setHeightForWidth(pauseEmission->sizePolicy().hasHeightForWidth());
        pauseEmission->setSizePolicy(sizePolicy);
        pauseEmission->setMinimumSize(QSize(225, 35));
        pauseEmission->setMaximumSize(QSize(225, 35));
        pauseEmission->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        pauseEmission->setFrameShape(QFrame::Panel);
        pauseEmission->setFrameShadow(QFrame::Raised);
        pauseEmission_btn = new QPushButton(pauseEmission);
        pauseEmission_btn->setObjectName(QStringLiteral("pauseEmission_btn"));
        pauseEmission_btn->setGeometry(QRect(5, 4, 216, 25));
        pauseEmission_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        pauseEmission_btn->setDefault(false);

        meshLayout->addWidget(pauseEmission);

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

        dofFade = new QFrame(layoutWidget3);
        dofFade->setObjectName(QStringLiteral("dofFade"));
        dofFade->setMinimumSize(QSize(225, 35));
        dofFade->setMaximumSize(QSize(225, 35));
        dofFade->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        dofFade->setFrameShape(QFrame::NoFrame);
        dofFade->setFrameShadow(QFrame::Plain);
        dofFade->setLineWidth(0);
        dofFade_lbl = new QLabel(dofFade);
        dofFade_lbl->setObjectName(QStringLiteral("dofFade_lbl"));
        dofFade_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(dofFade_lbl->sizePolicy().hasHeightForWidth());
        dofFade_lbl->setSizePolicy(sizePolicy);
        dofFade_lbl->setMinimumSize(QSize(100, 30));
        dofFade_lbl->setMaximumSize(QSize(100, 30));
        dofFade_lbl->setFont(font1);
        dofFade_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        dofFade_lbl->setMargin(1);
        dofFade_value = new QDoubleSpinBox(dofFade);
        dofFade_value->setObjectName(QStringLiteral("dofFade_value"));
        dofFade_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(dofFade_value->sizePolicy().hasHeightForWidth());
        dofFade_value->setSizePolicy(sizePolicy);
        dofFade_value->setMinimumSize(QSize(86, 25));
        dofFade_value->setMaximumSize(QSize(86, 25));
        dofFade_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        dofFade_value->setFrame(false);
        dofFade_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        dofFade_value->setDecimals(3);
        dofFade_value->setMinimum(-9999);
        dofFade_value->setMaximum(9999);
        dofFade_value->setSingleStep(0.05);
        dofFade_dial = new QDial(dofFade);
        dofFade_dial->setObjectName(QStringLiteral("dofFade_dial"));
        dofFade_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(dofFade_dial->sizePolicy().hasHeightForWidth());
        dofFade_dial->setSizePolicy(sizePolicy);
        dofFade_dial->setMinimumSize(QSize(30, 30));
        dofFade_dial->setMaximumSize(QSize(30, 30));
        dofFade_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        dofFade_dial->setMinimum(-10000);
        dofFade_dial->setMaximum(10000);

        postLayout->addWidget(dofFade);

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

        saturation = new QFrame(layoutWidget3);
        saturation->setObjectName(QStringLiteral("saturation"));
        saturation->setMinimumSize(QSize(225, 35));
        saturation->setMaximumSize(QSize(225, 35));
        saturation->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        saturation->setFrameShape(QFrame::NoFrame);
        saturation->setFrameShadow(QFrame::Plain);
        saturation->setLineWidth(0);
        saturation_lbl = new QLabel(saturation);
        saturation_lbl->setObjectName(QStringLiteral("saturation_lbl"));
        saturation_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(saturation_lbl->sizePolicy().hasHeightForWidth());
        saturation_lbl->setSizePolicy(sizePolicy);
        saturation_lbl->setMinimumSize(QSize(100, 30));
        saturation_lbl->setMaximumSize(QSize(100, 30));
        saturation_lbl->setFont(font1);
        saturation_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        saturation_lbl->setMargin(1);
        saturation_value = new QDoubleSpinBox(saturation);
        saturation_value->setObjectName(QStringLiteral("saturation_value"));
        saturation_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(saturation_value->sizePolicy().hasHeightForWidth());
        saturation_value->setSizePolicy(sizePolicy);
        saturation_value->setMinimumSize(QSize(86, 25));
        saturation_value->setMaximumSize(QSize(86, 25));
        saturation_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        saturation_value->setFrame(false);
        saturation_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        saturation_value->setDecimals(3);
        saturation_value->setMinimum(-9999);
        saturation_value->setMaximum(9999);
        saturation_value->setSingleStep(0.05);
        saturation_dial = new QDial(saturation);
        saturation_dial->setObjectName(QStringLiteral("saturation_dial"));
        saturation_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(saturation_dial->sizePolicy().hasHeightForWidth());
        saturation_dial->setSizePolicy(sizePolicy);
        saturation_dial->setMinimumSize(QSize(30, 30));
        saturation_dial->setMaximumSize(QSize(30, 30));
        saturation_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        saturation_dial->setMinimum(-10000);
        saturation_dial->setMaximum(10000);

        postLayout->addWidget(saturation);

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

        fog = new QFrame(layoutWidget3);
        fog->setObjectName(QStringLiteral("fog"));
        fog->setMinimumSize(QSize(225, 35));
        fog->setMaximumSize(QSize(225, 35));
        fog->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        fog->setFrameShape(QFrame::NoFrame);
        fog->setFrameShadow(QFrame::Plain);
        fog->setLineWidth(0);
        fog_value = new QDoubleSpinBox(fog);
        fog_value->setObjectName(QStringLiteral("fog_value"));
        fog_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(fog_value->sizePolicy().hasHeightForWidth());
        fog_value->setSizePolicy(sizePolicy);
        fog_value->setMinimumSize(QSize(86, 25));
        fog_value->setMaximumSize(QSize(86, 25));
        fog_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        fog_value->setFrame(false);
        fog_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        fog_value->setDecimals(3);
        fog_value->setMinimum(-9999);
        fog_value->setMaximum(9999);
        fog_value->setSingleStep(0.05);
        fog_dial = new QDial(fog);
        fog_dial->setObjectName(QStringLiteral("fog_dial"));
        fog_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(fog_dial->sizePolicy().hasHeightForWidth());
        fog_dial->setSizePolicy(sizePolicy);
        fog_dial->setMinimumSize(QSize(30, 30));
        fog_dial->setMaximumSize(QSize(30, 30));
        fog_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        fog_dial->setMinimum(-10000);
        fog_dial->setMaximum(10000);
        fog_box = new QComboBox(fog);
        fog_box->setObjectName(QStringLiteral("fog_box"));
        fog_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(fog_box->sizePolicy().hasHeightForWidth());
        fog_box->setSizePolicy(sizePolicy);
        fog_box->setMinimumSize(QSize(95, 22));
        fog_box->setMaximumSize(QSize(90, 22));
        fog_box->setBaseSize(QSize(90, 25));
        fog_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        fog_box->setFrame(false);

        postLayout->addWidget(fog);

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
        timer_lbl->setText(QApplication::translate("Tweaker", "Timer", 0));
        timer_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        cameraX_lbl->setText(QApplication::translate("Tweaker", "Camera X", 0));
        cameraY_lbl->setText(QApplication::translate("Tweaker", "Camera Y", 0));
        cameraZ_lbl->setText(QApplication::translate("Tweaker", "Camera Z", 0));
        cameraPitch_lbl->setText(QApplication::translate("Tweaker", "Camera Pitch", 0));
        cameraYaw_lbl->setText(QApplication::translate("Tweaker", "Camera Yaw", 0));
        cameraRoll_lbl->setText(QApplication::translate("Tweaker", "Camera Roll", 0));
        reloadScene_btn->setText(QApplication::translate("Tweaker", "Reload Scene", 0));
        saveScene_btn->setText(QApplication::translate("Tweaker", "Save Scene to File", 0));
        savePost_btn->setText(QApplication::translate("Tweaker", "Save Post Processing to File", 0));
        TabMenu->setTabText(TabMenu->indexOf(Scene), QApplication::translate("Tweaker", "Scene", 0));
        lightDiagnostics_btn->setText(QApplication::translate("Tweaker", "Toggle Light Diagnostics", 0));
        selectedLight_lbl->setText(QApplication::translate("Tweaker", "Light", 0));
        positionX_lbl->setText(QApplication::translate("Tweaker", "Position X", 0));
        positionY_lbl->setText(QApplication::translate("Tweaker", "Position Y", 0));
        positionZ_lbl->setText(QApplication::translate("Tweaker", "Position Z", 0));
        attenuationX_lbl->setText(QApplication::translate("Tweaker", "Attenuation X", 0));
        attenuationY_lbl->setText(QApplication::translate("Tweaker", "Attenuation Y", 0));
        attenuationZ_lbl->setText(QApplication::translate("Tweaker", "Attenuation Z", 0));
        specularity_lbl->setText(QApplication::translate("Tweaker", "Specularity", 0));
        TabMenu->setTabText(TabMenu->indexOf(Light), QApplication::translate("Tweaker", "Light", 0));
        selectedMesh_lbl->setText(QApplication::translate("Tweaker", "Mesh", 0));
        shader_lbl->setText(QApplication::translate("Tweaker", "Shader", 0));
        shader_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        instances_lbl->setText(QApplication::translate("Tweaker", "Instances", 0));
        instances_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        selectedWater_lbl->setText(QApplication::translate("Tweaker", "Water", 0));
        waveNumber_lbl->setText(QApplication::translate("Tweaker", "Wave Number", 0));
        selectedParticles_lbl->setText(QApplication::translate("Tweaker", "Emitter", 0));
        particleAmount_lbl->setText(QApplication::translate("Tweaker", "Amount", 0));
        pauseEmission_btn->setText(QApplication::translate("Tweaker", "Pause Emission", 0));
        TabMenu->setTabText(TabMenu->indexOf(Mesh), QApplication::translate("Tweaker", "Mesh", 0));
        depthNear_lbl->setText(QApplication::translate("Tweaker", "Depth Near", 0));
        depthFar_lbl->setText(QApplication::translate("Tweaker", "Depth Far", 0));
        blurring_lbl->setText(QApplication::translate("Tweaker", "Blur Amount", 0));
        blurStep_lbl->setText(QApplication::translate("Tweaker", "Blur Step", 0));
        glowIntensity_lbl->setText(QApplication::translate("Tweaker", "Glow Intensity", 0));
        dofDistance_lbl->setText(QApplication::translate("Tweaker", "DOF Start", 0));
        dofFade_lbl->setText(QApplication::translate("Tweaker", "DOF Fade", 0));
        contrast_lbl->setText(QApplication::translate("Tweaker", "Contrast", 0));
        saturation_lbl->setText(QApplication::translate("Tweaker", "Saturation", 0));
        TabMenu->setTabText(TabMenu->indexOf(Post), QApplication::translate("Tweaker", "Post", 0));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
