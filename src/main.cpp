// Grid view with detail project template
#include "Flickagram.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include <Qt/qdeclarativedebug.h>

using ::bb::cascades::Application;

Q_DECL_EXPORT int main(int argc, char **argv) {
	// this is where the server is started etc
	Application app(argc, argv);

	// localization support
	QTranslator translator;
	QString locale_string = QLocale().name();
	QString filename = QString("Flickagram_%1").arg(locale_string);
	if (translator.load(filename, "app/native/qm")) {
		qDebug() << "install a translator";
		Application::instance()->installTranslator(&translator);
	} else {
		qDebug() << "CANNOT install a translator";
	}

	// create the application pane object to init UI etc.
	Flickagram mainApp;
	mainApp.initLocalization(&translator);


	// we complete the transaction started in the app constructor and start the client event loop here
	return Application::exec();
	// when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
