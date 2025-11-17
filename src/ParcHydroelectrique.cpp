#include "ParcHydroelectrique.hpp"
#include <iostream>

void ParcHydroelectrique::ajouterCentrale(std::unique_ptr<Centrale> centrale)
{
    if (centrale) {
        m_centrales.push_back(std::move(centrale));
    }
}

int ParcHydroelectrique::getNbCentrales() const
{
    return m_centrales.size();
}

const Centrale* ParcHydroelectrique::getCentrale(std::size_t index) const
{
    if (index >= m_centrales.size()) return nullptr;
    return m_centrales[index].get();
}

void ParcHydroelectrique::mettreAJour()
{
    for (auto& c : m_centrales) {
        if (c) c->mettreAJour();
    }
}

float ParcHydroelectrique::getProductionInstantanee() const
{
    float somme = 0.f;

    for (const auto& c : m_centrales) {
        if (!c) continue;
        float Ptot = c->getProductionInstantanee();
        somme += Ptot;
    }

    return somme;
}

void ParcHydroelectrique::print_Production_parc() const
{
    std::cout << "===== PRODUCTION PARC =====\n";
    for (std::size_t i = 0; i < m_centrales.size(); ++i) {
        const auto* c = m_centrales[i].get();
        if (!c) continue;

        float H    = c->calculerHauteurChute();
        float Ptot = c->getProductionInstantanee();

        std::cout << "Centrale " << c->getId()
                  << " : H = " << H
                  << " | Prod = " << Ptot << " MW\n";
    }
    std::cout << "Production totale parc : " << getProductionInstantanee() << " MW\n";
    std::cout << "===========================\n";
}

void ParcHydroelectrique::print_Production_parc_detail() const
{
    std::cout << "===== DETAIL PARC =====\n";
    for (const auto& c : m_centrales) {
        if (!c) continue;
        c->print_Production_centrale_detail();
    }
    std::cout << "Production totale parc : " << getProductionInstantanee() << " MW\n";
    std::cout << "=======================\n";
}
