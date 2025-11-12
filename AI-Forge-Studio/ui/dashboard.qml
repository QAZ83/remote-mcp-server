/**
 * Dashboard - AI Forge Studio
 *
 * Real-time hardware monitoring dashboard with GPU, CPU, RAM, and
 * thermal metrics display. Features real-time graphs and statistics.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Rectangle {
    id: dashboard
    color: "transparent"

    property color accentCyan: "#00FFFF"
    property color accentPurple: "#AA00FF"

    // Padding
    anchors.margins: 20

    // Main layout
    GridLayout {
        anchors.fill: parent
        anchors.margins: 20
        columns: 3
        rowSpacing: 20
        columnSpacing: 20

        // GPU Status Card
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 200
            title: "GPU Status"
            icon: "🎮"

            content: ColumnLayout {
                spacing: 15
                anchors.fill: parent

                // GPU Name
                Text {
                    text: "NVIDIA GeForce RTX 5090"
                    font.pixelSize: 16
                    font.bold: true
                    color: dashboard.accentCyan
                }

                // GPU Utilization
                MetricRow {
                    label: "Utilization"
                    value: "67%"
                    maxValue: 100
                    currentValue: 67
                    color: dashboard.accentCyan
                }

                // Temperature
                MetricRow {
                    label: "Temperature"
                    value: "65°C"
                    maxValue: 100
                    currentValue: 65
                    color: currentValue > 80 ? "#FF4444" : dashboard.accentCyan
                }

                // Power
                MetricRow {
                    label: "Power"
                    value: "420W"
                    maxValue: 600
                    currentValue: 420
                    color: dashboard.accentPurple
                }
            }
        }

        // VRAM Card
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 200
            title: "VRAM"
            icon: "💾"

            content: ColumnLayout {
                spacing: 15
                anchors.fill: parent

                // Total VRAM
                Text {
                    text: "24 GB GDDR7"
                    font.pixelSize: 16
                    font.bold: true
                    color: dashboard.accentCyan
                }

                // VRAM Usage
                RowLayout {
                    spacing: 10
                    Layout.fillWidth: true

                    Text {
                        text: "Used:"
                        font.pixelSize: 12
                        color: "white"
                    }

                    Text {
                        text: "16.2 GB"
                        font.pixelSize: 16
                        font.bold: true
                        color: dashboard.accentPurple
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: "Free: 7.8 GB"
                        font.pixelSize: 12
                        color: Qt.rgba(1, 1, 1, 0.6)
                    }
                }

                // Progress bar
                Rectangle {
                    Layout.fillWidth: true
                    height: 20
                    color: Qt.rgba(0, 0, 0, 0.3)
                    radius: 10
                    border.color: Qt.rgba(1, 1, 1, 0.2)
                    border.width: 1

                    Rectangle {
                        width: parent.width * 0.675  // 16.2/24
                        height: parent.height
                        color: dashboard.accentPurple
                        radius: 10

                        gradient: Gradient {
                            GradientStop { position: 0.0; color: dashboard.accentCyan }
                            GradientStop { position: 1.0; color: dashboard.accentPurple }
                        }
                    }
                }

                // Clock speeds
                RowLayout {
                    Layout.fillWidth: true

                    Column {
                        Text {
                            text: "Core Clock"
                            font.pixelSize: 10
                            color: Qt.rgba(1, 1, 1, 0.6)
                        }
                        Text {
                            text: "2650 MHz"
                            font.pixelSize: 12
                            font.bold: true
                            color: dashboard.accentCyan
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Column {
                        Text {
                            text: "Memory Clock"
                            font.pixelSize: 10
                            color: Qt.rgba(1, 1, 1, 0.6)
                        }
                        Text {
                            text: "11000 MHz"
                            font.pixelSize: 12
                            font.bold: true
                            color: dashboard.accentCyan
                        }
                    }
                }
            }
        }

        // System Card
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 200
            title: "System"
            icon: "💻"

            content: ColumnLayout {
                spacing: 15
                anchors.fill: parent

                // CPU
                MetricRow {
                    label: "CPU"
                    value: "45%"
                    maxValue: 100
                    currentValue: 45
                    color: dashboard.accentCyan
                }

                // RAM
                MetricRow {
                    label: "RAM"
                    value: "24.5 / 64 GB"
                    maxValue: 64
                    currentValue: 24.5
                    color: dashboard.accentPurple
                }

                // FPS
                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "Current FPS:"
                        font.pixelSize: 12
                        color: "white"
                    }

                    Text {
                        text: "60"
                        font.pixelSize: 24
                        font.bold: true
                        color: dashboard.accentCyan
                    }
                }
            }
        }

        // Performance Graph
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
            Layout.preferredHeight: 300
            title: "Performance History"
            icon: "📈"

            content: ChartView {
                anchors.fill: parent
                antialiasing: true
                backgroundColor: "transparent"
                legend.visible: true
                legend.alignment: Qt.AlignBottom
                legend.labelColor: "white"

                theme: ChartView.ChartThemeDark

                LineSeries {
                    name: "GPU Usage %"
                    color: dashboard.accentCyan
                    width: 2

                    XYPoint { x: 0; y: 45 }
                    XYPoint { x: 1; y: 52 }
                    XYPoint { x: 2; y: 67 }
                    XYPoint { x: 3; y: 71 }
                    XYPoint { x: 4; y: 65 }
                    XYPoint { x: 5; y: 68 }
                    XYPoint { x: 6; y: 72 }
                    XYPoint { x: 7; y: 70 }
                    XYPoint { x: 8; y: 67 }
                    XYPoint { x: 9; y: 69 }
                }

                LineSeries {
                    name: "Temperature °C"
                    color: dashboard.accentPurple
                    width: 2

                    XYPoint { x: 0; y: 55 }
                    XYPoint { x: 1; y: 58 }
                    XYPoint { x: 2; y: 62 }
                    XYPoint { x: 3; y: 65 }
                    XYPoint { x: 4; y: 63 }
                    XYPoint { x: 5; y: 64 }
                    XYPoint { x: 6; y: 66 }
                    XYPoint { x: 7; y: 65 }
                    XYPoint { x: 8; y: 65 }
                    XYPoint { x: 9; y: 65 }
                }

                ValueAxis {
                    id: axisX
                    min: 0
                    max: 10
                    labelFormat: "%d"
                    titleText: "Time (s)"
                    color: "white"
                    labelsColor: "white"
                }

                ValueAxis {
                    id: axisY
                    min: 0
                    max: 100
                    labelFormat: "%d"
                    titleText: "Value"
                    color: "white"
                    labelsColor: "white"
                }
            }
        }

        // Action Buttons
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 300
            title: "Quick Actions"
            icon: "⚡"

            content: ColumnLayout {
                spacing: 15
                anchors.fill: parent

                GlowButton {
                    text: "Generate Image"
                    icon: "🎨"
                    Layout.fillWidth: true
                    glowColor: dashboard.accentCyan
                    onClicked: {
                        // Navigate to render view
                        console.log("Generate Image clicked")
                    }
                }

                GlowButton {
                    text: "Load Model"
                    icon: "📥"
                    Layout.fillWidth: true
                    glowColor: dashboard.accentPurple
                    onClicked: {
                        console.log("Load Model clicked")
                    }
                }

                GlowButton {
                    text: "Benchmark GPU"
                    icon: "🏃"
                    Layout.fillWidth: true
                    glowColor: "#FFaa00"
                    onClicked: {
                        console.log("Benchmark clicked")
                    }
                }

                Item { Layout.fillHeight: true }

                // Status indicator
                Rectangle {
                    Layout.fillWidth: true
                    height: 40
                    color: Qt.rgba(0, 1, 0, 0.1)
                    radius: 10
                    border.color: "#00FF00"
                    border.width: 2

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10

                        Rectangle {
                            width: 12
                            height: 12
                            radius: 6
                            color: "#00FF00"

                            // Pulsing animation
                            SequentialAnimation on opacity {
                                loops: Animation.Infinite
                                NumberAnimation { to: 0.3; duration: 1000 }
                                NumberAnimation { to: 1.0; duration: 1000 }
                            }
                        }

                        Text {
                            text: "System Ready"
                            font.pixelSize: 14
                            color: "#00FF00"
                        }
                    }
                }
            }
        }
    }

    // Update timer (simulates real-time updates)
    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            // In production, fetch real metrics from C++ backend
            console.log("Updating metrics...")
        }
    }
}
