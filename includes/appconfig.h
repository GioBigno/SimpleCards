#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QSettings>
#include <QSize>
#include <QUrl>
#include <QDir>
#include <QStandardPaths>
#include <QtQml/qqmlregistration.h>

class AppConfig : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(bool firstOpening READ getFirstOpening WRITE setFirstOpening NOTIFY firstOpeningChanged);
	Q_PROPERTY(bool hardButton READ getHardButton WRITE setHardButton NOTIFY hardButtonChanged);
	Q_PROPERTY(bool cardsCounter READ getCardsCounter WRITE setCardsCounter NOTIFY cardsCounterChanged);
	Q_PROPERTY(bool rememberSize READ getRememberSize WRITE setRememberSize NOTIFY rememberSizeChanged);
	Q_PROPERTY(bool windowMaximized READ getWindowMaximized WRITE setWindowMaximized NOTIFY windowMaximizedChanged);
	Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);
	Q_PROPERTY(bool confirmDeleteDeck READ getConfirmDeleteDeck WRITE setConfirmDeleteDeck NOTIFY confirmDeleteDeckChanged);
	Q_PROPERTY(bool confirmDeleteCard READ getConfirmDeleteCard WRITE setConfirmDeleteCard NOTIFY confirmDeleteCardChanged);
	Q_PROPERTY(QUrl decksDirectory READ getDecksDirectory WRITE setDecksDirectory NOTIFY decksDirectoryChanged);

	const bool firstOpening_default = true;
	const bool hardButton_default = true;
	const bool cardsCounter_default = true;
	const bool rememberSize_default = false;
	const bool windowMaximized_default = false;
	const QSize windowSize_default = QSize(800, 550);
	const bool confirmDeleteDeck_default = true;
	const bool confirmDeleteCard_default = false;

signals:
	void firstOpeningChanged();
	void hardButtonChanged();
	void cardsCounterChanged();
	void rememberSizeChanged();
	void windowMaximizedChanged();
	void windowSizeChanged();
	void confirmDeleteDeckChanged();
	void confirmDeleteCardChanged();
	void decksDirectoryChanged();

public:
	explicit AppConfig(QObject *parent = nullptr);
	
	static QUrl decksDirectory_default();

	bool getFirstOpening();
	bool getHardButton();
	bool getCardsCounter();
	bool getRememberSize();
	bool getWindowMaximized();
	QSize getWindowSize();
	bool getConfirmDeleteDeck();
	bool getConfirmDeleteCard();
	QUrl getDecksDirectory();
	void setFirstOpening(bool opt);
	void setHardButton(bool opt);
	void setCardsCounter(bool opt);
	void setRememberSize(bool opt);
	void setWindowMaximized(bool opt);
	void setWindowSize(const QSize& s);
	void setConfirmDeleteDeck(bool opt);
	void setConfirmDeleteCard(bool opt);
	void setDecksDirectory(QUrl dir);

private:
	QSettings settings;
};
