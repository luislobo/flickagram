/*
 * Copyright (c) 2012 SSP Europe GmbH, Munich
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */import bb.cascades 1.0
import "../common"

/*
 * Preferences and Options set by the user
 * 
 * Author: Ekkehard Gentz (ekke), Rosenheim, Germany
 * 
 */

// we're using a NavigationPane to be able to push/pop Pages with special editors like language selection

NavigationPane {
    // SIGNAL
    signal done(bool ok)
    id: navigationPane
    attachedObjects: [
        // special editor to select the Language
        LanguageSelection {
            id: languageSelection
            paneProperties: NavigationPaneProperties {
                backButton: ActionItem {
                    onTriggered: {
                        navigationPane.pop();
                    }
                }
            }
        }
    ]
    Page {

        // seems not to work
        resizeBehavior: PageResizeBehavior.Resize
        titleBar: TitleBar {
            id: theBar
            title: qsTr("Preferences") + Retranslate.onLanguageChanged
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLanguageChanged
                onTriggered: {
                    navigationPane.done(false);
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Save") + Retranslate.onLanguageChanged
                onTriggered: {
                    navigationPane.done(true);
                }
            }
        }
        //
        Container {
            layout: DockLayout {
            }
            id: mainContainer
            //ScrollView {
            Container {
                id: fieldsContainer
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                topPadding: 25
                leftPadding: 25
                rightPadding: 25
                // Label displays the current Locale
                Label {
                    id: languageLabel
                    text: qsTr("Language")  + Retranslate.onLanguageChanged
                    leftMargin: 20
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                    }
                    // open the language editor
                    onTouch: {
                        languageSelection.setLanguage(ods.getCurrentLocale())
                        navigationPane.push(languageSelection)
                    }
                }
                Divider {
                    topMargin: 20
                    bottomMargin: 20
                }
                Label {
                    id: versionLabel
                    visible: false
                    text: qsTr("Version %1").arg("1.0.4") + Retranslate.onLanguageChanged
                    leftMargin: 20
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                    }
                }
            }
            //} // ScrollView
        }
    }
    // SLOTS
    function newLanguage(locale) {
        console.debug("new locale: " + locale)
		if (locale == "es") {
            languageLabel.text = qsTr("Spanish") + Retranslate.onLanguageChanged
        } else {
            languageLabel.text = qsTr("English") + Retranslate.onLanguageChanged
        }
    }
    onCreationCompleted: {
    }
}
