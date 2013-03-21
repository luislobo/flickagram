#include "FlickagramData.hpp"
#include "Singleton.hpp"
#include <QUrl>
#include <bb/cascades/Application>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/controls/dropdown.h>
#include <bb/cascades/controls/option.h>
#include <bb/system/SystemProgressDialog.hpp>
#include <bb/system/SystemUiButton.hpp>
#include <bb/system/SystemUiProgressState>
#include <bb/system/SystemUiResult>

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;

FlickagramData::FlickagramData() {

}

FlickagramData::~FlickagramData() {
	// TODO Auto-generated destructor stub
}

