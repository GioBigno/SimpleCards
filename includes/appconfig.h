#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QSize>
#include <QtQml/qqmlregistration.h>

class AppConfig : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool hardButton READ getHardButton WRITE setHardButton NOTIFY hardButtonChanged);
	Q_PROPERTY(bool cardsCounter READ getCardsCounter WRITE setCardsCounter NOTIFY cardsCounterChanged);
	Q_PROPERTY(bool rememberSize READ getRememberSize WRITE setRememberSize NOTIFY rememberSizeChanged);
	Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);

signals:
	void hardButtonChanged();
	void cardsCounterChanged();
	void rememberSizeChanged();
	void windowSizeChanged();

public:
	explicit AppConfig(QObject *parent = nullptr);

	bool getHardButton();
	bool getCardsCounter();
	bool getRememberSize();
	QSize getWindowSize();
	void setHardButton(bool opt);
	void setCardsCounter(bool opt);
	void setRememberSize(bool opt);
	void setWindowSize(const QSize& s);

private:
	QSettings settings;
};
