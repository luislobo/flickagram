// List with context menu project template
#include "Flickagram.hpp"
#include "FlickrRequest.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>
#include <bb/cascades/GroupDataModel>
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

Flickagram::Flickagram(bb::cascades::Application *app) :
		QObject(app), m_active(false), m_error(false), m_model(
				new GroupDataModel(QStringList() << "id", this)) {

	m_model->setGrouping(ItemGrouping::None);

	// create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("_flickr", this);

	// create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	// set created root object as a scene
	app->setScene(root);

	/*{
	 // load JSON data from file to QVariant
	 bb::data::JsonDataAccess jda;
	 QVariantList lst =
	 jda.load("app/native/assets/mydata.json").toList();
	 if (jda.hasError()) {
	 bb::data::DataAccessError error = jda.error();
	 qDebug() << "JSON loading error: " << error.errorType() << ": "
	 << error.errorMessage();
	 } else {
	 qDebug() << "JSON data loaded OK!";
	 GroupDataModel *m = new GroupDataModel();
	 // insert the JSON data to model
	 m->insertList(lst);
	 // make the model flat
	 m->setGrouping(ItemGrouping::None);
	 // find cascades component of type ListView with an objectName property set to the value 'listView'
	 // usable if one do not want to expose GroupDataModel to QML (qmlRegisterType<GroupDataModel>("com.example", 1, 0, "MyListModel");)
	 ListView *list_view = root->findChild<ListView*>("listView");
	 // assign data model object (m) to its GUI representation object (list_view)
	 if (list_view)
	 list_view->setDataModel(m);
	 }
	 }*/

}

void Flickagram::reset() {
	m_error = false;
	m_errorMessage.clear();

	emit statusChanged();
}

/*
 *  App::requestInterestingnessPhotos()
 *
 *  Initiates an http request to retrieve the latest interestingness photos
 *  When the network request is complete
 *  onFlickrInterestingnessPhotos method is called with the result
 */
void Flickagram::requestInterestingnessPhotos() {
	//if (m_active)
	//	return;

	FlickrRequest* request = new FlickrRequest(this);
	connect(request, SIGNAL(complete(QString, bool)), this,
			SLOT(onFlickrInterestingnessPhotos(QString, bool)));
	request->requestInterestingnessPhotos();

	m_active = true;
	emit activeChanged();
}
//! [2]
void Flickagram::onFlickrInterestingnessPhotos(const QString &info, bool success) {
	FlickrRequest *request = qobject_cast<FlickrRequest*>(sender());

	if (success) {
		parseResponse(info);

		emit photosLoaded();
	} else {
		m_errorMessage = info;
		m_error = true;
		emit statusChanged();
	}

	m_active = false;
	emit activeChanged();

	request->deleteLater();
}
//! [3]

/*
 * App::parseResponse()
 *
 * Parses the JSON data from the twitter response and populates the ListView.
 */
//! [4]
void Flickagram::parseResponse(const QString &response) {
	m_model->clear();

	if (response.trimmed().isEmpty())
		return;

	// Parse the json response with JsonDataAccess
	JsonDataAccess dataAccess;
	const QVariant variant = dataAccess.loadFromBuffer(response);

	// The qvariant is an array of tweets which is extracted as a list
	const QVariantList feed = variant.toList();

	// For each object in the array, push the variantmap in its raw form
	// into the ListView
	foreach (const QVariant &photo, feed){
		m_model->insert(photo.toMap());
	}
}

bool Flickagram::active() const {
	return m_active;
}

bool Flickagram::error() const {
	return m_error;
}

QString Flickagram::errorMessage() const {
	return m_errorMessage;
}

bb::cascades::DataModel* Flickagram::model() const {
	return m_model;
}
