import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: testingView

	required property var onBackBtn

	property var deckmodel: DeckUtils.deckModel

	RowLayout{
		Layout.preferredWidth: parent.width
		Layout.alignment: Qt.AlignHCenter

		RoundButton{
			id: backBtn
			Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
			Layout.leftMargin: 10
			icon.source: "/qt/qml/simplecardsModule/assets/back.svg"
			icon.color: hovered ? mypalette.highlight : mypalette.buttonText
			icon.width: 20
			icon.height: 20
			flat: true
			display: AbstractButton.IconOnly
			onClicked: onBackBtn()
		}

		Pane{
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			Label{
				text: deckmodel.deckName
				font.bold: true
				font.pointSize: 30
			}
		}

		Item{
			Layout.preferredWidth: backBtn.width
			Layout.rightMargin: 10
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

		//currentCard: deckManager.currentCard
		question: "test q"
		answer: "test a"

		onAnimationFinished: {
			//DeckManager.goNext()
		}
	}
	
	RowLayout{
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
		Layout.bottomMargin: 15
		spacing: 50
		
		RoundButton{
			visible: card.revealed
			Layout.preferredWidth: height
			Layout.alignment: Qt.AlignVCenter
			icon.source: "/qt/qml/simplecardsModule/assets/wrong.svg"
			icon.color: hovered ? mypalette.highlight : mypalette.buttonText
			icon.width: 40
			icon.height: 40
			flat: true
			display: AbstractButton.IconOnly
			onClicked: {card.triggerWrong()}
		}
		RoundButton{
			visible: card.revealed
			Layout.preferredWidth: height
			Layout.alignment: Qt.AlignVCenter
			icon.source: "/qt/qml/simplecardsModule/assets/correct.svg"
			icon.color: hovered ? mypalette.highlight : mypalette.buttonText
			icon.width: 40
			icon.height: 40
			flat: true
			display: AbstractButton.IconOnly
			onClicked: {card.triggerCorrect()}
		}
		RoundButton{
			visible: !card.revealed
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			Layout.preferredWidth: height
			icon.source: "/qt/qml/simplecardsModule/assets/reveal.svg"
			icon.color: hovered ? mypalette.highlight : mypalette.buttonText
			icon.width: 40
			icon.height: 40
			flat: true
			display: AbstractButton.IconOnly
			onClicked: card.reveal();
		}
	}
}
