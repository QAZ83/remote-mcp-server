/**
 * Metric Row Component
 *
 * Displays a labeled metric with progress bar.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: metricRow

    property string label: "Metric"
    property string value: "0"
    property real maxValue: 100
    property real currentValue: 0
    property color color: "#00FFFF"

    spacing: 5

    // Label and value
    RowLayout {
        Layout.fillWidth: true

        Text {
            text: metricRow.label + ":"
            font.pixelSize: 12
            color: "white"
        }

        Item { Layout.fillWidth: true }

        Text {
            text: metricRow.value
            font.pixelSize: 14
            font.bold: true
            color: metricRow.color
        }
    }

    // Progress bar
    Rectangle {
        Layout.fillWidth: true
        height: 8
        color: Qt.rgba(0, 0, 0, 0.3)
        radius: 4
        border.color: Qt.rgba(1, 1, 1, 0.2)
        border.width: 1

        Rectangle {
            width: Math.min(parent.width * (metricRow.currentValue / metricRow.maxValue), parent.width)
            height: parent.height
            color: metricRow.color
            radius: 4

            Behavior on width {
                NumberAnimation { duration: 500; easing.type: Easing.OutQuad }
            }

            // Shimmer effect
            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: Qt.rgba(1, 1, 1, 0.3) }
                    GradientStop { position: 1.0; color: "transparent" }
                }

                // Animated shimmer
                x: -width
                SequentialAnimation on x {
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: -metricRow.width
                        to: metricRow.width
                        duration: 2000
                    }
                }
            }
        }
    }
}
