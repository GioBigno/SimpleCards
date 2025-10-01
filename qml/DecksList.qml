import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ListView{

	required property var onOpen
	required property var onEdit
	required property var onStats

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	spacing: 2

	delegate: Rectangle{
		color: index%2 == 0 ? mypalette.base : mypalette.alternateBase
		height: label.height
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		RowLayout{
			anchors.fill: parent
			anchors.rightMargin: 5
			spacing: 10

			Label{
				Layout.alignment: Qt.AlignLeft
				id: label
				padding: 5
				text: modelData.base_name
     				font.pixelSize: 20
			}

			Item {Layout.fillWidth: true}

			RoundButton{
				Layout.alignment: Qt.AlignRight
				Layout.preferredWidth: height
				icon.source: "/qt/qml/simplecardsModule/assets/play.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				flat: true
				display: AbstractButton.IconOnly
				onClicked: onOpen(modelData.file_path)
			}
			RoundButton{
				Layout.alignment: Qt.AlignRight
				Layout.preferredWidth: height
				icon.source: "/qt/qml/simplecardsModule/assets/edit.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				flat: true
				display: AbstractButton.IconOnly
				onClicked: onEdit(modelData.file_path)
			}
			RoundButton{
				Layout.alignment: Qt.AlignRight
				Layout.preferredWidth: height
				icon.source: "/qt/qml/simplecardsModule/assets/stats.svg"
				icon.color: hovered ? mypalette.highlight : mypalette.buttonText
				flat: true
				display: AbstractButton.IconOnly
				onClicked: onStats(modelData.file_path)
			}
		}
	}
}
