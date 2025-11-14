#pragma once

#include <memory>
#include "ProductionPolynomiale.hpp"

namespace StratTurbine
{
    std::unique_ptr<ProductionPolynomiale> make_turbine1();
    std::unique_ptr<ProductionPolynomiale> make_turbine2();
    std::unique_ptr<ProductionPolynomiale> make_turbine3();
    std::unique_ptr<ProductionPolynomiale> make_turbine4();
    std::unique_ptr<ProductionPolynomiale> make_turbine5();
}
