/**
 * Navigation Button Component
 *
 * Custom navigation button for the side panel with hover and active states.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: navButton

    property string icon: "⚡"
    property string label: "Nav"
    property bool active: false
    property color accentColor: "#00FFFF"

    Layout.preferredWidth: 60
    Layout.preferredHeight: 60

    background: Rectangle {
        color: navButton.active ?
               Qt.rgba(0, 1, 1, 0.2) :
               (navButton.hovered ? Qt.rgba(1, 1, 1, 0.05) : "transparent")
        radius: 10
        border.color: navButton.active ? navButton.accentColor : "transparent"
        border.width: 2

        Behavior on color {
            ColorAnimation { duration: 200 }
        }

        Behavior on border.color {
            ColorAnimation { duration: 200 }
        }
    }

    contentItem: Column {
        spacing: 5

        Text {
            text: navButton.icon
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: navButton.label
            font.pixelSize: 8
            color: navButton.active ? navButton.accentColor : "white"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }
    }

    // Click animation
    scale: pressed ? 0.95 : 1.0

    Behavior on scale {
        NumberAnimation { duration: 100 }
    }
}
