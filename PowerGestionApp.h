#ifndef _POWER_GESTION_APP_H
#define _POWER_GESTION_APP_H

#include <qmainwindow.h>
#include <QKeyEvent>
#include <qstring.h>
#include "Turbine.hpp"
#include "Centrale.hpp"
#include <qapplication.h>
#include "ParcHydroelectrique.hpp"
#include "factory/CentraleFactory.hpp"
#include "QTimer.h"
#include "qdatetime.h"

#include "input.h"

class PowerGestionApp : public QMainWindow {
	Q_OBJECT
protected:
	void keyPressEvent(QKeyEvent* event) override;
	void paintEvent(QPaintEvent* event) override;

public:
	PowerGestionApp();
	QString GetTitle();

private:
	void SwitchGraphButton(int idTurbine);
	void Update();

	std::unique_ptr<Centrale> mainCentrale;
	AppInput* appInput;
	QString titleApp;
	QTimer* timer;
};

#endif;