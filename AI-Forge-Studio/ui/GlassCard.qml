/**
 * Glass Card Component
 *
 * Glassmorphism card with blur effect and border glow.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: glassCard

    property string title: "Card Title"
    property string icon: "⚡"
    property alias content: contentItem.children

    color: Qt.rgba(0.1, 0.1, 0.15, 0.85)
    radius: 15
    border.color: Qt.rgba(1, 1, 1, 0.15)
    border.width: 2

    // Glass effect shimmer
    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(1, 1, 1, 0.05) }
            GradientStop { position: 0.5; color: "transparent" }
            GradientStop { position: 1.0; color: Qt.rgba(1, 1, 1, 0.02) }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Header
        RowLayout {
            spacing: 10

            Text {
                text: glassCard.icon
                font.pixelSize: 20
            }

            Text {
                text: glassCard.title
                font.pixelSize: 16
                font.bold: true
                color: "white"
                Layout.fillWidth: true
            }
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Qt.rgba(1, 1, 1, 0.2)

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.5; color: Qt.rgba(0, 1, 1, 0.5) }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        // Content area
        Item {
            id: contentItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    // Hover effect
    scale: hovered ? 1.02 : 1.0

    property bool hovered: false

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onEntered: parent.hovered = true
        onExited: parent.hovered = false
    }

    Behavior on scale {
        NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
    }

    Behavior on border.color {
        ColorAnimation { duration: 300 }
    }
}
