#pragma once
#include "InterfaceProductionStrategy.hpp"
#include <iostream>
#include <memory>

enum class Status
{
    Arret,
    Marche,
    Maintenance
};

class Turbine
{
public:
    Turbine(int id, float debitInitial, Status statusInitial,std::unique_ptr<InterfaceProductionStrategy> strategie);

    int   getId() const;
    float getDebit() const;
    float getDebitMin() const;
    float getDebitMax() const;
    Status getStatus() const;

    void setDebit(float nouveauDebit);
    void setDebitMin(float nouveauDebit);
    void setDebitMax(float nouveauDebit);
    void setStatus(Status nouveauStatus);

    
    float getProduction(float hauteur_de_chute_nette) const;

private:
    int    m_id;
    float  m_debit;
    float  m_debit_min;
    float  m_debit_max;
    Status m_status;
    std::unique_ptr<InterfaceProductionStrategy> m_strategie;
};
