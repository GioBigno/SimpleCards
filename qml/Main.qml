import QtQuick
import QtQuick.Controls
import simplecardsModule

ApplicationWindow {
	width: 640
	height: 480
	visible: true
	title: qsTr("Hello World")

	ListView{
		anchors.fill: parent
		model: DeckManager.availableDecks
	
		delegate: Frame{
			Label {
            		text: modelData
            		font.pixelSize: 20
        		}
		}
	}
}
