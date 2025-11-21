import QtQuick
//import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import simplecardsModule

ApplicationWindow {
	id: root
	width: 800
	height: 550
	visible: true
	title: qsTr("Simple cards")

	font.family: "Space Mono"

	Component.onCompleted: {
		if(!AppConfig.rememberSize)
			return;

		if(AppConfig.windowMaximized){
			visibility = Window.Maximized
		}else{
			let s = AppConfig.windowSize
			width = s.width
			height = s.height
		}
	}

	onClosing: {
		
		if(!AppConfig.rememberSize)
			return;

		if(visibility === Window.Maximized){
			AppConfig.windowMaximized = true;
		}else{
			AppConfig.windowMaximized = false;
			AppConfig.windowSize = Qt.size(width, height)
		}
	}

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	StackView {
      	id: stack
		initialItem: decksList
		anchors.fill: parent
	}

	MessageDialog{
		id: loadingErrorDialog
		property string filePathToDelete: ""
		buttons: MessageDialog.Yes | MessageDialog.No
		text: "Error opening deck, do you want to delete it?"
		onButtonClicked: function (button, role) {
        		switch (button) {
        			case MessageDialog.Yes:
            		DeckUtils.deleteDeck(filePathToDelete);
            		break;
			}
			filePathToDelete = ""
    		}
	}

	Component{
		id: decksList
		DecksListView{
			onOpen: (deckFilePath) => {	
				let error = DeckUtils.loadDeck(deckFilePath, DeckMode.Test)
				if(error===""){
					stack.push(testingComponent)
				}else{
					loadingErrorDialog.informativeText = "Details:" + error
					loadingErrorDialog.filePathToDelete = deckFilePath
					loadingErrorDialog.open()
				}
			}
			onEdit: (deckFilePath) => {
				let error = DeckUtils.loadDeck(deckFilePath, DeckMode.Edit)
				if(error===""){
					stack.push(editingListComponent)
				}else{
					loadingErrorDialog.informativeText = "Details: " + error
					loadingErrorDialog.filePathToDelete = deckFilePath
					loadingErrorDialog.open()
				}
			}
			onStats: (deckFilePath) => {
				let error = DeckUtils.loadDeck(deckFilePath, DeckMode.Edit)
				if(error===""){
					stack.push(statsComponent)
				}else{
					loadingErrorDialog.informativeText = "Details: " + error
					loadingErrorDialog.filePathToDelete = deckFilePath
					loadingErrorDialog.open()
				}
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
			onDeleteCard: function(idx){
				deckmodel.deleteCardAt(idx)
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
