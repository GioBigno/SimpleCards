import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.VectorImage

ColumnLayout{
	id: editingListView

	required property var onBackBtn
	required property var onSelected
	required property string filePath

	property var deckmodel: DeckUtils.deckModel
	property int currentIdx: 0
	
	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
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
				onClicked: onBackBtn()
				HoverHandler {cursorShape: Qt.PointingHandCursor}
			}
		}
		TextInput{
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			text: deckmodel.deckName	
			font.family: "Space Mono"
			font.bold: true
			font.pointSize: 30
			onEditingFinished: {
				deckmodel.changeTitle(text)
				editingListView.filePath = DeckUtils.changeFileName(filePath, text)
			}
		}
		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
		}
	}

	Flickable{
		id: cardGridFlickable
		Layout.fillWidth: true
		Layout.fillHeight: true

		flickableDirection: Flickable.VerticalFlick
		clip: true
		contentWidth: width
		contentHeight: cardGrid.height 

		FlexboxLayout{
			id: cardGrid
			width: cardGridFlickable.contentWidth
			gap: 10
			wrap: FlexboxLayout.Wrap
			justifyContent: FlexboxLayout.JustifyCenter

			Rectangle{
				Layout.preferredWidth: 300
				Layout.preferredHeight: 200
				color: mypalette.alternateBase
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
					Layout.preferredWidth: 300
					Layout.preferredHeight: 200
					question: model.question
					answer: model.answer
					revealed: false
					MouseArea{
						anchors.fill: parent
						hoverEnabled: true
						onClicked: {onSelected(model.index)}
						HoverHandler{
							cursorShape: Qt.PointingHandCursor
							onHoveredChanged: {
								card.color = hovered ? mypalette.alternateBase : mypalette.base
							}
						}
					}
				}
			}	
		}
	}
	
}
