#! [0]
HEADERS     = shaderEditorGUI.h \
    shaderEditorGUI.h
#! [0] #! [1]
FORMS       = shaderEditorGUI.ui \
    shaderEditorGUI.ui
#! [1]
SOURCES     = shaderEditorGUI.cpp \
              main.cpp \
    shaderEditorGUI.cpp
QT += widgets

target.path = $$[QT_INSTALL_EXAMPLES]/designer/shaderEditorGUI
INSTALLS += target
