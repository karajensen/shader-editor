/********************************************************************************
** Form generated from reading UI file 'tweaker.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TWEAKER_H
#define UI_TWEAKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
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
    QFrame *cameraForwardSpeed;
    QLabel *cameraForwardSpeed_lbl;
    QDoubleSpinBox *cameraForwardSpeed_value;
    QDial *cameraForwardSpeed_dial;
    QFrame *cameraRotationSpeed;
    QLabel *cameraRotationSpeed_lbl;
    QDoubleSpinBox *cameraRotationSpeed_value;
    QDial *cameraRotationSpeed_dial;
    QFrame *reloadScene;
    QPushButton *reloadScene_btn;
    QFrame *reloadPlacement;
    QPushButton *reloadPlacement_btn;
    QFrame *reloadEngine;
    QPushButton *reloadEngine_btn;
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
    QFrame *instancesTerrain;
    QLabel *instancesTerrain_lbl;
    QLabel *instancesTerrain_text;
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
    QFrame *instancesMesh;
    QLabel *instancesMesh_lbl;
    QLabel *instancesMesh_text;
    QFrame *meshAttributes;
    QDoubleSpinBox *meshAttributes_value;
    QDial *meshAttributes_dial;
    QComboBox *meshAttributes_box;
    QSpacerItem *_meshSpacer_2;
    QFrame *selectedWater;
    QLabel *selectedWater_lbl;
    QComboBox *selectedWater_box;
    QFrame *instancesWater;
    QLabel *instancesWater_lbl;
    QLabel *instancesWater_text;
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
    QFrame *instancesEmitter;
    QLabel *instancesEmitter_lbl;
    QLabel *instancesEmitter_text;
    QFrame *emitter;
    QDoubleSpinBox *emitter_value;
    QDial *emitter_dial;
    QComboBox *emitter_box;
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
    QFrame *toggleWireframe;
    QPushButton *toggleWireframe_btn;
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
    QFrame *light_specular;
    QDoubleSpinBox *light_specular_value;
    QDial *light_specular_dial;
    QComboBox *light_specular_box;
    QFrame *light_render_only;
    QPushButton *light_render_only_btn;
    QFrame *light_diagnostics;
    QPushButton *light_diagnostics_btn;
    QSpacerItem *_postSpacer;

    void setupUi(QWidget *Tweaker)
    {
        if (Tweaker->objectName().isEmpty())
            Tweaker->setObjectName(QString::fromUtf8("Tweaker"));
        Tweaker->resize(250, 580);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tweaker->sizePolicy().hasHeightForWidth());
        Tweaker->setSizePolicy(sizePolicy);
        Tweaker->setMinimumSize(QSize(250, 580));
        Tweaker->setMaximumSize(QSize(300, 580));
        verticalLayout_3 = new QVBoxLayout(Tweaker);
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 4, 4);
        TabMenu = new QTabWidget(Tweaker);
        TabMenu->setObjectName(QString::fromUtf8("TabMenu"));
        TabMenu->setStyleSheet(QString::fromUtf8(""));
        TabMenu->setTabPosition(QTabWidget::North);
        TabMenu->setTabShape(QTabWidget::Triangular);
        TabMenu->setMovable(true);
        Scene = new QWidget();
        Scene->setObjectName(QString::fromUtf8("Scene"));
        Scene->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);"));
        layoutWidget = new QWidget(Scene);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 230, 561));
        sceneLayout = new QVBoxLayout(layoutWidget);
        sceneLayout->setSpacing(2);
        sceneLayout->setObjectName(QString::fromUtf8("sceneLayout"));
        sceneLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        sceneLayout->setContentsMargins(4, 4, 0, 0);
        switchEngine = new QFrame(layoutWidget);
        switchEngine->setObjectName(QString::fromUtf8("switchEngine"));
        sizePolicy.setHeightForWidth(switchEngine->sizePolicy().hasHeightForWidth());
        switchEngine->setSizePolicy(sizePolicy);
        switchEngine->setMinimumSize(QSize(225, 35));
        switchEngine->setMaximumSize(QSize(225, 35));
        switchEngine->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        switchEngine->setFrameShape(QFrame::Panel);
        switchEngine->setFrameShadow(QFrame::Raised);
        switchEngine_lbl = new QLabel(switchEngine);
        switchEngine_lbl->setObjectName(QString::fromUtf8("switchEngine_lbl"));
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
        switchEngine_box->setObjectName(QString::fromUtf8("switchEngine_box"));
        switchEngine_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(switchEngine_box->sizePolicy().hasHeightForWidth());
        switchEngine_box->setSizePolicy(sizePolicy);
        switchEngine_box->setMinimumSize(QSize(115, 22));
        switchEngine_box->setMaximumSize(QSize(115, 22));
        switchEngine_box->setBaseSize(QSize(115, 25));
        switchEngine_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        switchEngine_box->setFrame(false);

        sceneLayout->addWidget(switchEngine);

        fps = new QFrame(layoutWidget);
        fps->setObjectName(QString::fromUtf8("fps"));
        sizePolicy.setHeightForWidth(fps->sizePolicy().hasHeightForWidth());
        fps->setSizePolicy(sizePolicy);
        fps->setMinimumSize(QSize(225, 35));
        fps->setMaximumSize(QSize(225, 35));
        fps->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        fps->setFrameShape(QFrame::Panel);
        fps->setFrameShadow(QFrame::Raised);
        fps_lbl = new QLabel(fps);
        fps_lbl->setObjectName(QString::fromUtf8("fps_lbl"));
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
        fps_text->setObjectName(QString::fromUtf8("fps_text"));
        fps_text->setGeometry(QRect(106, 5, 115, 25));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fps_text->sizePolicy().hasHeightForWidth());
        fps_text->setSizePolicy(sizePolicy1);
        fps_text->setMinimumSize(QSize(115, 25));
        fps_text->setMaximumSize(QSize(115, 25));
        fps_text->setAutoFillBackground(false);
        fps_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        fps_text->setFrameShape(QFrame::NoFrame);
        fps_text->setFrameShadow(QFrame::Plain);
        fps_text->setLineWidth(0);
        fps_text->setMargin(3);

        sceneLayout->addWidget(fps);

        deltaTime = new QFrame(layoutWidget);
        deltaTime->setObjectName(QString::fromUtf8("deltaTime"));
        sizePolicy.setHeightForWidth(deltaTime->sizePolicy().hasHeightForWidth());
        deltaTime->setSizePolicy(sizePolicy);
        deltaTime->setMinimumSize(QSize(225, 35));
        deltaTime->setMaximumSize(QSize(225, 35));
        deltaTime->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        deltaTime->setFrameShape(QFrame::Panel);
        deltaTime->setFrameShadow(QFrame::Raised);
        deltaTime_lbl = new QLabel(deltaTime);
        deltaTime_lbl->setObjectName(QString::fromUtf8("deltaTime_lbl"));
        deltaTime_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(deltaTime_lbl->sizePolicy().hasHeightForWidth());
        deltaTime_lbl->setSizePolicy(sizePolicy);
        deltaTime_lbl->setMinimumSize(QSize(100, 30));
        deltaTime_lbl->setMaximumSize(QSize(100, 30));
        deltaTime_lbl->setFont(font1);
        deltaTime_text = new QLabel(deltaTime);
        deltaTime_text->setObjectName(QString::fromUtf8("deltaTime_text"));
        deltaTime_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(deltaTime_text->sizePolicy().hasHeightForWidth());
        deltaTime_text->setSizePolicy(sizePolicy1);
        deltaTime_text->setMinimumSize(QSize(115, 25));
        deltaTime_text->setMaximumSize(QSize(115, 25));
        deltaTime_text->setAutoFillBackground(false);
        deltaTime_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        deltaTime_text->setFrameShape(QFrame::NoFrame);
        deltaTime_text->setFrameShadow(QFrame::Plain);
        deltaTime_text->setLineWidth(0);
        deltaTime_text->setMargin(3);

        sceneLayout->addWidget(deltaTime);

        cameraX = new QFrame(layoutWidget);
        cameraX->setObjectName(QString::fromUtf8("cameraX"));
        cameraX->setMinimumSize(QSize(225, 35));
        cameraX->setMaximumSize(QSize(225, 35));
        cameraX->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraX->setFrameShape(QFrame::NoFrame);
        cameraX->setFrameShadow(QFrame::Plain);
        cameraX->setLineWidth(0);
        cameraX_lbl = new QLabel(cameraX);
        cameraX_lbl->setObjectName(QString::fromUtf8("cameraX_lbl"));
        cameraX_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraX_lbl->sizePolicy().hasHeightForWidth());
        cameraX_lbl->setSizePolicy(sizePolicy);
        cameraX_lbl->setMinimumSize(QSize(100, 30));
        cameraX_lbl->setMaximumSize(QSize(100, 30));
        cameraX_lbl->setFont(font1);
        cameraX_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraX_lbl->setMargin(1);
        cameraX_value = new QDoubleSpinBox(cameraX);
        cameraX_value->setObjectName(QString::fromUtf8("cameraX_value"));
        cameraX_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraX_value->sizePolicy().hasHeightForWidth());
        cameraX_value->setSizePolicy(sizePolicy);
        cameraX_value->setMinimumSize(QSize(116, 25));
        cameraX_value->setMaximumSize(QSize(116, 25));
        cameraX_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraX_value->setFrame(false);
        cameraX_value->setReadOnly(true);
        cameraX_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraX_value->setDecimals(3);
        cameraX_value->setMinimum(-9999.000000000000000);
        cameraX_value->setMaximum(9999.000000000000000);
        cameraX_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraX);

        cameraY = new QFrame(layoutWidget);
        cameraY->setObjectName(QString::fromUtf8("cameraY"));
        cameraY->setMinimumSize(QSize(225, 35));
        cameraY->setMaximumSize(QSize(225, 35));
        cameraY->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraY->setFrameShape(QFrame::NoFrame);
        cameraY->setFrameShadow(QFrame::Plain);
        cameraY->setLineWidth(0);
        cameraY_lbl = new QLabel(cameraY);
        cameraY_lbl->setObjectName(QString::fromUtf8("cameraY_lbl"));
        cameraY_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraY_lbl->sizePolicy().hasHeightForWidth());
        cameraY_lbl->setSizePolicy(sizePolicy);
        cameraY_lbl->setMinimumSize(QSize(100, 30));
        cameraY_lbl->setMaximumSize(QSize(100, 30));
        cameraY_lbl->setFont(font1);
        cameraY_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraY_lbl->setMargin(1);
        cameraY_value = new QDoubleSpinBox(cameraY);
        cameraY_value->setObjectName(QString::fromUtf8("cameraY_value"));
        cameraY_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraY_value->sizePolicy().hasHeightForWidth());
        cameraY_value->setSizePolicy(sizePolicy);
        cameraY_value->setMinimumSize(QSize(116, 25));
        cameraY_value->setMaximumSize(QSize(116, 25));
        cameraY_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraY_value->setFrame(false);
        cameraY_value->setReadOnly(true);
        cameraY_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraY_value->setDecimals(3);
        cameraY_value->setMinimum(-9999.000000000000000);
        cameraY_value->setMaximum(9999.000000000000000);
        cameraY_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraY);

        cameraZ = new QFrame(layoutWidget);
        cameraZ->setObjectName(QString::fromUtf8("cameraZ"));
        cameraZ->setMinimumSize(QSize(225, 35));
        cameraZ->setMaximumSize(QSize(225, 35));
        cameraZ->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraZ->setFrameShape(QFrame::NoFrame);
        cameraZ->setFrameShadow(QFrame::Plain);
        cameraZ->setLineWidth(0);
        cameraZ_lbl = new QLabel(cameraZ);
        cameraZ_lbl->setObjectName(QString::fromUtf8("cameraZ_lbl"));
        cameraZ_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraZ_lbl->sizePolicy().hasHeightForWidth());
        cameraZ_lbl->setSizePolicy(sizePolicy);
        cameraZ_lbl->setMinimumSize(QSize(100, 30));
        cameraZ_lbl->setMaximumSize(QSize(100, 30));
        cameraZ_lbl->setFont(font1);
        cameraZ_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraZ_lbl->setMargin(1);
        cameraZ_value = new QDoubleSpinBox(cameraZ);
        cameraZ_value->setObjectName(QString::fromUtf8("cameraZ_value"));
        cameraZ_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraZ_value->sizePolicy().hasHeightForWidth());
        cameraZ_value->setSizePolicy(sizePolicy);
        cameraZ_value->setMinimumSize(QSize(116, 25));
        cameraZ_value->setMaximumSize(QSize(116, 25));
        cameraZ_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraZ_value->setFrame(false);
        cameraZ_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraZ_value->setDecimals(3);
        cameraZ_value->setMinimum(-9999.000000000000000);
        cameraZ_value->setMaximum(9999.000000000000000);
        cameraZ_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraZ);

        cameraPitch = new QFrame(layoutWidget);
        cameraPitch->setObjectName(QString::fromUtf8("cameraPitch"));
        cameraPitch->setMinimumSize(QSize(225, 35));
        cameraPitch->setMaximumSize(QSize(225, 35));
        cameraPitch->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraPitch->setFrameShape(QFrame::NoFrame);
        cameraPitch->setFrameShadow(QFrame::Plain);
        cameraPitch->setLineWidth(0);
        cameraPitch_lbl = new QLabel(cameraPitch);
        cameraPitch_lbl->setObjectName(QString::fromUtf8("cameraPitch_lbl"));
        cameraPitch_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraPitch_lbl->sizePolicy().hasHeightForWidth());
        cameraPitch_lbl->setSizePolicy(sizePolicy);
        cameraPitch_lbl->setMinimumSize(QSize(100, 30));
        cameraPitch_lbl->setMaximumSize(QSize(100, 30));
        cameraPitch_lbl->setFont(font1);
        cameraPitch_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraPitch_lbl->setMargin(1);
        cameraPitch_value = new QDoubleSpinBox(cameraPitch);
        cameraPitch_value->setObjectName(QString::fromUtf8("cameraPitch_value"));
        cameraPitch_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraPitch_value->sizePolicy().hasHeightForWidth());
        cameraPitch_value->setSizePolicy(sizePolicy);
        cameraPitch_value->setMinimumSize(QSize(116, 25));
        cameraPitch_value->setMaximumSize(QSize(116, 25));
        cameraPitch_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraPitch_value->setFrame(false);
        cameraPitch_value->setReadOnly(true);
        cameraPitch_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraPitch_value->setDecimals(3);
        cameraPitch_value->setMinimum(-9999.000000000000000);
        cameraPitch_value->setMaximum(9999.000000000000000);
        cameraPitch_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraPitch);

        cameraYaw = new QFrame(layoutWidget);
        cameraYaw->setObjectName(QString::fromUtf8("cameraYaw"));
        cameraYaw->setMinimumSize(QSize(225, 35));
        cameraYaw->setMaximumSize(QSize(225, 35));
        cameraYaw->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraYaw->setFrameShape(QFrame::NoFrame);
        cameraYaw->setFrameShadow(QFrame::Plain);
        cameraYaw->setLineWidth(0);
        cameraYaw_lbl = new QLabel(cameraYaw);
        cameraYaw_lbl->setObjectName(QString::fromUtf8("cameraYaw_lbl"));
        cameraYaw_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraYaw_lbl->sizePolicy().hasHeightForWidth());
        cameraYaw_lbl->setSizePolicy(sizePolicy);
        cameraYaw_lbl->setMinimumSize(QSize(100, 30));
        cameraYaw_lbl->setMaximumSize(QSize(100, 30));
        cameraYaw_lbl->setFont(font1);
        cameraYaw_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraYaw_lbl->setMargin(1);
        cameraYaw_value = new QDoubleSpinBox(cameraYaw);
        cameraYaw_value->setObjectName(QString::fromUtf8("cameraYaw_value"));
        cameraYaw_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraYaw_value->sizePolicy().hasHeightForWidth());
        cameraYaw_value->setSizePolicy(sizePolicy);
        cameraYaw_value->setMinimumSize(QSize(116, 25));
        cameraYaw_value->setMaximumSize(QSize(116, 25));
        cameraYaw_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraYaw_value->setFrame(false);
        cameraYaw_value->setReadOnly(true);
        cameraYaw_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraYaw_value->setDecimals(3);
        cameraYaw_value->setMinimum(-9999.000000000000000);
        cameraYaw_value->setMaximum(9999.000000000000000);
        cameraYaw_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraYaw);

        cameraRoll = new QFrame(layoutWidget);
        cameraRoll->setObjectName(QString::fromUtf8("cameraRoll"));
        cameraRoll->setMinimumSize(QSize(225, 35));
        cameraRoll->setMaximumSize(QSize(225, 35));
        cameraRoll->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraRoll->setFrameShape(QFrame::NoFrame);
        cameraRoll->setFrameShadow(QFrame::Plain);
        cameraRoll->setLineWidth(0);
        cameraRoll_lbl = new QLabel(cameraRoll);
        cameraRoll_lbl->setObjectName(QString::fromUtf8("cameraRoll_lbl"));
        cameraRoll_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraRoll_lbl->sizePolicy().hasHeightForWidth());
        cameraRoll_lbl->setSizePolicy(sizePolicy);
        cameraRoll_lbl->setMinimumSize(QSize(100, 30));
        cameraRoll_lbl->setMaximumSize(QSize(100, 30));
        cameraRoll_lbl->setFont(font1);
        cameraRoll_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraRoll_lbl->setMargin(1);
        cameraRoll_value = new QDoubleSpinBox(cameraRoll);
        cameraRoll_value->setObjectName(QString::fromUtf8("cameraRoll_value"));
        cameraRoll_value->setGeometry(QRect(104, 5, 116, 25));
        sizePolicy.setHeightForWidth(cameraRoll_value->sizePolicy().hasHeightForWidth());
        cameraRoll_value->setSizePolicy(sizePolicy);
        cameraRoll_value->setMinimumSize(QSize(116, 25));
        cameraRoll_value->setMaximumSize(QSize(116, 25));
        cameraRoll_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        cameraRoll_value->setFrame(false);
        cameraRoll_value->setReadOnly(true);
        cameraRoll_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
        cameraRoll_value->setDecimals(3);
        cameraRoll_value->setMinimum(-9999.000000000000000);
        cameraRoll_value->setMaximum(9999.000000000000000);
        cameraRoll_value->setSingleStep(0.050000000000000);

        sceneLayout->addWidget(cameraRoll);

        cameraForwardSpeed = new QFrame(layoutWidget);
        cameraForwardSpeed->setObjectName(QString::fromUtf8("cameraForwardSpeed"));
        cameraForwardSpeed->setMinimumSize(QSize(225, 35));
        cameraForwardSpeed->setMaximumSize(QSize(225, 35));
        cameraForwardSpeed->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraForwardSpeed->setFrameShape(QFrame::NoFrame);
        cameraForwardSpeed->setFrameShadow(QFrame::Plain);
        cameraForwardSpeed->setLineWidth(0);
        cameraForwardSpeed_lbl = new QLabel(cameraForwardSpeed);
        cameraForwardSpeed_lbl->setObjectName(QString::fromUtf8("cameraForwardSpeed_lbl"));
        cameraForwardSpeed_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraForwardSpeed_lbl->sizePolicy().hasHeightForWidth());
        cameraForwardSpeed_lbl->setSizePolicy(sizePolicy);
        cameraForwardSpeed_lbl->setMinimumSize(QSize(100, 30));
        cameraForwardSpeed_lbl->setMaximumSize(QSize(100, 30));
        cameraForwardSpeed_lbl->setFont(font1);
        cameraForwardSpeed_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraForwardSpeed_lbl->setMargin(1);
        cameraForwardSpeed_value = new QDoubleSpinBox(cameraForwardSpeed);
        cameraForwardSpeed_value->setObjectName(QString::fromUtf8("cameraForwardSpeed_value"));
        cameraForwardSpeed_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraForwardSpeed_value->sizePolicy().hasHeightForWidth());
        cameraForwardSpeed_value->setSizePolicy(sizePolicy);
        cameraForwardSpeed_value->setMinimumSize(QSize(86, 25));
        cameraForwardSpeed_value->setMaximumSize(QSize(86, 25));
        cameraForwardSpeed_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraForwardSpeed_value->setFrame(false);
        cameraForwardSpeed_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraForwardSpeed_value->setDecimals(3);
        cameraForwardSpeed_value->setMinimum(0.000000000000000);
        cameraForwardSpeed_value->setMaximum(999999999.000000000000000);
        cameraForwardSpeed_value->setSingleStep(0.010000000000000);
        cameraForwardSpeed_dial = new QDial(cameraForwardSpeed);
        cameraForwardSpeed_dial->setObjectName(QString::fromUtf8("cameraForwardSpeed_dial"));
        cameraForwardSpeed_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraForwardSpeed_dial->sizePolicy().hasHeightForWidth());
        cameraForwardSpeed_dial->setSizePolicy(sizePolicy);
        cameraForwardSpeed_dial->setMinimumSize(QSize(30, 30));
        cameraForwardSpeed_dial->setMaximumSize(QSize(30, 30));
        cameraForwardSpeed_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraForwardSpeed_dial->setMinimum(-10000);
        cameraForwardSpeed_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraForwardSpeed);

        cameraRotationSpeed = new QFrame(layoutWidget);
        cameraRotationSpeed->setObjectName(QString::fromUtf8("cameraRotationSpeed"));
        cameraRotationSpeed->setMinimumSize(QSize(225, 35));
        cameraRotationSpeed->setMaximumSize(QSize(225, 35));
        cameraRotationSpeed->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        cameraRotationSpeed->setFrameShape(QFrame::NoFrame);
        cameraRotationSpeed->setFrameShadow(QFrame::Plain);
        cameraRotationSpeed->setLineWidth(0);
        cameraRotationSpeed_lbl = new QLabel(cameraRotationSpeed);
        cameraRotationSpeed_lbl->setObjectName(QString::fromUtf8("cameraRotationSpeed_lbl"));
        cameraRotationSpeed_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(cameraRotationSpeed_lbl->sizePolicy().hasHeightForWidth());
        cameraRotationSpeed_lbl->setSizePolicy(sizePolicy);
        cameraRotationSpeed_lbl->setMinimumSize(QSize(100, 30));
        cameraRotationSpeed_lbl->setMaximumSize(QSize(100, 30));
        cameraRotationSpeed_lbl->setFont(font1);
        cameraRotationSpeed_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraRotationSpeed_lbl->setMargin(1);
        cameraRotationSpeed_value = new QDoubleSpinBox(cameraRotationSpeed);
        cameraRotationSpeed_value->setObjectName(QString::fromUtf8("cameraRotationSpeed_value"));
        cameraRotationSpeed_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(cameraRotationSpeed_value->sizePolicy().hasHeightForWidth());
        cameraRotationSpeed_value->setSizePolicy(sizePolicy);
        cameraRotationSpeed_value->setMinimumSize(QSize(86, 25));
        cameraRotationSpeed_value->setMaximumSize(QSize(86, 25));
        cameraRotationSpeed_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        cameraRotationSpeed_value->setFrame(false);
        cameraRotationSpeed_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        cameraRotationSpeed_value->setDecimals(3);
        cameraRotationSpeed_value->setMinimum(0.000000000000000);
        cameraRotationSpeed_value->setMaximum(999999999.000000000000000);
        cameraRotationSpeed_value->setSingleStep(0.010000000000000);
        cameraRotationSpeed_dial = new QDial(cameraRotationSpeed);
        cameraRotationSpeed_dial->setObjectName(QString::fromUtf8("cameraRotationSpeed_dial"));
        cameraRotationSpeed_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(cameraRotationSpeed_dial->sizePolicy().hasHeightForWidth());
        cameraRotationSpeed_dial->setSizePolicy(sizePolicy);
        cameraRotationSpeed_dial->setMinimumSize(QSize(30, 30));
        cameraRotationSpeed_dial->setMaximumSize(QSize(30, 30));
        cameraRotationSpeed_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        cameraRotationSpeed_dial->setMinimum(-10000);
        cameraRotationSpeed_dial->setMaximum(10000);

        sceneLayout->addWidget(cameraRotationSpeed);

        reloadScene = new QFrame(layoutWidget);
        reloadScene->setObjectName(QString::fromUtf8("reloadScene"));
        sizePolicy.setHeightForWidth(reloadScene->sizePolicy().hasHeightForWidth());
        reloadScene->setSizePolicy(sizePolicy);
        reloadScene->setMinimumSize(QSize(225, 35));
        reloadScene->setMaximumSize(QSize(225, 35));
        reloadScene->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadScene->setFrameShape(QFrame::Panel);
        reloadScene->setFrameShadow(QFrame::Raised);
        reloadScene_btn = new QPushButton(reloadScene);
        reloadScene_btn->setObjectName(QString::fromUtf8("reloadScene_btn"));
        reloadScene_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadScene_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        sceneLayout->addWidget(reloadScene);

        reloadPlacement = new QFrame(layoutWidget);
        reloadPlacement->setObjectName(QString::fromUtf8("reloadPlacement"));
        sizePolicy.setHeightForWidth(reloadPlacement->sizePolicy().hasHeightForWidth());
        reloadPlacement->setSizePolicy(sizePolicy);
        reloadPlacement->setMinimumSize(QSize(225, 35));
        reloadPlacement->setMaximumSize(QSize(225, 35));
        reloadPlacement->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadPlacement->setFrameShape(QFrame::Panel);
        reloadPlacement->setFrameShadow(QFrame::Raised);
        reloadPlacement_btn = new QPushButton(reloadPlacement);
        reloadPlacement_btn->setObjectName(QString::fromUtf8("reloadPlacement_btn"));
        reloadPlacement_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadPlacement_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        sceneLayout->addWidget(reloadPlacement);

        reloadEngine = new QFrame(layoutWidget);
        reloadEngine->setObjectName(QString::fromUtf8("reloadEngine"));
        sizePolicy.setHeightForWidth(reloadEngine->sizePolicy().hasHeightForWidth());
        reloadEngine->setSizePolicy(sizePolicy);
        reloadEngine->setMinimumSize(QSize(225, 35));
        reloadEngine->setMaximumSize(QSize(225, 35));
        reloadEngine->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadEngine->setFrameShape(QFrame::Panel);
        reloadEngine->setFrameShadow(QFrame::Raised);
        reloadEngine_btn = new QPushButton(reloadEngine);
        reloadEngine_btn->setObjectName(QString::fromUtf8("reloadEngine_btn"));
        reloadEngine_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadEngine_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        sceneLayout->addWidget(reloadEngine);

        _sceneSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sceneLayout->addItem(_sceneSpacer);

        TabMenu->addTab(Scene, QString());
        Area = new QWidget();
        Area->setObjectName(QString::fromUtf8("Area"));
        Area->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);"));
        layoutWidget1 = new QWidget(Area);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 0, 260, 626));
        areaLayout = new QVBoxLayout(layoutWidget1);
        areaLayout->setSpacing(2);
        areaLayout->setObjectName(QString::fromUtf8("areaLayout"));
        areaLayout->setContentsMargins(4, 4, 0, 0);
        selectedTexture = new QFrame(layoutWidget1);
        selectedTexture->setObjectName(QString::fromUtf8("selectedTexture"));
        sizePolicy.setHeightForWidth(selectedTexture->sizePolicy().hasHeightForWidth());
        selectedTexture->setSizePolicy(sizePolicy);
        selectedTexture->setMinimumSize(QSize(225, 35));
        selectedTexture->setMaximumSize(QSize(225, 35));
        selectedTexture->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedTexture->setFrameShape(QFrame::Panel);
        selectedTexture->setFrameShadow(QFrame::Raised);
        selectedTexture_lbl = new QLabel(selectedTexture);
        selectedTexture_lbl->setObjectName(QString::fromUtf8("selectedTexture_lbl"));
        selectedTexture_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedTexture_lbl->sizePolicy().hasHeightForWidth());
        selectedTexture_lbl->setSizePolicy(sizePolicy);
        selectedTexture_lbl->setMinimumSize(QSize(100, 30));
        selectedTexture_lbl->setMaximumSize(QSize(100, 30));
        selectedTexture_lbl->setFont(font);
        selectedTexture_lbl->setMargin(1);
        selectedTexture_box = new QComboBox(selectedTexture);
        selectedTexture_box->setObjectName(QString::fromUtf8("selectedTexture_box"));
        selectedTexture_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedTexture_box->sizePolicy().hasHeightForWidth());
        selectedTexture_box->setSizePolicy(sizePolicy);
        selectedTexture_box->setMinimumSize(QSize(115, 22));
        selectedTexture_box->setMaximumSize(QSize(115, 22));
        selectedTexture_box->setBaseSize(QSize(115, 25));
        selectedTexture_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        displayTexture->setObjectName(QString::fromUtf8("displayTexture"));
        sizePolicy.setHeightForWidth(displayTexture->sizePolicy().hasHeightForWidth());
        displayTexture->setSizePolicy(sizePolicy);
        displayTexture->setMinimumSize(QSize(225, 225));
        displayTexture->setMaximumSize(QSize(225, 225));
        displayTexture->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        displayTexture->setFrameShape(QFrame::Panel);
        displayTexture->setFrameShadow(QFrame::Raised);
        displayTexture_btn = new QPushButton(displayTexture);
        displayTexture_btn->setObjectName(QString::fromUtf8("displayTexture_btn"));
        displayTexture_btn->setGeometry(QRect(5, 5, 216, 216));
        displayTexture_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        displayTexture_btn->setAutoRepeat(true);

        areaLayout->addWidget(displayTexture);

        textureAttributes = new QFrame(layoutWidget1);
        textureAttributes->setObjectName(QString::fromUtf8("textureAttributes"));
        textureAttributes->setMinimumSize(QSize(225, 35));
        textureAttributes->setMaximumSize(QSize(225, 35));
        textureAttributes->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        textureAttributes->setFrameShape(QFrame::NoFrame);
        textureAttributes->setFrameShadow(QFrame::Plain);
        textureAttributes->setLineWidth(0);
        textureAttributes_value = new QDoubleSpinBox(textureAttributes);
        textureAttributes_value->setObjectName(QString::fromUtf8("textureAttributes_value"));
        textureAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(textureAttributes_value->sizePolicy().hasHeightForWidth());
        textureAttributes_value->setSizePolicy(sizePolicy);
        textureAttributes_value->setMinimumSize(QSize(86, 25));
        textureAttributes_value->setMaximumSize(QSize(86, 25));
        textureAttributes_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        textureAttributes_value->setFrame(false);
        textureAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        textureAttributes_value->setDecimals(3);
        textureAttributes_value->setMinimum(-9999.000000000000000);
        textureAttributes_value->setMaximum(9999.000000000000000);
        textureAttributes_value->setSingleStep(0.050000000000000);
        textureAttributes_dial = new QDial(textureAttributes);
        textureAttributes_dial->setObjectName(QString::fromUtf8("textureAttributes_dial"));
        textureAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(textureAttributes_dial->sizePolicy().hasHeightForWidth());
        textureAttributes_dial->setSizePolicy(sizePolicy);
        textureAttributes_dial->setMinimumSize(QSize(30, 30));
        textureAttributes_dial->setMaximumSize(QSize(30, 30));
        textureAttributes_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        textureAttributes_dial->setMinimum(-10000);
        textureAttributes_dial->setMaximum(10000);
        textureAttributes_box = new QComboBox(textureAttributes);
        textureAttributes_box->setObjectName(QString::fromUtf8("textureAttributes_box"));
        textureAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(textureAttributes_box->sizePolicy().hasHeightForWidth());
        textureAttributes_box->setSizePolicy(sizePolicy);
        textureAttributes_box->setMinimumSize(QSize(95, 22));
        textureAttributes_box->setMaximumSize(QSize(90, 22));
        textureAttributes_box->setBaseSize(QSize(90, 25));
        textureAttributes_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        textureAttributes_box->setFrame(false);

        areaLayout->addWidget(textureAttributes);

        reloadTexture = new QFrame(layoutWidget1);
        reloadTexture->setObjectName(QString::fromUtf8("reloadTexture"));
        sizePolicy.setHeightForWidth(reloadTexture->sizePolicy().hasHeightForWidth());
        reloadTexture->setSizePolicy(sizePolicy);
        reloadTexture->setMinimumSize(QSize(225, 35));
        reloadTexture->setMaximumSize(QSize(225, 35));
        reloadTexture->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadTexture->setFrameShape(QFrame::Panel);
        reloadTexture->setFrameShadow(QFrame::Raised);
        reloadTexture_btn = new QPushButton(reloadTexture);
        reloadTexture_btn->setObjectName(QString::fromUtf8("reloadTexture_btn"));
        reloadTexture_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadTexture_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        areaLayout->addWidget(reloadTexture);

        _meshSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        areaLayout->addItem(_meshSpacer_5);

        selectedTerrain = new QFrame(layoutWidget1);
        selectedTerrain->setObjectName(QString::fromUtf8("selectedTerrain"));
        sizePolicy.setHeightForWidth(selectedTerrain->sizePolicy().hasHeightForWidth());
        selectedTerrain->setSizePolicy(sizePolicy);
        selectedTerrain->setMinimumSize(QSize(225, 35));
        selectedTerrain->setMaximumSize(QSize(225, 35));
        selectedTerrain->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedTerrain->setFrameShape(QFrame::Panel);
        selectedTerrain->setFrameShadow(QFrame::Raised);
        selectedTerrain_lbl = new QLabel(selectedTerrain);
        selectedTerrain_lbl->setObjectName(QString::fromUtf8("selectedTerrain_lbl"));
        selectedTerrain_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedTerrain_lbl->sizePolicy().hasHeightForWidth());
        selectedTerrain_lbl->setSizePolicy(sizePolicy);
        selectedTerrain_lbl->setMinimumSize(QSize(100, 30));
        selectedTerrain_lbl->setMaximumSize(QSize(100, 30));
        selectedTerrain_lbl->setFont(font);
        selectedTerrain_lbl->setMargin(1);
        selectedTerrain_box = new QComboBox(selectedTerrain);
        selectedTerrain_box->setObjectName(QString::fromUtf8("selectedTerrain_box"));
        selectedTerrain_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedTerrain_box->sizePolicy().hasHeightForWidth());
        selectedTerrain_box->setSizePolicy(sizePolicy);
        selectedTerrain_box->setMinimumSize(QSize(115, 22));
        selectedTerrain_box->setMaximumSize(QSize(115, 22));
        selectedTerrain_box->setBaseSize(QSize(115, 25));
        selectedTerrain_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        terrainShader->setObjectName(QString::fromUtf8("terrainShader"));
        sizePolicy.setHeightForWidth(terrainShader->sizePolicy().hasHeightForWidth());
        terrainShader->setSizePolicy(sizePolicy);
        terrainShader->setMinimumSize(QSize(225, 35));
        terrainShader->setMaximumSize(QSize(225, 35));
        terrainShader->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        terrainShader->setFrameShape(QFrame::Panel);
        terrainShader->setFrameShadow(QFrame::Raised);
        terrainShader_lbl = new QLabel(terrainShader);
        terrainShader_lbl->setObjectName(QString::fromUtf8("terrainShader_lbl"));
        terrainShader_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(terrainShader_lbl->sizePolicy().hasHeightForWidth());
        terrainShader_lbl->setSizePolicy(sizePolicy);
        terrainShader_lbl->setMinimumSize(QSize(100, 30));
        terrainShader_lbl->setMaximumSize(QSize(100, 30));
        terrainShader_lbl->setFont(font1);
        terrainShader_text = new QLabel(terrainShader);
        terrainShader_text->setObjectName(QString::fromUtf8("terrainShader_text"));
        terrainShader_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(terrainShader_text->sizePolicy().hasHeightForWidth());
        terrainShader_text->setSizePolicy(sizePolicy1);
        terrainShader_text->setMinimumSize(QSize(115, 25));
        terrainShader_text->setMaximumSize(QSize(115, 25));
        terrainShader_text->setAutoFillBackground(false);
        terrainShader_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        terrainShader_text->setFrameShape(QFrame::NoFrame);
        terrainShader_text->setFrameShadow(QFrame::Plain);
        terrainShader_text->setLineWidth(0);
        terrainShader_text->setMargin(3);

        areaLayout->addWidget(terrainShader);

        instancesTerrain = new QFrame(layoutWidget1);
        instancesTerrain->setObjectName(QString::fromUtf8("instancesTerrain"));
        sizePolicy.setHeightForWidth(instancesTerrain->sizePolicy().hasHeightForWidth());
        instancesTerrain->setSizePolicy(sizePolicy);
        instancesTerrain->setMinimumSize(QSize(225, 35));
        instancesTerrain->setMaximumSize(QSize(225, 35));
        instancesTerrain->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        instancesTerrain->setFrameShape(QFrame::Panel);
        instancesTerrain->setFrameShadow(QFrame::Raised);
        instancesTerrain_lbl = new QLabel(instancesTerrain);
        instancesTerrain_lbl->setObjectName(QString::fromUtf8("instancesTerrain_lbl"));
        instancesTerrain_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(instancesTerrain_lbl->sizePolicy().hasHeightForWidth());
        instancesTerrain_lbl->setSizePolicy(sizePolicy);
        instancesTerrain_lbl->setMinimumSize(QSize(100, 30));
        instancesTerrain_lbl->setMaximumSize(QSize(100, 30));
        instancesTerrain_lbl->setFont(font1);
        instancesTerrain_text = new QLabel(instancesTerrain);
        instancesTerrain_text->setObjectName(QString::fromUtf8("instancesTerrain_text"));
        instancesTerrain_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(instancesTerrain_text->sizePolicy().hasHeightForWidth());
        instancesTerrain_text->setSizePolicy(sizePolicy1);
        instancesTerrain_text->setMinimumSize(QSize(115, 25));
        instancesTerrain_text->setMaximumSize(QSize(115, 25));
        instancesTerrain_text->setAutoFillBackground(false);
        instancesTerrain_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        instancesTerrain_text->setFrameShape(QFrame::NoFrame);
        instancesTerrain_text->setFrameShadow(QFrame::Plain);
        instancesTerrain_text->setLineWidth(0);
        instancesTerrain_text->setMargin(3);

        areaLayout->addWidget(instancesTerrain);

        terrainAttributes = new QFrame(layoutWidget1);
        terrainAttributes->setObjectName(QString::fromUtf8("terrainAttributes"));
        terrainAttributes->setMinimumSize(QSize(225, 35));
        terrainAttributes->setMaximumSize(QSize(225, 35));
        terrainAttributes->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        terrainAttributes->setFrameShape(QFrame::NoFrame);
        terrainAttributes->setFrameShadow(QFrame::Plain);
        terrainAttributes->setLineWidth(0);
        terrainAttributes_value = new QDoubleSpinBox(terrainAttributes);
        terrainAttributes_value->setObjectName(QString::fromUtf8("terrainAttributes_value"));
        terrainAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(terrainAttributes_value->sizePolicy().hasHeightForWidth());
        terrainAttributes_value->setSizePolicy(sizePolicy);
        terrainAttributes_value->setMinimumSize(QSize(86, 25));
        terrainAttributes_value->setMaximumSize(QSize(86, 25));
        terrainAttributes_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        terrainAttributes_value->setFrame(false);
        terrainAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        terrainAttributes_value->setDecimals(3);
        terrainAttributes_value->setMinimum(-9999.000000000000000);
        terrainAttributes_value->setMaximum(9999.000000000000000);
        terrainAttributes_value->setSingleStep(0.050000000000000);
        terrainAttributes_dial = new QDial(terrainAttributes);
        terrainAttributes_dial->setObjectName(QString::fromUtf8("terrainAttributes_dial"));
        terrainAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(terrainAttributes_dial->sizePolicy().hasHeightForWidth());
        terrainAttributes_dial->setSizePolicy(sizePolicy);
        terrainAttributes_dial->setMinimumSize(QSize(30, 30));
        terrainAttributes_dial->setMaximumSize(QSize(30, 30));
        terrainAttributes_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        terrainAttributes_dial->setMinimum(-10000);
        terrainAttributes_dial->setMaximum(10000);
        terrainAttributes_box = new QComboBox(terrainAttributes);
        terrainAttributes_box->setObjectName(QString::fromUtf8("terrainAttributes_box"));
        terrainAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(terrainAttributes_box->sizePolicy().hasHeightForWidth());
        terrainAttributes_box->setSizePolicy(sizePolicy);
        terrainAttributes_box->setMinimumSize(QSize(95, 22));
        terrainAttributes_box->setMaximumSize(QSize(90, 22));
        terrainAttributes_box->setBaseSize(QSize(90, 25));
        terrainAttributes_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        terrainAttributes_box->setFrame(false);

        areaLayout->addWidget(terrainAttributes);

        reloadTerrain = new QFrame(layoutWidget1);
        reloadTerrain->setObjectName(QString::fromUtf8("reloadTerrain"));
        sizePolicy.setHeightForWidth(reloadTerrain->sizePolicy().hasHeightForWidth());
        reloadTerrain->setSizePolicy(sizePolicy);
        reloadTerrain->setMinimumSize(QSize(225, 35));
        reloadTerrain->setMaximumSize(QSize(225, 35));
        reloadTerrain->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        reloadTerrain->setFrameShape(QFrame::Panel);
        reloadTerrain->setFrameShadow(QFrame::Raised);
        reloadTerrain_btn = new QPushButton(reloadTerrain);
        reloadTerrain_btn->setObjectName(QString::fromUtf8("reloadTerrain_btn"));
        reloadTerrain_btn->setGeometry(QRect(5, 4, 216, 25));
        reloadTerrain_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        areaLayout->addWidget(reloadTerrain);

        _areaSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        areaLayout->addItem(_areaSpacer);

        TabMenu->addTab(Area, QString());
        Mesh = new QWidget();
        Mesh->setObjectName(QString::fromUtf8("Mesh"));
        Mesh->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);"));
        layoutWidget2 = new QWidget(Mesh);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 0, 230, 584));
        meshLayout = new QVBoxLayout(layoutWidget2);
        meshLayout->setSpacing(2);
        meshLayout->setObjectName(QString::fromUtf8("meshLayout"));
        meshLayout->setContentsMargins(4, 4, 0, 0);
        selectedMesh = new QFrame(layoutWidget2);
        selectedMesh->setObjectName(QString::fromUtf8("selectedMesh"));
        sizePolicy.setHeightForWidth(selectedMesh->sizePolicy().hasHeightForWidth());
        selectedMesh->setSizePolicy(sizePolicy);
        selectedMesh->setMinimumSize(QSize(225, 35));
        selectedMesh->setMaximumSize(QSize(225, 35));
        selectedMesh->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedMesh->setFrameShape(QFrame::Panel);
        selectedMesh->setFrameShadow(QFrame::Raised);
        selectedMesh_lbl = new QLabel(selectedMesh);
        selectedMesh_lbl->setObjectName(QString::fromUtf8("selectedMesh_lbl"));
        selectedMesh_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedMesh_lbl->sizePolicy().hasHeightForWidth());
        selectedMesh_lbl->setSizePolicy(sizePolicy);
        selectedMesh_lbl->setMinimumSize(QSize(100, 30));
        selectedMesh_lbl->setMaximumSize(QSize(100, 30));
        selectedMesh_lbl->setFont(font);
        selectedMesh_lbl->setMargin(1);
        selectedMesh_box = new QComboBox(selectedMesh);
        selectedMesh_box->setObjectName(QString::fromUtf8("selectedMesh_box"));
        selectedMesh_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedMesh_box->sizePolicy().hasHeightForWidth());
        selectedMesh_box->setSizePolicy(sizePolicy);
        selectedMesh_box->setMinimumSize(QSize(115, 22));
        selectedMesh_box->setMaximumSize(QSize(115, 22));
        selectedMesh_box->setBaseSize(QSize(115, 25));
        selectedMesh_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedMesh_box->setFrame(false);

        meshLayout->addWidget(selectedMesh);

        shader = new QFrame(layoutWidget2);
        shader->setObjectName(QString::fromUtf8("shader"));
        sizePolicy.setHeightForWidth(shader->sizePolicy().hasHeightForWidth());
        shader->setSizePolicy(sizePolicy);
        shader->setMinimumSize(QSize(225, 35));
        shader->setMaximumSize(QSize(225, 35));
        shader->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        shader->setFrameShape(QFrame::Panel);
        shader->setFrameShadow(QFrame::Raised);
        shader_lbl = new QLabel(shader);
        shader_lbl->setObjectName(QString::fromUtf8("shader_lbl"));
        shader_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(shader_lbl->sizePolicy().hasHeightForWidth());
        shader_lbl->setSizePolicy(sizePolicy);
        shader_lbl->setMinimumSize(QSize(100, 30));
        shader_lbl->setMaximumSize(QSize(100, 30));
        shader_lbl->setFont(font1);
        shader_text = new QLabel(shader);
        shader_text->setObjectName(QString::fromUtf8("shader_text"));
        shader_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(shader_text->sizePolicy().hasHeightForWidth());
        shader_text->setSizePolicy(sizePolicy1);
        shader_text->setMinimumSize(QSize(115, 25));
        shader_text->setMaximumSize(QSize(115, 25));
        shader_text->setAutoFillBackground(false);
        shader_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        shader_text->setFrameShape(QFrame::NoFrame);
        shader_text->setFrameShadow(QFrame::Plain);
        shader_text->setLineWidth(0);
        shader_text->setMargin(3);

        meshLayout->addWidget(shader);

        instancesMesh = new QFrame(layoutWidget2);
        instancesMesh->setObjectName(QString::fromUtf8("instancesMesh"));
        sizePolicy.setHeightForWidth(instancesMesh->sizePolicy().hasHeightForWidth());
        instancesMesh->setSizePolicy(sizePolicy);
        instancesMesh->setMinimumSize(QSize(225, 35));
        instancesMesh->setMaximumSize(QSize(225, 35));
        instancesMesh->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        instancesMesh->setFrameShape(QFrame::Panel);
        instancesMesh->setFrameShadow(QFrame::Raised);
        instancesMesh_lbl = new QLabel(instancesMesh);
        instancesMesh_lbl->setObjectName(QString::fromUtf8("instancesMesh_lbl"));
        instancesMesh_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(instancesMesh_lbl->sizePolicy().hasHeightForWidth());
        instancesMesh_lbl->setSizePolicy(sizePolicy);
        instancesMesh_lbl->setMinimumSize(QSize(100, 30));
        instancesMesh_lbl->setMaximumSize(QSize(100, 30));
        instancesMesh_lbl->setFont(font1);
        instancesMesh_text = new QLabel(instancesMesh);
        instancesMesh_text->setObjectName(QString::fromUtf8("instancesMesh_text"));
        instancesMesh_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(instancesMesh_text->sizePolicy().hasHeightForWidth());
        instancesMesh_text->setSizePolicy(sizePolicy1);
        instancesMesh_text->setMinimumSize(QSize(115, 25));
        instancesMesh_text->setMaximumSize(QSize(115, 25));
        instancesMesh_text->setAutoFillBackground(false);
        instancesMesh_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        instancesMesh_text->setFrameShape(QFrame::NoFrame);
        instancesMesh_text->setFrameShadow(QFrame::Plain);
        instancesMesh_text->setLineWidth(0);
        instancesMesh_text->setMargin(3);

        meshLayout->addWidget(instancesMesh);

        meshAttributes = new QFrame(layoutWidget2);
        meshAttributes->setObjectName(QString::fromUtf8("meshAttributes"));
        meshAttributes->setMinimumSize(QSize(225, 35));
        meshAttributes->setMaximumSize(QSize(225, 35));
        meshAttributes->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        meshAttributes->setFrameShape(QFrame::NoFrame);
        meshAttributes->setFrameShadow(QFrame::Plain);
        meshAttributes->setLineWidth(0);
        meshAttributes_value = new QDoubleSpinBox(meshAttributes);
        meshAttributes_value->setObjectName(QString::fromUtf8("meshAttributes_value"));
        meshAttributes_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(meshAttributes_value->sizePolicy().hasHeightForWidth());
        meshAttributes_value->setSizePolicy(sizePolicy);
        meshAttributes_value->setMinimumSize(QSize(86, 25));
        meshAttributes_value->setMaximumSize(QSize(86, 25));
        meshAttributes_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        meshAttributes_value->setFrame(false);
        meshAttributes_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        meshAttributes_value->setDecimals(3);
        meshAttributes_value->setMinimum(-9999.000000000000000);
        meshAttributes_value->setMaximum(9999.000000000000000);
        meshAttributes_value->setSingleStep(0.050000000000000);
        meshAttributes_dial = new QDial(meshAttributes);
        meshAttributes_dial->setObjectName(QString::fromUtf8("meshAttributes_dial"));
        meshAttributes_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(meshAttributes_dial->sizePolicy().hasHeightForWidth());
        meshAttributes_dial->setSizePolicy(sizePolicy);
        meshAttributes_dial->setMinimumSize(QSize(30, 30));
        meshAttributes_dial->setMaximumSize(QSize(30, 30));
        meshAttributes_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        meshAttributes_dial->setMinimum(-10000);
        meshAttributes_dial->setMaximum(10000);
        meshAttributes_box = new QComboBox(meshAttributes);
        meshAttributes_box->setObjectName(QString::fromUtf8("meshAttributes_box"));
        meshAttributes_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(meshAttributes_box->sizePolicy().hasHeightForWidth());
        meshAttributes_box->setSizePolicy(sizePolicy);
        meshAttributes_box->setMinimumSize(QSize(95, 22));
        meshAttributes_box->setMaximumSize(QSize(90, 22));
        meshAttributes_box->setBaseSize(QSize(90, 25));
        meshAttributes_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        selectedWater->setObjectName(QString::fromUtf8("selectedWater"));
        sizePolicy.setHeightForWidth(selectedWater->sizePolicy().hasHeightForWidth());
        selectedWater->setSizePolicy(sizePolicy);
        selectedWater->setMinimumSize(QSize(225, 35));
        selectedWater->setMaximumSize(QSize(225, 35));
        selectedWater->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedWater->setFrameShape(QFrame::Panel);
        selectedWater->setFrameShadow(QFrame::Raised);
        selectedWater_lbl = new QLabel(selectedWater);
        selectedWater_lbl->setObjectName(QString::fromUtf8("selectedWater_lbl"));
        selectedWater_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedWater_lbl->sizePolicy().hasHeightForWidth());
        selectedWater_lbl->setSizePolicy(sizePolicy);
        selectedWater_lbl->setMinimumSize(QSize(100, 30));
        selectedWater_lbl->setMaximumSize(QSize(100, 30));
        selectedWater_lbl->setFont(font);
        selectedWater_lbl->setMargin(1);
        selectedWater_box = new QComboBox(selectedWater);
        selectedWater_box->setObjectName(QString::fromUtf8("selectedWater_box"));
        selectedWater_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedWater_box->sizePolicy().hasHeightForWidth());
        selectedWater_box->setSizePolicy(sizePolicy);
        selectedWater_box->setMinimumSize(QSize(115, 22));
        selectedWater_box->setMaximumSize(QSize(115, 22));
        selectedWater_box->setBaseSize(QSize(115, 25));
        selectedWater_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedWater_box->setFrame(false);

        meshLayout->addWidget(selectedWater);

        instancesWater = new QFrame(layoutWidget2);
        instancesWater->setObjectName(QString::fromUtf8("instancesWater"));
        sizePolicy.setHeightForWidth(instancesWater->sizePolicy().hasHeightForWidth());
        instancesWater->setSizePolicy(sizePolicy);
        instancesWater->setMinimumSize(QSize(225, 35));
        instancesWater->setMaximumSize(QSize(225, 35));
        instancesWater->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        instancesWater->setFrameShape(QFrame::Panel);
        instancesWater->setFrameShadow(QFrame::Raised);
        instancesWater_lbl = new QLabel(instancesWater);
        instancesWater_lbl->setObjectName(QString::fromUtf8("instancesWater_lbl"));
        instancesWater_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(instancesWater_lbl->sizePolicy().hasHeightForWidth());
        instancesWater_lbl->setSizePolicy(sizePolicy);
        instancesWater_lbl->setMinimumSize(QSize(100, 30));
        instancesWater_lbl->setMaximumSize(QSize(100, 30));
        instancesWater_lbl->setFont(font1);
        instancesWater_text = new QLabel(instancesWater);
        instancesWater_text->setObjectName(QString::fromUtf8("instancesWater_text"));
        instancesWater_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(instancesWater_text->sizePolicy().hasHeightForWidth());
        instancesWater_text->setSizePolicy(sizePolicy1);
        instancesWater_text->setMinimumSize(QSize(115, 25));
        instancesWater_text->setMaximumSize(QSize(115, 25));
        instancesWater_text->setAutoFillBackground(false);
        instancesWater_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        instancesWater_text->setFrameShape(QFrame::NoFrame);
        instancesWater_text->setFrameShadow(QFrame::Plain);
        instancesWater_text->setLineWidth(0);
        instancesWater_text->setMargin(3);

        meshLayout->addWidget(instancesWater);

        waveNumber = new QFrame(layoutWidget2);
        waveNumber->setObjectName(QString::fromUtf8("waveNumber"));
        waveNumber->setMinimumSize(QSize(225, 35));
        waveNumber->setMaximumSize(QSize(225, 35));
        waveNumber->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        waveNumber->setFrameShape(QFrame::NoFrame);
        waveNumber->setFrameShadow(QFrame::Plain);
        waveNumber->setLineWidth(0);
        waveNumber_lbl = new QLabel(waveNumber);
        waveNumber_lbl->setObjectName(QString::fromUtf8("waveNumber_lbl"));
        waveNumber_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(waveNumber_lbl->sizePolicy().hasHeightForWidth());
        waveNumber_lbl->setSizePolicy(sizePolicy);
        waveNumber_lbl->setMinimumSize(QSize(100, 30));
        waveNumber_lbl->setMaximumSize(QSize(100, 30));
        waveNumber_lbl->setFont(font1);
        waveNumber_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        waveNumber_lbl->setMargin(1);
        waveNumber_value = new QDoubleSpinBox(waveNumber);
        waveNumber_value->setObjectName(QString::fromUtf8("waveNumber_value"));
        waveNumber_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(waveNumber_value->sizePolicy().hasHeightForWidth());
        waveNumber_value->setSizePolicy(sizePolicy);
        waveNumber_value->setMinimumSize(QSize(86, 25));
        waveNumber_value->setMaximumSize(QSize(86, 25));
        waveNumber_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        waveNumber_value->setFrame(false);
        waveNumber_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        waveNumber_value->setDecimals(0);
        waveNumber_value->setMinimum(-9999.000000000000000);
        waveNumber_value->setMaximum(9999.000000000000000);
        waveNumber_value->setSingleStep(0.050000000000000);
        waveNumber_dial = new QDial(waveNumber);
        waveNumber_dial->setObjectName(QString::fromUtf8("waveNumber_dial"));
        waveNumber_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(waveNumber_dial->sizePolicy().hasHeightForWidth());
        waveNumber_dial->setSizePolicy(sizePolicy);
        waveNumber_dial->setMinimumSize(QSize(30, 30));
        waveNumber_dial->setMaximumSize(QSize(30, 30));
        waveNumber_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        waveNumber_dial->setMinimum(-10000);
        waveNumber_dial->setMaximum(10000);

        meshLayout->addWidget(waveNumber);

        wave = new QFrame(layoutWidget2);
        wave->setObjectName(QString::fromUtf8("wave"));
        wave->setMinimumSize(QSize(225, 35));
        wave->setMaximumSize(QSize(225, 35));
        wave->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        wave->setFrameShape(QFrame::NoFrame);
        wave->setFrameShadow(QFrame::Plain);
        wave->setLineWidth(0);
        wave_value = new QDoubleSpinBox(wave);
        wave_value->setObjectName(QString::fromUtf8("wave_value"));
        wave_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(wave_value->sizePolicy().hasHeightForWidth());
        wave_value->setSizePolicy(sizePolicy);
        wave_value->setMinimumSize(QSize(86, 25));
        wave_value->setMaximumSize(QSize(86, 25));
        wave_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        wave_value->setFrame(false);
        wave_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        wave_value->setDecimals(3);
        wave_value->setMinimum(-9999.000000000000000);
        wave_value->setMaximum(9999.000000000000000);
        wave_value->setSingleStep(0.050000000000000);
        wave_dial = new QDial(wave);
        wave_dial->setObjectName(QString::fromUtf8("wave_dial"));
        wave_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(wave_dial->sizePolicy().hasHeightForWidth());
        wave_dial->setSizePolicy(sizePolicy);
        wave_dial->setMinimumSize(QSize(30, 30));
        wave_dial->setMaximumSize(QSize(30, 30));
        wave_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        wave_dial->setMinimum(-10000);
        wave_dial->setMaximum(10000);
        wave_box = new QComboBox(wave);
        wave_box->setObjectName(QString::fromUtf8("wave_box"));
        wave_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(wave_box->sizePolicy().hasHeightForWidth());
        wave_box->setSizePolicy(sizePolicy);
        wave_box->setMinimumSize(QSize(95, 22));
        wave_box->setMaximumSize(QSize(90, 22));
        wave_box->setBaseSize(QSize(90, 25));
        wave_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        wave_box->setFrame(false);

        meshLayout->addWidget(wave);

        water = new QFrame(layoutWidget2);
        water->setObjectName(QString::fromUtf8("water"));
        water->setMinimumSize(QSize(225, 35));
        water->setMaximumSize(QSize(225, 35));
        water->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        water->setFrameShape(QFrame::NoFrame);
        water->setFrameShadow(QFrame::Plain);
        water->setLineWidth(0);
        water_value = new QDoubleSpinBox(water);
        water_value->setObjectName(QString::fromUtf8("water_value"));
        water_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(water_value->sizePolicy().hasHeightForWidth());
        water_value->setSizePolicy(sizePolicy);
        water_value->setMinimumSize(QSize(86, 25));
        water_value->setMaximumSize(QSize(86, 25));
        water_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        water_value->setFrame(false);
        water_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        water_value->setDecimals(3);
        water_value->setMinimum(-9999.000000000000000);
        water_value->setMaximum(9999.000000000000000);
        water_value->setSingleStep(0.050000000000000);
        water_dial = new QDial(water);
        water_dial->setObjectName(QString::fromUtf8("water_dial"));
        water_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(water_dial->sizePolicy().hasHeightForWidth());
        water_dial->setSizePolicy(sizePolicy);
        water_dial->setMinimumSize(QSize(30, 30));
        water_dial->setMaximumSize(QSize(30, 30));
        water_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        water_dial->setMinimum(-10000);
        water_dial->setMaximum(10000);
        water_box = new QComboBox(water);
        water_box->setObjectName(QString::fromUtf8("water_box"));
        water_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(water_box->sizePolicy().hasHeightForWidth());
        water_box->setSizePolicy(sizePolicy);
        water_box->setMinimumSize(QSize(95, 22));
        water_box->setMaximumSize(QSize(90, 22));
        water_box->setBaseSize(QSize(90, 25));
        water_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        selectedParticles->setObjectName(QString::fromUtf8("selectedParticles"));
        sizePolicy.setHeightForWidth(selectedParticles->sizePolicy().hasHeightForWidth());
        selectedParticles->setSizePolicy(sizePolicy);
        selectedParticles->setMinimumSize(QSize(225, 35));
        selectedParticles->setMaximumSize(QSize(225, 35));
        selectedParticles->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        selectedParticles->setFrameShape(QFrame::Panel);
        selectedParticles->setFrameShadow(QFrame::Raised);
        selectedParticles_lbl = new QLabel(selectedParticles);
        selectedParticles_lbl->setObjectName(QString::fromUtf8("selectedParticles_lbl"));
        selectedParticles_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(selectedParticles_lbl->sizePolicy().hasHeightForWidth());
        selectedParticles_lbl->setSizePolicy(sizePolicy);
        selectedParticles_lbl->setMinimumSize(QSize(100, 30));
        selectedParticles_lbl->setMaximumSize(QSize(100, 30));
        selectedParticles_lbl->setFont(font);
        selectedParticles_lbl->setMargin(1);
        selectedParticles_box = new QComboBox(selectedParticles);
        selectedParticles_box->setObjectName(QString::fromUtf8("selectedParticles_box"));
        selectedParticles_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(selectedParticles_box->sizePolicy().hasHeightForWidth());
        selectedParticles_box->setSizePolicy(sizePolicy);
        selectedParticles_box->setMinimumSize(QSize(115, 22));
        selectedParticles_box->setMaximumSize(QSize(115, 22));
        selectedParticles_box->setBaseSize(QSize(115, 25));
        selectedParticles_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        selectedParticles_box->setFrame(false);

        meshLayout->addWidget(selectedParticles);

        instancesEmitter = new QFrame(layoutWidget2);
        instancesEmitter->setObjectName(QString::fromUtf8("instancesEmitter"));
        sizePolicy.setHeightForWidth(instancesEmitter->sizePolicy().hasHeightForWidth());
        instancesEmitter->setSizePolicy(sizePolicy);
        instancesEmitter->setMinimumSize(QSize(225, 35));
        instancesEmitter->setMaximumSize(QSize(225, 35));
        instancesEmitter->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        instancesEmitter->setFrameShape(QFrame::Panel);
        instancesEmitter->setFrameShadow(QFrame::Raised);
        instancesEmitter_lbl = new QLabel(instancesEmitter);
        instancesEmitter_lbl->setObjectName(QString::fromUtf8("instancesEmitter_lbl"));
        instancesEmitter_lbl->setGeometry(QRect(3, 2, 100, 30));
        sizePolicy.setHeightForWidth(instancesEmitter_lbl->sizePolicy().hasHeightForWidth());
        instancesEmitter_lbl->setSizePolicy(sizePolicy);
        instancesEmitter_lbl->setMinimumSize(QSize(100, 30));
        instancesEmitter_lbl->setMaximumSize(QSize(100, 30));
        instancesEmitter_lbl->setFont(font1);
        instancesEmitter_text = new QLabel(instancesEmitter);
        instancesEmitter_text->setObjectName(QString::fromUtf8("instancesEmitter_text"));
        instancesEmitter_text->setGeometry(QRect(106, 5, 115, 25));
        sizePolicy1.setHeightForWidth(instancesEmitter_text->sizePolicy().hasHeightForWidth());
        instancesEmitter_text->setSizePolicy(sizePolicy1);
        instancesEmitter_text->setMinimumSize(QSize(115, 25));
        instancesEmitter_text->setMaximumSize(QSize(115, 25));
        instancesEmitter_text->setAutoFillBackground(false);
        instancesEmitter_text->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
""));
        instancesEmitter_text->setFrameShape(QFrame::NoFrame);
        instancesEmitter_text->setFrameShadow(QFrame::Plain);
        instancesEmitter_text->setLineWidth(0);
        instancesEmitter_text->setMargin(3);

        meshLayout->addWidget(instancesEmitter);

        emitter = new QFrame(layoutWidget2);
        emitter->setObjectName(QString::fromUtf8("emitter"));
        emitter->setMinimumSize(QSize(225, 35));
        emitter->setMaximumSize(QSize(225, 35));
        emitter->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        emitter->setFrameShape(QFrame::NoFrame);
        emitter->setFrameShadow(QFrame::Plain);
        emitter->setLineWidth(0);
        emitter_value = new QDoubleSpinBox(emitter);
        emitter_value->setObjectName(QString::fromUtf8("emitter_value"));
        emitter_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(emitter_value->sizePolicy().hasHeightForWidth());
        emitter_value->setSizePolicy(sizePolicy);
        emitter_value->setMinimumSize(QSize(86, 25));
        emitter_value->setMaximumSize(QSize(86, 25));
        emitter_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitter_value->setFrame(false);
        emitter_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        emitter_value->setDecimals(3);
        emitter_value->setMinimum(-9999.000000000000000);
        emitter_value->setMaximum(9999.000000000000000);
        emitter_value->setSingleStep(0.050000000000000);
        emitter_dial = new QDial(emitter);
        emitter_dial->setObjectName(QString::fromUtf8("emitter_dial"));
        emitter_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(emitter_dial->sizePolicy().hasHeightForWidth());
        emitter_dial->setSizePolicy(sizePolicy);
        emitter_dial->setMinimumSize(QSize(30, 30));
        emitter_dial->setMaximumSize(QSize(30, 30));
        emitter_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        emitter_dial->setMinimum(-10000);
        emitter_dial->setMaximum(10000);
        emitter_box = new QComboBox(emitter);
        emitter_box->setObjectName(QString::fromUtf8("emitter_box"));
        emitter_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(emitter_box->sizePolicy().hasHeightForWidth());
        emitter_box->setSizePolicy(sizePolicy);
        emitter_box->setMinimumSize(QSize(95, 22));
        emitter_box->setMaximumSize(QSize(90, 22));
        emitter_box->setBaseSize(QSize(90, 25));
        emitter_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitter_box->setMaxVisibleItems(20);
        emitter_box->setFrame(false);

        meshLayout->addWidget(emitter);

        emitterMinMax = new QFrame(layoutWidget2);
        emitterMinMax->setObjectName(QString::fromUtf8("emitterMinMax"));
        emitterMinMax->setMinimumSize(QSize(225, 35));
        emitterMinMax->setMaximumSize(QSize(225, 35));
        emitterMinMax->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        emitterMinMax->setFrameShape(QFrame::NoFrame);
        emitterMinMax->setFrameShadow(QFrame::Plain);
        emitterMinMax->setLineWidth(0);
        emitterMinMax_value = new QDoubleSpinBox(emitterMinMax);
        emitterMinMax_value->setObjectName(QString::fromUtf8("emitterMinMax_value"));
        emitterMinMax_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(emitterMinMax_value->sizePolicy().hasHeightForWidth());
        emitterMinMax_value->setSizePolicy(sizePolicy);
        emitterMinMax_value->setMinimumSize(QSize(86, 25));
        emitterMinMax_value->setMaximumSize(QSize(86, 25));
        emitterMinMax_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterMinMax_value->setFrame(false);
        emitterMinMax_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        emitterMinMax_value->setDecimals(3);
        emitterMinMax_value->setMinimum(-9999.000000000000000);
        emitterMinMax_value->setMaximum(9999.000000000000000);
        emitterMinMax_value->setSingleStep(0.050000000000000);
        emitterMinMax_dial = new QDial(emitterMinMax);
        emitterMinMax_dial->setObjectName(QString::fromUtf8("emitterMinMax_dial"));
        emitterMinMax_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(emitterMinMax_dial->sizePolicy().hasHeightForWidth());
        emitterMinMax_dial->setSizePolicy(sizePolicy);
        emitterMinMax_dial->setMinimumSize(QSize(30, 30));
        emitterMinMax_dial->setMaximumSize(QSize(30, 30));
        emitterMinMax_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        emitterMinMax_dial->setMinimum(-10000);
        emitterMinMax_dial->setMaximum(10000);
        emitterMinMax_box = new QComboBox(emitterMinMax);
        emitterMinMax_box->setObjectName(QString::fromUtf8("emitterMinMax_box"));
        emitterMinMax_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(emitterMinMax_box->sizePolicy().hasHeightForWidth());
        emitterMinMax_box->setSizePolicy(sizePolicy);
        emitterMinMax_box->setMinimumSize(QSize(95, 22));
        emitterMinMax_box->setMaximumSize(QSize(90, 22));
        emitterMinMax_box->setBaseSize(QSize(90, 25));
        emitterMinMax_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        emitterMinMax_box->setMaxVisibleItems(20);
        emitterMinMax_box->setFrame(false);

        meshLayout->addWidget(emitterMinMax);

        pauseEmission = new QFrame(layoutWidget2);
        pauseEmission->setObjectName(QString::fromUtf8("pauseEmission"));
        sizePolicy.setHeightForWidth(pauseEmission->sizePolicy().hasHeightForWidth());
        pauseEmission->setSizePolicy(sizePolicy);
        pauseEmission->setMinimumSize(QSize(225, 35));
        pauseEmission->setMaximumSize(QSize(225, 35));
        pauseEmission->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        pauseEmission->setFrameShape(QFrame::Panel);
        pauseEmission->setFrameShadow(QFrame::Raised);
        pauseEmission_btn = new QPushButton(pauseEmission);
        pauseEmission_btn->setObjectName(QString::fromUtf8("pauseEmission_btn"));
        pauseEmission_btn->setGeometry(QRect(5, 4, 216, 25));
        pauseEmission_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        meshLayout->addWidget(pauseEmission);

        _meshSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        meshLayout->addItem(_meshSpacer);

        TabMenu->addTab(Mesh, QString());
        Post = new QWidget();
        Post->setObjectName(QString::fromUtf8("Post"));
        Post->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);"));
        layoutWidget3 = new QWidget(Post);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(0, 0, 230, 561));
        postLayout = new QVBoxLayout(layoutWidget3);
        postLayout->setSpacing(2);
        postLayout->setObjectName(QString::fromUtf8("postLayout"));
        postLayout->setContentsMargins(4, 4, 0, 0);
        postImage = new QFrame(layoutWidget3);
        postImage->setObjectName(QString::fromUtf8("postImage"));
        sizePolicy.setHeightForWidth(postImage->sizePolicy().hasHeightForWidth());
        postImage->setSizePolicy(sizePolicy);
        postImage->setMinimumSize(QSize(225, 35));
        postImage->setMaximumSize(QSize(225, 35));
        postImage->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        postImage->setFrameShape(QFrame::Panel);
        postImage->setFrameShadow(QFrame::Raised);
        postImage_lbl = new QLabel(postImage);
        postImage_lbl->setObjectName(QString::fromUtf8("postImage_lbl"));
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
        postImage_box->setObjectName(QString::fromUtf8("postImage_box"));
        postImage_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(postImage_box->sizePolicy().hasHeightForWidth());
        postImage_box->setSizePolicy(sizePolicy);
        postImage_box->setMinimumSize(QSize(115, 22));
        postImage_box->setMaximumSize(QSize(115, 22));
        postImage_box->setBaseSize(QSize(115, 25));
        postImage_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        postImage_box->setFrame(false);

        postLayout->addWidget(postImage);

        post = new QFrame(layoutWidget3);
        post->setObjectName(QString::fromUtf8("post"));
        post->setMinimumSize(QSize(225, 35));
        post->setMaximumSize(QSize(225, 35));
        post->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        post->setFrameShape(QFrame::NoFrame);
        post->setFrameShadow(QFrame::Plain);
        post->setLineWidth(0);
        post_value = new QDoubleSpinBox(post);
        post_value->setObjectName(QString::fromUtf8("post_value"));
        post_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(post_value->sizePolicy().hasHeightForWidth());
        post_value->setSizePolicy(sizePolicy);
        post_value->setMinimumSize(QSize(86, 25));
        post_value->setMaximumSize(QSize(86, 25));
        post_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        post_value->setFrame(false);
        post_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        post_value->setDecimals(3);
        post_value->setMinimum(-9999.000000000000000);
        post_value->setMaximum(9999.000000000000000);
        post_value->setSingleStep(0.050000000000000);
        post_dial = new QDial(post);
        post_dial->setObjectName(QString::fromUtf8("post_dial"));
        post_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(post_dial->sizePolicy().hasHeightForWidth());
        post_dial->setSizePolicy(sizePolicy);
        post_dial->setMinimumSize(QSize(30, 30));
        post_dial->setMaximumSize(QSize(30, 30));
        post_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        post_dial->setMinimum(-10000);
        post_dial->setMaximum(10000);
        post_box = new QComboBox(post);
        post_box->setObjectName(QString::fromUtf8("post_box"));
        post_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(post_box->sizePolicy().hasHeightForWidth());
        post_box->setSizePolicy(sizePolicy);
        post_box->setMinimumSize(QSize(95, 22));
        post_box->setMaximumSize(QSize(90, 22));
        post_box->setBaseSize(QSize(90, 25));
        post_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        post_box->setFrame(false);

        postLayout->addWidget(post);

        correction = new QFrame(layoutWidget3);
        correction->setObjectName(QString::fromUtf8("correction"));
        correction->setMinimumSize(QSize(225, 35));
        correction->setMaximumSize(QSize(225, 35));
        correction->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        correction->setFrameShape(QFrame::NoFrame);
        correction->setFrameShadow(QFrame::Plain);
        correction->setLineWidth(0);
        correction_value = new QDoubleSpinBox(correction);
        correction_value->setObjectName(QString::fromUtf8("correction_value"));
        correction_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(correction_value->sizePolicy().hasHeightForWidth());
        correction_value->setSizePolicy(sizePolicy);
        correction_value->setMinimumSize(QSize(86, 25));
        correction_value->setMaximumSize(QSize(86, 25));
        correction_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        correction_value->setFrame(false);
        correction_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        correction_value->setDecimals(3);
        correction_value->setMinimum(-9999.000000000000000);
        correction_value->setMaximum(9999.000000000000000);
        correction_value->setSingleStep(0.050000000000000);
        correction_dial = new QDial(correction);
        correction_dial->setObjectName(QString::fromUtf8("correction_dial"));
        correction_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(correction_dial->sizePolicy().hasHeightForWidth());
        correction_dial->setSizePolicy(sizePolicy);
        correction_dial->setMinimumSize(QSize(30, 30));
        correction_dial->setMaximumSize(QSize(30, 30));
        correction_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        correction_dial->setMinimum(-10000);
        correction_dial->setMaximum(10000);
        correction_box = new QComboBox(correction);
        correction_box->setObjectName(QString::fromUtf8("correction_box"));
        correction_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(correction_box->sizePolicy().hasHeightForWidth());
        correction_box->setSizePolicy(sizePolicy);
        correction_box->setMinimumSize(QSize(95, 22));
        correction_box->setMaximumSize(QSize(90, 22));
        correction_box->setBaseSize(QSize(90, 25));
        correction_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        correction_box->setFrame(false);

        postLayout->addWidget(correction);

        fog = new QFrame(layoutWidget3);
        fog->setObjectName(QString::fromUtf8("fog"));
        fog->setMinimumSize(QSize(225, 35));
        fog->setMaximumSize(QSize(225, 35));
        fog->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        fog->setFrameShape(QFrame::NoFrame);
        fog->setFrameShadow(QFrame::Plain);
        fog->setLineWidth(0);
        fog_value = new QDoubleSpinBox(fog);
        fog_value->setObjectName(QString::fromUtf8("fog_value"));
        fog_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(fog_value->sizePolicy().hasHeightForWidth());
        fog_value->setSizePolicy(sizePolicy);
        fog_value->setMinimumSize(QSize(86, 25));
        fog_value->setMaximumSize(QSize(86, 25));
        fog_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        fog_value->setFrame(false);
        fog_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        fog_value->setDecimals(3);
        fog_value->setMinimum(-9999.000000000000000);
        fog_value->setMaximum(9999.000000000000000);
        fog_value->setSingleStep(0.050000000000000);
        fog_dial = new QDial(fog);
        fog_dial->setObjectName(QString::fromUtf8("fog_dial"));
        fog_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(fog_dial->sizePolicy().hasHeightForWidth());
        fog_dial->setSizePolicy(sizePolicy);
        fog_dial->setMinimumSize(QSize(30, 30));
        fog_dial->setMaximumSize(QSize(30, 30));
        fog_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        fog_dial->setMinimum(-10000);
        fog_dial->setMaximum(10000);
        fog_box = new QComboBox(fog);
        fog_box->setObjectName(QString::fromUtf8("fog_box"));
        fog_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(fog_box->sizePolicy().hasHeightForWidth());
        fog_box->setSizePolicy(sizePolicy);
        fog_box->setMinimumSize(QSize(95, 22));
        fog_box->setMaximumSize(QSize(90, 22));
        fog_box->setBaseSize(QSize(90, 25));
        fog_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        fog_box->setFrame(false);

        postLayout->addWidget(fog);

        toggleWireframe = new QFrame(layoutWidget3);
        toggleWireframe->setObjectName(QString::fromUtf8("toggleWireframe"));
        sizePolicy.setHeightForWidth(toggleWireframe->sizePolicy().hasHeightForWidth());
        toggleWireframe->setSizePolicy(sizePolicy);
        toggleWireframe->setMinimumSize(QSize(225, 35));
        toggleWireframe->setMaximumSize(QSize(225, 35));
        toggleWireframe->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        toggleWireframe->setFrameShape(QFrame::Panel);
        toggleWireframe->setFrameShadow(QFrame::Raised);
        toggleWireframe_btn = new QPushButton(toggleWireframe);
        toggleWireframe_btn->setObjectName(QString::fromUtf8("toggleWireframe_btn"));
        toggleWireframe_btn->setGeometry(QRect(5, 4, 216, 25));
        toggleWireframe_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        postLayout->addWidget(toggleWireframe);

        _meshSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        postLayout->addItem(_meshSpacer_4);

        lightSelected = new QFrame(layoutWidget3);
        lightSelected->setObjectName(QString::fromUtf8("lightSelected"));
        sizePolicy.setHeightForWidth(lightSelected->sizePolicy().hasHeightForWidth());
        lightSelected->setSizePolicy(sizePolicy);
        lightSelected->setMinimumSize(QSize(225, 35));
        lightSelected->setMaximumSize(QSize(225, 35));
        lightSelected->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        lightSelected->setFrameShape(QFrame::Panel);
        lightSelected->setFrameShadow(QFrame::Raised);
        lightSelected_lbl = new QLabel(lightSelected);
        lightSelected_lbl->setObjectName(QString::fromUtf8("lightSelected_lbl"));
        lightSelected_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(lightSelected_lbl->sizePolicy().hasHeightForWidth());
        lightSelected_lbl->setSizePolicy(sizePolicy);
        lightSelected_lbl->setMinimumSize(QSize(100, 30));
        lightSelected_lbl->setMaximumSize(QSize(100, 30));
        lightSelected_lbl->setFont(font);
        lightSelected_lbl->setMargin(1);
        lightSelected_box = new QComboBox(lightSelected);
        lightSelected_box->setObjectName(QString::fromUtf8("lightSelected_box"));
        lightSelected_box->setGeometry(QRect(104, 6, 115, 22));
        sizePolicy.setHeightForWidth(lightSelected_box->sizePolicy().hasHeightForWidth());
        lightSelected_box->setSizePolicy(sizePolicy);
        lightSelected_box->setMinimumSize(QSize(115, 22));
        lightSelected_box->setMaximumSize(QSize(115, 22));
        lightSelected_box->setBaseSize(QSize(115, 25));
        lightSelected_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        light_active->setObjectName(QString::fromUtf8("light_active"));
        light_active->setMinimumSize(QSize(225, 35));
        light_active->setMaximumSize(QSize(225, 35));
        light_active->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_active->setFrameShape(QFrame::NoFrame);
        light_active->setFrameShadow(QFrame::Plain);
        light_active->setLineWidth(0);
        light_active_lbl = new QLabel(light_active);
        light_active_lbl->setObjectName(QString::fromUtf8("light_active_lbl"));
        light_active_lbl->setGeometry(QRect(2, 2, 100, 30));
        sizePolicy.setHeightForWidth(light_active_lbl->sizePolicy().hasHeightForWidth());
        light_active_lbl->setSizePolicy(sizePolicy);
        light_active_lbl->setMinimumSize(QSize(100, 30));
        light_active_lbl->setMaximumSize(QSize(100, 30));
        light_active_lbl->setFont(font1);
        light_active_lbl->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_active_lbl->setMargin(1);
        light_active_value = new QDoubleSpinBox(light_active);
        light_active_value->setObjectName(QString::fromUtf8("light_active_value"));
        light_active_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_active_value->sizePolicy().hasHeightForWidth());
        light_active_value->setSizePolicy(sizePolicy);
        light_active_value->setMinimumSize(QSize(86, 25));
        light_active_value->setMaximumSize(QSize(86, 25));
        light_active_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_active_value->setFrame(false);
        light_active_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_active_value->setDecimals(3);
        light_active_value->setMinimum(0.000000000000000);
        light_active_value->setMaximum(1.000000000000000);
        light_active_value->setSingleStep(0.050000000000000);
        light_active_dial = new QDial(light_active);
        light_active_dial->setObjectName(QString::fromUtf8("light_active_dial"));
        light_active_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_active_dial->sizePolicy().hasHeightForWidth());
        light_active_dial->setSizePolicy(sizePolicy);
        light_active_dial->setMinimumSize(QSize(30, 30));
        light_active_dial->setMaximumSize(QSize(30, 30));
        light_active_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_active_dial->setMinimum(-10000);
        light_active_dial->setMaximum(10000);

        postLayout->addWidget(light_active);

        light_position = new QFrame(layoutWidget3);
        light_position->setObjectName(QString::fromUtf8("light_position"));
        light_position->setMinimumSize(QSize(225, 35));
        light_position->setMaximumSize(QSize(225, 35));
        light_position->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_position->setFrameShape(QFrame::NoFrame);
        light_position->setFrameShadow(QFrame::Plain);
        light_position->setLineWidth(0);
        light_position_value = new QDoubleSpinBox(light_position);
        light_position_value->setObjectName(QString::fromUtf8("light_position_value"));
        light_position_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_position_value->sizePolicy().hasHeightForWidth());
        light_position_value->setSizePolicy(sizePolicy);
        light_position_value->setMinimumSize(QSize(86, 25));
        light_position_value->setMaximumSize(QSize(86, 25));
        light_position_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_position_value->setFrame(false);
        light_position_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_position_value->setDecimals(3);
        light_position_value->setMinimum(-9999.000000000000000);
        light_position_value->setMaximum(9999.000000000000000);
        light_position_value->setSingleStep(0.050000000000000);
        light_position_dial = new QDial(light_position);
        light_position_dial->setObjectName(QString::fromUtf8("light_position_dial"));
        light_position_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_position_dial->sizePolicy().hasHeightForWidth());
        light_position_dial->setSizePolicy(sizePolicy);
        light_position_dial->setMinimumSize(QSize(30, 30));
        light_position_dial->setMaximumSize(QSize(30, 30));
        light_position_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_position_dial->setMinimum(-10000);
        light_position_dial->setMaximum(10000);
        light_position_box = new QComboBox(light_position);
        light_position_box->setObjectName(QString::fromUtf8("light_position_box"));
        light_position_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_position_box->sizePolicy().hasHeightForWidth());
        light_position_box->setSizePolicy(sizePolicy);
        light_position_box->setMinimumSize(QSize(95, 22));
        light_position_box->setMaximumSize(QSize(90, 22));
        light_position_box->setBaseSize(QSize(90, 25));
        light_position_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        light_attenuation->setObjectName(QString::fromUtf8("light_attenuation"));
        light_attenuation->setMinimumSize(QSize(225, 35));
        light_attenuation->setMaximumSize(QSize(225, 35));
        light_attenuation->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_attenuation->setFrameShape(QFrame::NoFrame);
        light_attenuation->setFrameShadow(QFrame::Plain);
        light_attenuation->setLineWidth(0);
        light_attenuation_value = new QDoubleSpinBox(light_attenuation);
        light_attenuation_value->setObjectName(QString::fromUtf8("light_attenuation_value"));
        light_attenuation_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_attenuation_value->sizePolicy().hasHeightForWidth());
        light_attenuation_value->setSizePolicy(sizePolicy);
        light_attenuation_value->setMinimumSize(QSize(86, 25));
        light_attenuation_value->setMaximumSize(QSize(86, 25));
        light_attenuation_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_attenuation_value->setFrame(false);
        light_attenuation_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_attenuation_value->setDecimals(3);
        light_attenuation_value->setMinimum(-9999.000000000000000);
        light_attenuation_value->setMaximum(9999.000000000000000);
        light_attenuation_value->setSingleStep(0.050000000000000);
        light_attenuation_dial = new QDial(light_attenuation);
        light_attenuation_dial->setObjectName(QString::fromUtf8("light_attenuation_dial"));
        light_attenuation_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_attenuation_dial->sizePolicy().hasHeightForWidth());
        light_attenuation_dial->setSizePolicy(sizePolicy);
        light_attenuation_dial->setMinimumSize(QSize(30, 30));
        light_attenuation_dial->setMaximumSize(QSize(30, 30));
        light_attenuation_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_attenuation_dial->setMinimum(-10000);
        light_attenuation_dial->setMaximum(10000);
        light_attenuation_box = new QComboBox(light_attenuation);
        light_attenuation_box->setObjectName(QString::fromUtf8("light_attenuation_box"));
        light_attenuation_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_attenuation_box->sizePolicy().hasHeightForWidth());
        light_attenuation_box->setSizePolicy(sizePolicy);
        light_attenuation_box->setMinimumSize(QSize(95, 22));
        light_attenuation_box->setMaximumSize(QSize(90, 22));
        light_attenuation_box->setBaseSize(QSize(90, 25));
        light_attenuation_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
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
        light_colour->setObjectName(QString::fromUtf8("light_colour"));
        light_colour->setMinimumSize(QSize(225, 35));
        light_colour->setMaximumSize(QSize(225, 35));
        light_colour->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_colour->setFrameShape(QFrame::NoFrame);
        light_colour->setFrameShadow(QFrame::Plain);
        light_colour->setLineWidth(0);
        light_colour_value = new QDoubleSpinBox(light_colour);
        light_colour_value->setObjectName(QString::fromUtf8("light_colour_value"));
        light_colour_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_colour_value->sizePolicy().hasHeightForWidth());
        light_colour_value->setSizePolicy(sizePolicy);
        light_colour_value->setMinimumSize(QSize(86, 25));
        light_colour_value->setMaximumSize(QSize(86, 25));
        light_colour_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_colour_value->setFrame(false);
        light_colour_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_colour_value->setDecimals(3);
        light_colour_value->setMinimum(-9999.000000000000000);
        light_colour_value->setMaximum(9999.000000000000000);
        light_colour_value->setSingleStep(0.050000000000000);
        light_colour_dial = new QDial(light_colour);
        light_colour_dial->setObjectName(QString::fromUtf8("light_colour_dial"));
        light_colour_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_colour_dial->sizePolicy().hasHeightForWidth());
        light_colour_dial->setSizePolicy(sizePolicy);
        light_colour_dial->setMinimumSize(QSize(30, 30));
        light_colour_dial->setMaximumSize(QSize(30, 30));
        light_colour_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_colour_dial->setMinimum(-10000);
        light_colour_dial->setMaximum(10000);
        light_colour_box = new QComboBox(light_colour);
        light_colour_box->setObjectName(QString::fromUtf8("light_colour_box"));
        light_colour_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_colour_box->sizePolicy().hasHeightForWidth());
        light_colour_box->setSizePolicy(sizePolicy);
        light_colour_box->setMinimumSize(QSize(95, 22));
        light_colour_box->setMaximumSize(QSize(90, 22));
        light_colour_box->setBaseSize(QSize(90, 25));
        light_colour_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_colour_box->setMaxVisibleItems(30);
        light_colour_box->setFrame(false);

        postLayout->addWidget(light_colour);

        light_specular = new QFrame(layoutWidget3);
        light_specular->setObjectName(QString::fromUtf8("light_specular"));
        light_specular->setMinimumSize(QSize(225, 35));
        light_specular->setMaximumSize(QSize(225, 35));
        light_specular->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_specular->setFrameShape(QFrame::NoFrame);
        light_specular->setFrameShadow(QFrame::Plain);
        light_specular->setLineWidth(0);
        light_specular_value = new QDoubleSpinBox(light_specular);
        light_specular_value->setObjectName(QString::fromUtf8("light_specular_value"));
        light_specular_value->setGeometry(QRect(104, 5, 86, 25));
        sizePolicy.setHeightForWidth(light_specular_value->sizePolicy().hasHeightForWidth());
        light_specular_value->setSizePolicy(sizePolicy);
        light_specular_value->setMinimumSize(QSize(86, 25));
        light_specular_value->setMaximumSize(QSize(86, 25));
        light_specular_value->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_specular_value->setFrame(false);
        light_specular_value->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        light_specular_value->setDecimals(3);
        light_specular_value->setMinimum(-9999.000000000000000);
        light_specular_value->setMaximum(9999.000000000000000);
        light_specular_value->setSingleStep(0.050000000000000);
        light_specular_dial = new QDial(light_specular);
        light_specular_dial->setObjectName(QString::fromUtf8("light_specular_dial"));
        light_specular_dial->setGeometry(QRect(192, 2, 30, 30));
        sizePolicy.setHeightForWidth(light_specular_dial->sizePolicy().hasHeightForWidth());
        light_specular_dial->setSizePolicy(sizePolicy);
        light_specular_dial->setMinimumSize(QSize(30, 30));
        light_specular_dial->setMaximumSize(QSize(30, 30));
        light_specular_dial->setStyleSheet(QString::fromUtf8("border-width: 0px;"));
        light_specular_dial->setMinimum(-10000);
        light_specular_dial->setMaximum(10000);
        light_specular_box = new QComboBox(light_specular);
        light_specular_box->setObjectName(QString::fromUtf8("light_specular_box"));
        light_specular_box->setGeometry(QRect(3, 6, 95, 22));
        sizePolicy.setHeightForWidth(light_specular_box->sizePolicy().hasHeightForWidth());
        light_specular_box->setSizePolicy(sizePolicy);
        light_specular_box->setMinimumSize(QSize(95, 22));
        light_specular_box->setMaximumSize(QSize(90, 22));
        light_specular_box->setBaseSize(QSize(90, 25));
        light_specular_box->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(180, 180, 180);\n"
"border-left-color: rgb(180, 180, 180);\n"
"border-bottom-color: rgb(255, 255, 255);\n"
"border-right-color: rgb(255, 255, 255);\n"
"border-style: solid;\n"
"border-width: 2px;"));
        light_specular_box->setMaxVisibleItems(30);
        light_specular_box->setFrame(false);

        postLayout->addWidget(light_specular);

        light_render_only = new QFrame(layoutWidget3);
        light_render_only->setObjectName(QString::fromUtf8("light_render_only"));
        sizePolicy.setHeightForWidth(light_render_only->sizePolicy().hasHeightForWidth());
        light_render_only->setSizePolicy(sizePolicy);
        light_render_only->setMinimumSize(QSize(225, 35));
        light_render_only->setMaximumSize(QSize(225, 35));
        light_render_only->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_render_only->setFrameShape(QFrame::Panel);
        light_render_only->setFrameShadow(QFrame::Raised);
        light_render_only_btn = new QPushButton(light_render_only);
        light_render_only_btn->setObjectName(QString::fromUtf8("light_render_only_btn"));
        light_render_only_btn->setGeometry(QRect(5, 4, 216, 25));
        light_render_only_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        postLayout->addWidget(light_render_only);

        light_diagnostics = new QFrame(layoutWidget3);
        light_diagnostics->setObjectName(QString::fromUtf8("light_diagnostics"));
        sizePolicy.setHeightForWidth(light_diagnostics->sizePolicy().hasHeightForWidth());
        light_diagnostics->setSizePolicy(sizePolicy);
        light_diagnostics->setMinimumSize(QSize(225, 35));
        light_diagnostics->setMaximumSize(QSize(225, 35));
        light_diagnostics->setStyleSheet(QString::fromUtf8("background-color: rgb(245, 245, 245);\n"
"border-radius: 4px;"));
        light_diagnostics->setFrameShape(QFrame::Panel);
        light_diagnostics->setFrameShadow(QFrame::Raised);
        light_diagnostics_btn = new QPushButton(light_diagnostics);
        light_diagnostics_btn->setObjectName(QString::fromUtf8("light_diagnostics_btn"));
        light_diagnostics_btn->setGeometry(QRect(5, 4, 216, 25));
        light_diagnostics_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(230, 230, 230);\n"
"border-top-color: rgb(255, 255, 255);\n"
"border-left-color: rgb(255, 255, 255);\n"
"border-bottom-color: rgb(180, 180, 180);\n"
"border-right-color: rgb(180, 180, 180);\n"
"border-style: solid;\n"
"border-width: 2px;"));

        postLayout->addWidget(light_diagnostics);

        _postSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        postLayout->addItem(_postSpacer);

        TabMenu->addTab(Post, QString());

        verticalLayout_3->addWidget(TabMenu);


        retranslateUi(Tweaker);

        TabMenu->setCurrentIndex(0);
        reloadScene_btn->setDefault(false);
        reloadPlacement_btn->setDefault(false);
        reloadEngine_btn->setDefault(false);
        selectedTexture_box->setCurrentIndex(-1);
        displayTexture_btn->setDefault(false);
        reloadTexture_btn->setDefault(false);
        selectedTerrain_box->setCurrentIndex(-1);
        reloadTerrain_btn->setDefault(false);
        pauseEmission_btn->setDefault(false);
        toggleWireframe_btn->setDefault(false);
        lightSelected_box->setCurrentIndex(-1);
        light_render_only_btn->setDefault(false);
        light_diagnostics_btn->setDefault(false);


        QMetaObject::connectSlotsByName(Tweaker);
    } // setupUi

    void retranslateUi(QWidget *Tweaker)
    {
        Tweaker->setWindowTitle(QCoreApplication::translate("Tweaker", "Shader Editor", nullptr));
        switchEngine_lbl->setText(QCoreApplication::translate("Tweaker", "Engine", nullptr));
        fps_lbl->setText(QCoreApplication::translate("Tweaker", "Frames Per Sec", nullptr));
        fps_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        deltaTime_lbl->setText(QCoreApplication::translate("Tweaker", "Delta Time", nullptr));
        deltaTime_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        cameraX_lbl->setText(QCoreApplication::translate("Tweaker", "Camera X", nullptr));
        cameraY_lbl->setText(QCoreApplication::translate("Tweaker", "Camera Y", nullptr));
        cameraZ_lbl->setText(QCoreApplication::translate("Tweaker", "Camera Z", nullptr));
        cameraPitch_lbl->setText(QCoreApplication::translate("Tweaker", "Camera Pitch", nullptr));
        cameraYaw_lbl->setText(QCoreApplication::translate("Tweaker", "Camera Yaw", nullptr));
        cameraRoll_lbl->setText(QCoreApplication::translate("Tweaker", "Camera Roll", nullptr));
        cameraForwardSpeed_lbl->setText(QCoreApplication::translate("Tweaker", "Forward Speed", nullptr));
        cameraRotationSpeed_lbl->setText(QCoreApplication::translate("Tweaker", "Rotation Speed", nullptr));
        reloadScene_btn->setText(QCoreApplication::translate("Tweaker", "Reload Scene", nullptr));
        reloadPlacement_btn->setText(QCoreApplication::translate("Tweaker", "Reload Placement", nullptr));
        reloadEngine_btn->setText(QCoreApplication::translate("Tweaker", "Reload Engine", nullptr));
        TabMenu->setTabText(TabMenu->indexOf(Scene), QCoreApplication::translate("Tweaker", "Scene", nullptr));
        selectedTexture_lbl->setText(QCoreApplication::translate("Tweaker", "Texture", nullptr));
        displayTexture_btn->setText(QString());
        reloadTexture_btn->setText(QCoreApplication::translate("Tweaker", "Reload Texture", nullptr));
        selectedTerrain_lbl->setText(QCoreApplication::translate("Tweaker", "Terrain", nullptr));
        terrainShader_lbl->setText(QCoreApplication::translate("Tweaker", "Shader", nullptr));
        terrainShader_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        instancesTerrain_lbl->setText(QCoreApplication::translate("Tweaker", "Instances", nullptr));
        instancesTerrain_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        reloadTerrain_btn->setText(QCoreApplication::translate("Tweaker", "Reload Terrain", nullptr));
        TabMenu->setTabText(TabMenu->indexOf(Area), QCoreApplication::translate("Tweaker", "PGT", nullptr));
        selectedMesh_lbl->setText(QCoreApplication::translate("Tweaker", "Mesh", nullptr));
        shader_lbl->setText(QCoreApplication::translate("Tweaker", "Shader", nullptr));
        shader_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        instancesMesh_lbl->setText(QCoreApplication::translate("Tweaker", "Instances", nullptr));
        instancesMesh_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        selectedWater_lbl->setText(QCoreApplication::translate("Tweaker", "Water", nullptr));
        instancesWater_lbl->setText(QCoreApplication::translate("Tweaker", "Instances", nullptr));
        instancesWater_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        waveNumber_lbl->setText(QCoreApplication::translate("Tweaker", "Wave Number", nullptr));
        selectedParticles_lbl->setText(QCoreApplication::translate("Tweaker", "Emitter", nullptr));
        instancesEmitter_lbl->setText(QCoreApplication::translate("Tweaker", "Instances", nullptr));
        instancesEmitter_text->setText(QCoreApplication::translate("Tweaker", "TextLabel", nullptr));
        pauseEmission_btn->setText(QCoreApplication::translate("Tweaker", "Pause Emission", nullptr));
        TabMenu->setTabText(TabMenu->indexOf(Mesh), QCoreApplication::translate("Tweaker", "Mesh", nullptr));
        postImage_lbl->setText(QCoreApplication::translate("Tweaker", "Rendering", nullptr));
        toggleWireframe_btn->setText(QCoreApplication::translate("Tweaker", "Toggle Wireframe", nullptr));
        lightSelected_lbl->setText(QCoreApplication::translate("Tweaker", "Light", nullptr));
        light_active_lbl->setText(QCoreApplication::translate("Tweaker", "Activity", nullptr));
        light_render_only_btn->setText(QCoreApplication::translate("Tweaker", "Render Lights Only", nullptr));
        light_diagnostics_btn->setText(QCoreApplication::translate("Tweaker", "Toggle Light Diagnostics", nullptr));
        TabMenu->setTabText(TabMenu->indexOf(Post), QCoreApplication::translate("Tweaker", "Post", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tweaker: public Ui_Tweaker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TWEAKER_H
