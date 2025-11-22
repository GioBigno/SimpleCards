import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item{
	id: root

	required property string labelText
	property url displayPath
	property url selectedPath
	property string prettyUrl

	onDisplayPathChanged: {
		var urlObj = new URL(displayPath);
		prettyUrl = decodeURIComponent(urlObj.pathname);
	}

	FolderDialog{
		id: folderDialog
		currentFolder: displayPath
		onAccepted: {selectedPath = selectedFolder}
	}
	
	implicitWidth: rowlayout.width
	implicitHeight: rowlayout.height

	RowLayout{
		id: rowlayout
		
		Label{
			Layout.alignment: Qt.AlignVCenter
			text: labelText
			font.pointSize: 14
		}
		
		Button{
			id: dirBtn
			text: prettyUrl
			onHoveredChanged:{ contentItem.color = hovered ? mypalette.highlight : mypalette.buttonText}
			HoverHandler {cursorShape: Qt.PointingHandCursor}
			background: Rectangle {
      	   		color: dirBtn.hovered ? mypalette.alternateBase : mypalette.button
      			radius: 4
      		}
			onClicked: {folderDialog.open()}
		}
	}
}
