import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ColumnLayout{
	id: editingView

	required property var onBackBtn
	required property var onDeleteCard

	property var deckmodel: DeckUtils.deckModel
	required property int currentIdx
	
	MessageDialog{
		id: deleteCardDialog
		buttons: MessageDialog.Yes | MessageDialog.No
		text: "Delete the card?"
		onButtonClicked: function (button, role){
        		switch(button){
        			case MessageDialog.Yes:
            			onDeleteCard(currentIdx)
            		break;
			}
    		}
	}

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
				onClicked: {
					if(card.modifiedQuestion === "" && card.modifiedAnswer === ""){
						onDeleteCard(currentIdx)
					}else{
						onBackBtn()
					}
				}
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
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
			RoundButton{
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter
				anchors.rightMargin: 10
				icon.source: "/qt/qml/simplecardsModule/assets/trash.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				icon.width: 25
				icon.height: 25
				flat: true
				display: AbstractButton.IconOnly
				onClicked: {
					if(AppConfig.confirmDeleteCard)
						deleteCardDialog.open()	
					else
						onDeleteCard(currentIdx)
				}
				HoverHandler {cursorShape: Qt.PointingHandCursor}
				ToolTip.visible: hovered
    				ToolTip.text: qsTr("Delete card")
				ToolTip.delay: 1000
			}
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
