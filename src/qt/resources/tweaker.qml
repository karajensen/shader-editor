////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Tweaker.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14

Item {
    id: root

    ColumnLayout {
        spacing: 0
        anchors.margins: 3
        anchors.fill: parent

        TabBar {
            id: tabBar

            Layout.fillWidth: true

            TabButton {
                text: qsTr("Scene")
            }
            TabButton {
                text: qsTr("PGT")
            }
            TabButton {
                text: qsTr("Mesh")
            }
            TabButton {
                text: qsTr("Post")
            }
        }

        StackLayout {
            currentIndex: tabBar.currentIndex

            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                anchors.fill: parent
                color: "red"
            }
            Rectangle {
                anchors.fill: parent
                color: "green"
            }
            Rectangle {
                anchors.fill: parent
                color: "blue"
            }
            Rectangle {
                anchors.fill: parent
                color: "pink"
            }
        }
    }
}
