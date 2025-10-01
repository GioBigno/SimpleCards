#pragma once
#include <QObject>
#include <QtQml/qqmlregistration.h>

namespace DeckModeNS {
	Q_NAMESPACE
	QML_NAMED_ELEMENT(DeckMode)

	enum class DeckMode {
		Test,
		Edit
	};
	Q_ENUM_NS(DeckMode)
}

using DeckMode = DeckModeNS::DeckMode;
