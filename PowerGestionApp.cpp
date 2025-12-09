#include "PowerGestionApp.h"
#include <qpainter.h>
#include <qchart.h>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <Qlabel.h>
#include <QPushButton>

PowerGestionApp::PowerGestionApp()
{
    appInput = new AppInput;
    titleApp = "Gestion Centrale Electrique";

    showFullScreen();

	auto src = std::make_shared<SourceDonnees>();
	mainCentrale = CentraleFactory::creerCentraleStandard(src);
	mainCentrale->SetParentWidget(this);
    for(int i = 0; i < mainCentrale->getNbTurbines(); ++i)
    {
        mainCentrale->ajouterTurbineWidget(mainCentrale->getTurbine(i), this);
	}
	mainCentrale->SetupPositionTurbineWidgets(this->width() * 0.05, this->height() * 0.2, this->width());
    mainCentrale->mettreAJour();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
		Update();
        });
    timer->start(50);
}

void PowerGestionApp::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
    else {
        appInput->keyPressed(event->key());
    }
}

void PowerGestionApp::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);


    // ----- Background gradient -----
    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0.0, QColor(230, 242, 255));
    bg.setColorAt(1.0, QColor(180, 210, 255));
    p.fillRect(rect(), bg);

    // ----- Hydro-Québec Logo -----
    QPixmap logo(":/Images/resources/logo-hq-couleur-fb.jpg");
    logo = logo.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    p.drawPixmap(0, 0, logo);


    // ----- Title -----
    p.setPen(QColor(20, 40, 90));
    p.setFont(QFont("Arial", 32, QFont::Bold));
    p.drawText(500, 50, "Centrale Hydroelectrique 1");

    mainCentrale->draw(p, this->height(), this->width());
}

QString PowerGestionApp::GetTitle()
{
    return titleApp;
}

void PowerGestionApp::SwitchGraphButton(int idTurbine){

}

void PowerGestionApp::Update()
{
    mainCentrale->UpdateScreen();
    this->update();
}
