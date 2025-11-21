import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: deckListView
	required property var onOpen
	required property var onEdit
	required property var onStats
	required property var onSettings

	RowLayout{
		Layout.preferredWidth: parent.width
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

		Item{
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
			RoundButton{
				id: backBtn
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				anchors.leftMargin: 10
				icon.source: "/qt/qml/simplecardsModule/assets/settings.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				icon.width: 20
				icon.height: 20
				flat: true
				display: AbstractButton.IconOnly
				onClicked: {onSettings()}
				HoverHandler {cursorShape: Qt.PointingHandCursor}
			}
		}
		Label{
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			text: qsTr("Decks")
			font.bold: true
			font.pointSize: 30
		}
		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
		}
	}

	DecksList{
		id: deckList
		Layout.fillWidth: true
		Layout.maximumWidth: 750
		Layout.preferredHeight: contentHeight
		Layout.alignment: Qt.AlignHCenter
		clip: true

		model: DeckUtils.availableDecks

		onOpen: (deckFilePath) => {
			deckListView.onOpen(deckFilePath)
		}
		onEdit: (deckFilePath) => {
			deckListView.onEdit(deckFilePath)
		}
		onStats: (deckFilePath) => {
			deckListView.onStats(deckFilePath)
		}
	}

	Item{
		Layout.fillWidth: true
		Layout.maximumWidth: 750
		Layout.preferredHeight: newDeckBtn.height
		Layout.alignment: Qt.AlignHCenter
		Layout.rightMargin: 25

		Button{
			id: newDeckBtn
			anchors.right: parent.right
			text: qsTr("New deck")
			font.pointSize: 12
			onHoveredChanged:{ contentItem.color = hovered ? mypalette.highlight : mypalette.buttonText}
			HoverHandler {cursorShape: Qt.PointingHandCursor}
			background: Rectangle {
            		color: newDeckBtn.hovered ? mypalette.alternateBase : mypalette.button
            		radius: 4
        		}
			onClicked: {
				deckListView.onEdit(DeckUtils.createEmptyDeckFile())
			}
		}
	}

	Item{
		Layout.fillHeight: true
		Layout.fillWidth: true
	}
}
