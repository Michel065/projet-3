#include "main.hpp"

void test_print_data(int nbr=10){
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

void test_print_turbine_1(int nbr=5)
{
    std::cout << "===== TEST TURBINE 1 =====\n";
    auto src = std::make_shared<SourceDonnees>();
    Capteur capteurNivAmont(src, TypeMesure::NivAmont);
    
    //pour init la turbine
    auto capteurDebitQ1 = std::make_shared<Capteur>(src, TypeMesure::Q1);
    auto strategie = std::make_unique<ProductionPolynomiale>( // on utilie le pattern strategie pour definir nos fonctions de calculs et rendre nos turbines plus propre
        1.102f,         // p00
       -0.03187f,       // p10
       -0.04866f,       // p01
        0.003308f,      // p11
         0.0f,          // p20 (non utilisé pour Turbine 1 → DegX = 1)
        0.002182f,      // p02
         0.0f,          // p21 (non utilisé)
        3.638e-05f,     // p12
       -1.277e-05f      // p03
    );
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

int main()
{
    test_print_turbine_1();
    return 0;
}