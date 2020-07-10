////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerWindow.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9

ReloadableWindow {
    width: 250
    height: 620
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    contentSourcePath: Reloader.qmlSourcePath + "Tweaker.qml"
}
