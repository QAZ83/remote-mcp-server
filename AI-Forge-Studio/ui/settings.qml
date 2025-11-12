/**
 * Settings - AI Forge Studio
 *
 * Application settings and configuration panel.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: settings
    color: "transparent"

    property color accentCyan: "#00FFFF"
    property color accentPurple: "#AA00FF"

    ScrollView {
        anchors.fill: parent
        anchors.margins: 20
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: 20

            Text {
                text: "Settings"
                font.pixelSize: 24
                font.bold: true
                color: settings.accentCyan
            }

            // Performance Settings
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                title: "Performance"
                icon: "⚡"

                content: ColumnLayout {
                    anchors.fill: parent
                    spacing: 20

                    // Power Mode
                    Column {
                        width: parent.width
                        spacing: 10

                        Text {
                            text: "Power Mode:"
                            font.pixelSize: 14
                            color: "white"
                        }

                        RowLayout {
                            spacing: 10
                            width: parent.width

                            RadioButton {
                                text: "Eco"
                                font.pixelSize: 12
                            }

                            RadioButton {
                                text: "Balanced"
                                font.pixelSize: 12
                                checked: true
                            }

                            RadioButton {
                                text: "Performance"
                                font.pixelSize: 12
                            }
                        }
                    }

                    // VRAM Management
                    Column {
                        width: parent.width
                        spacing: 10

                        CheckBox {
                            text: "Enable VRAM ↔ RAM Off-loading"
                            font.pixelSize: 12
                            checked: false
                        }

                        CheckBox {
                            text: "Multi-GPU Link (SLI/NVLink)"
                            font.pixelSize: 12
                            checked: false
                        }

                        CheckBox {
                            text: "Enable TensorRT Optimization"
                            font.pixelSize: 12
                            checked: true
                        }

                        CheckBox {
                            text: "Use FP16 Precision (Faster)"
                            font.pixelSize: 12
                            checked: true
                        }
                    }
                }
            }

            // Graphics Settings
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 250
                title: "Graphics"
                icon: "🎮"

                content: ColumnLayout {
                    anchors.fill: parent
                    spacing: 20

                    CheckBox {
                        text: "Enable Ray Tracing"
                        font.pixelSize: 12
                        checked: true
                    }

                    CheckBox {
                        text: "Enable DLSS 3.5"
                        font.pixelSize: 12
                        checked: true
                    }

                    CheckBox {
                        text: "Enable HDR"
                        font.pixelSize: 12
                        checked: false
                    }

                    CheckBox {
                        text: "V-Sync"
                        font.pixelSize: 12
                        checked: true
                    }

                    // Quality preset
                    Column {
                        width: parent.width
                        spacing: 10

                        Text {
                            text: "Quality Preset:"
                            font.pixelSize: 14
                            color: "white"
                        }

                        ComboBox {
                            width: parent.width
                            model: ["Low", "Medium", "High", "Ultra"]
                            currentIndex: 2

                            background: Rectangle {
                                color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                                radius: 5
                                border.color: Qt.rgba(1, 1, 1, 0.3)
                                border.width: 1
                            }
                        }
                    }
                }
            }

            // UI Settings
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                title: "Interface"
                icon: "🎨"

                content: ColumnLayout {
                    anchors.fill: parent
                    spacing: 20

                    // Theme
                    Column {
                        width: parent.width
                        spacing: 10

                        Text {
                            text: "Theme:"
                            font.pixelSize: 14
                            color: "white"
                        }

                        RowLayout {
                            spacing: 10

                            RadioButton {
                                text: "Dark"
                                font.pixelSize: 12
                                checked: true
                            }

                            RadioButton {
                                text: "Light"
                                font.pixelSize: 12
                            }

                            RadioButton {
                                text: "Neon"
                                font.pixelSize: 12
                            }
                        }
                    }

                    CheckBox {
                        text: "Show FPS Counter"
                        font.pixelSize: 12
                        checked: true
                    }

                    CheckBox {
                        text: "Enable Notifications"
                        font.pixelSize: 12
                        checked: true
                    }
                }
            }

            // About
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 250
                title: "About"
                icon: "ℹ️"

                content: ColumnLayout {
                    anchors.fill: parent
                    spacing: 15

                    Text {
                        text: "AI Forge Studio"
                        font.pixelSize: 18
                        font.bold: true
                        color: settings.accentCyan
                    }

                    Text {
                        text: "RTX 50-Series Edition"
                        font.pixelSize: 14
                        color: settings.accentPurple
                    }

                    Text {
                        text: "Version: 1.0.0"
                        font.pixelSize: 12
                        color: "white"
                    }

                    Text {
                        text: "Build: " + Qt.application.version
                        font.pixelSize: 11
                        color: Qt.rgba(1, 1, 1, 0.6)
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: Qt.rgba(1, 1, 1, 0.2)
                    }

                    Text {
                        text: "Powered by CUDA, TensorRT, and Qt 6"
                        font.pixelSize: 10
                        color: Qt.rgba(1, 1, 1, 0.6)
                    }

                    Item { Layout.fillHeight: true }

                    GlowButton {
                        text: "Check for Updates"
                        icon: "🔄"
                        Layout.fillWidth: true
                        glowColor: settings.accentCyan
                        onClicked: {
                            console.log("Check for updates")
                        }
                    }
                }
            }

            Item { height: 20 }  // Bottom spacer
        }
    }
}
