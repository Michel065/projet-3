#include "Turbine.hpp"

Turbine::Turbine(int id, Status statusInitial,std::unique_ptr<InterfaceProductionStrategy> strategie,std::shared_ptr<Capteur> capteurDebit)
    : m_id(id),
      m_status(statusInitial),
      m_strategie(std::move(strategie)),
      m_capteurDebit(std::move(capteurDebit))
{
    setDebitMin(0);
    setDebitMax(160);
    if (m_capteurDebit) {
        setDebit(m_capteurDebit->lire());
    }
}

int Turbine::getId() const
{
    return m_id;
}

float Turbine::getDebit()const
{
    return m_debit;
}

float Turbine::getDebitMin() const
{
    return m_debit_min;
}

float Turbine::getDebitMax() const
{
    return m_debit_max;
}

Status Turbine::getStatus() const
{
    return m_status;
}

void Turbine::setDebit(float nouveauDebit)
{
    if (nouveauDebit < m_debit_min) {std::cout << "Turbine " << m_id << " : erreur, debit inférieur au debit_min !" << std::endl;return;}
    if (nouveauDebit > m_debit_max) {std::cout << "Turbine " << m_id << " : erreur, debit supérieur au debit_max !" << std::endl;return;}
    /*uniquement si nv debit valide*/
    m_debit = nouveauDebit;
}

void Turbine::setDebitMin(float nouveauDebit)
{
    m_debit_min = nouveauDebit;
}

void Turbine::setDebitMax(float nouveauDebit)
{
    m_debit_max = nouveauDebit;
}

void Turbine::setStatus(Status nouveauStatus)
{
    m_status = nouveauStatus;
}


float Turbine::getProduction(float hauteur_de_chute_nette)const
{
    if (m_status != Status::Marche) return 0.f;//la turbine marche pas

    if (m_debit < m_debit_min || m_debit > m_debit_max) {
        /*on est pas dans les bornes donc erreur*/
        std::cout<<"Turbine "<<m_id<<", erreur debit invalide !"<< std::endl;
        return 0.f;
    }
    if(m_debit==0.f){return 0.f;}
    return m_strategie->compute(hauteur_de_chute_nette, m_debit);
}

void Turbine::mettreAJourDepuisCapteur()
{
    if (!m_capteurDebit) //on sait jamais on verifie
        return;
    setDebit(m_capteurDebit->lire());
}
