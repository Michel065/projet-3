#pragma once

#include <vector>
#include <memory>
#include "Centrale.hpp"
#include "composite/Iproduction.hpp"

class ParcHydroelectrique: public IProducteur
{
public:
    void ajouterCentrale(std::unique_ptr<Centrale> centrale);

    int getNbCentrales() const;
    const Centrale* getCentrale(std::size_t index) const;

    void mettreAJour();                   
    float getProductionInstantanee() const override;

    void print_Production_parc() const;
    void print_Production_parc_detail() const;

private:
    std::vector<std::unique_ptr<Centrale>> m_centrales;
};
