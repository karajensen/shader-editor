cmake_minimum_required(VERSION 3.17)
project(ShaderEditor)

add_definitions(-DGLEW_STATIC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zi /Od /Ob0")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /DEBUG /OPT:REF /OPT:ICF")

set(SRC_LIST
    ../readme.txt
    animation.cpp
    animation.h
    app_gui.cpp
    app_gui.h
    application.cpp
    application.h
    cache.h
    camera.cpp
    camera.h
    colour.h
    diagnostic.cpp
    diagnostic.h
    directx_common.h
    directx_emitter.cpp
    directx_emitter.h
    directx_engine.cpp
    directx_engine.h
    directx_mesh.cpp
    directx_mesh.h
    directx_shader.cpp
    directx_shader.h
    directx_target.cpp
    directx_target.h
    directx_texture.cpp
    directx_texture.h
    emitter.cpp
    emitter.h
    float3.h
    fragmentlinker.cpp
    fragmentlinker.h
    grid.cpp
    grid.h
    int2.h
    light.cpp
    light.h
    logger.cpp
    logger.h
    main.cpp
    matrix.h
    mesh.cpp
    mesh.h
    mesh_attributes.cpp
    mesh_attributes.h
    mesh_data.cpp
    mesh_data.h
    mesh_group.cpp
    mesh_group.h
    opengl_common.h
    opengl_emitter.cpp
    opengl_emitter.h
    opengl_engine.cpp
    opengl_engine.h
    opengl_mesh.cpp
    opengl_mesh.h
    opengl_shader.cpp
    opengl_shader.h
    opengl_target.cpp
    opengl_target.h
    opengl_texture.cpp
    opengl_texture.h
    particle.cpp
    particle.h
    postprocessing.cpp
    postprocessing.h
    random_generator.cpp
    random_generator.h
    render_data.h
    render_engine.h
    scene.cpp
    scene.h
    scene_builder.cpp
    scene_builder.h
    scene_data.h
    scene_interface.h
    scene_placer.cpp
    scene_placer.h
    shader.cpp
    shader.h
    terrain.cpp
    terrain.h
    texture.cpp
    texture.h
    texture_procedural.cpp
    texture_procedural.h
    timer.cpp
    timer.h
    tweakable_enums.h
    tweakable_enums.cpp
    water.cpp
    water.h
    utils.h
)

set(QT_LIST
    qt/attribute.cpp
    qt/attribute.h
    qt/attribute_model.cpp
    qt/attribute_model.h
    qt/attribute_filter_model.cpp
    qt/attribute_filter_model.h
    qt/qt_gui.cpp
    qt/qt_gui.h
    qt/qt_reloader.h
    qt/qt_reloader.cpp
    qt/tweaker_model.h
    qt/tweaker_model.cpp
    qt/editor_model.h
    qt/editor_model.cpp
    qt/stringlist_model.h
    qt/stringlist_model.cpp
    qt/property_setter.h
    qt/property_setter.cpp
)

set(QML_LIST
    qt/resources/DoubleSpinBox.qml
    qt/resources/Tweaker.qml
    qt/resources/TweakerButton.qml
    qt/resources/TweakerComboBox.qml
    qt/resources/TweakerComboSpinBox.qml
    qt/resources/TweakerControl.qml
    qt/resources/TweakerImagePanel.qml
    qt/resources/TweakerLabel.qml
    qt/resources/TweakerListView.qml
    qt/resources/TweakerSpinBox.qml
    qt/resources/TweakerWindow.qml
    qt/resources/Theme.qml
    qt/resources/ThemeData.qml
    qt/resources/Editor.qml
    qt/resources/EditorWindow.qml
    qt/resources/EditorTextArea.qml
    qt/resources/ReloadableWindow.qml
    qt/resources/StackColumnLayout.qml
)

set(GLEW_LIST
    glew/include/GL/glew.h
    glew/include/GL/wglew.h
    glew/src/glew.c
)

set(DXSDK_DIR $ENV{DXSDK_DIR})
set(BOOST_DIR $ENV{BOOST_DIR})
set(BOOST_LIB $ENV{BOOST_LIB})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
include_directories(${BOOST_DIR})
include_directories(${DXSDK_DIR}/Include)
link_directories(${BOOST_DIR}/${BOOST_LIB})

find_package(Qt5 COMPONENTS Core Widgets Quick QuickControls2)
qt5_add_resources(RESOURCES qt/resources/qml.qrc)

add_executable(ShaderEditor ${SRC_LIST} ${QT_LIST} ${GLEW_LIST} ${QML_LIST} ${RESOURCES})
qt5_use_modules(ShaderEditor Core Widgets Quick QuickControls2)

# QML Reloading
set(QML_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/qt/resources/")
target_compile_definitions(ShaderEditor PRIVATE 
    $<$<CONFIG:DEBUG>:QML_FILE_PATH="${QML_FILE_PATH}">
    $<$<CONFIG:RELWITHDEBINFO>:QML_FILE_PATH="${QML_FILE_PATH}">
)

# Console Window
target_compile_definitions(ShaderEditor PRIVATE 
    $<$<CONFIG:DEBUG>:USE_CONSOLE>
    $<$<CONFIG:RELWITHDEBINFO>:USE_CONSOLE>
)

# https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-warning-lnk4286
set_target_properties(ShaderEditor PROPERTIES LINK_FLAGS "/ignore:4286")

source_group("Qt" FILES ${QT_LIST})
source_group("QML" FILES ${QML_LIST})
source_group("Glew" FILES ${GLEW_LIST})

target_link_libraries(ShaderEditor OpenGL32.lib)
target_link_libraries(ShaderEditor ${CMAKE_CURRENT_SOURCE_DIR}/assimp/assimp.lib)
target_link_libraries(ShaderEditor ${DXSDK_DIR}/Lib/x86/d3d11.lib)
target_link_libraries(ShaderEditor ${DXSDK_DIR}/Lib/x86/d3dx11.lib)
target_link_libraries(ShaderEditor ${DXSDK_DIR}/Lib/x86/d3dx10.lib)
target_link_libraries(ShaderEditor ${DXSDK_DIR}/Lib/x86/d3dcompiler.lib)
target_link_libraries(ShaderEditor ${DXSDK_DIR}/Lib/x86/dxguid.lib)
target_link_libraries(ShaderEditor debug ${CMAKE_CURRENT_SOURCE_DIR}/soil/debug/soil.lib)
target_link_libraries(ShaderEditor optimized ${CMAKE_CURRENT_SOURCE_DIR}/soil/release/soil.lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})