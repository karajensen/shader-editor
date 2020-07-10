////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - ReloadableWindow.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

ApplicationWindow {
    flags: Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowStaysOnTopHint | Qt.Window
    visible: true
    title: " " // Empty title uses default for application

    property alias contentSourcePath: contentLoader.source

    Shortcut {
        sequence: "F5"

        onActivated: {
            Reloader.Reload();
            Theme.reload();

            // Force a complete reload of the component
            const previousContentPath = contentSourcePath;
            contentSourcePath = "";
            contentSourcePath = previousContentPath;
        }
    }

    Loader {
        id: contentLoader
        anchors.fill: parent
        active: true
    }
}
