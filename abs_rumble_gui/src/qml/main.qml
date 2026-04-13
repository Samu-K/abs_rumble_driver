import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 400
    height: 300
    visible: true
    title: qsTr("ABS Rumble Controller")

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        ColumnLayout {
            Label {
                text: "ABS brake threshold: " + absSlider.value.toFixed(2)
                font.pixelSize: 16
            }
            Slider {
                id: absSlider
                from: 0
                to: 1
                value: backend.absThreshold
                onMoved: backend.absThreshold = value
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Label {
                text: "Rumble intensity: " + rumbleSlider.value.toFixed(2)
                font.pixelSize: 16
            }
            Slider {
                id: rumbleSlider
                from: 0
                to: 1
                value: backend.rumbleIntensity
                onMoved: backend.rumbleIntensity = value
                Layout.fillWidth: true
            }
        }
    }
}
