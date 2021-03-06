////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - DoubleSpinBox.qml
////////////////////////////////////////////////////////////////////////////////////////

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.14
import Application.Controls 1.0

RowLayout {
    id: control
    spacing: 0

    property real value: 0.0
    property real stepSize: 1.0
    property real from: 0
    property real to: 100
    property int precision: 0

    function numberToString(number) {
        return number.toLocaleString(spinBox.locale, 'f', control.precision);
    }

    function stringToNumber(str) {
        return Number.fromLocaleString(spinBox.locale, str)
    }

    function increase() {
        PropertySetter.SetValue(control, "value", 
            Math.min(control.to, Math.max(control.from, control.value + control.stepSize)));
    }

    function decrease() {
        PropertySetter.SetValue(control, "value", 
            Math.min(control.to, Math.max(control.from, control.value - control.stepSize)));
    }

    function almostEquals(valueA, valueB) {
        return Math.abs(valueA - valueB) < Number.EPSILON;
    }

    // Qml Spinbox does not support real values and cannot support large integers
    // Because of this we bypass SpinBox completely and only use it for its design
    SpinBox {
        id: spinBox
        palette.base: Theme.midlightColor
        leftPadding: Theme.margin
        rightPadding: Theme.margin
        font.pixelSize: Theme.fontSize
        locale: Qt.locale("C")
        value: forcedValue
        from: 0
        to: 100

        Layout.fillWidth: true
        Layout.preferredHeight: 24
        Layout.alignment: Qt.AlignVCenter

        // Only way to change the spinbox value is to use the up/down buttons
        readonly property int forcedValue: 50
        onValueModified: {
            if (!almostEquals(forcedValue, value)) {
                if (forcedValue - value < 0) {
                    control.increase();
                }
                else {
                    control.decrease();
                }
                spinBox.value = spinBox.forcedValue;
            }
        }

        contentItem: Item {
            implicitWidth: textInput.implicitWidth
            implicitHeight: 18

            TextInput {
                id: textInput
                anchors.fill: parent
                anchors.rightMargin: spinBox.up.indicator.width + spinBox.rightPadding
                font: spinBox.font
                selectByMouse: true
                selectionColor: Theme.highlightColor
                horizontalAlignment: Qt.AlignLeft
                verticalAlignment: Qt.AlignVCenter
                inputMethodHints: Qt.ImhFormattedNumbersOnly

                property bool ignoreValueChange: false

                function updateValueFromText() {
                    if (acceptableInput) {
                        ignoreValueChange = true;
                        PropertySetter.SetValue(control, "value", stringToNumber(text));
                        ignoreValueChange = false;
                    }
                }

                validator: DoubleValidator {
                    locale: spinBox.locale.name
                    notation: DoubleValidator.StandardNotation
                    decimals: control.precision
                    bottom: stringToNumber(numberToString(control.from))
                    top: stringToNumber(numberToString(control.to))
                }

                onTextEdited: {
                    updateValueFromText();
                }

                onEditingFinished: {
                    updateValueFromText();
                }

                onAccepted: {
                    updateValueFromText();
                }

                Keys.onUpPressed: {
                    control.increase();
                }

                Keys.onDownPressed: {
                    control.decrease();
                }

                Component.onCompleted: {
                    text = numberToString(control.value);
                }

                Connections {
                    target: control
                    onValueChanged: {
                        if (!textInput.ignoreValueChange) {
                            textInput.text = numberToString(control.value);
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    cursorShape: Qt.IBeamCursor
                    enabled: parent.activeFocus

                    onWheel: {
                        if (wheel.angleDelta.y > 0) {
                            control.increase();
                        } 
                        else {
                            control.decrease();
                        }
                    }
                }
            }
        }
    }

    Dial {
        id: dial
        from: control.from
        to: control.to
        value: control.value
        Layout.preferredWidth: 30
        Layout.preferredHeight: 30
        Layout.alignment: Qt.AlignVCenter

        onMoved: {
            PropertySetter.SetValue(control, "value", dial.value);
        }
    }
}
