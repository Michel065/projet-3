#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "Status.hpp"

struct CommandeTurbine {
    bool   forceDebit   = false;
    float  debitImpose  = 0.f;
    bool   forceStatus  = false;
    Status statusImpose = Status::Arret;
};

struct EtatTurbine {
    int    id;
    Status status;
    float  debitMin;
    float  debitMax;
    float  debitActuel;
};

struct ResultatRepartition {
    std::vector<float> debitsTurbines;
    float              debitVanne = 0.f;
    bool               repartitionPossible = true;
    std::string        message;
};

class ModuleRepartitionDebit {
public:
    ResultatRepartition calculer(const std::vector<EtatTurbine>& etats, float debitTotal,const std::unordered_map<int, CommandeTurbine>& commandes) const;
};
