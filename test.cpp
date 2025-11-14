#include "test.hpp"

void test_print_data(int nbr){
    std::cout << "=== Test SourceDonnees ===\n";
    SourceDonnees data;
    int x=0;
    data.print_ligneCourante();
    while(data.avancer() && x<nbr){
        data.print_ligneCourante();
        x++;
    }
    std::cout << "======================\n";
}

void test_print_capteur()
{
    auto src = std::make_shared<SourceDonnees>();
    Capteur capteurElav(src, TypeMesure::Elav);

    std::cout << "=== Test Capteur  ===\n";

    
    float v = capteurElav.lire();
    std::cout << "Ligne " << src->get_index_courrant() << " -> Elav = " << v << "\n";
    src->avancer();
    
    v = capteurElav.lire();
    std::cout << "Ligne " << src->get_index_courrant() << " -> Elav = " << v << "\n";
        
    std::cout << "======================\n";
}

void test_print_turbine_1(int nbr)
{
    std::cout << "===== TEST TURBINE 1 =====\n";
    auto src = std::make_shared<SourceDonnees>();
    Capteur capteurNivAmont(src, TypeMesure::NivAmont);
    
    //pour init la turbine
    auto capteurDebitQ1 = std::make_shared<Capteur>(src, TypeMesure::Q1);
    auto strategie = StratTurbine::make_turbine1();

    Turbine turbine1(1, Status::Marche, std::move(strategie),capteurDebitQ1);
    for(int tmp=0;tmp<nbr;tmp++){
        float nivAmont = capteurNivAmont.lire();
        float q1       = turbine1.getDebit();
        float H = nivAmont - 0.0f; 
        float prod = turbine1.getProduction(H);

        std::cout << "Ligne "<< src->get_index_courrant()<<":"<<std::endl;
        std::cout << "NivAmont = " << nivAmont <<std::endl;
        std::cout << "Q1       = " << q1 <<std::endl;
        std::cout << "Hauteur  = " << H <<std::endl;
        std::cout << "Prod T1  = " << prod <<std::endl;
        std::cout << "Prod Reel  = " << src->ligneCourante().q[0] <<std::endl<<std::endl;
        src->avancer();
    }
    std::cout << "======================\n";
}

void test_print_reservoir(int nbr)
{
    auto src = std::make_shared<SourceDonnees>();
    auto capteurElav = std::make_shared<Capteur>(src, TypeMesure::Elav);

    Reservoir reservoirAval(1, capteurElav);

    std::cout << "=== Test Reservoir ===\n";

    // On lit par exemple les 5 premières lignes
    for (int i = 0; i < nbr; ++i)
    {
        std::cout << "Ligne " << i << " :\n";

        float niv = reservoirAval.getNiveau();
        std::cout << "  Niveau aval = " << niv << "\n";
        src->avancer();
    }

    std::cout << "=======================\n";
}

void test_print_centrale()
{
    std::cout << "===== TEST CENTRALE =====\n";

    auto src = std::make_shared<SourceDonnees>();
    auto capteurNivAmont = std::make_shared<Capteur>(src, TypeMesure::NivAmont);
    auto resAmont = std::make_shared<Reservoir>(1, capteurNivAmont);

    // Centrale
    Centrale centrale(1, Status::Marche, resAmont);

    // ---- Turbine 1 ----
    auto capteurQ1 = std::make_shared<Capteur>(src, TypeMesure::Q1);

    auto stratT1 = StratTurbine::make_turbine1();
    auto turbine1 = std::make_unique<Turbine>(
        1,
        Status::Marche,
        std::move(stratT1),
        capteurQ1
    );
    centrale.ajouterTurbine(std::move(turbine1));
    centrale.mettreAJour();
    
    centrale.print_Production_centrale();
    centrale.print_Production_centrale_detail();

    std::cout << "======================\n";
}