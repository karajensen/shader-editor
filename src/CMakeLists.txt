cmake_minimum_required(VERSION 3.17)
project(ShaderEditor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SRC_LIST
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
    common.h
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
    qt_gui.cpp
    qt_gui.h
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
    water.cpp
    water.h
    qt/tweakable.h
    qt/tweaker.h
    qt/tweaker.cpp
    qt/editor.h
    qt/editor.cpp
	glew/include/GL/glew.h
	glew/include/GL/wglew.h
	glew/src/glew.c	
    qt/resources/tweaker.qml
    qt/resources/editor.qml 
)

set(DXSDK_DIR $ENV{DXSDK_DIR})
set(BOOST_DIR $ENV{BOOST_DIR})
set(BOOST_LIB $ENV{BOOST_LIB})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assimp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
include_directories(${BOOST_DIR})
include_directories(${DXSDK_DIR}/Include)
link_directories(${BOOST_DIR}/${BOOST_LIB})

find_package(Qt5 COMPONENTS Core Widgets Quick)
qt5_add_resources(RESOURCES qt/resources/images.qrc)
qt5_add_resources(RESOURCES qt/resources/qml.qrc)

add_executable(ShaderEditor ${SRC_LIST} ${RESOURCES})
qt5_use_modules(ShaderEditor Core Widgets Quick)

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