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

#include <QObject>
#include <QIODevice>
#include <QDir>

using namespace bb::cascades;

Flickagram::Flickagram(bb::cascades::Application *app) :
		QObject(app) {

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
	mListView = root->findChild<ListView*>("list");

	// Create a network access manager and connect a custom slot to its
	// finished signal
	mNetworkAccessManager = new QNetworkAccessManager(this);

	bool result = connect(mNetworkAccessManager,
			SIGNAL(finished(QNetworkReply*)), this,
			SLOT(requestFinished(QNetworkReply*)));

	// Displays a warning message if there's an issue connecting the signal
	// and slot. This is a good practice with signals and slots as it can
	// be easier to mistype a slot or signal definition
	Q_ASSERT( result);
	Q_UNUSED(result);

// Create a file in the application's data directory
	mFile = new QFile("data/model.xml");

// set created root object as a scene
	app->setScene(root);

}

void Flickagram::initiateRequest() {
// Start the activity indicator
	mActivityIndicator->start();

// Create and send the network request
	QNetworkRequest request = QNetworkRequest();

	QString queryUri = QString(
			"%1?method=%2&api_key=%3&extras=url_t,url_m&format=rest").arg(
			"http://api.flickr.com/services/rest/",
			"flickr.interestingness.getList",
			"468f6dc4e3733eddca5084d4ec8405fd");
	qDebug() << queryUri;
	QUrl url(queryUri);
	request.setUrl(QUrl(queryUri));
	mNetworkAccessManager->get(request);
}

void Flickagram::requestFinished(QNetworkReply* reply) {
// Check the network reply for errors
	if (reply->error() == QNetworkReply::NoError) {

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

		// Set the new data model on the list and stop the activity indicator
		mListView->setDataModel(mGroupDataModel);
		mActivityIndicator->stop();

	} else {
		qDebug() << "\n Problem with the network";
		qDebug() << "\n" << reply->errorString();
	}
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

	QDomDocument doc("mydocument");

	if (!doc.setContent(mFile)) {
		return;
	}
	mFile->close();

	//Get the root element
	QDomElement docElem = doc.documentElement();

	// you could check the root tag name here if it matters
	QString rootTag = docElem.tagName(); // == photos

	// get the node's interested in, this time only caring about photos
	QDomNodeList nodeList = docElem.elementsByTagName("photo");

	//Check each node one by one.
	QMap<QString, QVariant> photo;
	for (int ii = 0; ii < nodeList.count(); ii++) {

		// get the current one as QDomElement
		QDomElement el = nodeList.at(ii).toElement();

		photo["id"] = el.attributes().namedItem("id").nodeValue();
		photo["title"] = el.attributes().namedItem("title").nodeValue();
		photo["url_t"] = el.attributes().namedItem("url_t").nodeValue();
		photo["url_m"] = el.attributes().namedItem("url_m").nodeValue();

		mGroupDataModel->insert(photo);
	}

}
