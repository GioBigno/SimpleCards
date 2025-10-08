import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle{
	id: cardRectangle

	property string question: ""
	property string answer: ""

	property bool revealed: false
	property int originalX: x

	signal animationFinished()

	function triggerCorrect(){
		cardRectangle.border.color = "green"
		correctAnimation.start()
	}
	
	function triggerWrong(){
		cardRectangle.border.color = "red"
		wrongAnimation.start()
	}

	onQuestionChanged: {
		//if(revealed === true)
		//	revealed = false
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
		id: cardContent
		anchors.fill: parent
		anchors.margins: 5

		Label{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.alignment: Qt.AlignCenter
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			font.pointSize: 15
			text: question
			wrapMode: Text.Wrap
			elide: Text.ElideRight
		}
		Label{
			Layout.fillWidth: true
			Layout.fillHeight: true
			Layout.alignment: Qt.AlignCenter
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			
			visible: revealed
			font.pointSize: 15
			text: answer
			wrapMode: Text.Wrap
			elide: Text.ElideRight
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
