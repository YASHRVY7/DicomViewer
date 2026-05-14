import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Window {

    width: 1400
    height: 860
    visible: true

    title: "Medical Imaging Viewer"

    color: "#0a0f16"

    property string dicomSource:
        "image://dicom/current?v=" +
        dicomController.imageVersion

    property real zoomLevel: 1.0
    property real minZoom: 0.5
    property real maxZoom: 8.0

    Rectangle {

        anchors.fill: parent
        color: "#0a0f16"

        ColumnLayout {

            anchors.fill: parent
            spacing: 0

            // =========================================
            // HEADER BAR
            // =========================================

            Rectangle {

                Layout.fillWidth: true
                height: 64

                color: "#121922"

                border.color: "#1f2b38"
                border.width: 1

                RowLayout {

                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    spacing: 14

                    Rectangle {

                        width: 5
                        height: 24
                        radius: 2
                        color: "#3b82f6"
                    }

                    Label {

                        text: "MEDICAL IMAGING VIEWER"

                        color: "#f1f5f9"

                        font.pixelSize: 20
                        font.bold: true
                        font.letterSpacing: 1
                    }

                    Rectangle {

                        width: 1
                        height: 28

                        color: "#263241"
                    }

                    Button {

                        text: "OPEN STUDY"

                        implicitWidth: 140
                        implicitHeight: 38

                        font.pixelSize: 13
                        font.bold: true

                        background: Rectangle {

                            radius: 4

                            color:
                                parent.down
                                ? "#2563eb"
                                : "#1d4ed8"

                            border.color: "#3b82f6"
                        }

                        contentItem: Text {

                            text: parent.text

                            color: "white"

                            font.pixelSize: 13
                            font.bold: true

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: folderDialog.open()
                    }

                    Button {

                        text: "RESET VIEW"

                        implicitWidth: 130
                        implicitHeight: 38

                        font.pixelSize: 13
                        font.bold: true

                        background: Rectangle {

                            radius: 4

                            color:
                                parent.down
                                ? "#243244"
                                : "#1a2430"

                            border.color: "#2c3c50"
                        }

                        contentItem: Text {

                            text: parent.text

                            color: "#d8dee9"

                            font.pixelSize: 13
                            font.bold: true

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {

                            zoomLevel = 1.0

                            imageContainer.x = 0
                            imageContainer.y = 0
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Rectangle {

                        width: 190
                        height: 38

                        radius: 4

                        color: "#0e141c"

                        border.color: "#263241"

                        Row {

                            anchors.centerIn: parent
                            spacing: 8

                            Label {

                                text: "SLICE"

                                color: "#8ea1b5"

                                font.pixelSize: 12
                                font.bold: true
                            }

                            Label {

                                text:
                                    dicomController.currentSlice +
                                    " / " +
                                    dicomController.totalSlices

                                color: "#f1f5f9"

                                font.pixelSize: 14
                                font.bold: true
                            }
                        }
                    }
                }
            }

            // =========================================
            // MAIN CONTENT
            // =========================================

            RowLayout {

                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                // =====================================
                // LEFT PATIENT PANEL
                // =====================================

                Rectangle {

                    Layout.preferredWidth: 280
                    Layout.fillHeight: true

                    color: "#101720"

                    border.color: "#1f2b38"
                    border.width: 1

                    Column {

                        anchors.fill: parent
                        anchors.margins: 18
                        spacing: 20

                        Label {

                            text: "PATIENT INFORMATION"

                            color: "#f1f5f9"

                            font.pixelSize: 15
                            font.bold: true
                        }

                        Rectangle {

                            width: parent.width
                            height: 1

                            color: "#263241"
                        }

                        Column {

                            spacing: 18

                            Column {

                                spacing: 6

                                Label {

                                    text: "Patient Name"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        dicomController.patientName

                                    color: "#f1f5f9"

                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {

                                    text: "Study Date"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        dicomController.studyDate

                                    color: "#f1f5f9"

                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {

                                    text: "Zoom Level"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        Math.round(
                                            zoomLevel * 100
                                            ) + "%"

                                    color: "#f1f5f9"

                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }
                        }

                        Rectangle {

                            width: parent.width
                            height: 1

                            color: "#263241"
                        }

                        Label {

                            text: "VIEWER STATUS"

                            color: "#f1f5f9"

                            font.pixelSize: 15
                            font.bold: true
                        }

                        Rectangle {

                            width: parent.width
                            height: 44

                            radius: 4

                            color: "#0d141c"

                            border.color: "#263241"

                            Row {

                                anchors.centerIn: parent
                                spacing: 10

                                Rectangle {

                                    width: 10
                                    height: 10

                                    radius: 5

                                    color:
                                        dicomController.imageLoaded
                                        ? "#22c55e"
                                        : "#ef4444"
                                }

                                Label {

                                    text:
                                        dicomController.imageLoaded
                                        ? "Study Loaded"
                                        : "Waiting For Study"

                                    color: "#dbe4ee"

                                    font.pixelSize: 13
                                    font.bold: true
                                }
                            }
                        }
                    }
                }

                // =====================================
                // IMAGE VIEWER
                // =====================================

                Rectangle {

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    color: "black"

                    border.color: "#1f2b38"
                    border.width: 1

                    clip: true

                    WheelHandler {

                        onWheel: function(event) {

                            // CTRL + wheel = zoom
                            if (event.modifiers & Qt.ControlModifier) {

                                if (event.angleDelta.y > 0)
                                    zoomLevel += 0.1
                                else
                                    zoomLevel -= 0.1

                                zoomLevel =
                                        Math.max(
                                            minZoom,
                                            Math.min(
                                                maxZoom,
                                                zoomLevel
                                                )
                                            )
                            }

                            // Normal wheel = slice scroll
                            else {

                                if (event.angleDelta.y > 0)
                                    dicomController.previousSlice()
                                else
                                    dicomController.nextSlice()
                            }
                        }
                    }

                    // ===============================
                    // TOP OVERLAY
                    // ===============================

                    Rectangle {

                        anchors.top: parent.top
                        anchors.horizontalCenter:
                            parent.horizontalCenter

                        anchors.topMargin: 14

                        width: 240
                        height: 40

                        radius: 4

                        color: "#111927dd"

                        border.color: "#263241"

                        z: 999

                        Row {

                            anchors.centerIn: parent
                            spacing: 20

                            Label {

                                text:
                                    "Zoom: " +
                                    Math.round(
                                        zoomLevel * 100
                                        ) + "%"

                                color: "#dbe4ee"

                                font.pixelSize: 13
                                font.bold: true
                            }

                            Label {

                                text:
                                    "Slice: " +
                                    dicomController.currentSlice

                                color: "#dbe4ee"

                                font.pixelSize: 13
                                font.bold: true
                            }
                        }
                    }

                    // ===============================
                    // IMAGE CONTAINER
                    // ===============================

                    Item {

                        id: imageContainer

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2

                        width:
                            dicomImage.paintedWidth *
                            zoomLevel

                        height:
                            dicomImage.paintedHeight *
                            zoomLevel

                        Image {

                            id: dicomImage

                            anchors.centerIn: parent

                            source:
                                dicomController.imageLoaded
                                ? dicomSource
                                : ""

                            fillMode:
                                Image.PreserveAspectFit

                            cache: false
                            smooth: true

                            scale: zoomLevel
                        }

                        MouseArea {

                            anchors.fill: parent

                            drag.target: imageContainer

                            cursorShape:
                                pressed
                                ? Qt.ClosedHandCursor
                                : Qt.OpenHandCursor
                        }
                    }

                    // ===============================
                    // EMPTY SCREEN
                    // ===============================

                    Column {

                        anchors.centerIn: parent
                        spacing: 10

                        visible:
                            !dicomController.imageLoaded

                        Label {

                            text: "NO ACTIVE STUDY"

                            color: "#f1f5f9"

                            font.pixelSize: 28
                            font.bold: true
                        }

                        Label {

                            text:
                                "Load a DICOM study to begin viewing"

                            color: "#7f92a8"

                            font.pixelSize: 14
                        }

                        Rectangle {

                            width: 320
                            height: 1

                            color: "#263241"
                        }

                        Label {

                            text:
                                "Mouse Wheel : Navigate Slices"

                            color: "#5f7187"

                            font.pixelSize: 12
                        }

                        Label {

                            text:
                                "CTRL + Mouse Wheel : Zoom"

                            color: "#5f7187"

                            font.pixelSize: 12
                        }
                    }
                }
            }

            // =========================================
            // FOOTER
            // =========================================

            Rectangle {

                Layout.fillWidth: true
                height: 34

                color: "#121922"

                border.color: "#1f2b38"
                border.width: 1

                RowLayout {

                    anchors.fill: parent
                    anchors.leftMargin: 18
                    anchors.rightMargin: 18

                    Label {

                        text:
                            "DICOM Medical Imaging System"

                        color: "#7f92a8"

                        font.pixelSize: 12
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Label {

                        text: "Ready"

                        color: "#22c55e"

                        font.pixelSize: 12
                        font.bold: true
                    }
                }
            }
        }
    }

    FolderDialog {

        id: folderDialog

        onAccepted: {

            var path =
                    selectedFolder.toString()
                    .replace("file:///", "")

            dicomController.loadFolder(path)
        }
    }
}
