#include "factory/CentraleFactory.hpp"

std::unique_ptr<Centrale> CentraleFactory::creerCentraleStandard(std::shared_ptr<SourceDonnees> src)
{
    auto capteurNivAmont = std::make_shared<Capteur>(src, TypeMesure::NivAmont);
    auto resAmont = std::make_shared<Reservoir>(1, capteurNivAmont);
    
    auto centrale = std::make_unique<Centrale>(1,Status::Marche,resAmont);

    centrale->ajouterTurbine(TurbineFactory::makeTurbine1(src));
    centrale->ajouterTurbine(TurbineFactory::makeTurbine2(src));
    centrale->ajouterTurbine(TurbineFactory::makeTurbine3(src));
    centrale->ajouterTurbine(TurbineFactory::makeTurbine4(src));
    centrale->ajouterTurbine(TurbineFactory::makeTurbine5(src));
    return centrale;
}
