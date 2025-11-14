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

int main()
{
    test_print_data(0);
    return 0;
}