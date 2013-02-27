/*
 * FlickrRequest.h
 *
 *  Created on: Feb 24, 2013
 *      Author: lobo
 */

#ifndef FLICKRREQUEST_H_
#define FLICKRREQUEST_H_

#include <QtCore/QObject>

class FlickrRequest: public QObject {

	static const QString defaultUri;
	static const QString flickrApiKey;
	static const QString flickrSecret;
	static const QString flickrInterestingnessGetList;

Q_OBJECT

public:

	FlickrRequest(QObject *parent = 0);

	void requestInterestingnessPhotos();

protected:
	QString getUnsignedMethodCall(const QString &methodCall);

Q_SIGNALS:
	/*
	 * This signal is emitted when the flickr request is received
	 * @param info - on success, this is the reply from the request
	 *               on failure, it is an error string
	 * @param success - true if flickr request succeed, false if not
	 */
	void complete(const QString &info, bool success);

private Q_SLOTS:
	/*
	 * Callback handler for QNetworkReply finished() signal
	 */
	void onInterestingnessPhotosReply();
};

#endif /* FLICKRREQUEST_H_ */
