#pragma once
#include "strategie/InterfaceProductionStrategy.hpp"
#include "composite/Iproduction.hpp"
#include "Capteur.hpp"
#include "Status.hpp"
#include <iostream>
#include <memory>

class Turbine : public IProducteur
{
public:
    Turbine(int id, Status statusInitial,std::unique_ptr<InterfaceProductionStrategy> strategie,std::shared_ptr<Capteur> capteurDebit);

    int   getId() const;
    float getDebit()const;
    float getDebitMin() const;
    float getDebitMax() const;
    Status getStatus() const;

    void setDebit(float nouveauDebit,bool aff=true);
    void setDebitMin(float nouveauDebit);
    void setDebitMax(float nouveauDebit);
    void setStatus(Status nouveauStatus);

    void setHauteurChute(float h);
    float getProductionInstantanee() const override;

    void mettreAJourDepuisCapteur();

private:
    int    m_id;
    float  m_debit;
    float  m_debit_min;
    float  m_debit_max;
    float m_hauteurChute = 0.f; 
    Status m_status;
    std::unique_ptr<InterfaceProductionStrategy> m_strategie;
    std::shared_ptr<Capteur> m_capteurDebit; 
};
