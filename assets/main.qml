import bb.cascades 1.0

TabbedPane {
    showTabsOnActionBar: false
    Tab {
        title: qsTr("Journal")
        Journal {
        }
    }
    Tab {
        title: qsTr("Progress")
        Page {
        }
    }
}

