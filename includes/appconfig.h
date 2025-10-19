#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QtQml/qqmlregistration.h>

class AppConfig : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool hardButton READ getHardButton WRITE setHardButton NOTIFY hardButtonChanged);
	Q_PROPERTY(bool cardsCounter READ getCardsCounter WRITE setCardsCounter NOTIFY cardsCounterChanged);

signals:
	void hardButtonChanged();
	void cardsCounterChanged();

public:
	explicit AppConfig(QObject *parent = nullptr);

	bool getHardButton();
	bool getCardsCounter();
	void setHardButton(bool opt);
	void setCardsCounter(bool opt);

private:
	QSettings settings;
};
