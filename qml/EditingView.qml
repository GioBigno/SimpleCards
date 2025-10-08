import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.VectorImage

ColumnLayout{
	id: testingView

	required property var onBackBtn
	required property string filePath

	property var deckmodel: DeckUtils.deckModel
	property int currentIdx: 0

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
		}
	}

	/*
	GridView{
		id: gridview
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.margins: 5
			cellWidth: 400
			cellHeight: 200

			model: deckmodel

			delegate: Card{
				id: card
				width: gridview.cellWidth - 5
				height: gridview.cellHeight - 5

				question: model.question
				answer: model.answer
				revealed: true
			}
		}
		*/	

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
			alignItems: FlexboxLayout.AlignCenter
			justifyContent: FlexboxLayout.JustifyCenter

			Repeater{
				model: deckmodel
				Card{
					width: 300
					height: 200
					implicitWidth: width
					implicitHeight: height
					question: model.question
					answer: model.answer
					revealed: true
				}
			}
		}
	}
}
