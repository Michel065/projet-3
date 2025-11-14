#pragma once
#include "InterfaceProductionStrategy.hpp"
#include "Capteur.hpp"
#include "Status.hpp"
#include <iostream>
#include <memory>

class Turbine
{
public:
    Turbine(int id, Status statusInitial,std::unique_ptr<InterfaceProductionStrategy> strategie,std::shared_ptr<Capteur> capteurDebit);

    int   getId() const;
    float getDebit()const;
    float getDebitMin() const;
    float getDebitMax() const;
    Status getStatus() const;

    void setDebit(float nouveauDebit);
    void setDebitMin(float nouveauDebit);
    void setDebitMax(float nouveauDebit);
    void setStatus(Status nouveauStatus);

    
    float getProduction(float hauteur_de_chute_nette) const;

    void mettreAJourDepuisCapteur();

private:
    int    m_id;
    float  m_debit;
    float  m_debit_min;
    float  m_debit_max;
    Status m_status;
    std::unique_ptr<InterfaceProductionStrategy> m_strategie;
    std::shared_ptr<Capteur> m_capteurDebit; 
};
