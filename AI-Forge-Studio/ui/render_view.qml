/**
 * Render View - AI Forge Studio
 *
 * Image generation interface with real-time preview and controls.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: renderView
    color: "transparent"

    property color accentCyan: "#00FFFF"
    property color accentPurple: "#AA00FF"
    property bool isGenerating: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Preview area
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Preview"
            icon: "🖼️"

            content: Rectangle {
                anchors.fill: parent
                color: "#000000"
                radius: 10

                // Image preview (placeholder)
                Image {
                    id: previewImage
                    anchors.centerIn: parent
                    width: Math.min(parent.width - 40, 512)
                    height: Math.min(parent.height - 40, 512)
                    fillMode: Image.PreserveAspectFit
                    source: ""  // Will be set from C++

                    // Placeholder
                    Rectangle {
                        anchors.fill: parent
                        visible: previewImage.source == ""
                        color: Qt.rgba(0.1, 0.1, 0.15, 0.5)
                        border.color: Qt.rgba(1, 1, 1, 0.2)
                        border.width: 2
                        radius: 10

                        Text {
                            anchors.centerIn: parent
                            text: renderView.isGenerating ?
                                  "Generating...\n⏳" :
                                  "No image generated\n🎨"
                            font.pixelSize: 20
                            color: Qt.rgba(1, 1, 1, 0.5)
                            horizontalAlignment: Text.AlignHCenter
                        }

                        // Progress bar during generation
                        Rectangle {
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: 40
                            width: parent.width * 0.8
                            height: 10
                            color: Qt.rgba(0, 0, 0, 0.3)
                            radius: 5
                            visible: renderView.isGenerating

                            Rectangle {
                                id: progressBar
                                width: parent.width * 0.5  // Animated
                                height: parent.height
                                color: renderView.accentCyan
                                radius: 5

                                gradient: Gradient {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: renderView.accentCyan }
                                    GradientStop { position: 1.0; color: renderView.accentPurple }
                                }

                                // Animate progress
                                NumberAnimation on width {
                                    running: renderView.isGenerating
                                    from: 0
                                    to: parent.width
                                    duration: 5000
                                    loops: 1
                                }
                            }
                        }
                    }
                }

                // Stats overlay
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 10
                    width: 200
                    height: 100
                    color: Qt.rgba(0, 0, 0, 0.8)
                    radius: 10
                    visible: previewImage.source != ""

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 5

                        Text {
                            text: "Generation Stats"
                            font.pixelSize: 11
                            font.bold: true
                            color: renderView.accentCyan
                        }

                        Text {
                            text: "Time: 12.3s"
                            font.pixelSize: 10
                            color: "white"
                        }

                        Text {
                            text: "Steps: 50"
                            font.pixelSize: 10
                            color: "white"
                        }

                        Text {
                            text: "VRAM: 8.2 GB"
                            font.pixelSize: 10
                            color: "white"
                        }
                    }
                }

                // Save button
                Button {
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 10
                    text: "💾 Save"
                    width: 100
                    height: 40
                    visible: previewImage.source != ""

                    background: Rectangle {
                        color: parent.pressed ?
                               Qt.rgba(0, 1, 1, 0.3) :
                               Qt.rgba(0, 1, 1, 0.1)
                        radius: 5
                        border.color: renderView.accentCyan
                        border.width: 2
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        console.log("Save image")
                    }
                }
            }
        }

        // Control panel
        GlassCard {
            Layout.preferredWidth: 400
            Layout.fillHeight: true
            title: "Generation Controls"
            icon: "⚙️"

            content: ScrollView {
                anchors.fill: parent
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: 20

                    // Model selection
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Model:"
                            font.pixelSize: 12
                            color: "white"
                        }

                        ComboBox {
                            id: modelCombo
                            width: parent.width
                            model: ["Stable Diffusion XL", "Stable Diffusion 2.1", "DALL-E"]

                            background: Rectangle {
                                color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                                radius: 5
                                border.color: Qt.rgba(1, 1, 1, 0.3)
                                border.width: 1
                            }
                        }
                    }

                    // Prompt
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Prompt:"
                            font.pixelSize: 12
                            color: "white"
                        }

                        TextArea {
                            id: promptText
                            width: parent.width
                            height: 100
                            placeholderText: "Describe the image you want to generate..."
                            wrapMode: TextEdit.Wrap
                            font.pixelSize: 11

                            background: Rectangle {
                                color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                                radius: 5
                                border.color: Qt.rgba(1, 1, 1, 0.3)
                                border.width: 1
                            }
                        }
                    }

                    // Negative prompt
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Negative Prompt:"
                            font.pixelSize: 12
                            color: "white"
                        }

                        TextArea {
                            id: negativePromptText
                            width: parent.width
                            height: 60
                            placeholderText: "What to avoid in the image..."
                            wrapMode: TextEdit.Wrap
                            font.pixelSize: 11

                            background: Rectangle {
                                color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                                radius: 5
                                border.color: Qt.rgba(1, 1, 1, 0.3)
                                border.width: 1
                            }
                        }
                    }

                    // Steps slider
                    Column {
                        width: parent.width
                        spacing: 8

                        RowLayout {
                            width: parent.width

                            Text {
                                text: "Inference Steps:"
                                font.pixelSize: 12
                                color: "white"
                            }

                            Item { Layout.fillWidth: true }

                            Text {
                                text: stepsSlider.value
                                font.pixelSize: 12
                                font.bold: true
                                color: renderView.accentCyan
                            }
                        }

                        Slider {
                            id: stepsSlider
                            width: parent.width
                            from: 10
                            to: 100
                            value: 50
                            stepSize: 1

                            background: Rectangle {
                                x: stepsSlider.leftPadding
                                y: stepsSlider.topPadding + stepsSlider.availableHeight / 2 - height / 2
                                width: stepsSlider.availableWidth
                                height: 6
                                radius: 3
                                color: Qt.rgba(0, 0, 0, 0.3)
                                border.color: Qt.rgba(1, 1, 1, 0.2)
                                border.width: 1

                                Rectangle {
                                    width: stepsSlider.visualPosition * parent.width
                                    height: parent.height
                                    color: renderView.accentCyan
                                    radius: 3
                                }
                            }

                            handle: Rectangle {
                                x: stepsSlider.leftPadding + stepsSlider.visualPosition * (stepsSlider.availableWidth - width)
                                y: stepsSlider.topPadding + stepsSlider.availableHeight / 2 - height / 2
                                width: 20
                                height: 20
                                radius: 10
                                color: renderView.accentCyan
                                border.color: "white"
                                border.width: 2
                            }
                        }
                    }

                    // Guidance scale
                    Column {
                        width: parent.width
                        spacing: 8

                        RowLayout {
                            width: parent.width

                            Text {
                                text: "Guidance Scale:"
                                font.pixelSize: 12
                                color: "white"
                            }

                            Item { Layout.fillWidth: true }

                            Text {
                                text: guidanceSlider.value.toFixed(1)
                                font.pixelSize: 12
                                font.bold: true
                                color: renderView.accentPurple
                            }
                        }

                        Slider {
                            id: guidanceSlider
                            width: parent.width
                            from: 1.0
                            to: 20.0
                            value: 7.5
                            stepSize: 0.5

                            background: Rectangle {
                                x: guidanceSlider.leftPadding
                                y: guidanceSlider.topPadding + guidanceSlider.availableHeight / 2 - height / 2
                                width: guidanceSlider.availableWidth
                                height: 6
                                radius: 3
                                color: Qt.rgba(0, 0, 0, 0.3)
                                border.color: Qt.rgba(1, 1, 1, 0.2)
                                border.width: 1

                                Rectangle {
                                    width: guidanceSlider.visualPosition * parent.width
                                    height: parent.height
                                    color: renderView.accentPurple
                                    radius: 3
                                }
                            }

                            handle: Rectangle {
                                x: guidanceSlider.leftPadding + guidanceSlider.visualPosition * (guidanceSlider.availableWidth - width)
                                y: guidanceSlider.topPadding + guidanceSlider.availableHeight / 2 - height / 2
                                width: 20
                                height: 20
                                radius: 10
                                color: renderView.accentPurple
                                border.color: "white"
                                border.width: 2
                            }
                        }
                    }

                    // Resolution
                    Column {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Resolution:"
                            font.pixelSize: 12
                            color: "white"
                        }

                        ComboBox {
                            id: resolutionCombo
                            width: parent.width
                            model: ["512x512", "768x768", "1024x1024", "1024x768", "768x1024"]

                            background: Rectangle {
                                color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                                radius: 5
                                border.color: Qt.rgba(1, 1, 1, 0.3)
                                border.width: 1
                            }
                        }
                    }

                    // Generate button
                    GlowButton {
                        text: renderView.isGenerating ? "Generating..." : "Generate Image"
                        icon: "🎨"
                        Layout.fillWidth: true
                        height: 60
                        glowColor: renderView.accentCyan
                        enabled: !renderView.isGenerating && promptText.text.length > 0

                        onClicked: {
                            console.log("Generate image with prompt:", promptText.text)
                            renderView.isGenerating = true

                            // Simulate generation (in production, call C++ backend)
                            generateTimer.start()
                        }
                    }

                    // Random seed
                    Row {
                        spacing: 10

                        CheckBox {
                            id: randomSeedCheck
                            text: "Random Seed"
                            checked: true
                        }

                        SpinBox {
                            from: 0
                            to: 999999999
                            value: 42
                            enabled: !randomSeedCheck.checked
                        }
                    }
                }
            }
        }
    }

    // Simulate generation timer
    Timer {
        id: generateTimer
        interval: 5000
        repeat: false
        onTriggered: {
            renderView.isGenerating = false
            console.log("Generation complete")
        }
    }
}
