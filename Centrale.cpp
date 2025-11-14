#include "Centrale.hpp"

Centrale::Centrale(int id,
                   Status statusInitial,
                   std::shared_ptr<Reservoir> reservoirAmont)
    : m_id(id),
      m_status(statusInitial),
      m_reservoirAmont(std::move(reservoirAmont))
{
}

int Centrale::getId() const{return m_id;}

Status Centrale::getStatus() const{return m_status;}

void Centrale::setStatus(Status s){m_status = s;}

void Centrale::ajouterTurbine(std::unique_ptr<Turbine> turbine)
{
    if (turbine) {
        m_turbines.push_back(std::move(turbine));
    }
}

int Centrale::getNbTurbines() const {return m_turbines.size();}

float Centrale::calculerHauteurChute() const
{
    if (!m_reservoirAmont) {
        return 0.f;
    }

    float nivAmont = m_reservoirAmont->getNiveau();

    float Qtot = 0.f;
    for (const auto& t : m_turbines) {
        if (t) {
            Qtot += t->getDebit();
        }
    }

    const float p1 = -1.453e-6f;
    const float p2 =  0.007022f;
    const float p3 = 99.98f;

    float elav_calc = p1 * Qtot * Qtot + p2 * Qtot + p3;

    return nivAmont - elav_calc;
}


float Centrale::calculerProductionTotale() const
{
    if (m_status != Status::Marche) {
        return 0.f;
    }

    float hauteurChute = calculerHauteurChute();
    float somme = 0.f;

    for (const auto& t : m_turbines) {
        if (t) {
            somme += t->getProduction(hauteurChute);
        }
    }

    return somme;
}

void Centrale::print_Production_centrale() const
{
    float H = calculerHauteurChute();
    float productionTotale = calculerProductionTotale();

    std::cout << "===== PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "===============================\n";
}

void Centrale::print_Production_centrale_detail() const
{
    float H = calculerHauteurChute();
    float productionTotale = calculerProductionTotale();

    std::cout << "===== DETAIL PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute calculee : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "---------------------------------------------\n";

    for (const auto& t : m_turbines)
    {
        if (!t) continue;

        float prod = t->getProduction(H);

        std::cout << " - Turbine " << t->getId()
                  << " | debit = " << t->getDebit()
                  << " | prod = " << prod << " MW\n";
    }

    std::cout << "===============================\n";
}

Turbine* Centrale::getTurbine(int index) const
{
    if (index < 0 || index >= getNbTurbines())
        return nullptr;

    return m_turbines[index].get();
}
