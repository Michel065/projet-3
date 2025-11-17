#include "factory/TurbineFactory.hpp"

std::unique_ptr<Turbine> TurbineFactory::makeTurbine1(std::shared_ptr<SourceDonnees> src)
{
    auto capteur = std::make_shared<Capteur>(src, TypeMesure::Q1);
    auto strat     = StratTurbineFactory::makeStratTurbine1();
    return std::make_unique<Turbine>(1, Status::Marche, std::move(strat), capteur);
}

std::unique_ptr<Turbine> TurbineFactory::makeTurbine2(std::shared_ptr<SourceDonnees> src)
{
    auto capteur = std::make_shared<Capteur>(src, TypeMesure::Q2);
    auto strat     = StratTurbineFactory::makeStratTurbine2();
    return std::make_unique<Turbine>(2, Status::Marche, std::move(strat), capteur);
}

std::unique_ptr<Turbine> TurbineFactory::makeTurbine3(std::shared_ptr<SourceDonnees> src)
{
    auto capteur = std::make_shared<Capteur>(src, TypeMesure::Q3);
    auto strat     = StratTurbineFactory::makeStratTurbine3();
    return std::make_unique<Turbine>(3, Status::Marche, std::move(strat), capteur);
}

std::unique_ptr<Turbine> TurbineFactory::makeTurbine4(std::shared_ptr<SourceDonnees> src)
{
    auto capteur = std::make_shared<Capteur>(src, TypeMesure::Q4);
    auto strat     = StratTurbineFactory::makeStratTurbine4();
    return std::make_unique<Turbine>(4, Status::Marche, std::move(strat), capteur);
}

std::unique_ptr<Turbine> TurbineFactory::makeTurbine5(std::shared_ptr<SourceDonnees> src)
{
    auto capteur = std::make_shared<Capteur>(src, TypeMesure::Q5);
    auto strat     = StratTurbineFactory::makeStratTurbine5();
    return std::make_unique<Turbine>(5, Status::Marche, std::move(strat), capteur);
}

