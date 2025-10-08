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
				DeckUtils.loadDeck(filePath, DeckMode.Test)
				stack.push(testingComponent, {"filePath": filePath});		
			}
			onEdit: (filePath) => {
				DeckUtils.loadDeck(filePath, DeckMode.Edit)
				stack.push(editingComponent, {"filePath": filePath});		
			}
			onStats: (filePath) => {
			}
		}
	}

	Component{
		id: testingComponent
		TestingView{
			id: testingView
			onBackBtn: function() {
				stack.pop()
				DeckUtils.saveDeck(testingView.filePath);
			}
		}
	}

	Component{
		id: editingComponent
		EditingView{
			id: editingView
			onBackBtn: function() {
				stack.pop()
				DeckUtils.saveDeck(editingView.filePath);
			}
		}
	}
}
