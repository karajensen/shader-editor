#! [0]
HEADERS     = gui.h
#! [0] #! [1]
FORMS       = gui.ui
#! [1]
SOURCES     = gui.cpp \
              main.cpp
QT += widgets

target.path = $$[QT_INSTALL_EXAMPLES]/designer/gui
INSTALLS += target
