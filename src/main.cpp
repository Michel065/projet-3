#include <iostream>
#include "factory/CentraleFactory.hpp"
#include "ParcHydroelectrique.hpp"
#include <vector>
#include <cmath>

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

int main()
{
    execution_centrale_complete(-1);
    return 0;
}