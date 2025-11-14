#pragma once

#include <vector>
#include <memory>
#include "Centrale.hpp"

class ParcHydroelectrique
{
public:
    void ajouterCentrale(std::unique_ptr<Centrale> centrale);

    int getNbCentrales() const;
    const Centrale* getCentrale(std::size_t index) const;

    void mettreAJour();                   
    float productionTotale() const;

    void print_Production_parc() const;
    void print_Production_parc_detail() const;

private:
    std::vector<std::unique_ptr<Centrale>> m_centrales;
};
