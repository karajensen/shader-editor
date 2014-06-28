#! [0]
HEADERS     = gui.h \
    tweaker.h
#! [0] #! [1]
FORMS       = gui.ui \
    tweaker.ui \
    tweak_button.ui \
    tweak_label.ui \
    tweak_value.ui
#! [1]
SOURCES     = gui.cpp \
              main.cpp \
    tweaker.cpp
QT += widgets

target.path = $$[QT_INSTALL_EXAMPLES]/designer/gui
INSTALLS += target
