import bb.cascades 1.0
import crossfit.app 1.0

Sheet {
    id: editWorkout
    property alias strength: strengthVal.text
    property alias wod: wodVal.text
    property string recordId: ""

	signal recordUpdated (string strength, string wod)

    Page {
        titleBar: TitleBar {
            title: recordId != "" ? qsTr("Edit") : qsTr("Add")
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    if (recordId != "") {
                        _app.dataManager.updateRecord(recordId, strengthVal.text, wodVal.text);
                        recordUpdated(strengthVal.text, wodVal.text);
                    }
                    else {
                        _app.dataManager.createRecord(strengthVal.text, wodVal.text);
                    }
                    editWorkout.close()
                }
            }
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    editWorkout.close()
                }
            }
        }
        Container {
            rightPadding: 16
            leftPadding: 16
            topPadding: 16
            bottomPadding: 16
            Label {
                text: qsTr("Strength")
            }
            TextArea {
                id: strengthVal
            }
            Label {
                text: qsTr("WOD")
            }
            TextArea {
                id: wodVal
            }
        }
    }
}
