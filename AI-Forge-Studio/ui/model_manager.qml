/**
 * Model Manager - AI Forge Studio
 *
 * Interface for loading, managing, and optimizing AI models.
 * Supports multiple model formats and provides optimization tools.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Rectangle {
    id: modelManager
    color: "transparent"

    property color accentCyan: "#00FFFF"
    property color accentPurple: "#AA00FF"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Header with actions
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "AI Model Manager"
                font.pixelSize: 24
                font.bold: true
                color: modelManager.accentCyan
            }

            Item { Layout.fillWidth: true }

            GlowButton {
                text: "Load Model"
                icon: "📥"
                glowColor: modelManager.accentCyan
                onClicked: fileDialog.open()
            }

            GlowButton {
                text: "Import from HF"
                icon: "🤗"
                glowColor: modelManager.accentPurple
                onClicked: huggingFaceDialog.open()
            }
        }

        // Model list
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Loaded Models"
            icon: "🤖"

            content: ScrollView {
                anchors.fill: parent
                clip: true

                ListView {
                    id: modelListView
                    anchors.fill: parent
                    spacing: 10

                    model: ListModel {
                        ListElement {
                            modelName: "Stable Diffusion XL"
                            modelType: "Text-to-Image"
                            modelFormat: "SafeTensors"
                            modelSize: "6.9 GB"
                            vramUsage: "8.2 GB"
                            isLoaded: true
                            isOptimized: true
                        }
                        ListElement {
                            modelName: "RealESRGAN 4x"
                            modelType: "Image Upscaling"
                            modelFormat: "PyTorch"
                            modelSize: "64 MB"
                            vramUsage: "512 MB"
                            isLoaded: true
                            isOptimized: false
                        }
                        ListElement {
                            modelName: "LLaMA 3.1 8B"
                            modelType: "Text Generation"
                            modelFormat: "GGUF"
                            modelSize: "4.1 GB"
                            vramUsage: "5.5 GB"
                            isLoaded: false
                            isOptimized: false
                        }
                    }

                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 120
                        color: Qt.rgba(0.05, 0.05, 0.1, 0.7)
                        radius: 10
                        border.color: model.isLoaded ?
                                     Qt.rgba(0, 1, 1, 0.5) :
                                     Qt.rgba(1, 1, 1, 0.2)
                        border.width: 2

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 20

                            // Model icon
                            Rectangle {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                color: Qt.rgba(0, 1, 1, 0.1)
                                radius: 10

                                Text {
                                    anchors.centerIn: parent
                                    text: "🤖"
                                    font.pixelSize: 40
                                }
                            }

                            // Model info
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 5

                                Text {
                                    text: model.modelName
                                    font.pixelSize: 16
                                    font.bold: true
                                    color: modelManager.accentCyan
                                }

                                RowLayout {
                                    spacing: 15

                                    Text {
                                        text: "Type: " + model.modelType
                                        font.pixelSize: 11
                                        color: Qt.rgba(1, 1, 1, 0.7)
                                    }

                                    Text {
                                        text: "Format: " + model.modelFormat
                                        font.pixelSize: 11
                                        color: Qt.rgba(1, 1, 1, 0.7)
                                    }

                                    Text {
                                        text: "Size: " + model.modelSize
                                        font.pixelSize: 11
                                        color: Qt.rgba(1, 1, 1, 0.7)
                                    }
                                }

                                RowLayout {
                                    spacing: 10

                                    // Status indicator
                                    Rectangle {
                                        width: 60
                                        height: 20
                                        radius: 10
                                        color: model.isLoaded ?
                                               Qt.rgba(0, 1, 0, 0.2) :
                                               Qt.rgba(1, 0, 0, 0.2)
                                        border.color: model.isLoaded ? "#00FF00" : "#FF0000"
                                        border.width: 1

                                        Text {
                                            anchors.centerIn: parent
                                            text: model.isLoaded ? "Loaded" : "Offline"
                                            font.pixelSize: 9
                                            color: model.isLoaded ? "#00FF00" : "#FF0000"
                                        }
                                    }

                                    // Optimized indicator
                                    Rectangle {
                                        width: 70
                                        height: 20
                                        radius: 10
                                        color: model.isOptimized ?
                                               Qt.rgba(0, 0.5, 1, 0.2) :
                                               Qt.rgba(1, 1, 0, 0.2)
                                        border.color: model.isOptimized ?
                                                     modelManager.accentCyan :
                                                     "#FFAA00"
                                        border.width: 1
                                        visible: model.isLoaded

                                        Text {
                                            anchors.centerIn: parent
                                            text: model.isOptimized ? "TensorRT" : "Not Opt"
                                            font.pixelSize: 9
                                            color: model.isOptimized ?
                                                   modelManager.accentCyan :
                                                   "#FFAA00"
                                        }
                                    }

                                    Text {
                                        text: "VRAM: " + model.vramUsage
                                        font.pixelSize: 11
                                        font.bold: true
                                        color: modelManager.accentPurple
                                        visible: model.isLoaded
                                    }
                                }
                            }

                            // Action buttons
                            ColumnLayout {
                                spacing: 5

                                Button {
                                    text: model.isLoaded ? "Unload" : "Load"
                                    width: 80
                                    height: 30
                                    onClicked: {
                                        console.log("Toggle load:", model.modelName)
                                    }

                                    background: Rectangle {
                                        color: parent.pressed ?
                                               Qt.rgba(0, 1, 1, 0.3) :
                                               Qt.rgba(0, 1, 1, 0.1)
                                        radius: 5
                                        border.color: modelManager.accentCyan
                                        border.width: 1
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        color: "white"
                                        font.pixelSize: 11
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }

                                Button {
                                    text: "Optimize"
                                    width: 80
                                    height: 30
                                    enabled: model.isLoaded && !model.isOptimized
                                    onClicked: {
                                        console.log("Optimize:", model.modelName)
                                    }

                                    background: Rectangle {
                                        color: parent.enabled ?
                                               (parent.pressed ?
                                                   Qt.rgba(0.7, 0, 1, 0.3) :
                                                   Qt.rgba(0.7, 0, 1, 0.1)) :
                                               Qt.rgba(0.3, 0.3, 0.3, 0.1)
                                        radius: 5
                                        border.color: parent.enabled ?
                                                     modelManager.accentPurple :
                                                     Qt.rgba(1, 1, 1, 0.2)
                                        border.width: 1
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        color: parent.enabled ? "white" : Qt.rgba(1, 1, 1, 0.3)
                                        font.pixelSize: 11
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }

                                Button {
                                    text: "Delete"
                                    width: 80
                                    height: 30
                                    onClicked: {
                                        console.log("Delete:", model.modelName)
                                    }

                                    background: Rectangle {
                                        color: parent.pressed ?
                                               Qt.rgba(1, 0, 0, 0.3) :
                                               Qt.rgba(1, 0, 0, 0.1)
                                        radius: 5
                                        border.color: "#FF0000"
                                        border.width: 1
                                    }

                                    contentItem: Text {
                                        text: parent.text
                                        color: "white"
                                        font.pixelSize: 11
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // File dialog for loading models
    FileDialog {
        id: fileDialog
        title: "Select AI Model File"
        folder: shortcuts.home
        nameFilters: [
            "All AI Models (*.safetensors *.gguf *.onnx *.pt *.pth)",
            "SafeTensors (*.safetensors)",
            "GGUF (*.gguf)",
            "ONNX (*.onnx)",
            "PyTorch (*.pt *.pth)"
        ]
        onAccepted: {
            console.log("Selected file:", fileDialog.fileUrl)
            // Load model in C++ backend
        }
    }

    // HuggingFace import dialog
    Dialog {
        id: huggingFaceDialog
        title: "Import from HuggingFace"
        width: 500
        height: 300
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        ColumnLayout {
            anchors.fill: parent
            spacing: 15

            Text {
                text: "HuggingFace Model ID:"
                font.pixelSize: 14
                color: "white"
            }

            TextField {
                id: hfModelId
                Layout.fillWidth: true
                placeholderText: "e.g., stabilityai/stable-diffusion-xl-base-1.0"
                font.pixelSize: 12

                background: Rectangle {
                    color: Qt.rgba(0.05, 0.05, 0.1, 0.9)
                    radius: 5
                    border.color: Qt.rgba(1, 1, 1, 0.3)
                    border.width: 1
                }
            }

            Text {
                text: "Precision:"
                font.pixelSize: 14
                color: "white"
            }

            ComboBox {
                id: precisionCombo
                Layout.fillWidth: true
                model: ["FP16 (Recommended)", "FP32 (Slower)", "INT8 (Fastest)"]
            }

            CheckBox {
                text: "Optimize with TensorRT after loading"
                checked: true
            }
        }

        onAccepted: {
            console.log("Import from HF:", hfModelId.text)
            // Import model from HuggingFace
        }
    }
}
