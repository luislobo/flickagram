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
private:
	ActivityIndicator *mActivityIndicator;
	GroupDataModel *mGroupDataModel;
	ListView *mListView;
	Label *mFirstLabel;
	QNetworkAccessManager *mNetworkAccessManager;
	QNetworkAccessManager *mNetworkAccessManagerImages;
	QFile *mFile;
	QFile *mFileWallpaper;

	void cleanupXml();

};

#endif /* Flickagram_HPP_ */
