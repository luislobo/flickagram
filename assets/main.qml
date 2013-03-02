import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    property Page imageDetailPage
    firstPage: Page {
        id: pgMain
        content: Container {
            layout: DockLayout {
            }
            opacity: 1.0
            Label {
                id: initmsg
                objectName: "initmsg"
                multiline: true
                text: "Welcome to Flickagram! This first version allows you to show Interesting Photos from Flickr, and set them as backgrounds. Press Refresh to get the images!"
            }
            ListView {
                id: listView
                objectName: "listView"
                layout: GridListLayout {
                    columnCount: 3
                    cellAspectRatio: 1.0
                    horizontalCellSpacing: 6.0
                    verticalCellSpacing: 6.0
                    spacingBeforeHeader: 8.0
                    spacingAfterHeader: 6.0
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        // list item GUI appearence component is defined in external MyListItem.qml file
                        MyListItem {
                        }
                    }
                ]
                // When an item is selected, update the text in the TextField
                // to display the status of the new item
                onTriggered: {
                    console.log("selected_index: " + indexPath)
                    var selectedItem = dataModel.data(indexPath);
                    setDetailItem(selectedItem)
                }
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                opacity: 1.0
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
        actions: [
            // An action item that calls the C++ function that retrieves
            // the contact list
            ActionItem {
                title: qsTr("Refresh")
                imageSource: "asset:///images/ic_refresh.png"
                onTriggered: {
                    initmsg.visible = false
                    app.initiateRequest()
                }
                ActionBar.placement: ActionBarPlacement.OnBar
            }
        ]
    }
    attachedObjects: [
        ComponentDefinition {
            id: imageDetailPageDefinition
            source: "ImageDetail.qml"
        }
    ]
    function setDetailItem(selectedItem) {       
        var imageDetailPage = getImageDetailPage();
        console.debug("pushing detail " + imageDetailPage)
        var patt = /^http/;
        if (patt.test(selectedItem.url_l)) {
            imageDetailPage.imageUrl = selectedItem.url_l
        } else {
            imageDetailPage.imageUrl = selectedItem.url_m
        }
        navigationPane.push(imageDetailPage);
    }
    function getImageDetailPage() {
        if (! imageDetailPage) {
            imageDetailPage = imageDetailPageDefinition.createObject();
        }
        return imageDetailPage;
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("NavigationPane - onCreationCompleted()")

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
