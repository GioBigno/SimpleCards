import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtGraphs
import Qt5Compat.GraphicalEffects

ColumnLayout{
	id: statsView

	required property var onBackBtn

	property var deckmodel: DeckUtils.deckModel

	property int historySize: deckmodel.statsHistory.length
	readonly property color masteredColor: "limegreen"
	readonly property color learningColor: "gold"
	readonly property color newColor: "orangered"
	readonly property color totalColor: "deepskyblue"

	ListModel{id: legendModel}

	function averageMastery(){
		let size = deckmodel.size
		if (size === 0) return "Not enough data"
        
		let sum = 0
        	for(let i=0; i<size; i++)
            	sum += deckmodel.getCardAt(i)["mastery"]

		let average = sum / size

		if(average <= 2.0)
			return "New"
		if(average <= 2.08)
			return "Learning"
		if(average <= 2.13)
			return "Familiar"
		if(average <= 2.20)
			return "Stable"
		else
			return "Mastered"
	}
	
	function populateSeries() {
		if(historySize < 2)
			return

		totalSerie.clear()
		masteredSerie.clear()
		learningSerie.clear()
		newSerie.clear()

		let minDate = new Date()
		let maxDate = new Date()
		let maxY = 0;

		if(historySize > 0){
			minDate = deckmodel.statsHistory[0]["date"]
			maxDate = deckmodel.statsHistory[0]["date"]
		}

		for(let i=0; i<historySize; i++) {
            	let entry = deckmodel.statsHistory[i]
			let currDate = entry["date"]

			if(currDate < minDate)
				minDate = currDate
			else if(currDate > maxDate)
				maxDate = currDate

			let entry_new = entry["new"]
			let entry_learning = entry["learning"]
			let entry_mastered = entry["mastered"]

			let total_cards = entry_new + entry_learning + entry_mastered

			if(total_cards > maxY)
				maxY = total_cards * (7/6)

			totalSerie.append(currDate, total_cards);
			masteredSerie.append(currDate, entry_mastered);
			learningSerie.append(currDate, entry_learning);
			newSerie.append(currDate, entry_new);
		}

		xAxis.min = minDate;
		xAxis.max = maxDate;
		yAxis.max = maxY;
	}

	function populateLegend(){
		let newCards = 0
		let learningCards = 0
		let masteredCards = 0
		let totalCards = 0

		if(historySize > 0){
			let entry = deckmodel.statsHistory[historySize-1]
			newCards = entry["new"]
			learningCards = entry["learning"]
			masteredCards = entry["mastered"]
			totalCards = newCards+learningCards+masteredCards
		}
		
        	legendModel.append({"color": totalColor, "label": "Total cards: ", "numberText": totalCards})
        	legendModel.append({"color": masteredColor, "label": "Mastered cards: ", "numberText": masteredCards})
		legendModel.append({"color": learningColor, "label": "Learning cards: ", "numberText": learningCards})
		legendModel.append({"color": newColor, "label": "New cards: ", "numberText": newCards})
	}

	Component.onCompleted: {
		populateSeries()
		populateLegend()
	}

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
			text: deckmodel.deckName	
			font.bold: true
			font.pointSize: 30
		}
		Item{
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
			Image{
				id: infosvg
				anchors.right: parent.right
				anchors.verticalCenter: parent.verticalCenter
				anchors.rightMargin: 20
				width: 20
				height: 20
				source: "/qt/qml/simplecardsModule/assets/info.svg"
				sourceSize.width: 20
				sourceSize.height: 20
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
    				ToolTip.text: qsTr("Deck's mastery levels: \n -New \n -Learning \n -Familiar \n -Stable \n -Mastered \nCard mastery levels: \n -New \n -Learning \n -Mastered \nGraph: \n Cards mastery levels over time")
				ToolTip.delay: 100
			}
		}
	}
	
	Label{
		Layout.fillWidth: true
		Layout.alignment: Qt.AlignLeft
		Layout.leftMargin: 20
		text: "Deck's mastery: " + averageMastery()
		font.bold: true
		font.pointSize: 25
	}

	Repeater{
		model: legendModel

		RowLayout{
			Layout.fillWidth: true
			Layout.alignment: Qt.AlignLeft
			Layout.leftMargin: 20
			spacing: 10

			Image{
				id: labelsvg
				Layout.alignment: Qt.AlignVCenter
				Layout.preferredWidth: 18
				source: "/qt/qml/simplecardsModule/assets/label.svg"
				sourceSize.width: 18
				sourceSize.height: 18
				smooth: true
				fillMode: Image.PreserveAspectFit
				ColorOverlay{
					anchors.fill: parent
					source: labelsvg
					color: model.color
				}
			}
	
			Label{
				text: model.label + model.numberText
				font.pointSize: 17
			}
		}
	}

	GraphsView{
		id: graph
		Layout.fillWidth: true
		Layout.fillHeight: true
		Layout.leftMargin: 10
		Layout.rightMargin: 10
		marginLeft: 0
		marginRight: 40
		marginTop: 10
		marginBottom: 5
		visible: historySize > 1

		theme: GraphsTheme {
        		theme: GraphsTheme.Theme.QtGreenNeon
			gridVisible: false
			backgroundVisible: false
		}

		axisX: DateTimeAxis {	
			id: xAxis
			//TODO adjust tickInterval
			tickInterval: 2
			labelFormat: "MMMM yyyy"
            }

            axisY: ValueAxis {
			id: yAxis
			min: 0
			labelDecimals: 0
			tickInterval: Math.max(1, Math.floor(max / 5))
	    }

		LineSeries {
			id: totalSerie
			name: "total"
			color: totalColor
			width: 4
		}
		
		LineSeries {
			id: masteredSerie
			name: "mastered"
			color: masteredColor
			width: 4
		}

		LineSeries {
			id: learningSerie
			name: "learning"
			color: learningColor
			width: 4
		}
		
		LineSeries {
			id: newSerie
			name: "new"
			color: newColor
			width: 4
		}
	}

	Item{
		Layout.fillHeight: true
		visible: historySize < 2
	}
}
