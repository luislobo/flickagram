
#ifndef FlickagramSettings_HPP_
#define FlickagramSettings_HPP_

#include <qobject.h>
#include <bb/cascades/Application>
#include <QSettings>

static const QString SETTINGS_KEY_SERVER_URL = "server/url";
static const QString SETTINGS_KEY_SERVER_CURRENT_USER = "server/current/user";
static const QString SETTINGS_KEY_SERVER_CURRENT_PASSWORD = "server/current/password";
static const QString SETTINGS_KEY_FILES_AVAILABLE = "files/available";
static const QString SETTINGS_KEY_FILES_LAST_SYNC = "files/last_sync";

// used from QML LoginSheet
static const QString SETTINGS_KEY_LOGIN_USER = "login/user";
static const QString SETTINGS_KEY_LOGIN_PASSWORD = "login/password";

static const QString SETTINGS_KEY_TESTDRIVE = "login/testdrive";


class FlickagramSettings: public QObject {
Q_OBJECT

public:
	FlickagramSettings();
	virtual ~FlickagramSettings();

	/**
	 * This Invokable function gets a value from the QSettings,
	 * if that value does not exist in the QSettings database, the default value is returned.
	 *
	 * @param objectName Index path to the item
	 * @param defaultValue Used to create the data in the database when adding
	 * @return If the objectName exists, the value of the QSettings object is returned.
	 *         If the objectName doesn't exist, the default value is returned.
	 */
	Q_INVOKABLE
	QString getValueFor(const QString &objectName, const QString &defaultValue);

	Q_INVOKABLE
	bool isTrueFor(const QString &objectName, const bool &defaultValue);

	/**
	 * This function sets a value in the QSettings database. This function should to be called
	 * when a data value has been updated from QML
	 *
	 * @param objectName Index path to the item
	 * @param inputValue new value to the QSettings database
	 */
	Q_INVOKABLE
	void saveValueFor(const QString &objectName, const QString &inputValue);

	Q_INVOKABLE
	void setTrueFor(const QString &objectName, const bool &inputValue);

	Q_INVOKABLE
	QString getUser();

	Q_INVOKABLE
	QString getServerUrl();

	Q_INVOKABLE
	QString getPassword();

	Q_INVOKABLE
	void setUser(const QString &user);

	Q_INVOKABLE
	void setPassword(const QString &password);

	Q_INVOKABLE
	void setServerUrl(const QString &url);

	Q_INVOKABLE
	void setTestdrive(const bool testdrive);

	Q_INVOKABLE
	bool testdrive();

private:
	QSettings mSettings;
	bool mTestdriving;

};

#endif /* FlickagramSettings_HPP_ */
