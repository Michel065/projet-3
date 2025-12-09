#pragma once
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <memory>
#include <vector>
#include "Status.hpp"
#include "Reservoir.hpp"
#include "Turbine.hpp"
#include <numeric> 
#include "UI/GraphUI.h"
#include "composite/Iproduction.hpp"
#include "UI/TurbineWidget.h"
#include "UI/SimpleButton.h"
#include "qdatetime.h"

class Centrale: public IProducteur
{
public:
    Centrale(int id,
             Status statusInitial,
             std::shared_ptr<Reservoir> reservoirAmont);

    int    getId()     const;
    Status getStatus() const;
    void   setStatus(Status s);

    void ajouterTurbine(std::unique_ptr<Turbine> turbine);
	void ajouterTurbineWidget(Turbine* turbine, QWidget* parent);

    int getNbTurbines() const;
    const Turbine* getTurbine(int index) const;
    Turbine* getTurbine(int index);

    float calculerHauteurChute() const;   
    float getProductionInstantanee() const override;
    std::vector<std::pair<int, float>> getProductionInstantaneeDetail() const;

    void print_Production_centrale() const;
    void print_Production_centrale_detail() const;

    void mettreAJour();

    void UpdateScreen();

	void draw(QPainter& p, int height, int width);
    void drawRepartitionPuissance(QPainter& p);
    
    void SetDataSource(std::shared_ptr<SourceDonnees> data);
    void SetParentWidget(QWidget* parent);

	void SetupPositionTurbineWidgets(int startX, int startY, int widthScreen);

private:
    int m_id;
	bool needUpdate = true;
    bool updateGraph = false;
    Status m_status;
    std::shared_ptr<SourceDonnees> m_src;
    std::shared_ptr<Reservoir> m_reservoirAmont;
    QChart* m_chart = nullptr;
    QChartView* m_chartView = nullptr;
    QPieSeries* m_series = nullptr;
    std::vector<std::unique_ptr<Turbine>> m_turbines;
	std::vector<std::unique_ptr<TurbineWidget>> m_turbineWidgets;
    int cpt = 0, timeStep = 40;
    int currentTurbineGraph = 0;
    GraphWidget* graphWidget = nullptr;
    QDateTime date;
	std::vector<SimpleBouton*> m_buttons;

};
