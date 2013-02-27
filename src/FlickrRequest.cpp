/*
 * FlickrRequest.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: lobo
 */

#include "FlickrRequest.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <string>

const QString FlickrRequest::defaultUri = "http://api.flickr.com/services/rest/";
const QString FlickrRequest::flickrApiKey = "468f6dc4e3733eddca5084d4ec8405fd";
const QString FlickrRequest::flickrSecret = "6a55ea762b7ca483";
const QString FlickrRequest::flickrInterestingnessGetList =
		"flickr.interestingness.getList";

/*
 * Default constructor
 */
FlickrRequest::FlickrRequest(QObject *parent) :
QObject(parent) {
}

/*
 * FlickrRequest::requestTimeline(const QString &screenName)
 *
 * Makes a network call to retrieve the twitter feed for the specified screenname
 */

void FlickrRequest::requestInterestingnessPhotos() {
	QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(
			this);

	const QString queryUri = this->getUnsignedMethodCall(
			FlickrRequest::flickrInterestingnessGetList);

	QNetworkRequest request(queryUri);

	QNetworkReply* reply = networkAccessManager->get(request);

	connect(reply, SIGNAL(finished()), this, SLOT(onInterestingnessPhotosReply()));
}
//! [0]

/*
 * FlickrRequest::onTimelineReply()
 *
 * Callback handler for QNetworkReply finished() signal
 */
//! [1]
void FlickrRequest::onInterestingnessPhotosReply() {
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString response;
	bool success = false;
	if (reply) {
		if (reply->error() == QNetworkReply::NoError) {
			const int available = reply->bytesAvailable();
			if (available > 0) {
				const QByteArray buffer = reply->readAll();
				response = QString::fromUtf8(buffer);
				success = true;
			}
		} else {
			response =
					tr("Error: %1 status: %2").arg(reply->errorString(),
							reply->attribute(
									QNetworkRequest::HttpStatusCodeAttribute).toString());
		}

		reply->deleteLater();
	}

	if (response.trimmed().isEmpty()) {
		response = tr("Flickr request failed. Check internet connection");
	}

	emit complete(response, success);
}

QString FlickrRequest::getUnsignedMethodCall(const QString &methodCall) {

	QString uri = QString("%1?method=%2&api_key=%3&format=json").arg(defaultUri,
			methodCall, flickrApiKey);
	return uri;
}
