
add_library(Qt5:: MODULE IMPORTED)

_populate_Sensors_plugin_properties( RELEASE "sensorgestures/qtsensorgestures_plugin.dll")
_populate_Sensors_plugin_properties( DEBUG "sensorgestures/qtsensorgestures_plugind.dll")

list(APPEND Qt5Sensors_PLUGINS Qt5::)
