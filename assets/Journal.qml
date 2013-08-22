import bb.cascades 1.0
import crossfit.app 1.0

NavigationPane {
    id: navPane
    Page {
        titleBar: TitleBar {
            title: qsTr("Journal")
        }
        ListView {
            dataModel: _app.dataManager.dataModel
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    JournalRecord {
                        recordId: ListItemData.id
                        strength: ListItemData.strength
                        wod: ListItemData.wod
                    }
                }
            ]
            onTriggered: {
                var selectedItem = dataModel.data(indexPath);
                var details = workoutDetailsDef.createObject();
                details.recordId = selectedItem.id;
                details.strength = selectedItem.strength;
                details.wod = selectedItem.wod;
                navPane.push(details);
            }
        }
        actions: [
            ActionItem {
                title: qsTr("Add")
                //imageSource: "asset:///images/actions/share.png"
                ActionBar.placement: ActionBarPlacement.OnBar

                onTriggered: {
                    var editWorkoutSheet = editWorkoutSheetDef.createObject();
                    editWorkoutSheet.open();
                }
            }
        ]
    }
    onPopTransitionEnded: {
        page.destroy();
    }
    attachedObjects: [
        ComponentDefinition {
            id: editWorkoutSheetDef
            source: "asset:///EditWorkoutSheet.qml"
        },
        ComponentDefinition {
            id: workoutDetailsDef
            source: "asset:///WorkoutDetails.qml"
        }
    ]
}
