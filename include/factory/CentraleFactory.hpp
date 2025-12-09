#pragma once
#include <memory>
#include "SourceDonnees.hpp"
#include "Centrale.hpp"
#include "Reservoir.hpp"
#include "Capteur.hpp"
#include "TurbineFactory.hpp"
#include "Status.hpp"

class SourceDonnees;
class Centrale;

class CentraleFactory
{
public:
    static std::unique_ptr<Centrale> creerCentraleStandard(std::shared_ptr<SourceDonnees>& src);
};
