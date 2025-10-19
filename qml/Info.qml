import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Item{
	required property string toolTipText

	SystemPalette{
		id: mypalette
		colorGroup: SystemPalette.Active
	}

	Image{
		id: infosvg
		anchors.fill: parent
		source: "/qt/qml/simplecardsModule/assets/info.svg"
		sourceSize.width: width
		sourceSize.height: height
		smooth: true
		fillMode: Image.PreserveAspectFit
		ColorOverlay{
			anchors.fill: parent
			source: infosvg
			color: mypalette.buttonText
		}
		HoverHandler {
			id: mouse
			cursorShape: Qt.WhatsThisCursor
		}
		ToolTip.visible: mouse.hovered
		ToolTip.text: toolTipText
		ToolTip.delay: 100
	}
}
