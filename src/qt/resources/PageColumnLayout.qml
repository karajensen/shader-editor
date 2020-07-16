////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - PageColumnLayout.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: control

    default property alias contentItem: columnLayout.data

    ColumnLayout {
        id: columnLayout
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 0
    }
}