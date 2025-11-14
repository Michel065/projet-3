#include "main.hpp"

std::unique_ptr<Centrale> creation_centrale(std::shared_ptr<SourceDonnees> src)
{
    auto capteurNivAmont = std::make_shared<Capteur>(src, TypeMesure::NivAmont);

    auto resAmont = std::make_shared<Reservoir>(1, capteurNivAmont);

    auto centrale = std::make_unique<Centrale>(
        1,
        Status::Marche,
        resAmont
    );

    //turbine 1:
    auto capteurQ1 = std::make_shared<Capteur>(src, TypeMesure::Q1);
    centrale->ajouterTurbine(std::make_unique<Turbine>(
        1, Status::Marche, StratTurbine::make_turbine1(), capteurQ1));

    //turbine 2:
    auto capteurQ2 = std::make_shared<Capteur>(src, TypeMesure::Q2);
    centrale->ajouterTurbine(std::make_unique<Turbine>(
        2, Status::Marche, StratTurbine::make_turbine2(), capteurQ2));

    //turbine 3:
    auto capteurQ3 = std::make_shared<Capteur>(src, TypeMesure::Q3);
    centrale->ajouterTurbine(std::make_unique<Turbine>(
        3, Status::Marche, StratTurbine::make_turbine3(), capteurQ3));

    //turbine 4:
    auto capteurQ4 = std::make_shared<Capteur>(src, TypeMesure::Q4);
    centrale->ajouterTurbine(std::make_unique<Turbine>(
        4, Status::Marche, StratTurbine::make_turbine4(), capteurQ4));

    //turbine 5:
    auto capteurQ5 = std::make_shared<Capteur>(src, TypeMesure::Q5);
    centrale->ajouterTurbine(std::make_unique<Turbine>(
        5, Status::Marche, StratTurbine::make_turbine5(), capteurQ5));

    return centrale;
}

void print_test_centrale_complete()
{
    auto src = std::make_shared<SourceDonnees>();
    auto centrale = creation_centrale(src);

    const auto& ld = src->ligneCourante();

    std::cout << "===== TEST CENTRALE (1 seule valeur) =====\n";

    float H = centrale->calculerHauteurChute();
    float Psim = centrale->calculerProductionTotale();

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
        float pSim = t->getProduction(H);
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


int main()
{
    print_test_centrale_complete();
    return 0;
}