import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


Window {
    width: 900
    height: 700
    visible: true
    color: "#1a1a1a"

    property string dicomSource:
        "image://dicom/current?v=" + dicomController.imageVersion

    ColumnLayout{
        anchors.fill: parent

        Button{
            text: "Open DICOM"
            onClicked: fileDialog.open()
        }

        Image{
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: dicomController.imageLoader?dicomSource:""
            cache: false
            fillMode: Image.PreserveAspectFit
        }
        Label {
            text: "Patient: " + dicomController.patientName
            color: "white"
        }

        Label {
            text: "Date: " + dicomController.studyDate
            color: "white"
        }
    }
    FileDialog {
        id: fileDialog
        nameFilters: ["DICOM (*.dcm)"]

        onAccepted: {
            var path = selectedFile.toString().replace("file:///", "")
            dicomController.loadDicom(path)
        }
    }
}
