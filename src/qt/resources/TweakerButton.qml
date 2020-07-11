////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerButton.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control
    headerVisible: false

    property alias buttonText: button.text

    signal clicked()

    contentItem: Button {
        id: button
        anchors.fill: parent
        anchors.margins: Theme.margin
        font.pixelSize: Theme.fontSize

        onClicked: {
            control.clicked()
        }
    }
}
