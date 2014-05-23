
add_library(Qt5::QSvgPlugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QSvgPlugin RELEASE "imageformats/qsvg.dll")
_populate_Gui_plugin_properties(QSvgPlugin DEBUG "imageformats/qsvgd.dll")

list(APPEND Qt5Gui_PLUGINS Qt5::QSvgPlugin)
