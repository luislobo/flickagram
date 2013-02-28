import bb.cascades 1.0
import org.labsquare 1.0

Page {
    property string imageUrl
    // page with a picture detail
    id: imageDetail
    actions: [
        ActionItem {
            title: qsTr("Set as Wallpaper")
            imageSource: "asset:///images/ic_set_wallpaper.png"
            onTriggered: {
                app.downloadImageInitiateRequest(imageDetail.imageUrl)
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                // define what happens when back button is pressed here
                // in this case is closed the detail page
                navigationPane.pop()
            }
        }
    }
    Container {
        background: Color.Black
        ProgressIndicator {
            value: img.loading
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            visible: (img.loading < 1.0)
        }
        WebImageView {
            id: img
            url: imageDetail.imageUrl
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            scalingMethod: ScalingMethod.AspectFill
            visible: (img.loading == 1.0)
            leftMargin: 10
            rightMargin: 10
            topMargin: 10
            bottomMargin: 10
        }
    }
}
