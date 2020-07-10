////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Theme.qml
////////////////////////////////////////////////////////////////////////////////////////

pragma Singleton  
import QtQuick 2.0  
  
Item {  
    readonly property int fontSize: theme.item ? theme.item.fontSize : 0
    readonly property int smallMargin: theme.item ? theme.item.smallMargin : 0
    readonly property int margin: theme.item ? theme.item.margin : 0
    readonly property int largeMargin: theme.item ? theme.item.largeMargin : 0
    readonly property int radius: theme.item ? theme.item.radius : 0
    readonly property color highlightColor: theme.item ? theme.item.highlightColor : noColor
    readonly property color midlightColor: theme.item ? theme.item.midlightColor : noColor
    readonly property color lightColor: theme.item ? theme.item.lightColor : noColor
    readonly property color lightestColor: theme.item ? theme.item.lightestColor : noColor
    readonly property color darkColor: theme.item ? theme.item.darkColor : noColor
    readonly property color noColor: Qt.rgba(0, 0, 0)

    function reload() {
        // Force a complete reload of the component
        const previousSource = theme.source;
        theme.source = "";
        theme.source = previousSource;
    }

    Loader {
        id: theme
        active: true
        source: Reloader.qmlSourcePath + "ThemeData.qml"
    }
}
