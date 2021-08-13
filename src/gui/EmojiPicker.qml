/*
 * Copyright (C) by Felix Weilbach <felix.weilbach@nextcloud.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import com.nextcloud.desktopclient 1.0 as NC

ColumnLayout {
    id: picker

    property string emojiCategory: "history"

    signal chosen(string emoji)

    spacing: 0

    FontMetrics {
        id: metrics
    }

    ListView {
        id: headerLayout
        Layout.fillWidth: true
        implicitWidth: contentItem.childrenRect.width
        implicitHeight: contentItem.childrenRect.height

        boundsBehavior: Flickable.DragOverBounds
        clip: true
        orientation: ListView.Horizontal

        model: ListModel {
            ListElement { label: "⌛️"; category: "history" }
            ListElement { label: "😏"; category: "people" }
            ListElement { label: "🌲"; category: "nature" }
            ListElement { label: "🍛"; category: "food" }
            ListElement { label: "🚁"; category: "activity" }
            ListElement { label: "🚅"; category: "travel" }
            ListElement { label: "💡"; category: "objects" }
            ListElement { label: "🔣"; category: "symbols" }
            ListElement { label: "🏁"; category: "flags" }
        }

        delegate: ItemDelegate {
            id: del

            required property string label
            required property string category

            width: metrics.height * 2
            height: metrics.height * 2

            Text {
                id: emoji
                anchors.centerIn: parent
                text: label
            }

            Rectangle {
                anchors.bottom: parent.bottom

                width: parent.width
                height: 2

                visible: ListView.isCurrentItem

                color: "grey"
            }

            onClicked: {
                emojiCategory = category
            }
        }

    }

    Rectangle {
        height: 1
        Layout.fillWidth: true
        color: "grey"
    }

    GridView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredHeight: metrics.height * 8

        cellWidth: metrics.height * 2
        cellHeight: metrics.height * 2

        boundsBehavior: Flickable.DragOverBounds
        clip: true

        model: {
            switch (emojiCategory) {
            case "history":
                return NC.EmojiModel.history
            case "people":
                return NC.EmojiModel.people
            case "nature":
                return NC.EmojiModel.nature
            case "food":
                return NC.EmojiModel.food
            case "activity":
                return NC.EmojiModel.activity
            case "travel":
                return NC.EmojiModel.travel
            case "objects":
                return NC.EmojiModel.objects
            case "symbols":
                return NC.EmojiModel.symbols
            case "flags":
                return NC.EmojiModel.flags
            }
            return null
        }

        delegate: ItemDelegate {

            width: metrics.height * 2
            height: metrics.height * 2

            Text {
                anchors.centerIn: parent
                text: modelData.unicode
            }

            onClicked: {
                chosen(modelData.unicode);
                NC.EmojiModel.emojiUsed(modelData);
            }
        }

        ScrollBar.vertical: ScrollBar {}
        
    }

}
