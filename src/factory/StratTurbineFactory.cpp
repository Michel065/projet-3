#include "factory/StratTurbineFactory.hpp"

std::unique_ptr<InterfaceProductionStrategy> StratTurbineFactory::makeStratTurbine1()
{
    return std::make_unique<ProductionPolynomiale>(
        1.102f,        // p00
        -0.03187f,      // p10
        -0.04866f,      // p01
        0.003308f,     // p11
            0.0f,         // p20
        0.002182f,     // p02
            0.0f,         // p21
        3.638e-05f,    // p12
        -1.277e-05f     // p03
    );
}

std::unique_ptr<InterfaceProductionStrategy> StratTurbineFactory::makeStratTurbine2()
{
    return std::make_unique<ProductionPolynomiale>(
        -1.382f,        // p00
        0.09969f,      // p10
        -1.945f,        // p01
        0.09224f,      // p11
        -0.001724f,     // p20
        0.007721f,     // p02
        -0.001096f,     // p21
        -6.622e-05f,    // p12
        -1.933e-05f     // p03
    );
}

std::unique_ptr<InterfaceProductionStrategy> StratTurbineFactory::makeStratTurbine3()
{
    return std::make_unique<ProductionPolynomiale>(
        0.7799f,       // p00
        -0.02261f,      // p10
        0.1995f,       // p01
        -0.001695f,     // p11
            0.0f,         // p20
        -3.519e-05f,    // p02
            0.0f,         // p21
        7.235e-05f,    // p12
        -9.338e-06f     // p03
    );
}

std::unique_ptr<InterfaceProductionStrategy> StratTurbineFactory::makeStratTurbine4()
{
    return std::make_unique<ProductionPolynomiale>(
        20.22f,         // p00
        -0.5777f,       // p10
        -0.4586f,       // p01
        0.01151f,      // p11
            0.0f,         // p20
        0.004886f,     // p02
            0.0f,         // p21
        1.379e-05f,    // p12
        -1.882e-05f     // p03
    );
}

std::unique_ptr<InterfaceProductionStrategy> StratTurbineFactory::makeStratTurbine5()
{
    return std::make_unique<ProductionPolynomiale>(
        -212.1f,        // p00
        12.17f,       // p10
        0.004397f,    // p01
        -0.006808f,    // p11
        -0.1746f,      // p20
        0.004529f,    // p02
        0.0002936f,   // p21
        -4.211e-05f,   // p12
        -1.176e-05f    // p03
    );
}

