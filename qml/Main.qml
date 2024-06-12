import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Window {
    visible: true
    width: 800
    height: 600
    title: "Word Frequency Histogram"

    FileDialog {
        id: fileDialog
        title: qsTr("Открыть файл")
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            filename.text = qsTr("Файл: ") + selectedFile.toString();
            fileProcessor.openFile(selectedFile);
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 4

        Text {
            id: filename
            Layout.leftMargin: 4
            Layout.topMargin: 4
            text: qsTr("Файл не выбран")
        }

        RowLayout {
            spacing: 10

            Button {
                text: qsTr("Открыть")
                onClicked: fileDialog.open()
            }
            Button {
                text: qsTr("Старт")
                onClicked: {
                    fileProcessor.startProcessing();
                    pauseButton.state = "pause";
                }
            }
            Button {
                id: pauseButton
                text: qsTr("Пауза")
                state: "disabled"
                onClicked: {
                    fileProcessor.pauseProcessing()
                    pauseButton.state === 'pause' ? pauseButton.state = "resume"
                                                  : pauseButton.state = "pause";
                }

                states: [
                    State {
                        name: "pause"
                        PropertyChanges { target: pauseButton; text: qsTr("Пауза"); enabled: true }
                    },
                    State {
                        name: "resume"
                        PropertyChanges { target: pauseButton; text: qsTr("Продолжить"); enabled: true }
                    },
                    State {
                        name: "disabled"
                        PropertyChanges { target: pauseButton; text: qsTr("Пауза"); enabled: false }
                    }
                ]
            }
            Button {
                text: qsTr("Отмена")
                onClicked: {
                    fileProcessor.cancelProcessing();
                    pauseButton.state = "disabled";
                }
            }
        }

        ProgressBar {
            Layout.fillWidth: true
            value: fileProcessor.progress
        }

        WordHistogram {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Connections {
            target: fileProcessor
            onProcessedFinished: {
                pauseButton.state = "disabled";
            }
        }
    }
}
