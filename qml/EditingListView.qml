import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.VectorImage

ColumnLayout{
	id: editingListView

	required property var onBackBtn
	required property var onSelected
	required property var onDeleteDeck

	property var deckmodel: DeckUtils.deckModel
	property int currentIdx: 0

	property bool titleModified: false
	function changeTitle(text){
		if(!titleModified)
			return
		titleModified = false
		deckmodel.changeTitle(text)
		DeckUtils.changeLoadedDeckFileName(text)
	}

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	MessageDialog{
		id: invalidDeckNameDialog
		buttons: MessageDialog.Ok
		text: "Invalid deck's name."
	}

	MessageDialog{
		id: deleteDeckDialog
		buttons: MessageDialog.Yes | MessageDialog.No
		text: "Delete \"" + (deckmodel ? deckmodel.deckName : "") + "\"?"
		onButtonClicked: function (button, role){
        		switch(button){
        			case MessageDialog.Yes:
            			onDeleteDeck()
            		break;
			}
    		}
	}

	RowLayout{
		z: 1
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
					if(titleInput.text === ""){
						invalidDeckNameDialog.open()
					}else{
						changeTitle(titleInput.text)
						onBackBtn()
					}
				}
				HoverHandler {cursorShape: Qt.PointingHandCursor}
			}
		}
		TextInput{
			id: titleInput
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			text: deckmodel ? deckmodel.deckName : ""
			color: mypalette.buttonText
			font.family: "Space Mono"
			font.bold: true
			font.pointSize: 30
			onTextEdited: {
				titleModified = true
			}
			HoverHandler {cursorShape: Qt.IBeamCursor}
		}
		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
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
					changeTitle(titleInput.text)
					if(AppConfig.confirmDeleteDeck){
						deleteDeckDialog.open()
					}else{
						onDeleteDeck()
					}
				}
				HoverHandler {cursorShape: Qt.PointingHandCursor}
				ToolTip.visible: hovered
    				ToolTip.text: qsTr("Delete deck")
				ToolTip.delay: 1000
			}
		}
	}

	ScrollView{
		id: cardGridScrollView
		Layout.fillWidth: true
		Layout.fillHeight: true

		contentWidth: width

		readonly property int cardWidth: 300
		readonly property int cardHeight: 200
		
		onWidthChanged: {
			let perRow = Math.max(Math.trunc((width+cardGrid.gap) / (cardWidth+cardGrid.gap)), 1)
			let rows = Math.ceil((deckmodel.size+1)/perRow)
			contentHeight = rows * (cardHeight+cardGrid.gap)
		}

		FlexboxLayout{
			id: cardGrid
			width: cardGridScrollView.contentWidth
			gap: 10
			wrap: FlexboxLayout.Wrap
			justifyContent: FlexboxLayout.JustifyCenter

			Rectangle{
				Layout.preferredWidth: cardGridScrollView.cardWidth
				Layout.preferredHeight: cardGridScrollView.cardHeight
				color: mypalette.window
				radius: 10
				
				Label{
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					font.pointSize: 15
					text: "Add new card"
				}
				
				MouseArea{
					anchors.fill: parent
					hoverEnabled: true
					onClicked: {
						changeTitle(titleInput.text)
						onSelected(deckmodel.addCard())
					}
					HoverHandler{cursorShape: Qt.PointingHandCursor}
				}
			}

			Repeater{
				id: repeater
				model: deckmodel

				Card{
					id: card
					Layout.preferredWidth: cardGridScrollView.cardWidth
					Layout.preferredHeight: cardGridScrollView.cardHeight
					question: model.question
					answer: model.answer
					revealed: false
					MouseArea{
						anchors.fill: parent
						hoverEnabled: true
						onClicked: {
							changeTitle(titleInput.text)
							onSelected(model.index)
						}
						HoverHandler{
							cursorShape: Qt.PointingHandCursor
							onHoveredChanged: {
								card.color = hovered ? mypalette.highlight : mypalette.alternateBase
							}
						}
					}
				}
			}	
		}
	}
}
