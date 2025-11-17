#pragma once

#include <memory>
#include <vector>
#include "Status.hpp"
#include "Reservoir.hpp"
#include "Turbine.hpp"
#include <numeric> 
#include "composite/Iproduction.hpp"

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
    int getNbTurbines() const;
    const Turbine* getTurbine(int index) const;
    Turbine* getTurbine(int index);

    float calculerHauteurChute() const;   
    float getProductionInstantanee() const override;

    void print_Production_centrale() const;
    void print_Production_centrale_detail() const;

    void mettreAJour();
    

private:
    int m_id;
    Status m_status;

    std::shared_ptr<Reservoir> m_reservoirAmont;

    std::vector<std::unique_ptr<Turbine>> m_turbines;
};
