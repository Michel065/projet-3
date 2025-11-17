#pragma once
#include <memory>
#include "../strategie/ProductionPolynomiale.hpp"

class StratTurbineFactory
{
public:
    static std::unique_ptr<ProductionPolynomiale> makeStratTurbine1();
    static std::unique_ptr<ProductionPolynomiale> makeStratTurbine2();
    static std::unique_ptr<ProductionPolynomiale> makeStratTurbine3();
    static std::unique_ptr<ProductionPolynomiale> makeStratTurbine4();
    static std::unique_ptr<ProductionPolynomiale> makeStratTurbine5();
};
