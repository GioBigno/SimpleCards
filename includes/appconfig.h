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
	Q_PROPERTY(bool confirmDeleteDeck READ getConfirmDeleteDeck WRITE setConfirmDeleteDeck NOTIFY confirmDeleteDeckChanged);
	Q_PROPERTY(bool confirmDeleteCard READ getConfirmDeleteCard WRITE setConfirmDeleteCard NOTIFY confirmDeleteCardChanged);

signals:
	void hardButtonChanged();
	void cardsCounterChanged();
	void rememberSizeChanged();
	void windowSizeChanged();
	void confirmDeleteDeckChanged();
	void confirmDeleteCardChanged();

public:
	explicit AppConfig(QObject *parent = nullptr);

	bool getHardButton();
	bool getCardsCounter();
	bool getRememberSize();
	QSize getWindowSize();
	bool getConfirmDeleteDeck();
	bool getConfirmDeleteCard();
	void setHardButton(bool opt);
	void setCardsCounter(bool opt);
	void setRememberSize(bool opt);
	void setWindowSize(const QSize& s);
	void setConfirmDeleteDeck(bool opt);
	void setConfirmDeleteCard(bool opt);

private:
	QSettings settings;
};
