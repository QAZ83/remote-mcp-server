/**
 * Main Window - AI Forge Studio
 *
 * Main application window with navigation and content areas.
 * Features glass-morphism design with Neon Cyan/Purple accents.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: mainWindow
    width: 1920
    height: 1080
    visible: true
    title: "AI Forge Studio - RTX 50-Series Edition"
    color: "#0A0A0F"  // Dark background

    // Window flags for custom frame
    flags: Qt.Window | Qt.FramelessWindowHint

    // Properties
    property int currentPage: 0  // 0: Dashboard, 1: Models, 2: Render, 3: Settings
    property bool isDarkMode: true
    property color accentCyan: "#00FFFF"
    property color accentPurple: "#AA00FF"
    property color glassBackground: "#1A1A2E"
    property real glassOpacity: 0.85

    // Background gradient
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0A0A0F" }
            GradientStop { position: 1.0; color: "#1A1A2E" }
        }

        // Animated background particles (optional effect)
        Canvas {
            id: particlesCanvas
            anchors.fill: parent
            opacity: 0.2

            property var particles: []

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                // Draw particles
                for (var i = 0; i < particles.length; i++) {
                    var p = particles[i]
                    ctx.fillStyle = p.color
                    ctx.beginPath()
                    ctx.arc(p.x, p.y, p.size, 0, Math.PI * 2)
                    ctx.fill()
                }
            }
        }
    }

    // Custom title bar
    Rectangle {
        id: titleBar
        width: parent.width
        height: 50
        color: Qt.rgba(0.05, 0.05, 0.1, 0.8)

        // Glass effect
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: Qt.rgba(1, 1, 1, 0.1)
            border.width: 1
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            // Logo and title
            Image {
                id: logo
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                source: "assets/logo.png"
                fillMode: Image.PreserveAspectFit
            }

            Text {
                text: "AI FORGE STUDIO"
                font.pixelSize: 18
                font.bold: true
                font.family: "Segoe UI"
                color: mainWindow.accentCyan

                // Glow effect
                style: Text.Outline
                styleColor: mainWindow.accentPurple
            }

            Item { Layout.fillWidth: true }  // Spacer

            // FPS counter
            Rectangle {
                Layout.preferredWidth: 100
                Layout.preferredHeight: 30
                color: Qt.rgba(0, 1, 1, 0.1)
                radius: 5
                border.color: mainWindow.accentCyan
                border.width: 1

                Text {
                    anchors.centerIn: parent
                    text: "FPS: 60"
                    font.pixelSize: 12
                    font.family: "Consolas"
                    color: mainWindow.accentCyan
                }
            }

            // Window controls
            Row {
                spacing: 5

                Button {
                    text: "−"
                    width: 40
                    height: 30
                    onClicked: mainWindow.showMinimized()

                    background: Rectangle {
                        color: parent.hovered ? Qt.rgba(1, 1, 1, 0.1) : "transparent"
                        radius: 3
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 18
                    }
                }

                Button {
                    text: "□"
                    width: 40
                    height: 30
                    onClicked: mainWindow.visibility === Window.Maximized ?
                              mainWindow.showNormal() : mainWindow.showMaximized()

                    background: Rectangle {
                        color: parent.hovered ? Qt.rgba(1, 1, 1, 0.1) : "transparent"
                        radius: 3
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14
                    }
                }

                Button {
                    text: "×"
                    width: 40
                    height: 30
                    onClicked: Qt.quit()

                    background: Rectangle {
                        color: parent.hovered ? "#FF0000" : "transparent"
                        radius: 3
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 20
                    }
                }
            }
        }
    }

    // Main content area
    RowLayout {
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 0

        // Side navigation
        Rectangle {
            id: sideNav
            Layout.preferredWidth: 80
            Layout.fillHeight: true
            color: Qt.rgba(0.05, 0.05, 0.1, 0.9)

            border.color: Qt.rgba(1, 1, 1, 0.1)
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                // Navigation buttons
                NavButton {
                    id: navDashboard
                    icon: "📊"
                    label: "Dashboard"
                    active: currentPage === 0
                    onClicked: currentPage = 0
                }

                NavButton {
                    id: navModels
                    icon: "🤖"
                    label: "Models"
                    active: currentPage === 1
                    onClicked: currentPage = 1
                }

                NavButton {
                    id: navRender
                    icon: "🎨"
                    label: "Render"
                    active: currentPage === 2
                    onClicked: currentPage = 2
                }

                Item { Layout.fillHeight: true }  // Spacer

                NavButton {
                    id: navSettings
                    icon: "⚙️"
                    label: "Settings"
                    active: currentPage === 3
                    onClicked: currentPage = 3
                }
            }
        }

        // Content area with page loader
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            Loader {
                id: pageLoader
                anchors.fill: parent
                source: {
                    switch(currentPage) {
                        case 0: return "dashboard.qml"
                        case 1: return "model_manager.qml"
                        case 2: return "render_view.qml"
                        case 3: return "settings.qml"
                        default: return "dashboard.qml"
                    }
                }

                // Smooth page transitions
                Behavior on opacity {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }
    }

    // Notification popup area
    Rectangle {
        id: notificationArea
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        anchors.margins: 20
        width: 300
        height: 100
        color: Qt.rgba(0.1, 0.1, 0.2, 0.95)
        radius: 10
        border.color: mainWindow.accentCyan
        border.width: 2
        visible: false
        z: 1000

        // Notification content
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 10

            Text {
                id: notificationTitle
                text: "Notification"
                font.pixelSize: 14
                font.bold: true
                color: mainWindow.accentCyan
            }

            Text {
                id: notificationMessage
                text: "Message"
                font.pixelSize: 12
                color: "white"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }
    }

    // Global functions
    function showNotification(title, message, duration) {
        notificationTitle.text = title
        notificationMessage.text = message
        notificationArea.visible = true

        notificationTimer.interval = duration || 3000
        notificationTimer.restart()
    }

    Timer {
        id: notificationTimer
        repeat: false
        onTriggered: notificationArea.visible = false
    }
}
