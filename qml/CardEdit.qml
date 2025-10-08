import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
	id: cardRectangle

	property string question: ""
	property string answer: ""
	required property var updateQuestion
	required property var updateAnswer

	property bool revealed: false

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	radius: 10
	color: mypalette.base
	clip: true

	ColumnLayout{
		id: cardContent
		anchors.fill: parent
		anchors.margins: 5

		ScrollView{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.alignment: Qt.AlignCenter
			TextArea{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				antialiasing: true
				text: question
				font.pointSize: 15
				wrapMode: Text.Wrap
				onEditingFinished: {updateQuestion(text)}
			}
		}
		
		ScrollView{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.alignment: Qt.AlignCenter
			TextArea{
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				antialiasing: true
				text: answer
				font.pointSize: 15
				wrapMode: Text.Wrap
				onEditingFinished: {updateAnswer(text)}
			}
		}
	}
}
