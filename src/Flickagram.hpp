// List with context menu project template
#ifndef Flickagram_HPP_
#define Flickagram_HPP_

#include <bb/cascades/GroupDataModel>
#include <QObject>

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

Q_PROPERTY(bool active READ active NOTIFY activeChanged)
Q_PROPERTY(bool error READ error NOTIFY statusChanged)
Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY statusChanged)

Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

public:
	Flickagram(bb::cascades::Application *app);

public Q_SLOTS:
	/*
	 * Called by the QML to get flickr interestingness photos
	 */
	void requestInterestingnessPhotos();

	/*
	 * Allows the QML to reset the state of the application
	 */
	void reset();

Q_SIGNALS:
	/*
	 * This signal is emitted whenever the photos have been loaded successfully
	 */
	void photosLoaded();

	/*
	 * The change notification signals of the properties
	 */
	void activeChanged();
	void statusChanged();

private Q_SLOTS:
	/*
	 * Handles the complete signal from FlickrRequest when
	 * the request is complete
	 * @see FlickrRequest::complete()
	 */
	void onFlickrInterestingnessPhotos(const QString &info, bool success);

private:
	/*
	 * Parses the JSON response from the twitter request
	 */
	void parseResponse(const QString&);

	/*
	 * The accessor methods of the properties
	 */
	bool active() const;
	bool error() const;
	QString errorMessage() const;
	bb::cascades::DataModel* model() const;

private:
	bool m_active;
	bool m_error;
	QString m_errorMessage;
	bb::cascades::GroupDataModel* m_model;

};

#endif /* Flickagram_HPP_ */
