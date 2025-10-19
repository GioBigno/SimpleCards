import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
	id: root

	property bool startingChecked: false
	property bool checked
	required property string text
	property string toolTipText: ""

	implicitWidth: rowlayout.width
	implicitHeight: rowlayout.height

	RowLayout{
		id: rowlayout
		CheckBox{
			Component.onCompleted: {checked = root.startingChecked}
			onCheckedChanged: {root.checked = checked}
		}

		Label{
			text: root.text
			font.pointSize: 14
		}

		Info{
			Layout.alignment: Qt.AlignVCenter
			Layout.leftMargin: 10
			Layout.preferredWidth: 15
			Layout.preferredHeight: 15
			visible: root.toolTipText !== ""
			toolTipText: root.toolTipText
		}
	}
}
