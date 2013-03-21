#ifndef FLICKAGRAMDATA_HPP_
#define FLICKAGRAMDATA_HPP_

#include <qobject.h>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/controls/listview.h>
#include "FlickagramSettings.hpp"
#include <bb/system/SystemProgressDialog.hpp>
#include <bb/system/SystemDialog>
#include <bb/system/SystemPrompt.hpp>
#include <bb/system/SystemToast.hpp>

using namespace bb::cascades;
using namespace bb::system;

class FlickagramData: public QObject {
Q_OBJECT
public:
	FlickagramData();
	virtual ~FlickagramData();
Q_SIGNALS:
private slots:
private:
};

#endif /* FLICKAGRAMDATA_HPP_ */
