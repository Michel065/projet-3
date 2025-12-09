#pragma once

#include <memory>
#include <vector>
#include "Status.hpp"
#include "Reservoir.hpp"
#include "Turbine.hpp"
#include <numeric> 
#include "composite/Iproduction.hpp"

//nouveau test
#include "RepartitionDebit.hpp"
#include <unordered_map>

class Centrale: public IProducteur
{
public:
    Centrale(int id,
             Status statusInitial,
             std::shared_ptr<Reservoir> reservoirAmont,
             std::shared_ptr<Capteur> capteurQturb,
             std::unique_ptr<ModuleRepartitionDebit>& moduleRepartition);

    int    getId()     const;
    Status getStatus() const;
    void   setStatus(Status s);

    void ajouterTurbine(std::unique_ptr<Turbine> turbine);
    int getNbTurbines() const;
    const Turbine* getTurbine(int index) const;
    Turbine* getTurbine(int index);

    float calculerHauteurChute() const;   
    float getProductionInstantanee() const override;
    std::vector<std::pair<int, float>> getProductionInstantaneeDetail() const;

    void print_Production_centrale() const;
    void print_Production_centrale_detail() const;

    ResultatRepartition mettreAJour();
    
    //test repartition
    const CommandeTurbine& getCommandeTurbine(int idTurbine)const;
    void clearCommandeTurbine(int idTurbine);
    void clearToutesCommandes();
    void setCommandeTurbine(int idTurbine, const CommandeTurbine& cmd);
    ResultatRepartition repartirDebit(float debitTotal);
    

private:
    int m_id;
    Status m_status;

    std::shared_ptr<Reservoir> m_reservoirAmont;

    std::vector<std::unique_ptr<Turbine>> m_turbines;

    //test repartition
    std::shared_ptr<Capteur> m_capteurQturb;
    std::unique_ptr<ModuleRepartitionDebit> m_moduleRepartition;
    std::unordered_map<int, CommandeTurbine> m_commandesTurbines;
    std::vector<EtatTurbine> construireEtatsTurbines() const;
};
