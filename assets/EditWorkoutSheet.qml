import bb.cascades 1.0
import crossfit.app 1.0

Sheet {
    id: editWorkout
    property alias strength: strengthVal.text
    property alias wod: wodVal.text
    property string recordId: ""

    signal recordUpdated(string strength, string wod)

    Page {
        titleBar: TitleBar {
            title: recordId != "" ? qsTr("Edit") : qsTr("Add")
            acceptAction: ActionItem {
                title: qsTr("Save")
                onTriggered: {
                    if (recordId != "") {
                        _app.dataManager.updateRecord(recordId, strengthVal.text, wodVal.text);
                        recordUpdated(strengthVal.text, wodVal.text);
                    } else {
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
        ScrollView {
            Container {
                rightPadding: 16
                leftPadding: 16
                topPadding: 16
                bottomPadding: 16

                DateTimePicker {
                    mode: DateTimePickerMode.DateTime
                    title: qsTr("Date")
                    minuteInterval: 15
                }
                Label {
                    text: qsTr("Strength")
                }

                DropDown {
                    id: exerciseList
                    title: qsTr("Exercise")
                }
                TextArea {
                    id: strengthVal
                    hintText: qsTr("Description…")
                    preferredHeight: 200
                    //autoSize.maxLineCount: 5
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    TextField {
                        hintText: qsTr("Result...")
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 3.0
                        }
                        verticalAlignment: VerticalAlignment.Center
                    }
                    DropDown {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1.0
                        }
                        title: qsTr("Units")
                        Option {
                            text: qsTr("kg")
                        }
                        Option {
                            text: qsTr("lb")
                            selected: true
                        }
                    }
                }
                Label {
                    text: qsTr("WOD")
                }
                DropDown {
                    id: wodList
                    title: qsTr("Name")
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    DropDown {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1.0
                        }
                        Option {
                            text: qsTr("Reps")
                            selected: true
                        }
                        Option {
                            text: qsTr("AMRAP")
                        }
                        Option {
                            text: qsTr("Other")
                        }
                    }
                    TextField {
                        hintText: qsTr("Count...")
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 2.0
                        }
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
                TextArea {
                    id: wodVal
                    preferredHeight: 200
                    hintText: qsTr("Description...")
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    TextField {
                        hintText: qsTr("Result...")
                        verticalAlignment: VerticalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 4.0
                        }
                    }
                    Label {
                        text: qsTr("Rx")
                        verticalAlignment: VerticalAlignment.Center
                    }
                    CheckBox {
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
                TextArea {
                    preferredHeight: 200
                    hintText: qsTr("Workout Notes...")
                }
            }
        }
    }
}
