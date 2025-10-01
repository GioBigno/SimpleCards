import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: testingView

	required property var onBackBtn

	property var deckmodel: DeckUtils.deckModel
	property int currentIdx: 0
	property bool deckFinished: deckmodel.size == 0
	property int answered: 0

	RowLayout{
		Layout.preferredWidth: parent.width
		Layout.alignment: Qt.AlignHCenter

		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
			Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
			RoundButton{
				id: backBtn
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				anchors.leftMargin: 10
				icon.source: "/qt/qml/simplecardsModule/assets/back.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				icon.width: 20
				icon.height: 20
				flat: true
				display: AbstractButton.IconOnly
				onClicked: onBackBtn()
				HoverHandler {cursorShape: Qt.PointingHandCursor}
			}
		}

		Label{
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			text: deckmodel.deckName	
			font.bold: true
			font.pointSize: 30
		}

		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
			Label{
				anchors.right: parent.right
                		anchors.verticalCenter: parent.verticalCenter
				anchors.rightMargin: 10
				text: answered + "/" + deckmodel.size
				font.bold: true
				font.pointSize: 20
			}
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
			if(currentIdx+1 >= deckmodel.size)
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
			onClicked: {
				card.triggerWrong()
				answered++
			}
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
			onClicked: {
				card.triggerCorrect()
				answered++
			}
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
