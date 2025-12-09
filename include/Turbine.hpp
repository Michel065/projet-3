#pragma once
#include "strategie/InterfaceProductionStrategy.hpp"
#include "composite/Iproduction.hpp"
#include "Capteur.hpp"
#include "Status.hpp"
#include <iostream>
#include <memory>
#include <QPushButton>
#include <qpainter.h>

enum ModeTurbine
{
    Manuel,
    Automatique
};


class Turbine : public IProducteur
{
public:
    struct DataTurbine
    {
        int id;
        float debit;
        float debit_min;
        float debit_max;
		float hauteur_chute;
        float puissance;
	};

    Turbine(int id, Status statusInitial,std::unique_ptr<InterfaceProductionStrategy> strategie,std::shared_ptr<Capteur> capteurDebit);

    int   getId() const;
    float getDebit()const;
    float getDebitMin() const;
    float getDebitMax() const;
    Status getStatus() const;

	ModeTurbine getMode() const { return m_mode; }
	void setMode(ModeTurbine mode) { m_mode = mode; }

    void setDebit(float nouveauDebit);
    void setDebitMin(float nouveauDebit);
    void setDebitMax(float nouveauDebit);
    void setStatus(Status nouveauStatus);

    void setHauteurChute(float h);
    float getProductionInstantanee() const override;
    float getHauteurChute() { return m_hauteurChute; }

    void mettreAJourDepuisCapteur();
    void addDataToHistorique(DataTurbine data) { m_historiqueData.push_back(data); };
	std::vector<DataTurbine> getHistoriqueData() const { return m_historiqueData; }

private:
    int    m_id;
    float  m_debit;
    float  m_debit_min;
    float  m_debit_max;
    float m_hauteurChute = 0.f; 
    Status m_status;
	ModeTurbine m_mode = Automatique;
    std::unique_ptr<InterfaceProductionStrategy> m_strategie;
    std::shared_ptr<Capteur> m_capteurDebit; 
    QPushButton* m_popupButton = nullptr;
    QWidget* m_parentWidget = nullptr;
	std::vector<DataTurbine> m_historiqueData;
};

