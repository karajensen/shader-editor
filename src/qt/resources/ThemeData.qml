////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ThemeData.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.0
  
Item {
    readonly property int tooltipDelay: 1000
    readonly property int fontSize: 15
    readonly property int smallMargin: 1
    readonly property int margin: 3
    readonly property int largeMargin: 10
    readonly property int radius: 4
    readonly property color highlightColor: palette.highlight
    readonly property color midlightColor: palette.midlight
    readonly property color lightColor: Qt.lighter(palette.midlight, 1.08)
    readonly property color lightestColor: palette.light
    readonly property color darkColor: palette.dark

    SystemPalette {
        id: palette
    }
}
