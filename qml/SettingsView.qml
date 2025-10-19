import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout{
	id: settingsView

	required property var onBackBtn

	RowLayout{
		Layout.preferredWidth: parent.width
		Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

		Item{
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
			RoundButton{
				id: backBtn
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				anchors.leftMargin: 10
				icon.source: "/qt/qml/simplecardsModule/assets/back.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				icon.width: 20
				icon.height: 20
				flat: true
				display: AbstractButton.IconOnly
				onClicked: {onBackBtn()}
				HoverHandler {cursorShape: Qt.PointingHandCursor}
			}
		}
		Label{
			Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
			text: qsTr("Settings")
			font.bold: true
			font.pointSize: 30
		}
		Item{
			Layout.fillWidth: true
			Layout.preferredWidth: 1
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
		}
	}
	
	Label{
		Layout.fillWidth: true
		Layout.alignment: Qt.AlignLeft
		Layout.leftMargin: 30
		text: "Testing"
		font.bold: true
		font.pointSize: 18
	}
	
	CheckBoxEntry{
		Layout.fillWidth: true
		Layout.alignment: Qt.AlignLeft
		Layout.leftMargin: 30
		text: qsTr("\"hard\" button")
		toolTipText: qsTr("Enables a third button when selecting the result of retrieving.\nThis should be used for correct but very difficult retrieves")
		startingChecked: AppConfig.hardButton
		onCheckedChanged: {
			AppConfig.hardButton = checked
		}
	}
	
	CheckBoxEntry{
		Layout.fillWidth: true
		Layout.alignment: Qt.AlignLeft
		Layout.leftMargin: 30
		text: qsTr("card counter")
		toolTipText: qsTr("Shows the number of cards tested and the number of cards to test")
		startingChecked: AppConfig.cardsCounter
		onCheckedChanged: {
			AppConfig.cardsCounter = checked
		}
	}

	Item{
		Layout.fillHeight: true
	}
}
