import bb.cascades 1.0
import org.labsquare 1.0

NavigationPane {
    id: navigationPane
    //backButtonsVisible: false
    Page {
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
        content: Container {
            layout: DockLayout {
            }

            // A list that has two list item components, one for a header
            // and one for contact names. The list has an object name so
            // that we can set the data model from C++.
            Label {
                id: initmsg
                multiline:true
                text: "Welcome to Flickagram! This first version allows you to show Interesting Photos from Flickr, and set them as backgrounds. Press Refresh to get the images!"
            }
            ListView {
                objectName: "list"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            ProgressIndicator {
                                value: img.loading
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                visible: (img.loading < 1.0)
                            }
                            WebImageView {
                                id: img
                                url: ListItemData.url_m
                                horizontalAlignment: HorizontalAlignment.Left
                                verticalAlignment: VerticalAlignment.Center
                                scalingMethod: ScalingMethod.AspectFit
                                visible: (img.loading == 1.0)
                                leftMargin: 10
                                rightMargin: 10
                                topMargin: 10
                                bottomMargin: 10
                            }
                            Label {
                                text: ListItemData.title
                            }
                            Divider {
                                horizontalAlignment: HorizontalAlignment.Fill
                            }
                        }
                    }
                ]
                // When an item is selected, update the text in the TextField
                // to display the status of the new item
                onTriggered: {
                    var selectedItem = dataModel.data(indexPath);
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
                opacity: 1.0
                property Page imageDetailPage
                function getImageDetailPage() {
                    if (! imageDetailPage) {
                        imageDetailPage = imageDetailPageDefinition.createObject();
                    }
                    return imageDetailPage;
                }
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
        attachedObjects: [
            ComponentDefinition {
                id: imageDetailPageDefinition
                source: "ImageDetail.qml"
            }
        ]
    }
}
