#include <iostream>
#include "factory/CentraleFactory.hpp"
#include "ParcHydroelectrique.hpp"
#include <vector>
#include <cmath>
#include <fstream>

void print_test_centrale_complete_1_valeur()
{
    auto src = std::make_shared<SourceDonnees>();
    auto centrale = CentraleFactory::creerCentraleStandard(src);

    const auto& ld = src->ligneCourante();

    std::cout << "===== TEST CENTRALE (1 seule valeur) =====\n";

    centrale->mettreAJour();
    float H = centrale->calculerHauteurChute();
    float Psim = centrale->getProductionInstantanee();

    // Production reelle = somme P1..P5
    float Preal = 0.f;
    for (int i = 0; i < 5; ++i)
        Preal += ld.p[i];

    std::cout << "Elav     = " << ld.elav << "\n";
    std::cout << "Hauteur  = " << H << "\n\n";

    for (int i = 0; i < centrale->getNbTurbines(); ++i)
    {
        auto* t = centrale->getTurbine(i);
        float q = t->getDebit();
        t->setHauteurChute(H);
        float pSim = t->getProductionInstantanee();
        float pReal = ld.p[i];

        std::cout << "Turbine " << (i+1)
                  << "  Q=" << q
                  << "  ProdSim=" << pSim
                  << "  ProdReal=" << pReal
                  << "\n";
    }

    std::cout << "\nProduction totale simulee : " << Psim << "\n";
    std::cout << "Production totale reelle   : " << Preal << "\n";
    std::cout << "ERREUR = " << (Psim - Preal) << "\n";

    std::cout << "=========================================\n";
}

void print_test_parc()
{
    auto src = std::make_shared<SourceDonnees>();

    ParcHydroelectrique parc;

    parc.ajouterCentrale(CentraleFactory::creerCentraleStandard(src));

    parc.mettreAJour();

    parc.print_Production_parc();
    std::cout<<std::endl;
    parc.print_Production_parc_detail();
}

void execution_centrale_complete(int nbr_data=10){
    auto src = std::make_shared<SourceDonnees>();
    auto centrale = CentraleFactory::creerCentraleStandard(src);


    std::cout << "===== EXECUTION CENTRALE DATA =====\n";
    int x=0;
    std::vector<float> erreurs;
    do{
        x++;
        std::cout << "\n===== Ligne "<<x<<" =====\n";
        centrale->mettreAJour();
        float H = centrale->calculerHauteurChute();
        float Psim = centrale->getProductionInstantanee();

        auto& ld = src->ligneCourante();
        // Production reelle = somme P1..P5
        float Preal = 0.f;
        for (int i = 0; i < 5; ++i)
            Preal += ld.p[i];

        float err = Psim - Preal;
        erreurs.push_back(err);

        std::cout << "Elav     = " << ld.elav << "\n";
        std::cout << "Hauteur  = " << H << "\n";
        std::cout << "\nProduction totale simulee : " << Psim << "\n";
        std::cout << "Production totale reelle   : " << Preal << "\n";
        std::cout << "ERREUR SIMU : " << err << "\n";

    }while(src->avancer() && (x<nbr_data || nbr_data==-1));

    if (!erreurs.empty())
    {
        float somme     = 0.f;
        float somme_abs = 0.f;
        float min_err   = erreurs[0];
        float max_err   = erreurs[0];

        for (float e : erreurs)
        {
            somme += e;
            somme_abs += std::fabs(e);
            if (e < min_err) min_err = e;
            if (e > max_err) max_err = e;
        }

        float moyenne = somme / erreurs.size();
        float moyenne_abs = somme_abs / erreurs.size();

        std::cout << "\n===== RESUME ERREURS =====\n";
        std::cout << "Nombre de lignes analysees : " << erreurs.size() << "\n";
        std::cout << "Erreur moyenne             : " << moyenne << "\n";
        std::cout << "Erreur absolue moyenne     : " << moyenne_abs << "\n";
        std::cout << "Erreur min                 : " << min_err << "\n";
        std::cout << "Erreur max                 : " << max_err << "\n";
        std::cout << "===========================\n";
    }
}

// en plus c'est nouveau du detail pour le rapport pour essayer d'avoir un premier visuel
struct HistoriqueTurbine
{
    int   ligne;
    int   idTurbine;
    float Psim;
    float Preal;
};

struct HistoriqueCentrale
{
    int   ligne;
    float PsimTot;
    float PrealTot;
};

void create_graph_from_historique(const std::vector<HistoriqueTurbine>& histoTurbines,
                                  const std::vector<HistoriqueCentrale>& histoCentrale)
{
    
    /*
    Conseil de chat pour pouvoir avoir une visualisation avant de finaliser l'interface
    */
    
    
    std::ofstream ft("data/historique_turbines.csv");
    if (!ft) {
        std::cout << "Erreur : impossible d'ouvrir data/historique_turbines.csv\n";
        return;
    }

    // ligne, id_turbine, P_sim, P_real
    ft << "ligne;id_turbine;P_sim;P_real\n";
    for (const auto& h : histoTurbines) {
        ft << h.ligne << ";"
           << h.idTurbine << ";"
           << h.Psim << ";"
           << h.Preal << "\n";
    }

    // 2) Historique centrale
    std::ofstream fc("data/historique_centrale.csv");
    if (!fc) {
        std::cout << "Erreur : impossible d'ouvrir data/historique_centrale.csv\n";
        return;
    }

    // ligne, P_sim_tot, P_real_tot
    fc << "ligne;P_sim_tot;P_real_tot\n";
    for (const auto& h : histoCentrale) {
        fc << h.ligne << ";"
           << h.PsimTot << ";"
           << h.PrealTot << "\n";
    }

    std::cout << "Fichiers d'historique generes dans data/.\n";
}

void execution_centrale_detail(int nbr_data = 10)
{
    auto src      = std::make_shared<SourceDonnees>();
    auto centrale = CentraleFactory::creerCentraleStandard(src);

    std::vector<HistoriqueTurbine>  histoTurbines;
    std::vector<HistoriqueCentrale> histoCentrale;

    std::cout << "===== EXECUTION CENTRALE DATA (DETAIL) =====\n";
    int x = 0;

    do {
        std::cout << "\n===== Ligne " << (x + 1) << " =====\n";

        centrale->mettreAJour();

        auto details = centrale->getProductionInstantaneeDetail();
        const auto& ld = src->ligneCourante();

        float PsimTot  = 0.f;
        float PrealTot = 0.f;

        for (const auto& [id, Psim] : details) {
            int idx = id - 1;
            float Preal = 0.f;
            if (idx >= 0 && idx < 5) {
                Preal = ld.p[idx];
            }

            float err = Psim - Preal;

            std::cout << "Turbine " << id
                      << " : P_sim = "  << Psim
                      << " ; P_real = " << Preal
                      << " ; erreur = " << err
                      << "\n";

            PsimTot  += Psim;
            PrealTot += Preal;

            histoTurbines.push_back(HistoriqueTurbine{
                x + 1, id, Psim, Preal
            });
        }

        std::cout << "Production totale simulee : " << PsimTot  << "\n";
        std::cout << "Production totale reelle   : " << PrealTot << "\n";
        std::cout << "Erreur globale = " << (PsimTot - PrealTot) << "\n";

        histoCentrale.push_back(HistoriqueCentrale{
            x + 1, PsimTot, PrealTot
        });
        ++x;
    } while (src->avancer() && (x < nbr_data || nbr_data == -1));
    create_graph_from_historique(histoTurbines, histoCentrale);
}

int main()
{
    execution_centrale_detail(-1);
    return 0;
}