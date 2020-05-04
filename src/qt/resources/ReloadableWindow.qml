////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ReloadableWindow.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14

ApplicationWindow {
    flags: Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowStaysOnTopHint | Qt.Window
    visible: true
    title: " " // Empty title uses default 'Shader Editor'

    property alias contentSourcePath: contentLoader.source

    Shortcut {
        sequence: "F5"
        onActivated: {
            Reloader.reload();

            // Force a complete reload of the component
            const previousContentPath = contentSourcePath;
            contentSourcePath = "";
            contentSourcePath = previousContentPath;
        }
    }

    Loader {
        id: contentLoader
        anchors.fill: parent
        anchors.margins: 2
        active: true
    }
}
