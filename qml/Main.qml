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
				stack.push(testingComponent);		
			}
			onEdit: (filePath) => {
				DeckUtils.loadDeck(filePath, DeckMode.Edit)
				stack.push(editingListComponent);		
			}
			onStats: (filePath) => {
				DeckUtils.loadDeck(filePath, DeckMode.Edit)
				stack.push(statsComponent);		
			}
			onSettings: () => {
				stack.push(settingsComponent)
			}
		}
	}

	Component{
		id: testingComponent
		TestingView{
			id: testingView
			onBackBtn: function() {
				stack.pop()
				DeckUtils.saveLoadedDeck();
			}
		}
	}

	Component{
		id: editingListComponent
		EditingListView{
			id: editingListView
			onBackBtn: function() {
				stack.pop()
				DeckUtils.saveLoadedDeck();
			}
			onSelected: function(idx){
				stack.push(editingComponent, {"currentIdx": idx})
			}
			onDeleteDeck: function(){
				stack.pop()
				DeckUtils.deleteLoadedDeck()
			}
		}
	}
	
	Component{
		id: editingComponent
		EditingView{
			id: editingView
			onBackBtn: function(){
				stack.pop()
			}
			onDeleteCard: function(){
				stack.pop()
			}
		}
	}

	Component{
		id: statsComponent
		StatsView{
			id: statsView
			onBackBtn: function(){
				stack.pop()
				DeckUtils.saveLoadedDeck();
			}
		}
	}
	
	Component{
		id: settingsComponent
		SettingsView{
			onBackBtn: function(){
				stack.pop()
			}
		}
	}
}
