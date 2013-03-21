// List with context menu project template
#ifndef Flickagram_HPP_
#define Flickagram_HPP_

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/ListView>
#include <bb/cascades/Label>
#include <QObject>
#include <QFile>
#include <bb/cascades/Menu>
#include <QTranslator>

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <bb/cascades/LocaleHandler>
#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeRequest.hpp>

#include "WebImageView.h"
#include "FlickagramData.hpp"
#include "FlickagramSettings.hpp"

using namespace bb::cascades;

namespace bb {
namespace cascades {
class Application;
}
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class Flickagram: public QObject {

Q_OBJECT

public:
	Flickagram(bb::cascades::Application *app);

	/*!
	 * Initiates the network request.
	 */
	Q_INVOKABLE
	void initLocalization(QTranslator* translator);

	/*
	* Refreshes the UI with the specified locale
	*
	* @param locale - the locale to change to
	*/
	Q_INVOKABLE
	void updateLocale(QString locale);

	/*
	* Allows the current language to be retrieved from QML
	*
	* @return the current language (translated)
	*/
	Q_INVOKABLE
	QString getCurrentLanguage();

	/*
	* Allows the current locale to be retrieved from QML
	*
	* @return the current locale
	*/
	Q_INVOKABLE
	QString getCurrentLocale();

	/*
	* Workaround to force keyboard to hide when readonly text area is touched
	*/
	Q_INVOKABLE
	void suppressKeyboard();


    Q_INVOKABLE
    void inviteBBM();

    Q_INVOKABLE
    void inviteFlickagram();

    Q_INVOKABLE
    void leaveReview();

	Q_INVOKABLE
	void initiateRequest();
	Q_INVOKABLE
	void downloadImageInitiateRequest(const QString & uri);

protected:
	void setAsWallpaper(QString url);

private Q_SLOTS:
	/*!
	 * Handles the network reply.
	 */
	void requestFinished(QNetworkReply* reply);
	void imageRequestFinished(QNetworkReply* reply);

    // slots from AppMenu Sheets:
    void feedbackTriggered();
    void helpTriggered();
    void settingsTriggered();
    // locale changed by user from device settings
    void localeChanged();

    void shareTextWithBBM(const QString& text);
    void shareTextWithMail(const QString& text);

private:
	ActivityIndicator *mActivityIndicator;
	GroupDataModel *mGroupDataModel;
	ListView *mListView;
	Label *mFirstLabel;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkAccessManager *mNetworkAccessManagerImages;
	QFile *mFile;
	QFile *mFileWallpaper;

    Menu* createApplicationMenu();

    HelpActionItem* mHelpItem;
    ActionItem* mFeedbackItem;
    SettingsActionItem* mSettingsItem;
    ActionItem* mLogoutItem;

    FlickagramData* mFlickagramData;
    FlickagramSettings* mOdsSettings;

    QString mCurrentLocale;
    LocaleHandler* mLocaleHandler;
    QTranslator* mTranslator;

    bb::system::InvokeManager *mInvokeManager;

    void translateMenuItems();
    void initTheApplication();

	void cleanupXml();

};

#endif /* Flickagram_HPP_ */
