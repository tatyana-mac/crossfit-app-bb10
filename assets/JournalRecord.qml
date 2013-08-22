import bb.cascades 1.0

Container {
    property alias recordId: idLabel.text
    property alias strength: strengthLabel.text
    property alias wod: wodLabel.text
    
    Container {
        preferredHeight: 80
        leftPadding: 16
        rightPadding: 16
        topPadding: 8
        bottomPadding: 8

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            Label {
                id: idLabel
            }
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
            Label {
                id: strengthLabel
                multiline: true
            }
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
            Label {
                id: wodLabel
                multiline: true
            }
        }
    }
    Divider {
    }
}