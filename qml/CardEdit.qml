import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
	id: cardRectangle

	property string question: ""
	property string answer: ""
	required property var updateQuestion
	required property var updateAnswer
	property string modifiedQuestion: questionTextArea.text
	property string modifiedAnswer: answerTextArea.text

	property bool revealed: false

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	radius: 10
	color: mypalette.alternateBase
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
				id: questionTextArea
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				antialiasing: true
				placeholderText: "Insert question..."
				placeholderTextColor: mypalette.buttonText
				text: question
				font.pointSize: 15
				wrapMode: Text.Wrap
				textFormat: TextEdit.PlainText
				onEditingFinished: {updateQuestion(text)}
			}
		}
		
		ScrollView{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.alignment: Qt.AlignCenter
			TextArea{
				id: answerTextArea
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				antialiasing: true
				placeholderText: "Insert answer..."
				placeholderTextColor: mypalette.buttonText
				text: answer
				font.pointSize: 15
				wrapMode: Text.Wrap
				textFormat: TextEdit.PlainText
				onEditingFinished: {updateAnswer(text)}
			}
		}
	}
}
