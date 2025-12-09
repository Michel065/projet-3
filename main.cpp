#include <QSplashScreen>
#include <QApplication.h>
#include <QTimer>
#include "PowerGestionApp.h"
#include <iostream>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
   
    // ---- Create the gestion app ----
    PowerGestionApp gestionApp;

    gestionApp.setWindowTitle(gestionApp.GetTitle());

    return app.exec();
}