import bb.cascades 1.0

Page {
    property alias strength: strengthVal.text
    property alias wod: wodVal.text
    property string recordId: ""

    titleBar: TitleBar {
        title: qsTr("Workout Details")
    }
    Container {
        rightPadding: 16
        leftPadding: 16
        topPadding: 16
        bottomPadding: 16
        Label {
            text: qsTr("Strength")
        }
        Label {
            id: strengthVal
        }
        Label {
            text: qsTr("WOD")
        }
        Label {
            id: wodVal
        }
    }
    actions: [
        ActionItem {
            title: qsTr("Edit")
            //imageSource: "asset:///images/actions/share.png"
            ActionBar.placement: ActionBarPlacement.OnBar

            onTriggered: {
                var editWorkoutSheet = editWorkoutSheetDef.createObject();
                editWorkoutSheet.recordId = recordId;
                editWorkoutSheet.strength = strength;
                editWorkoutSheet.wod = wod;
                editWorkoutSheet.open();
                editWorkoutSheet.recordUpdated.connect(updateDetails);
            }
        }
    ]
    attachedObjects: [
        ComponentDefinition {
            id: editWorkoutSheetDef
            source: "asset:///EditWorkoutSheet.qml"
        }
    ]
    function updateDetails(strength, wod) {
        console.log("got update signal " + strength + " " + wod);
        strengthVal.text = strength;
        wodVal.text = wod;
    }
}
