#pragma once

#include <memory>
#include "../Turbine.hpp"
#include "../Capteur.hpp"
#include "../SourceDonnees.hpp"
#include "StratTurbineFactory.hpp"

class TurbineFactory
{
public:
    static std::unique_ptr<Turbine> makeTurbine1(std::shared_ptr<SourceDonnees> src);
    static std::unique_ptr<Turbine> makeTurbine2(std::shared_ptr<SourceDonnees> src);
    static std::unique_ptr<Turbine> makeTurbine3(std::shared_ptr<SourceDonnees> src);
    static std::unique_ptr<Turbine> makeTurbine4(std::shared_ptr<SourceDonnees> src);
    static std::unique_ptr<Turbine> makeTurbine5(std::shared_ptr<SourceDonnees> src);
};
