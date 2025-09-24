import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
	id: cardRectangle
	
	required property string question
	required property string answer

	property bool revealed: false

	function reveal(){
		revealed = !revealed;
	}
	
	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	radius: 10
	color: mypalette.base
	clip: true

	ColumnLayout{
		anchors.fill: parent
		anchors.margins: 5

		Label{
			Layout.alignment: Qt.AlignHCenter
			font.pointSize: 15
			text: question
			wrapMode: Text.Wrap
		}
		Label{
			Layout.alignment: Qt.AlignHCenter
			visible: revealed
			font.pointSize: 15
			text: answer
			wrapMode: Text.Wrap
			clip: true
		}
	}
}
