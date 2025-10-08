import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: deckListView
	required property var onOpen
	required property var onEdit
	required property var onStats

	Pane{
		Layout.alignment: Qt.AlignHCenter
		Label{
			text: "Decks"
			font.bold: true
			font.pointSize: 30
		}
	}

	DecksList{
		Layout.fillWidth: true
		Layout.maximumWidth: 750
		Layout.fillHeight: true
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
}
