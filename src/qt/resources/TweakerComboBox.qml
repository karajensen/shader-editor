////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerComboBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Application.Controls 1.0

TweakerControl {
    id: control

    property alias model: comboBox.model
    property alias headerText: header.text

    headerItem: Text {
        id: header
        anchors.fill: parent
        font.pixelSize: Theme.fontSize
        font.weight: Font.DemiBold
        verticalAlignment: Text.AlignVCenter
    }

    contentItem: ComboBox {
        id: comboBox
        anchors.fill: parent
        anchors.topMargin: Theme.margin
        anchors.bottomMargin: Theme.margin
        font.pixelSize: Theme.fontSize
        textRole: "display"
        currentIndex: model ? model.selectedIndex : -1
                                    
        onActivated: {
            model.selectedIndex = currentIndex;
        }
    }
}
