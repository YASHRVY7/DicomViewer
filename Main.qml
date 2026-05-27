import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import DicomViewer.Backend 1.0

Window {

    width: 1400
    height: 860
    visible: true

    title: "Medical Imaging Viewer"

    color: "#0a0f16"

    property string dicomSource:
        "image://dicom/current?v=" +
        DicomController.imageVersion

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

                            imageContainer.x =
                                    (viewerArea.width -
                                     imageContainer.width) / 2

                            imageContainer.y =
                                    (viewerArea.height -
                                     imageContainer.height) / 2
                        }
                    }

                    Button {

                        text: "BONE"

                        implicitWidth: 90
                        implicitHeight: 38

                        font.pixelSize: 12
                        font.bold: true

                        background: Rectangle {

                            radius: 4
                            color: parent.down ? "#243244" : "#1a2430"
                            border.color: "#2c3c50"
                        }

                        contentItem: Text {

                            text: parent.text
                            color: "#d8dee9"
                            font.pixelSize: 12
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {

                            DicomController.setWindowWidth(1500)
                            DicomController.setWindowLevel(300)
                        }
                    }

                    Button {

                        text: "LUNG"

                        implicitWidth: 90
                        implicitHeight: 38

                        font.pixelSize: 12
                        font.bold: true

                        background: Rectangle {

                            radius: 4
                            color: parent.down ? "#243244" : "#1a2430"
                            border.color: "#2c3c50"
                        }

                        contentItem: Text {

                            text: parent.text
                            color: "#d8dee9"
                            font.pixelSize: 12
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {

                            DicomController.setWindowWidth(1500)
                            DicomController.setWindowLevel(-600)
                        }
                    }

                    Button {

                        text: "BRAIN"

                        implicitWidth: 90
                        implicitHeight: 38

                        font.pixelSize: 12
                        font.bold: true

                        background: Rectangle {

                            radius: 4
                            color: parent.down ? "#243244" : "#1a2430"
                            border.color: "#2c3c50"
                        }

                        contentItem: Text {

                            text: parent.text
                            color: "#d8dee9"
                            font.pixelSize: 12
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {

                            DicomController.setWindowWidth(80)
                            DicomController.setWindowLevel(40)
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
                                    DicomController.currentSlice +
                                    " / " +
                                    DicomController.totalSlices

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

                    Layout.preferredWidth: 300
                    Layout.fillHeight: true

                    color: "#101720"

                    border.color: "#1f2b38"
                    border.width: 1

                    Flickable {

                        anchors.fill: parent
                        anchors.margins: 18

                        contentWidth: width
                        contentHeight: panelColumn.height

                        clip: true
                        boundsBehavior: Flickable.StopAtBounds

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }

                        Column {

                            id: panelColumn

                            width: parent.width
                            spacing: 18

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

                            spacing: 16

                            Column {

                                spacing: 6

                                Label {

                                    text: "Patient Name"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        DicomController.patientName

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
                                        DicomController.studyDate

                                    color: "#f1f5f9"

                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Patient ID"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.patientId
                                    color: "#f1f5f9"
                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Modality"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.modality
                                    color: "#f1f5f9"
                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Study Description"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.studyDescription
                                    color: "#f1f5f9"
                                    font.pixelSize: 14
                                    font.bold: true
                                    wrapMode: Text.WordWrap
                                    width: 240
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Series Description"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.seriesDescription
                                    color: "#f1f5f9"
                                    font.pixelSize: 14
                                    font.bold: true
                                    wrapMode: Text.WordWrap
                                    width: 240
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Institution"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.institutionName
                                    color: "#f1f5f9"
                                    font.pixelSize: 14
                                    font.bold: true
                                    wrapMode: Text.WordWrap
                                    width: 240
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Slice Thickness"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.sliceThickness
                                    color: "#f1f5f9"
                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Pixel Spacing"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text: DicomController.pixelSpacing
                                    color: "#f1f5f9"
                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {
                                    text: "Image Size"
                                    color: "#7f92a8"
                                    font.pixelSize: 12
                                }

                                Label {
                                    text:
                                        DicomController.imageWidth +
                                        " x " +
                                        DicomController.imageHeight

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

                            Column {

                                spacing: 6

                                Label {

                                    text: "Window Width"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        Math.round(
                                            DicomController.windowWidth
                                            )

                                    color: "#f1f5f9"

                                    font.pixelSize: 15
                                    font.bold: true
                                }
                            }

                            Column {

                                spacing: 6

                                Label {

                                    text: "Window Level"

                                    color: "#7f92a8"

                                    font.pixelSize: 12
                                }

                                Label {

                                    text:
                                        Math.round(
                                            DicomController.windowLevel
                                            )

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

                            text: "WINDOW CONTROLS"

                            color: "#f1f5f9"

                            font.pixelSize: 15
                            font.bold: true
                        }

                        Column {

                            width: parent.width
                            spacing: 10

                            Label {

                                text: "Width / Contrast"

                                color: "#8ea1b5"

                                font.pixelSize: 12
                            }

                            Slider {

                                width: parent.width

                                from: 1
                                to: 3000

                                value:
                                    DicomController.windowWidth

                                enabled:
                                    DicomController.imageLoaded

                                onMoved: {

                                    DicomController
                                    .setWindowWidth(value)
                                }
                            }

                            Label {

                                text: "Level / Brightness"

                                color: "#8ea1b5"

                                font.pixelSize: 12
                            }

                            Slider {

                                width: parent.width

                                from: -1000
                                to: 1000

                                value:
                                    DicomController.windowLevel

                                enabled:
                                    DicomController.imageLoaded

                                onMoved: {

                                    DicomController
                                    .setWindowLevel(value)
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
                                        DicomController.imageLoaded
                                        ? "#22c55e"
                                        : "#ef4444"
                                }

                                Label {

                                    text:
                                        DicomController.imageLoaded
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
                }

                // =====================================
                // IMAGE VIEWER
                // =====================================

                Rectangle {

                    id: viewerArea

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
                                    DicomController.previousSlice()
                                else
                                    DicomController.nextSlice()
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

                        width: 380
                        height: 40

                        radius: 4

                        color: "#111927dd"

                        border.color: "#263241"

                        z: 999

                        Row {

                            anchors.centerIn: parent
                            spacing: 18

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
                                    DicomController.currentSlice

                                color: "#dbe4ee"

                                font.pixelSize: 13
                                font.bold: true
                            }

                            Label {

                                text:
                                    "WW/WL: " +
                                    Math.round(DicomController.windowWidth) +
                                    " / " +
                                    Math.round(DicomController.windowLevel)

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
                                DicomController.imageLoaded
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
                            !DicomController.imageLoaded

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

                        text:
                            DicomController.imageLoaded
                            ? "Study Ready"
                            : "Ready"

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

            DicomController.loadFolder(path)
        }
    }
}
