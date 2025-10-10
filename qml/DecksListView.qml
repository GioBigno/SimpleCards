import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: deckListView
	required property var onOpen
	required property var onEdit
	required property var onStats

	Pane{
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		Label{
			text: "Decks"
			font.bold: true
			font.pointSize: 30
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

		onOpen: (filePath) => {
			deckListView.onOpen(filePath)
		}
		onEdit: (filePath) => {
			deckListView.onEdit(filePath)
		}
		onStats: (filePath) => {
			deckListView.onStats(filePath)
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
            		color: newDeckBtn.hovered ? mypalette.alternateBase : mypalette.base
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
