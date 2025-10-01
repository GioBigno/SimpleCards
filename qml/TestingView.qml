import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: testingView

	required property var onBackBtn

	property var deckmodel: DeckUtils.deckModel
	property int currentIdx: 0
	property bool deckFinished: deckmodel.size() == 0

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
			HoverHandler {cursorShape: Qt.PointingHandCursor}
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
		visible: !deckFinished
		Layout.alignment: Qt.AlignCenter
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.maximumWidth: 600
		Layout.maximumHeight: 400
		Layout.margins: 15

		question: deckFinished ? "" : deckmodel.getCardAt(currentIdx).question
		answer: deckFinished ? "" : deckmodel.getCardAt(currentIdx).answer

		onAnimationFinished: {
			//add logic to check if deck is finished
			if(currentIdx+1 >= deckmodel.size())
				deckFinished = true
			else
				currentIdx++
		}
	}

	Rectangle{
		visible: deckFinished
		Layout.alignment: Qt.AlignCenter
		Layout.fillWidth: true
		Layout.fillHeight: true

		color: "red"
	}
	
	RowLayout{
		visible: !deckFinished
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
			HoverHandler {cursorShape: Qt.PointingHandCursor}
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
			HoverHandler {cursorShape: Qt.PointingHandCursor}
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
			HoverHandler {cursorShape: Qt.PointingHandCursor}
		}
	}
}
