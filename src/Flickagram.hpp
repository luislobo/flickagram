// List with context menu project template
#ifndef Flickagram_HPP_
#define Flickagram_HPP_

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/ListView>
#include <bb/cascades/Label>
#include <QObject>
#include <QFile>

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "WebImageView.h"

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

	// locale changed by user from device settings
	void localeChanged();

private:
	ActivityIndicator *mActivityIndicator;
	GroupDataModel *mGroupDataModel;
	ListView *mListView;
	Label *mFirstLabel;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkAccessManager *mNetworkAccessManagerImages;
	QFile *mFile;
	QFile *mFileWallpaper;
	Notifier * m_notifier;

	Application *m_app;
    Menu* createApplicationMenu();

    HelpActionItem* mHelpItem;
    ActionItem* mFeedbackItem;
    SettingsActionItem* mSettingsItem;
    ActionItem* mLogoutItem;

    ODSData* mOdsData;
    ODSSettings* mOdsSettings;

    QString mCurrentLocale;
    LocaleHandler* mLocaleHandler;
    QTranslator* mTranslator;

    bb::system::InvokeManager *mInvokeManager;

    void translateMenuItems();
    void initTheApplication();

	void cleanupXml();

};

#endif /* Flickagram_HPP_ */
