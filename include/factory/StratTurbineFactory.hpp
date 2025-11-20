#pragma once
#include <memory>
#include "../strategie/ProductionPolynomiale.hpp"

class StratTurbineFactory
{
public:
    static std::unique_ptr<InterfaceProductionStrategy> makeStratTurbine1();
    static std::unique_ptr<InterfaceProductionStrategy> makeStratTurbine2();
    static std::unique_ptr<InterfaceProductionStrategy> makeStratTurbine3();
    static std::unique_ptr<InterfaceProductionStrategy> makeStratTurbine4();
    static std::unique_ptr<InterfaceProductionStrategy> makeStratTurbine5();
};
