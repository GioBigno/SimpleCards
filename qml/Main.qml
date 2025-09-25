import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import simplecardsModule

ApplicationWindow {
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
			onOpen: (idx) => {
				if(!DeckManager.loadDeck(idx))
					console.log("errore nel caricamento del mazzo")
		
				stack.push(testingView)
			}
			onEdit: (idx) => {
			}
			onStats: (idx) => {
			}
		}
	}

	Component{
		id: testingView
		TestingView{
			deckName: DeckManager.currentDeckName
			onBackBtn: function() {
				stack.pop()
			}
		}
	}
}
