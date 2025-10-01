import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import simplecardsModule

ApplicationWindow {
	id: root
	width: 800
	height: 550
	visible: true
	title: qsTr("Simple cards")

	font.family: "Space Mono"

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	StackView {
      	id: stack
		initialItem: decksList
		anchors.fill: parent
	}

	Component{
		id: decksList
		DecksListView{
			onOpen: (filePath) => {	
				//const view = testingComponent.createObject(root);
				//view.loadDeck(filePath);
				DeckUtils.loadDeck(filePath, DeckMode.Test)
				stack.push(testingComponent);		
			}
			onEdit: (filePath) => {
			}
			onStats: (filePath) => {
			}
		}
	}

	Component{
		id: testingComponent
		TestingView{
			onBackBtn: function() {
				stack.pop()
			}
		}
	}
}
