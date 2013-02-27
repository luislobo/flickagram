import bb.cascades 1.0

Page {
    actions: [
        // An action item that calls the C++ function that retrieves
        // the contact list
        ActionItem {
            title: "Refresh"
            onTriggered: app.initiateRequest()
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
    content: Container {
        layout: DockLayout {
        }

        // A list that has two list item components, one for a header
        // and one for contact names. The list has an object name so
        // that we can set the data model from C++.
        ListView {
            objectName: "list"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            layout: FlowListLayout {
            }
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                        text: qsTr("Interesting photos")
                    }
                },
                ListItemComponent {
                    type: "item"
                    Container {
                        Label {
                            text: ListItemData.id
                        }
                        Label {
                            text: ListItemData.title
                        }
                        Label {
                            text: ListItemData.url_t
                        }
                    }
                }
            ]
        }
        // The activity indicator has an object name set so that
        // we can start and stop it from C++
        ActivityIndicator {
            objectName: "indicator"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 200
            preferredHeight: 200
        }
    }
}
