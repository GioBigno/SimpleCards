import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
	id: cardRectangle
	
	required property string question
	required property string answer
	property bool revealed: false
	property int originalX: x

	signal animationFinished()

	function reveal(){
		revealed = !revealed;
	}

	function triggerCorrect(){
		cardRectangle.border.color = "green"
		correctAnimation.start()
	}
	
	function triggerWrong(){
		cardRectangle.border.color = "red"
		wrongAnimation.start()
	}

	onQuestionChanged: {
		cardRectangle.revealed = false
		border.width = 0
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
	
	SequentialAnimation{
		id: correctAnimation
		NumberAnimation{target: cardRectangle; property: "border.width"; from: 0; to: 5; easing.type: Easing.OutBack}
		PauseAnimation{duration: 500}
		NumberAnimation{target: cardRectangle; property: "border.width"; from: 5; to: 0; easing.type: Easing.InOutExpo; duration: 400}
		onFinished: animationFinished()
	}

	ParallelAnimation{
		id: wrongAnimation
		SequentialAnimation{
			NumberAnimation{target: cardRectangle; property: "border.width"; from: 0; to: 5; easing.type: Easing.OutBack}
			PauseAnimation{duration: 500}
			NumberAnimation{target: cardRectangle; property: "border.width"; from: 5; to: 0; easing.type: Easing.InOutExpo; duration: 400}
		}
		SequentialAnimation{
           		PropertyAnimation {target: cardRectangle; property: "x"; to: originalX - 10; duration: 90 }
            	PropertyAnimation {target: cardRectangle; property: "x"; to: originalX + 10; duration: 90 }
            	PropertyAnimation {target: cardRectangle; property: "x"; to: originalX - 8; duration: 90 }
            	PropertyAnimation {target: cardRectangle; property: "x"; to: originalX + 8; duration: 90 }
            	PropertyAnimation {target: cardRectangle; property: "x"; to: originalX; duration: 90 }
		}
		onFinished: animationFinished()
	}
}
