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
    QFrame *cameraY;
    QLabel *cameraY_lbl;
    QDoubleSpinBox *cameraY_value;
    QFrame *cameraZ;
    QLabel *cameraZ_lbl;
    QDoubleSpinBox *cameraZ_value;
    QFrame *cameraPitch;
    QLabel *cameraPitch_lbl;
    QDoubleSpinBox *cameraPitch_value;
    QFrame *cameraYaw;
    QLabel *cameraYaw_lbl;
    QDoubleSpinBox *cameraYaw_value;
    QFrame *cameraRoll;
    QLabel *cameraRoll_lbl;
    QDoubleSpinBox *cameraRoll_value;
    QFrame *reloadScene;
    QPushButton *reloadScene_btn;
    QFrame *reloadEngine;
    QPushButton *reloadEngine_btn;
    QFrame *saveScene;
    QPushButton *saveScene_btn;
    QSpacerItem *_sceneSpacer;
    QWidget *Area;
    QWidget *layoutWidget1;
    QVBoxLayout *areaLayout;
    QFrame *selectedTexture;
    QLabel *selectedTexture_lbl;
    QComboBox *selectedTexture_box;
    QFrame *displayTexture;
    QPushButton *displayTexture_btn;
    QFrame *textureAttributes;
    QDoubleSpinBox *textureAttributes_value;
    QDial *textureAttributes_dial;
    QComboBox *textureAttributes_box;
    QFrame *reloadTexture;
    QPushButton *reloadTexture_btn;
    QSpacerItem *_meshSpacer_5;
    QFrame *selectedTerrain;
    QLabel *selectedTerrain_lbl;
    QComboBox *selectedTerrain_box;
    QFrame *terrainShader;
    QLabel *terrainShader_lbl;
    QLabel *terrainShader_text;
    QFrame *terrainAttributes;
    QDoubleSpinBox *terrainAttributes_value;
    QDial *terrainAttributes_dial;
    QComboBox *terrainAttributes_box;
    QFrame *reloadTerrain;
    QPushButton *reloadTerrain_btn;
    QSpacerItem *_areaSpacer;
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
    QFrame *postImage;
    QLabel *postImage_lbl;
    QComboBox *postImage_box;
    QFrame *post;
    QDoubleSpinBox *post_value;
    QDial *post_dial;
    QComboBox *post_box;
    QFrame *correction;
    QDoubleSpinBox *correction_value;
    QDial *correction_dial;
    QComboBox *correction_box;
    QFrame *fog;
    QDoubleSpinBox *fog_value;
    QDial *fog_dial;
    QComboBox *fog_box;
    QSpacerItem *_meshSpacer_4;
    QFrame *lightSelected;
    QLabel *lightSelected_lbl;
    QComboBox *lightSelected_box;
    QFrame *light_active;
    QLabel *light_active_lbl;
    QDoubleSpinBox *light_active_value;
    QDial *light_active_dial;
    QFrame *light_position;
    QDoubleSpinBox *light_position_value;
    QDial *light_position_dial;
    QComboBox *light_position_box;
    QFrame *light_attenuation;
    QDoubleSpinBox *light_attenuation_value;
    QDial *light_attenuation_dial;
    QComboBox *light_attenuation_box;
    QFrame *light_colour;
    QDoubleSpinBox *light_colour_value;
    QDial *light_colour_dial;
    QComboBox *light_colour_box;
    QFrame *light_render_only;
    QPushButton *light_render_only_btn;
    QFrame *light_diagnostics;
    QPushButton *light_diagnostics_btn;
    QFrame *toggleWireframe;
    QPushButton *toggleWireframe_btn;
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
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
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
        cameraX_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraX_value->sizePolicy().hasHeightForWidth());
        cameraX_value->setSizePolicy(sizePolicy);
        cameraX_value->setMinimumSize(QSize(116, 25));
        cameraX_value->setMaximumSize(QSize(116, 25));
        cameraX_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraX_value->setFrame(false);
        cameraX_value->setReadOnly(true);
        cameraX_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraX_value->setDecimals(3);
        cameraX_value->setMinimum(-9999);
        cameraX_value->setMaximum(9999);
        cameraX_value->setSingleStep(0.05);

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
        cameraY_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraY_value->sizePolicy().hasHeightForWidth());
        cameraY_value->setSizePolicy(sizePolicy);
        cameraY_value->setMinimumSize(QSize(116, 25));
        cameraY_value->setMaximumSize(QSize(116, 25));
        cameraY_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraY_value->setFrame(false);
        cameraY_value->setReadOnly(true);
        cameraY_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraY_value->setDecimals(3);
        cameraY_value->setMinimum(-9999);
        cameraY_value->setMaximum(9999);
        cameraY_value->setSingleStep(0.05);

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
        cameraZ_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraZ_value->sizePolicy().hasHeightForWidth());
        cameraZ_value->setSizePolicy(sizePolicy);
        cameraZ_value->setMinimumSize(QSize(116, 25));
        cameraZ_value->setMaximumSize(QSize(116, 25));
        cameraZ_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraZ_value->setFrame(false);
        cameraZ_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraZ_value->setDecimals(3);
        cameraZ_value->setMinimum(-9999);
        cameraZ_value->setMaximum(9999);
        cameraZ_value->setSingleStep(0.05);

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
        cameraPitch_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraPitch_value->sizePolicy().hasHeightForWidth());
        cameraPitch_value->setSizePolicy(sizePolicy);
        cameraPitch_value->setMinimumSize(QSize(116, 25));
        cameraPitch_value->setMaximumSize(QSize(116, 25));
        cameraPitch_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraPitch_value->setFrame(false);
        cameraPitch_value->setReadOnly(true);
        cameraPitch_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraPitch_value->setDecimals(3);
        cameraPitch_value->setMinimum(-9999);
        cameraPitch_value->setMaximum(9999);
        cameraPitch_value->setSingleStep(0.05);

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
        cameraYaw_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraYaw_value->sizePolicy().hasHeightForWidth());
        cameraYaw_value->setSizePolicy(sizePolicy);
        cameraYaw_value->setMinimumSize(QSize(116, 25));
        cameraYaw_value->setMaximumSize(QSize(116, 25));
        cameraYaw_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraYaw_value->setFrame(false);
        cameraYaw_value->setReadOnly(true);
        cameraYaw_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraYaw_value->setDecimals(3);
        cameraYaw_value->setMinimum(-9999);
        cameraYaw_value->setMaximum(9999);
        cameraYaw_value->setSingleStep(0.05);

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
        cameraRoll_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraRoll_value->sizePolicy().hasHeightForWidth());
        cameraRoll_value->setSizePolicy(sizePolicy);
        cameraRoll_value->setMinimumSize(QSize(116, 25));
        cameraRoll_value->setMaximumSize(QSize(116, 25));
        cameraRoll_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        cameraRoll_value->setFrame(false);
        cameraRoll_value->setReadOnly(true);
        cameraRoll_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraRoll_value->setDecimals(3);
        cameraRoll_value->setMinimum(-9999);
        cameraRoll_value->setMaximum(9999);
        cameraRoll_value->setSingleStep(0.05);

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

        reloadEngine = new QFrame(layoutWidget);
        reloadEngine->setObjectName(QStringLiteral("reloadEngine"));
        sizePolicy.setHeightForWidth(reloadEngine->sizePolicy().hasHeightForWidth());
        reloadEngine->setSizePolicy(sizePolicy);
        reloadEngine->setMinimumSize(QSize(225, 35));
        reloadEngine->setMaximumSize(QSize(225, 35));
        reloadEngine->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadEngine->setFrameShape(QFrame::Panel);
        reloadEngine->setFrameShadow(QFrame::Raised);
        reloadEngine_btn = new QPushButton(reloadEngine);
        reloadEngine_btn->setObjectName(QStringLiteral("reloadEngine_btn"));
        reloadEngine_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadEngine_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        reloadEngine_btn->setDefault(false);

        sceneLayout->addWidget(reloadEngine);

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

        _sceneSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sceneLayout->addItem(_sceneSpacer);

        TabMenu->addTab(Scene, QString());
        Area = new QWidget();
        Area->setObjectName(QStringLiteral("Area"));
        Area->setStyleSheet(QStringLiteral("background-color: rgb(230, 230, 230);"));
        layoutWidget1 = new QWidget(Area);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 260, 626));
        areaLayout = new QVBoxLayout(layoutWidget1);
        areaLayout->setSpacing(2);
        areaLayout->setObjectName(QStringLiteral("areaLayout"));
        areaLayout->setContentsMargins(4, 4, 0, 0);
        selectedTexture = new QFrame(layoutWidget1);
        selectedTexture->setObjectName(QStringLiteral("selectedTexture"));
        sizePolicy.setHeightForWidth(selectedTexture->sizePolicy().hasHeightForWidth());
        selectedTexture->setSizePolicy(sizePolicy);
        selectedTexture->setMinimumSize(QSize(225, 35));
        selectedTexture->setMaximumSize(QSize(225, 35));
        selectedTexture->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedTexture->setFrameShape(QFrame::Panel);
        selectedTexture->setFrameShadow(QFrame::Raised);
        selectedTexture_lbl = new QLabel(selectedTexture);
        selectedTexture_lbl->setObjectName(QStringLiteral("selectedTexture_lbl"));
        selectedTexture_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedTexture_lbl->sizePolicy().hasHeightForWidth());
        selectedTexture_lbl->setSizePolicy(sizePolicy);
        selectedTexture_lbl->setMinimumSize(QSize(100, 30));
        selectedTexture_lbl->setMaximumSize(QSize(100, 30));
        selectedTexture_lbl->setFont(font);
        selectedTexture_lbl->setMargin(1);
        selectedTexture_box = new QComboBox(selectedTexture);
        selectedTexture_box->setObjectName(QStringLiteral("selectedTexture_box"));
        selectedTexture_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedTexture_box->sizePolicy().hasHeightForWidth());
        selectedTexture_box->setSizePolicy(sizePolicy);
        selectedTexture_box->setMinimumSize(QSize(115, 22));
        selectedTexture_box->setMaximumSize(QSize(115, 22));
        selectedTexture_box->setBaseSize(QSize(115, 25));
        selectedTexture_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedTexture_box->setEditable(false);
        selectedTexture_box->setMinimumContentsLength(0);
        selectedTexture_box->setFrame(false);

        areaLayout->addWidget(selectedTexture);

        displayTexture = new QFrame(layoutWidget1);
        displayTexture->setObjectName(QStringLiteral("displayTexture"));
        sizePolicy.setHeightForWidth(displayTexture->sizePolicy().hasHeightForWidth());
        displayTexture->setSizePolicy(sizePolicy);
        displayTexture->setMinimumSize(QSize(225, 225));
        displayTexture->setMaximumSize(QSize(225, 225));
        displayTexture->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        displayTexture->setFrameShape(QFrame::Panel);
        displayTexture->setFrameShadow(QFrame::Raised);
        displayTexture_btn = new QPushButton(displayTexture);
        displayTexture_btn->setObjectName(QStringLiteral("displayTexture_btn"));
        displayTexture_btn->setGeometry(QRect(5, 5, 216, 216));
        displayTexture_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        displayTexture_btn->setAutoRepeat(true);
        displayTexture_btn->setDefault(false);

        areaLayout->addWidget(displayTexture);

        textureAttributes = new QFrame(layoutWidget1);
        textureAttributes->setObjectName(QStringLiteral("textureAttributes"));
        textureAttributes->setMinimumSize(QSize(225, 35));
        textureAttributes->setMaximumSize(QSize(225, 35));
        textureAttributes->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        textureAttributes->setFrameShape(QFrame::NoFrame);
        textureAttributes->setFrameShadow(QFrame::Plain);
        textureAttributes->setLineWidth(0);
        textureAttributes_value = new QDoubleSpinBox(textureAttributes);
        textureAttributes_value->setObjectName(QStringLiteral("textureAttributes_value"));
        textureAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(textureAttributes_value->sizePolicy().hasHeightForWidth());
        textureAttributes_value->setSizePolicy(sizePolicy);
        textureAttributes_value->setMinimumSize(QSize(86, 25));
        textureAttributes_value->setMaximumSize(QSize(86, 25));
        textureAttributes_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        textureAttributes_value->setFrame(false);
        textureAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        textureAttributes_value->setDecimals(3);
        textureAttributes_value->setMinimum(-9999);
        textureAttributes_value->setMaximum(9999);
        textureAttributes_value->setSingleStep(0.05);
        textureAttributes_dial = new QDial(textureAttributes);
        textureAttributes_dial->setObjectName(QStringLiteral("textureAttributes_dial"));
        textureAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(textureAttributes_dial->sizePolicy().hasHeightForWidth());
        textureAttributes_dial->setSizePolicy(sizePolicy);
        textureAttributes_dial->setMinimumSize(QSize(30, 30));
        textureAttributes_dial->setMaximumSize(QSize(30, 30));
        textureAttributes_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        textureAttributes_dial->setMinimum(-10000);
        textureAttributes_dial->setMaximum(10000);
        textureAttributes_box = new QComboBox(textureAttributes);
        textureAttributes_box->setObjectName(QStringLiteral("textureAttributes_box"));
        textureAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(textureAttributes_box->sizePolicy().hasHeightForWidth());
        textureAttributes_box->setSizePolicy(sizePolicy);
        textureAttributes_box->setMinimumSize(QSize(95, 22));
        textureAttributes_box->setMaximumSize(QSize(90, 22));
        textureAttributes_box->setBaseSize(QSize(90, 25));
        textureAttributes_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        textureAttributes_box->setFrame(false);

        areaLayout->addWidget(textureAttributes);

        reloadTexture = new QFrame(layoutWidget1);
        reloadTexture->setObjectName(QStringLiteral("reloadTexture"));
        sizePolicy.setHeightForWidth(reloadTexture->sizePolicy().hasHeightForWidth());
        reloadTexture->setSizePolicy(sizePolicy);
        reloadTexture->setMinimumSize(QSize(225, 35));
        reloadTexture->setMaximumSize(QSize(225, 35));
        reloadTexture->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadTexture->setFrameShape(QFrame::Panel);
        reloadTexture->setFrameShadow(QFrame::Raised);
        reloadTexture_btn = new QPushButton(reloadTexture);
        reloadTexture_btn->setObjectName(QStringLiteral("reloadTexture_btn"));
        reloadTexture_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadTexture_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        reloadTexture_btn->setDefault(false);

        areaLayout->addWidget(reloadTexture);

        _meshSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        areaLayout->addItem(_meshSpacer_5);

        selectedTerrain = new QFrame(layoutWidget1);
        selectedTerrain->setObjectName(QStringLiteral("selectedTerrain"));
        sizePolicy.setHeightForWidth(selectedTerrain->sizePolicy().hasHeightForWidth());
        selectedTerrain->setSizePolicy(sizePolicy);
        selectedTerrain->setMinimumSize(QSize(225, 35));
        selectedTerrain->setMaximumSize(QSize(225, 35));
        selectedTerrain->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedTerrain->setFrameShape(QFrame::Panel);
        selectedTerrain->setFrameShadow(QFrame::Raised);
        selectedTerrain_lbl = new QLabel(selectedTerrain);
        selectedTerrain_lbl->setObjectName(QStringLiteral("selectedTerrain_lbl"));
        selectedTerrain_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedTerrain_lbl->sizePolicy().hasHeightForWidth());
        selectedTerrain_lbl->setSizePolicy(sizePolicy);
        selectedTerrain_lbl->setMinimumSize(QSize(100, 30));
        selectedTerrain_lbl->setMaximumSize(QSize(100, 30));
        selectedTerrain_lbl->setFont(font);
        selectedTerrain_lbl->setMargin(1);
        selectedTerrain_box = new QComboBox(selectedTerrain);
        selectedTerrain_box->setObjectName(QStringLiteral("selectedTerrain_box"));
        selectedTerrain_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedTerrain_box->sizePolicy().hasHeightForWidth());
        selectedTerrain_box->setSizePolicy(sizePolicy);
        selectedTerrain_box->setMinimumSize(QSize(115, 22));
        selectedTerrain_box->setMaximumSize(QSize(115, 22));
        selectedTerrain_box->setBaseSize(QSize(115, 25));
        selectedTerrain_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedTerrain_box->setEditable(false);
        selectedTerrain_box->setMinimumContentsLength(0);
        selectedTerrain_box->setFrame(false);

        areaLayout->addWidget(selectedTerrain);

        terrainShader = new QFrame(layoutWidget1);
        terrainShader->setObjectName(QStringLiteral("terrainShader"));
        sizePolicy.setHeightForWidth(terrainShader->sizePolicy().hasHeightForWidth());
        terrainShader->setSizePolicy(sizePolicy);
        terrainShader->setMinimumSize(QSize(225, 35));
        terrainShader->setMaximumSize(QSize(225, 35));
        terrainShader->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        terrainShader->setFrameShape(QFrame::Panel);
        terrainShader->setFrameShadow(QFrame::Raised);
        terrainShader_lbl = new QLabel(terrainShader);
        terrainShader_lbl->setObjectName(QStringLiteral("terrainShader_lbl"));
        terrainShader_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(terrainShader_lbl->sizePolicy().hasHeightForWidth());
        terrainShader_lbl->setSizePolicy(sizePolicy);
        terrainShader_lbl->setMinimumSize(QSize(100, 30));
        terrainShader_lbl->setMaximumSize(QSize(100, 30));
        terrainShader_lbl->setFont(font1);
        terrainShader_text = new QLabel(terrainShader);
        terrainShader_text->setObjectName(QStringLiteral("terrainShader_text"));
        terrainShader_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(terrainShader_text->sizePolicy().hasHeightForWidth());
        terrainShader_text->setSizePolicy(sizePolicy1);
        terrainShader_text->setMinimumSize(QSize(115, 25));
        terrainShader_text->setMaximumSize(QSize(115, 25));
        terrainShader_text->setAutoFillBackground(false);
        terrainShader_text->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
""));
        terrainShader_text->setFrameShape(QFrame::NoFrame);
        terrainShader_text->setFrameShadow(QFrame::Plain);
        terrainShader_text->setLineWidth(0);
        terrainShader_text->setMargin(3);

        areaLayout->addWidget(terrainShader);

        terrainAttributes = new QFrame(layoutWidget1);
        terrainAttributes->setObjectName(QStringLiteral("terrainAttributes"));
        terrainAttributes->setMinimumSize(QSize(225, 35));
        terrainAttributes->setMaximumSize(QSize(225, 35));
        terrainAttributes->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        terrainAttributes->setFrameShape(QFrame::NoFrame);
        terrainAttributes->setFrameShadow(QFrame::Plain);
        terrainAttributes->setLineWidth(0);
        terrainAttributes_value = new QDoubleSpinBox(terrainAttributes);
        terrainAttributes_value->setObjectName(QStringLiteral("terrainAttributes_value"));
        terrainAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(terrainAttributes_value->sizePolicy().hasHeightForWidth());
        terrainAttributes_value->setSizePolicy(sizePolicy);
        terrainAttributes_value->setMinimumSize(QSize(86, 25));
        terrainAttributes_value->setMaximumSize(QSize(86, 25));
        terrainAttributes_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        terrainAttributes_value->setFrame(false);
        terrainAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        terrainAttributes_value->setDecimals(3);
        terrainAttributes_value->setMinimum(-9999);
        terrainAttributes_value->setMaximum(9999);
        terrainAttributes_value->setSingleStep(0.05);
        terrainAttributes_dial = new QDial(terrainAttributes);
        terrainAttributes_dial->setObjectName(QStringLiteral("terrainAttributes_dial"));
        terrainAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(terrainAttributes_dial->sizePolicy().hasHeightForWidth());
        terrainAttributes_dial->setSizePolicy(sizePolicy);
        terrainAttributes_dial->setMinimumSize(QSize(30, 30));
        terrainAttributes_dial->setMaximumSize(QSize(30, 30));
        terrainAttributes_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        terrainAttributes_dial->setMinimum(-10000);
        terrainAttributes_dial->setMaximum(10000);
        terrainAttributes_box = new QComboBox(terrainAttributes);
        terrainAttributes_box->setObjectName(QStringLiteral("terrainAttributes_box"));
        terrainAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(terrainAttributes_box->sizePolicy().hasHeightForWidth());
        terrainAttributes_box->setSizePolicy(sizePolicy);
        terrainAttributes_box->setMinimumSize(QSize(95, 22));
        terrainAttributes_box->setMaximumSize(QSize(90, 22));
        terrainAttributes_box->setBaseSize(QSize(90, 25));
        terrainAttributes_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        terrainAttributes_box->setFrame(false);

        areaLayout->addWidget(terrainAttributes);

        reloadTerrain = new QFrame(layoutWidget1);
        reloadTerrain->setObjectName(QStringLiteral("reloadTerrain"));
        sizePolicy.setHeightForWidth(reloadTerrain->sizePolicy().hasHeightForWidth());
        reloadTerrain->setSizePolicy(sizePolicy);
        reloadTerrain->setMinimumSize(QSize(225, 35));
        reloadTerrain->setMaximumSize(QSize(225, 35));
        reloadTerrain->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadTerrain->setFrameShape(QFrame::Panel);
        reloadTerrain->setFrameShadow(QFrame::Raised);
        reloadTerrain_btn = new QPushButton(reloadTerrain);
        reloadTerrain_btn->setObjectName(QStringLiteral("reloadTerrain_btn"));
        reloadTerrain_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadTerrain_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        reloadTerrain_btn->setDefault(false);

        areaLayout->addWidget(reloadTerrain);

        _areaSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        areaLayout->addItem(_areaSpacer);

        TabMenu->addTab(Area, QString());
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
        postImage = new QFrame(layoutWidget3);
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
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        postImage_lbl->setFont(font2);
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

        postLayout->addWidget(postImage);

        post = new QFrame(layoutWidget3);
        post->setObjectName(QStringLiteral("post"));
        post->setMinimumSize(QSize(225, 35));
        post->setMaximumSize(QSize(225, 35));
        post->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        post->setFrameShape(QFrame::NoFrame);
        post->setFrameShadow(QFrame::Plain);
        post->setLineWidth(0);
        post_value = new QDoubleSpinBox(post);
        post_value->setObjectName(QStringLiteral("post_value"));
        post_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(post_value->sizePolicy().hasHeightForWidth());
        post_value->setSizePolicy(sizePolicy);
        post_value->setMinimumSize(QSize(86, 25));
        post_value->setMaximumSize(QSize(86, 25));
        post_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        post_value->setFrame(false);
        post_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        post_value->setDecimals(3);
        post_value->setMinimum(-9999);
        post_value->setMaximum(9999);
        post_value->setSingleStep(0.05);
        post_dial = new QDial(post);
        post_dial->setObjectName(QStringLiteral("post_dial"));
        post_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(post_dial->sizePolicy().hasHeightForWidth());
        post_dial->setSizePolicy(sizePolicy);
        post_dial->setMinimumSize(QSize(30, 30));
        post_dial->setMaximumSize(QSize(30, 30));
        post_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        post_dial->setMinimum(-10000);
        post_dial->setMaximum(10000);
        post_box = new QComboBox(post);
        post_box->setObjectName(QStringLiteral("post_box"));
        post_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(post_box->sizePolicy().hasHeightForWidth());
        post_box->setSizePolicy(sizePolicy);
        post_box->setMinimumSize(QSize(95, 22));
        post_box->setMaximumSize(QSize(90, 22));
        post_box->setBaseSize(QSize(90, 25));
        post_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        post_box->setFrame(false);

        postLayout->addWidget(post);

        correction = new QFrame(layoutWidget3);
        correction->setObjectName(QStringLiteral("correction"));
        correction->setMinimumSize(QSize(225, 35));
        correction->setMaximumSize(QSize(225, 35));
        correction->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        correction->setFrameShape(QFrame::NoFrame);
        correction->setFrameShadow(QFrame::Plain);
        correction->setLineWidth(0);
        correction_value = new QDoubleSpinBox(correction);
        correction_value->setObjectName(QStringLiteral("correction_value"));
        correction_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(correction_value->sizePolicy().hasHeightForWidth());
        correction_value->setSizePolicy(sizePolicy);
        correction_value->setMinimumSize(QSize(86, 25));
        correction_value->setMaximumSize(QSize(86, 25));
        correction_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        correction_value->setFrame(false);
        correction_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        correction_value->setDecimals(3);
        correction_value->setMinimum(-9999);
        correction_value->setMaximum(9999);
        correction_value->setSingleStep(0.05);
        correction_dial = new QDial(correction);
        correction_dial->setObjectName(QStringLiteral("correction_dial"));
        correction_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(correction_dial->sizePolicy().hasHeightForWidth());
        correction_dial->setSizePolicy(sizePolicy);
        correction_dial->setMinimumSize(QSize(30, 30));
        correction_dial->setMaximumSize(QSize(30, 30));
        correction_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        correction_dial->setMinimum(-10000);
        correction_dial->setMaximum(10000);
        correction_box = new QComboBox(correction);
        correction_box->setObjectName(QStringLiteral("correction_box"));
        correction_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(correction_box->sizePolicy().hasHeightForWidth());
        correction_box->setSizePolicy(sizePolicy);
        correction_box->setMinimumSize(QSize(95, 22));
        correction_box->setMaximumSize(QSize(90, 22));
        correction_box->setBaseSize(QSize(90, 25));
        correction_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        correction_box->setFrame(false);

        postLayout->addWidget(correction);

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

        _meshSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        postLayout->addItem(_meshSpacer_4);

        lightSelected = new QFrame(layoutWidget3);
        lightSelected->setObjectName(QStringLiteral("lightSelected"));
        sizePolicy.setHeightForWidth(lightSelected->sizePolicy().hasHeightForWidth());
        lightSelected->setSizePolicy(sizePolicy);
        lightSelected->setMinimumSize(QSize(225, 35));
        lightSelected->setMaximumSize(QSize(225, 35));
        lightSelected->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        lightSelected->setFrameShape(QFrame::Panel);
        lightSelected->setFrameShadow(QFrame::Raised);
        lightSelected_lbl = new QLabel(lightSelected);
        lightSelected_lbl->setObjectName(QStringLiteral("lightSelected_lbl"));
        lightSelected_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(lightSelected_lbl->sizePolicy().hasHeightForWidth());
        lightSelected_lbl->setSizePolicy(sizePolicy);
        lightSelected_lbl->setMinimumSize(QSize(100, 30));
        lightSelected_lbl->setMaximumSize(QSize(100, 30));
        lightSelected_lbl->setFont(font);
        lightSelected_lbl->setMargin(1);
        lightSelected_box = new QComboBox(lightSelected);
        lightSelected_box->setObjectName(QStringLiteral("lightSelected_box"));
        lightSelected_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(lightSelected_box->sizePolicy().hasHeightForWidth());
        lightSelected_box->setSizePolicy(sizePolicy);
        lightSelected_box->setMinimumSize(QSize(115, 22));
        lightSelected_box->setMaximumSize(QSize(115, 22));
        lightSelected_box->setBaseSize(QSize(115, 25));
        lightSelected_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        lightSelected_box->setEditable(false);
        lightSelected_box->setMinimumContentsLength(0);
        lightSelected_box->setFrame(false);

        postLayout->addWidget(lightSelected);

        light_active = new QFrame(layoutWidget3);
        light_active->setObjectName(QStringLiteral("light_active"));
        light_active->setMinimumSize(QSize(225, 35));
        light_active->setMaximumSize(QSize(225, 35));
        light_active->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_active->setFrameShape(QFrame::NoFrame);
        light_active->setFrameShadow(QFrame::Plain);
        light_active->setLineWidth(0);
        light_active_lbl = new QLabel(light_active);
        light_active_lbl->setObjectName(QStringLiteral("light_active_lbl"));
        light_active_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(light_active_lbl->sizePolicy().hasHeightForWidth());
        light_active_lbl->setSizePolicy(sizePolicy);
        light_active_lbl->setMinimumSize(QSize(100, 30));
        light_active_lbl->setMaximumSize(QSize(100, 30));
        light_active_lbl->setFont(font1);
        light_active_lbl->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_active_lbl->setMargin(1);
        light_active_value = new QDoubleSpinBox(light_active);
        light_active_value->setObjectName(QStringLiteral("light_active_value"));
        light_active_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_active_value->sizePolicy().hasHeightForWidth());
        light_active_value->setSizePolicy(sizePolicy);
        light_active_value->setMinimumSize(QSize(86, 25));
        light_active_value->setMaximumSize(QSize(86, 25));
        light_active_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_active_value->setFrame(false);
        light_active_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_active_value->setDecimals(3);
        light_active_value->setMinimum(0);
        light_active_value->setMaximum(1);
        light_active_value->setSingleStep(0.05);
        light_active_dial = new QDial(light_active);
        light_active_dial->setObjectName(QStringLiteral("light_active_dial"));
        light_active_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_active_dial->sizePolicy().hasHeightForWidth());
        light_active_dial->setSizePolicy(sizePolicy);
        light_active_dial->setMinimumSize(QSize(30, 30));
        light_active_dial->setMaximumSize(QSize(30, 30));
        light_active_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_active_dial->setMinimum(-10000);
        light_active_dial->setMaximum(10000);

        postLayout->addWidget(light_active);

        light_position = new QFrame(layoutWidget3);
        light_position->setObjectName(QStringLiteral("light_position"));
        light_position->setMinimumSize(QSize(225, 35));
        light_position->setMaximumSize(QSize(225, 35));
        light_position->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_position->setFrameShape(QFrame::NoFrame);
        light_position->setFrameShadow(QFrame::Plain);
        light_position->setLineWidth(0);
        light_position_value = new QDoubleSpinBox(light_position);
        light_position_value->setObjectName(QStringLiteral("light_position_value"));
        light_position_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_position_value->sizePolicy().hasHeightForWidth());
        light_position_value->setSizePolicy(sizePolicy);
        light_position_value->setMinimumSize(QSize(86, 25));
        light_position_value->setMaximumSize(QSize(86, 25));
        light_position_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_position_value->setFrame(false);
        light_position_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_position_value->setDecimals(3);
        light_position_value->setMinimum(-9999);
        light_position_value->setMaximum(9999);
        light_position_value->setSingleStep(0.05);
        light_position_dial = new QDial(light_position);
        light_position_dial->setObjectName(QStringLiteral("light_position_dial"));
        light_position_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_position_dial->sizePolicy().hasHeightForWidth());
        light_position_dial->setSizePolicy(sizePolicy);
        light_position_dial->setMinimumSize(QSize(30, 30));
        light_position_dial->setMaximumSize(QSize(30, 30));
        light_position_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_position_dial->setMinimum(-10000);
        light_position_dial->setMaximum(10000);
        light_position_box = new QComboBox(light_position);
        light_position_box->setObjectName(QStringLiteral("light_position_box"));
        light_position_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_position_box->sizePolicy().hasHeightForWidth());
        light_position_box->setSizePolicy(sizePolicy);
        light_position_box->setMinimumSize(QSize(95, 22));
        light_position_box->setMaximumSize(QSize(90, 22));
        light_position_box->setBaseSize(QSize(90, 25));
        light_position_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_position_box->setMaxVisibleItems(30);
        light_position_box->setFrame(false);

        postLayout->addWidget(light_position);

        light_attenuation = new QFrame(layoutWidget3);
        light_attenuation->setObjectName(QStringLiteral("light_attenuation"));
        light_attenuation->setMinimumSize(QSize(225, 35));
        light_attenuation->setMaximumSize(QSize(225, 35));
        light_attenuation->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_attenuation->setFrameShape(QFrame::NoFrame);
        light_attenuation->setFrameShadow(QFrame::Plain);
        light_attenuation->setLineWidth(0);
        light_attenuation_value = new QDoubleSpinBox(light_attenuation);
        light_attenuation_value->setObjectName(QStringLiteral("light_attenuation_value"));
        light_attenuation_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_attenuation_value->sizePolicy().hasHeightForWidth());
        light_attenuation_value->setSizePolicy(sizePolicy);
        light_attenuation_value->setMinimumSize(QSize(86, 25));
        light_attenuation_value->setMaximumSize(QSize(86, 25));
        light_attenuation_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_attenuation_value->setFrame(false);
        light_attenuation_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_attenuation_value->setDecimals(3);
        light_attenuation_value->setMinimum(-9999);
        light_attenuation_value->setMaximum(9999);
        light_attenuation_value->setSingleStep(0.05);
        light_attenuation_dial = new QDial(light_attenuation);
        light_attenuation_dial->setObjectName(QStringLiteral("light_attenuation_dial"));
        light_attenuation_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_attenuation_dial->sizePolicy().hasHeightForWidth());
        light_attenuation_dial->setSizePolicy(sizePolicy);
        light_attenuation_dial->setMinimumSize(QSize(30, 30));
        light_attenuation_dial->setMaximumSize(QSize(30, 30));
        light_attenuation_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_attenuation_dial->setMinimum(-10000);
        light_attenuation_dial->setMaximum(10000);
        light_attenuation_box = new QComboBox(light_attenuation);
        light_attenuation_box->setObjectName(QStringLiteral("light_attenuation_box"));
        light_attenuation_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_attenuation_box->sizePolicy().hasHeightForWidth());
        light_attenuation_box->setSizePolicy(sizePolicy);
        light_attenuation_box->setMinimumSize(QSize(95, 22));
        light_attenuation_box->setMaximumSize(QSize(90, 22));
        light_attenuation_box->setBaseSize(QSize(90, 25));
        light_attenuation_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_attenuation_box->setMaxVisibleItems(30);
        light_attenuation_box->setFrame(false);

        postLayout->addWidget(light_attenuation);

        light_colour = new QFrame(layoutWidget3);
        light_colour->setObjectName(QStringLiteral("light_colour"));
        light_colour->setMinimumSize(QSize(225, 35));
        light_colour->setMaximumSize(QSize(225, 35));
        light_colour->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_colour->setFrameShape(QFrame::NoFrame);
        light_colour->setFrameShadow(QFrame::Plain);
        light_colour->setLineWidth(0);
        light_colour_value = new QDoubleSpinBox(light_colour);
        light_colour_value->setObjectName(QStringLiteral("light_colour_value"));
        light_colour_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_colour_value->sizePolicy().hasHeightForWidth());
        light_colour_value->setSizePolicy(sizePolicy);
        light_colour_value->setMinimumSize(QSize(86, 25));
        light_colour_value->setMaximumSize(QSize(86, 25));
        light_colour_value->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_colour_value->setFrame(false);
        light_colour_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_colour_value->setDecimals(3);
        light_colour_value->setMinimum(-9999);
        light_colour_value->setMaximum(9999);
        light_colour_value->setSingleStep(0.05);
        light_colour_dial = new QDial(light_colour);
        light_colour_dial->setObjectName(QStringLiteral("light_colour_dial"));
        light_colour_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_colour_dial->sizePolicy().hasHeightForWidth());
        light_colour_dial->setSizePolicy(sizePolicy);
        light_colour_dial->setMinimumSize(QSize(30, 30));
        light_colour_dial->setMaximumSize(QSize(30, 30));
        light_colour_dial->setStyleSheet(QStringLiteral("border-width: 0px;"));
        light_colour_dial->setMinimum(-10000);
        light_colour_dial->setMaximum(10000);
        light_colour_box = new QComboBox(light_colour);
        light_colour_box->setObjectName(QStringLiteral("light_colour_box"));
        light_colour_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_colour_box->sizePolicy().hasHeightForWidth());
        light_colour_box->setSizePolicy(sizePolicy);
        light_colour_box->setMinimumSize(QSize(95, 22));
        light_colour_box->setMaximumSize(QSize(90, 22));
        light_colour_box->setBaseSize(QSize(90, 25));
        light_colour_box->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_colour_box->setMaxVisibleItems(30);
        light_colour_box->setFrame(false);

        postLayout->addWidget(light_colour);

        light_render_only = new QFrame(layoutWidget3);
        light_render_only->setObjectName(QStringLiteral("light_render_only"));
        sizePolicy.setHeightForWidth(light_render_only->sizePolicy().hasHeightForWidth());
        light_render_only->setSizePolicy(sizePolicy);
        light_render_only->setMinimumSize(QSize(225, 35));
        light_render_only->setMaximumSize(QSize(225, 35));
        light_render_only->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_render_only->setFrameShape(QFrame::Panel);
        light_render_only->setFrameShadow(QFrame::Raised);
        light_render_only_btn = new QPushButton(light_render_only);
        light_render_only_btn->setObjectName(QStringLiteral("light_render_only_btn"));
        light_render_only_btn->setGeometry(QRect(5, 4, 216, 25));
        light_render_only_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_render_only_btn->setDefault(false);

        postLayout->addWidget(light_render_only);

        light_diagnostics = new QFrame(layoutWidget3);
        light_diagnostics->setObjectName(QStringLiteral("light_diagnostics"));
        sizePolicy.setHeightForWidth(light_diagnostics->sizePolicy().hasHeightForWidth());
        light_diagnostics->setSizePolicy(sizePolicy);
        light_diagnostics->setMinimumSize(QSize(225, 35));
        light_diagnostics->setMaximumSize(QSize(225, 35));
        light_diagnostics->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_diagnostics->setFrameShape(QFrame::Panel);
        light_diagnostics->setFrameShadow(QFrame::Raised);
        light_diagnostics_btn = new QPushButton(light_diagnostics);
        light_diagnostics_btn->setObjectName(QStringLiteral("light_diagnostics_btn"));
        light_diagnostics_btn->setGeometry(QRect(5, 4, 216, 25));
        light_diagnostics_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_diagnostics_btn->setDefault(false);

        postLayout->addWidget(light_diagnostics);

        toggleWireframe = new QFrame(layoutWidget3);
        toggleWireframe->setObjectName(QStringLiteral("toggleWireframe"));
        sizePolicy.setHeightForWidth(toggleWireframe->sizePolicy().hasHeightForWidth());
        toggleWireframe->setSizePolicy(sizePolicy);
        toggleWireframe->setMinimumSize(QSize(225, 35));
        toggleWireframe->setMaximumSize(QSize(225, 35));
        toggleWireframe->setStyleSheet(QLatin1String("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        toggleWireframe->setFrameShape(QFrame::Panel);
        toggleWireframe->setFrameShadow(QFrame::Raised);
        toggleWireframe_btn = new QPushButton(toggleWireframe);
        toggleWireframe_btn->setObjectName(QStringLiteral("toggleWireframe_btn"));
        toggleWireframe_btn->setGeometry(QRect(5, 4, 216, 25));
        toggleWireframe_btn->setStyleSheet(QLatin1String("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        toggleWireframe_btn->setDefault(false);

        postLayout->addWidget(toggleWireframe);

        _postSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        postLayout->addItem(_postSpacer);

        TabMenu->addTab(Post, QString());

        verticalLayout_3->addWidget(TabMenu);


        retranslateUi(Tweaker);

        TabMenu->setCurrentIndex(0);
        selectedTexture_box->setCurrentIndex(-1);
        selectedTerrain_box->setCurrentIndex(-1);
        lightSelected_box->setCurrentIndex(-1);


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
        timer_lbl->setText(QApplication::translate("Tweaker", "Timer", 0));
        timer_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        cameraX_lbl->setText(QApplication::translate("Tweaker", "Camera X", 0));
        cameraY_lbl->setText(QApplication::translate("Tweaker", "Camera Y", 0));
        cameraZ_lbl->setText(QApplication::translate("Tweaker", "Camera Z", 0));
        cameraPitch_lbl->setText(QApplication::translate("Tweaker", "Camera Pitch", 0));
        cameraYaw_lbl->setText(QApplication::translate("Tweaker", "Camera Yaw", 0));
        cameraRoll_lbl->setText(QApplication::translate("Tweaker", "Camera Roll", 0));
        reloadScene_btn->setText(QApplication::translate("Tweaker", "Reload Scene", 0));
        reloadEngine_btn->setText(QApplication::translate("Tweaker", "Reload Engine", 0));
        saveScene_btn->setText(QApplication::translate("Tweaker", "Save Scene to File", 0));
        TabMenu->setTabText(TabMenu->indexOf(Scene), QApplication::translate("Tweaker", "Scene", 0));
        selectedTexture_lbl->setText(QApplication::translate("Tweaker", "Texture", 0));
        displayTexture_btn->setText(QString());
        reloadTexture_btn->setText(QApplication::translate("Tweaker", "Reload Texture", 0));
        selectedTerrain_lbl->setText(QApplication::translate("Tweaker", "Terrain", 0));
        terrainShader_lbl->setText(QApplication::translate("Tweaker", "Shader", 0));
        terrainShader_text->setText(QApplication::translate("Tweaker", "TextLabel", 0));
        reloadTerrain_btn->setText(QApplication::translate("Tweaker", "Reload Terrain", 0));
        TabMenu->setTabText(TabMenu->indexOf(Area), QApplication::translate("Tweaker", "PGT", 0));
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
        postImage_lbl->setText(QApplication::translate("Tweaker", "Rendering", 0));
        lightSelected_lbl->setText(QApplication::translate("Tweaker", "Light", 0));
        light_active_lbl->setText(QApplication::translate("Tweaker", "Activity", 0));
        light_render_only_btn->setText(QApplication::translate("Tweaker", "Render Lights Only", 0));
        light_diagnostics_btn->setText(QApplication::translate("Tweaker", "Toggle Light Diagnostics", 0));
        toggleWireframe_btn->setText(QApplication::translate("Tweaker", "Toggle Wireframe", 0));
        TabMenu->setTabText(TabMenu->indexOf(Post), QApplication::translate("Tweaker", "Post", 0));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
