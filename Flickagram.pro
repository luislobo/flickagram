APP_NAME = Flickagram

QT += core network xml

CONFIG += qt warn_on cascades10
LIBS += -lbbdata -lbbplatform -lbbsystem -lbbsystemlocale

lupdate_inclusion {
	SOURCES += ../assets/*.qml
	SOURCES += ../assets/appmenu/*.qml
	SOURCES += ../assets/common/*.qml
}
TRANSLATIONS = \
$${TARGET}_es.ts \
$${TARGET}.ts

include(config.pri)
                    
