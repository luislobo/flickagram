// List with context menu project template
#include "Flickagram.hpp"

#include <QtXml/QDomDocument>
#include <bb/cascades/QListDataModel>

#include <bb/cascades/GroupDataModel>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>
#include <bb/cascades/XmlDataModel>
#include <bb/platform/HomeScreen>
#include <bb/system/SystemToast>

#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeRequest.hpp>
#include <bb/cascades/LocaleHandler>

#include <QObject>
#include <QIODevice>
#include <QDir>
#include <QUrl>

using namespace bb::cascades;
using namespace bb::system;

Flickagram::Flickagram(bb::cascades::Application *app) :
		QObject(app) {

	qmlRegisterType<WebImageView>("org.labsquare", 1, 0, "WebImageView");

	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("app", this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	mGroupDataModel = new GroupDataModel(QStringList());
	mGroupDataModel->setGrouping(ItemGrouping::None);

	// Retrieve the activity indicator from QML so that we can start
	// and stop it from C++
	mActivityIndicator = root->findChild<ActivityIndicator*>("indicator");

	// Retrieve the list so we can set the data model on it once
	// we retrieve it
	mListView = root->findChild<ListView*>("listView");

	mFirstLabel = root->findChild<Label*>("initmsg");

	// Create a network access manager and connect a custom slot to its
	// finished signal
	mNetworkAccessManager = new QNetworkAccessManager(this);
	mNetworkAccessManagerImages = new QNetworkAccessManager(this);

	// BUG in SDK: IDE reports error with bb::system::, but compiles and runs
	// removing bb::system:: compiles also well but running says "signal not found"
	// ODS is a Invocation Target
	bool ok = connect(mInvokeManager,
			SIGNAL(invoked(const bb::system::InvokeRequest&)), this,
			SLOT(handleInvoke(const bb::system::InvokeRequest&)));
	if (!ok) {
		qDebug() << "connect handleInvoke failed";
	}

	bool result = connect(mNetworkAccessManager,
			SIGNAL(finished(QNetworkReply*)), this,
			SLOT(requestFinished(QNetworkReply*)));

	bool resultImages = connect(mNetworkAccessManagerImages,
			SIGNAL(finished(QNetworkReply*)), this,
			SLOT(imageRequestFinished(QNetworkReply*)));

	// Displays a warning message if there's an issue connecting the signal
	// and slot. This is a good practice with signals and slots as it can
	// be easier to mistype a slot or signal definition
	Q_ASSERT(result);
	Q_UNUSED(result);
	Q_ASSERT(resultImages);
	Q_UNUSED(resultImages);

	QDir * oDir = new QDir("data");
	oDir->remove("model.xml");

	// Create a file in the application's data directory
	mFile = new QFile("data/model.xml");

	// get the first set of images
	this->initiateRequest();

	Notifier * m_notifier = new Notifier();
	connect(m_notifier, SIGNAL(changed()), this, SLOT(localeChanged()));

// set created root object as a scene
	app->setScene(root);

	qDebug() << "set the scene";

	if (!mIsLaunchedEmbedded) {
		initTheApplication();
	} else {
		qDebug() << "we are running EMBEDDED";
	}
	qDebug() << "INIT done";
}
/**
 * some stuff we only need if Opened from HomeScreen
 * or Invoked as Application from another app thru Invocation Framework
 */
void Flickagram::initTheApplication() {
		qDebug() << "we are NOT running EMBEDDED, so do some APPLICATION stuff";
		// ApplicationMenu
		// Hint: first set the scene - then set the menu
		Menu* menu = createApplicationMenu();
		qDebug() << "created ApplicationMenu";

		Application::instance()->setMenu(menu);
		qDebug() << "set ApplicationMenu";

		// first translation
		translateMenuItems();
		qDebug() << "did first translations";

		//
		Application *app = Application::instance();
		bool ok = connect(app, SIGNAL(thumbnail()), this, SLOT(onThumbnail()));
		if (!ok) {
				qDebug() << "connect thumbnail failed";
		}
}

// INTERNATIONALIZATION (i18n)
/**
 *
 * This method initializes translation engine based on current locale
 * at runtime.
 *
 */
void Flickagram::initLocalization(QTranslator* translator) {
		// remember current locale set
		mCurrentLocale = QLocale().name();
		qDebug() << "init with locale: " << mCurrentLocale;
		mTranslator = translator;

		// watch if user changes locale from device settings
		mLocaleHandler = new LocaleHandler(this);
		// connect the handler
		connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), this,
						SLOT(localeChanged()));
		qDebug() << "connected systemLanguageChanged";

}

/**
 * App::updateLocale(QString locale)
 *
 * Update view content basing on the given locale.
 *
 */
void Flickagram::updateLocale(QString locale) {
	qDebug() << "updateLocale: " << locale;

	// if locale is empty - refresh current. otherwise change the local
	if (!locale.trimmed().isEmpty() && mCurrentLocale != locale) {
		mCurrentLocale = locale;

		qDebug() << "updating UI to language: " << mCurrentLocale;
		QString filename = QString("Flickagram_%1").arg(mCurrentLocale);
		if (mTranslator->load(filename, "app/native/qm")) {
			// multiple translators can be installed but for this
			// app we only use one translator instance for brevity
			Application::instance()->removeTranslator(mTranslator);
			Application::instance()->installTranslator(mTranslator);
			// retranslate System menu items
			translateMenuItems();
		}

	}
}

/**
 * (re)translates the titles of System menus
 *
 */
void Flickagram::translateMenuItems() {
	if (mHelpItem) {
		mHelpItem->setTitle(tr("Help"));
	}
	if (mFeedbackItem) {
		mFeedbackItem->setTitle(tr("Feedback"));
	}
//	  if (mLogoutItem) {
//			  mLogoutItem->setTitle(tr("Logout"));
//	  }
	if (mSettingsItem) {
		mSettingsItem->setTitle(tr("Settings"));
	}
}


void Flickagram::initiateRequest() {

// Start the activity indicator
	mActivityIndicator->start();

// Create and send the network request
	QNetworkRequest request = QNetworkRequest();

	QString queryUri =
			QString(
					"%1?method=%2&api_key=%3&extras=url_t,url_m,url_o,url_l&format=rest").arg(
					"http://api.flickr.com/services/rest/",
					"flickr.interestingness.getList",
					"468f6dc4e3733eddca5084d4ec8405fd");
	QUrl url(queryUri);
	request.setUrl(QUrl(queryUri));
	mNetworkAccessManager->get(request);
}

void Flickagram::downloadImageInitiateRequest(const QString & uri) {
	// Start the activity indicator
	mActivityIndicator->start();

	// Create and send the network request
	QNetworkRequest request = QNetworkRequest();

	request.setUrl(QUrl(uri));
	mNetworkAccessManagerImages->get(request);
}

void Flickagram::imageRequestFinished(QNetworkReply* reply) {
// Check the network reply for errors
	if (reply->error() == QNetworkReply::NoError) {

		mFileWallpaper = new QFile("data/wallpaper.png");
		// Open the file and print an error if the file cannot be opened
		if (!mFileWallpaper->open(QIODevice::ReadWrite)) {
			qDebug() << "\n Failed to open file";
			return;
		}

		// Write to the file using the reply data and close the file
		mFileWallpaper->write(reply->readAll());
		mFileWallpaper->flush();
		mFileWallpaper->close();

		bb::platform::HomeScreen homeScreen;
		// Setting wallpaper to image bundled with an application's assets.
		homeScreen.setWallpaper(QUrl("data/wallpaper.png"));

		SystemToast *toast = new SystemToast(this);
		QString msg = tr("The photo was set as a wallpaper successfully!");
		toast->setBody(msg);
		toast->show();

		mActivityIndicator->stop();

	} else {
		qDebug() << "\n Problem with the network";
		qDebug() << "\n" << reply->errorString();
	}
}

void Flickagram::requestFinished(QNetworkReply* reply) {
// Check the network reply for errors
	if (reply->error() == QNetworkReply::NoError) {

		QDir * oDir = new QDir("data");
		oDir->remove("model.xml");

		// Open the file and print an error if the file cannot be opened
		if (!mFile->open(QIODevice::ReadWrite)) {
			qDebug() << "\n Failed to open file";
			return;
		}

		// Write to the file using the reply data and close the file
		mFile->write(reply->readAll());
		mFile->flush();
		mFile->close();

		cleanupXml();

		mFirstLabel->setVisible(false);

		// Set the new data model on the list and stop the activity indicator
		mListView->setDataModel(mGroupDataModel);
		mActivityIndicator->stop();

	} else {
		qDebug() << "\n Problem with the network";
		qDebug() << "\n" << reply->errorString();
	}
}

void Flickagram::setAsWallpaper(QString url) {

	downloadImageInitiateRequest(url);

}

void Flickagram::cleanupXml() {

	// clear group data model
	mGroupDataModel->clear();

	if (!mFile->open(QFile::ReadOnly | QFile::Text)) {
		fprintf(stderr, "Error to open file.\n");
		return;
	} else {
		fprintf(stderr, "File opened.\n");
	}

	QList<QMap<QString, QString> > photos;

	qDebug() << mFile;

	QDomDocument doc("mydocument");

	if (!doc.setContent(mFile)) {
		fprintf(stderr, "Couldn't init XML Dom Object.\n");
		mFile->close();
		return;
	}
	mFile->close();

	fprintf(stderr, "About to process photos\n");
	//Get the root element
	QDomElement docElem = doc.documentElement();

	// you could check the root tag name here if it matters
	QString rootTag = docElem.tagName(); // == photos

	// get the node's interested in, this time only caring about photos
	QDomNodeList nodeList = docElem.elementsByTagName("photo");

	//Check each node one by one.
	QMap<QString, QVariant> photo;
	for (int ii = 0; ii < nodeList.count(); ii++) {
		fprintf(stderr, "Loaded photo %u.\n",ii);
		// get the current one as QDomElement
				QDomElement el = nodeList.at(ii).toElement();

				photo["id"] = el.attributes().namedItem("id").nodeValue();
				photo["title"] = el.attributes().namedItem("title").nodeValue();
				photo["url_m"] = el.attributes().namedItem("url_m").nodeValue();
				photo["url_o"] = el.attributes().namedItem("url_o").nodeValue();
				photo["url_l"] = el.attributes().namedItem("url_l").nodeValue();
				//photo["url_n"] = el.attributes().namedItem("url_n").nodeValue();

				mGroupDataModel->insert(photo);
			}

		}

void Flickagram::setApplication(bb::cascades::Application * app,
		QTranslator * translator, QString currentLocale) {
	m_app = app;

}

void Flickagram::updateLocale(QString locale) {
	m_currentLocale = locale;
	QString filename = QString("Flickagram_%1").arg(m_currentLocale);
	if (m_translator->load(filename, "app/native/qm")) {
		m_app->removeTranslator(m_translator);
		m_app->installTranslator(m_translator);
	}
}

// S L O T S

// handles SLOT from Locale Chaned by user at Device
void Flickagram::localeChanged() {
        updateLocale(QLocale().name());
}


// handles SLOT from feedbackItem
void Flickagram::feedbackTriggered() {
//      Sheet *s = Application::instance()->scene()->findChild<Sheet*>(
//                      "feedbackSheet");
//      if (s) {
//              qDebug() << "feedback triggered and Feedback Sheet found";
//              s->open();
//      } else {
//              qDebug() << "feedback triggered, but no Feedback Sheet found";
//      }
        //
        qDebug() << "invoke sendFeedback";
        InvokeRequest request;
        request.setAction("bb.action.SENDEMAIL");
        request.setTarget("sys.pim.uib.email.hybridcomposer");
        request.setMimeType("settings/view");
        request.setUri("mailto:luislobo@gmail.com?subject=Feedback%20Flickagram");
        mInvokeManager->invoke(request);
}

// handles SLOT from helpItem
void Flickagram::helpTriggered() {
        Sheet *s = Application::instance()->scene()->findChild<Sheet*>("helpSheet");
        if (s) {
                qDebug() << "help triggered and helpSheet found";
                s->open();
        } else {
                qDebug() << "help triggered, but no helpSheet found";
        }
}


// handles SLOT from settingsItem
void Flickagram::settingsTriggered() {
        Sheet *s = Application::instance()->scene()->findChild<Sheet*>(
                        "preferencesSheet");
        if (s) {
                qDebug() << "preferences triggered and preferencesSheet found";
                s->open();

        } else {
                qDebug() << "preferences triggered, but no FpreferencesSheet found";
        }
}

void Flickagram::inviteBBM() {
        InvokeRequest bbmRequest;
        bbmRequest.setTarget("sys.bbm.invitehandler");
        bbmRequest.setAction("bb.action.INVITEBBM");
        qDebug() << "invite to BBM";
        mInvokeManager->invoke(bbmRequest);
}

void OpenDataSpace::shareTextWithBBM(const QString& text) {
        InvokeRequest bbmRequest;
        bbmRequest.setTarget("sys.bbm.sharehandler");
        bbmRequest.setAction("bb.action.SHARE");
        bbmRequest.setData(text.toUtf8());
        qDebug() << "share with BBM: " << text;
        mInvokeManager->invoke(bbmRequest);
        // TODO listen to InvokeTargetReply *reply to see if invocation was successfull
        // https://developer.blackberry.com/cascades/documentation/device_platform/invocation/sending_invocation.html
}

void OpenDataSpace::shareTextWithMail(const QString& text) {
        InvokeRequest mailRequest;
        mailRequest.setTarget("sys.pim.uib.email.hybridcomposer");
        mailRequest.setAction("bb.action.SENDEMAIL");
        mailRequest.setMimeType("settings/view");
        mailRequest.setUri(
                        "mailto:?subject="+text);
        qDebug() << "share with Mail: " << text;
        mInvokeManager->invoke(mailRequest);
}

void OpenDataSpace::inviteFlickagram() {
        shareTextWithBBM(
                        tr(
                                        "Please download OpenDataSpace Application from BlackBerry World for FREE: ")
                                        + "http://appworld.blackberry.com/webstore/content/134203");
        qDebug() << "invite to BBM";
}

void OpenDataSpace::leaveReview() {
        InvokeRequest bbmRequest;
        bbmRequest.setAction("bb.action.OPEN");
        bbmRequest.setMimeType("application/x-bb-appworld");
        bbmRequest.setUri("appworld://content/134203");
        qDebug() << "leave review";
        mInvokeManager->invoke(bbmRequest);
}

/**


/**
 * App::getCurrentLanguage()
 *
 * Retrieve the language name corresponding to the current locale.
 */
QString Flickagram::getCurrentLanguage() {
        // TODO get language name from QLocale - we have now more languages
        qDebug() << "FlickagramApp getCurrentLanguage: " << mCurrentLocale;
        QLocale *loc = new QLocale(mCurrentLocale);
        return loc->languageToString(loc->language());
}

/**
 * App::getCurrentLocale()
 *
 * Retrieve the current locale.
 */
QString Flickagram::getCurrentLocale() {
        qDebug() << "getCurrentLocale: " << mCurrentLocale;
        return mCurrentLocale;
}

/**
 * App::suppressKeyboard()
 *
 * A helper function to force the keyboard to hide
 */
void Flickagram::suppressKeyboard() {
        virtualkeyboard_request_events(0);
        virtualkeyboard_hide();
}

// M E N U
// ApplicationMenu is available on all Screens
// opens using swipe-down
Menu* Flickagram::createApplicationMenu() {
        // HELP will open a website with Help Instructions from Flickagram
        mHelpItem = new HelpActionItem();
        // FEEDBACK will send an email to Flickagram
        mFeedbackItem = new ActionItem();
        mFeedbackItem->setImageSource(
                        QString("asset:///images/ics/5-content-email81.png"));
        // LOGOUT will do a LogOut and jump back to HomeScreen and open the LogIn Sheet
//      mLogoutItem = new ActionItem();
//      mLogoutItem->setImageSource(
//                      QString("asset:///images/ics/10-device-access-accounts81.png"));
        // SETTINGS will open the User Settings
        mSettingsItem = new SettingsActionItem();
        // set the translated Titles
        translateMenuItems();
        // plug it all together
        // TODO .addAction(mLogoutItem) need some more logic for testdrive
        Menu* menu = Menu::create().addAction(mFeedbackItem).help(mHelpItem).settings(
                                        mSettingsItem);
        // Connect SIGNALS and SLOTS
//      QObject::connect(mLogoutItem, SIGNAL(triggered()), this,
//                      SLOT(logoutTriggered()));
        QObject::connect(mFeedbackItem, SIGNAL(triggered()), this,
                        SLOT(feedbackTriggered()));
        QObject::connect(mHelpItem, SIGNAL(triggered()), this,
                        SLOT(helpTriggered()));
        QObject::connect(mSettingsItem, SIGNAL(triggered()), this,
                        SLOT(settingsTriggered()));
        return menu;
}

