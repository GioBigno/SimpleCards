import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: testingView

	Pane{
		Layout.alignment: Qt.AlignHCenter
		Label{
			text: "Testing"
			font.bold: true
			font.pointSize: 30
		}
	}
	
	Card{
		id: card
		Layout.alignment: Qt.AlignCenter
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.maximumWidth: 600
		Layout.maximumHeight: 400
		Layout.margins: 15

		question: DeckManager.currentCard["q"]
		answer: DeckManager.currentCard["a"]

		onQuestionChanged: {
			card.revealed = false
		}
	}

	RowLayout{
		Button{
			text: "reveal"
			onClicked: card.reveal();
		}
		Button{
			text: "next"
			onClicked: DeckManager.goNext()
		}
	}
}
