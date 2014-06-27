#! [0]
HEADERS     = gui.h \
    tweaker.h
#! [0] #! [1]
FORMS       = gui.ui \
    tweaker.ui
#! [1]
SOURCES     = gui.cpp \
              main.cpp \
    tweaker.cpp
QT += widgets

target.path = $$[QT_INSTALL_EXAMPLES]/designer/gui
INSTALLS += target
