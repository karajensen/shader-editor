////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - TweakerListView.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Controls 2.14
import Qt.labs.qmlmodels 1.0

ListView {
    id: control

    delegate: DelegateChooser {
        id: delegateChooser
        role: "enabled"

        DelegateChoice {
            roleValue: "true"
            TweakerSpinBox {
                headerText: model.name
                value: model.value
                stepSize: model.stepSize
                precision: model.precision
                width: control.width
                from: -10000.0
                to: 10000.0

                onValueChanged: {
                    model.value = value
                }
            }
        }

        DelegateChoice {
            roleValue: "false"
            TweakerLabel {
                headerText: model.name
                labelText: model.value.toLocaleString(Qt.locale(), 'f', model.precision)
                width: control.width
            }
        }
    }
}
