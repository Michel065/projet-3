#include <iostream>
#include "factory/CentraleFactory.hpp"
#include "ParcHydroelectrique.hpp"

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

    const auto& ld = src->ligneCourante();

    std::cout << "===== EXECUTION CENTRALE DATA =====\n";
    int x=0;
    do{
        x++;
        std::cout << "\n===== Ligne "<<x<<" =====\n";
        centrale->mettreAJour();
        float H = centrale->calculerHauteurChute();
        float Psim = centrale->getProductionInstantanee();

        // Production reelle = somme P1..P5
        float Preal = 0.f;
        for (int i = 0; i < 5; ++i)
            Preal += ld.p[i];

        std::cout << "Elav     = " << ld.elav << "\n";
        std::cout << "Hauteur  = " << H << "\n";
        std::cout << "\nProduction totale simulee : " << Psim << "\n";
        std::cout << "Production totale reelle   : " << Preal << "\n";
        std::cout << "ERREUR = " << (Psim - Preal) << "\n";
    }while(src->avancer() && x<nbr_data);
}

int main()
{
    execution_centrale_complete();
    return 0;
}