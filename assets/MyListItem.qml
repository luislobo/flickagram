// Grid view with detail project template
import bb.cascades 1.0
import org.labsquare 1.0

// Item used by the list to present a small thumbnail image with text.

Container {
    ProgressIndicator {
        value: img.loading
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        visible: (img.loading < 1.0)
        toValue: 1.0
        opacity: 1.0
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
    /*Label {
        text: ListItemData.title
    }
    Divider {
        horizontalAlignment: HorizontalAlignment.Fill
    }*/
}
