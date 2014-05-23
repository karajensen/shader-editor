
add_library(Qt5::AccessibleFactory MODULE IMPORTED)

_populate_Widgets_plugin_properties(AccessibleFactory RELEASE "accessible/qtaccessiblewidgets.dll")
_populate_Widgets_plugin_properties(AccessibleFactory DEBUG "accessible/qtaccessiblewidgetsd.dll")

list(APPEND Qt5Widgets_PLUGINS Qt5::AccessibleFactory)
