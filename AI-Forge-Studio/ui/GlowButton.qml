/**
 * Glow Button Component
 *
 * Animated button with glow effect and icon.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: glowButton

    property string icon: "⚡"
    property color glowColor: "#00FFFF"

    height: 50

    background: Rectangle {
        color: glowButton.pressed ?
               Qt.rgba(glowButton.glowColor.r, glowButton.glowColor.g, glowButton.glowColor.b, 0.3) :
               (glowButton.hovered ?
                   Qt.rgba(glowButton.glowColor.r, glowButton.glowColor.g, glowButton.glowColor.b, 0.2) :
                   Qt.rgba(glowButton.glowColor.r, glowButton.glowColor.g, glowButton.glowColor.b, 0.1))
        radius: 10
        border.color: glowButton.glowColor
        border.width: 2

        Behavior on color {
            ColorAnimation { duration: 200 }
        }

        // Glow effect
        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "transparent"
            border.color: glowButton.glowColor
            border.width: 4
            opacity: glowButton.hovered ? 0.5 : 0
            scale: glowButton.hovered ? 1.1 : 1.0

            Behavior on opacity {
                NumberAnimation { duration: 300 }
            }

            Behavior on scale {
                NumberAnimation { duration: 300 }
            }
        }
    }

    contentItem: Row {
        spacing: 10
        anchors.centerIn: parent

        Text {
            text: glowButton.icon
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: glowButton.text
            font.pixelSize: 14
            font.bold: true
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    // Press animation
    scale: pressed ? 0.95 : 1.0

    Behavior on scale {
        NumberAnimation { duration: 100 }
    }
}
