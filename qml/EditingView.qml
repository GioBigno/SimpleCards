import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: testingView

	required property var onBackBtn
	required property string filePath

	property var deckmodel: DeckUtils.deckModel
	required property int currentIdx

	RowLayout{
		Layout.preferredWidth: parent.width
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

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
				onClicked: {onBackBtn()}
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
		}
	}
	
	CardEdit{
		id: card
		Layout.alignment: Qt.AlignCenter
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.maximumWidth: 600
		Layout.maximumHeight: 400
		Layout.margins: 15

		question: deckmodel.getCardAt(currentIdx).question
		answer: deckmodel.getCardAt(currentIdx).answer
		revealed: true

		updateQuestion: function(text){deckmodel.setQuestionAt(currentIdx, text)}
		updateAnswer: function(text){deckmodel.setAnswerAt(currentIdx, text)}
	}
}
