#! [0]
HEADERS     = gui.h \
    gui.h
#! [0] #! [1]
FORMS       = gui.ui \
    gui.ui
#! [1]
SOURCES     = gui.cpp \
    gui.cpp \
    main_qt.cpp
QT += widgets

target.path = $$[QT_INSTALL_EXAMPLES]/designer/gui
INSTALLS += target
