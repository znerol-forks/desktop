import QtQuick 2.6
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import com.nextcloud.desktopclient 1.0 as NC

ColumnLayout {
    id: rootLayout
    anchors.fill: parent
    spacing: 0
    property NC.UserStatusDialogModel userStatusDialogModel

    implicitWidth: 450
    implicitHeight: 600

    Connections {
        target: userStatusDialogModel
        function onShowError() {
            errorDialog.open()
        }
    }

    Text {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        font.bold: true
        text: qsTr("Online status")
    }
        
    GridLayout {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop
        columns: 2
        rows: 2
        columnSpacing: 8
        rowSpacing: 8

        Button {
            Layout.fillWidth: true
            checked: NC.UserStatus.Online == userStatusDialogModel.onlineStatus
            checkable: true
            icon.source: userStatusDialogModel.onlineIcon
            icon.color: "transparent"
            text: qsTr("Online")
            onClicked: userStatusDialogModel.setOnlineStatus(NC.UserStatus.Online)
        }
        Button {
            Layout.fillWidth: true
            checked: NC.UserStatus.Away == userStatusDialogModel.onlineStatus
            checkable: true
            icon.source: userStatusDialogModel.awayIcon
            icon.color: "transparent"
            text: qsTr("Away")
            onClicked: userStatusDialogModel.setOnlineStatus(NC.UserStatus.Away)
        }
        Button {
            Layout.fillWidth: true
            checked: NC.UserStatus.DoNotDisturb == userStatusDialogModel.onlineStatus
            checkable: true
            icon.source: userStatusDialogModel.dndIcon
            icon.color: "transparent"
            text: qsTr("Do not disturb")
            onClicked: userStatusDialogModel.setOnlineStatus(NC.UserStatus.DoNotDisturb)
        }
        Button {
            Layout.fillWidth: true
            checked: NC.UserStatus.Invisible == userStatusDialogModel.onlineStatus
            checkable: true
            icon.source: userStatusDialogModel.invisibleIcon
            icon.color: "transparent"
            text: qsTr("Invisible")
            onClicked: userStatusDialogModel.setOnlineStatus(NC.UserStatus.Invisible)
        }
    }

    Text {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
        font.bold: true
        text: qsTr("Status message")
    }

    RowLayout {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop
        Layout.fillWidth: true

        Button {
            text: userStatusDialogModel.userStatusEmoji
            onClicked: emojiDialog.open()
        }

        Popup {
            id: emojiDialog
            padding: 0
            margins: 0

            anchors.centerIn: Overlay.overlay
            
            EmojiPicker {
                id: emojiPicker

                onChosen: {
                    userStatusDialogModel.userStatusEmoji = emoji
                    emojiDialog.close()
                }
            }
        }

        TextField {
            Layout.fillWidth: true
            placeholderText: qsTr("What is your Status?")
            text: userStatusDialogModel.userStatusMessage
            onEditingFinished: userStatusDialogModel.setUserStatusMessage(text)
        }
    }

    ColumnLayout {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop

        Repeater {
            model: userStatusDialogModel.predefinedStatusesCount

            Button {
                id: control
                Layout.fillWidth: true
                flat: !hovered
                hoverEnabled: true
                text: userStatusDialogModel.predefinedStatus(index).icon + " <b>" + userStatusDialogModel.predefinedStatus(index).message + "</b> - " + userStatusDialogModel.predefinedStatusClearAt(index)
                onClicked: userStatusDialogModel.setPredefinedStatus(index)
            }
        }
    }

   RowLayout {
       Layout.margins: 8
       Layout.alignment: Qt.AlignTop

       Text {
           text: qsTr("Clear status message after")
       }

       ComboBox {
           Layout.fillWidth: true
           model: userStatusDialogModel.clearAtStages
           displayText: userStatusDialogModel.clearAt
           onActivated: userStatusDialogModel.setClearAt(index)
       }
   } 

    RowLayout {
        Layout.margins: 8
        Layout.alignment: Qt.AlignTop
        
        Button {
            Layout.fillWidth: true
            text: qsTr("Clear status message")
            onClicked: userStatusDialogModel.clearUserStatus()
        }
        Button {
            highlighted: true
            Layout.fillWidth: true
            text: qsTr("Set status message")
            onClicked: userStatusDialogModel.setUserStatus()
        }
    }

    MessageDialog {
        id: errorDialog
        icon: StandardIcon.Critical
        title: qsTr("Set user status")
        text: userStatusDialogModel.errorMessage
        visible: false
    }
}
