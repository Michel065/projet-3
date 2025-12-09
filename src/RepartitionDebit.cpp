#include "RepartitionDebit.hpp"
#include <algorithm>
#include <numeric>

namespace {
    float clamp(float v, float vmin, float vmax) {
        return std::max(vmin, std::min(v, vmax));
    }
}

ResultatRepartition ModuleRepartitionDebit::calculer(const std::vector<EtatTurbine>& etats,
                                                     float debitTotal,
                                                     const std::unordered_map<int, CommandeTurbine>& commandes) const
{
    ResultatRepartition res;
    const std::size_t n = etats.size();
    res.debitsTurbines.assign(n, 0.f);
    res.debitVanne = 0.f;
    res.repartitionPossible = true;
    res.message.clear();

    if (n == 0) {
        res.debitVanne = debitTotal;
        res.repartitionPossible = false;
        res.message = "Aucune turbine disponible, tout passe par la vanne.";
        return res;
    }

    std::vector<float> minAut(n, 0.f);
    std::vector<float> maxAut(n, 0.f);

    for (std::size_t i = 0; i < n; ++i) {
        const auto& e   = etats[i];
        auto        it  = commandes.find(e.id);

        Status statusEffectif = e.status;
        bool   statusForce    = false;

        if (it != commandes.end() && it->second.forceStatus) {
            statusEffectif = it->second.statusImpose;
            statusForce    = true;
        }

        // 1) Cas bloqués
        // - si le statut est force et != Marche -> turbine interdite
        // - si le statut n'est pas force mais capteur indique Maintenance -> interdite
        if ((statusForce && statusEffectif != Status::Marche) ||
            (!statusForce && statusEffectif == Status::Maintenance))
        {
            minAut[i] = 0.f;
            maxAut[i] = 0.f;
            continue;
        }

        // 2) Ici :
        // - Marche
        // - ou Arret non force (la centrale peut la redemarrer)
        float dmin = e.debitMin;
        float dmax = e.debitMax;

        if (it != commandes.end() && it->second.forceDebit) {
            float d = clamp(it->second.debitImpose, dmin, dmax);
            dmin = d;
            dmax = d;
        }

        minAut[i] = dmin;
        maxAut[i] = dmax;
    }

    float sommeMax = std::accumulate(maxAut.begin(), maxAut.end(), 0.f);

    if (sommeMax <= 0.f) {
        res.debitVanne = debitTotal;
        res.repartitionPossible = false;
        res.message = "Toutes les turbines sont indisponibles, tout passe par la vanne.";
        return res;
    }

    float debitTurbinesCible = std::min(debitTotal, sommeMax);


    for (std::size_t i = 0; i < n; ++i) {
        if (maxAut[i] <= 0.f) {
            res.debitsTurbines[i] = 0.f;
            continue;
        }
        float proportion = maxAut[i] / sommeMax;
        float di = debitTurbinesCible * proportion;
        di = clamp(di, minAut[i], maxAut[i]);
        res.debitsTurbines[i] = di;
    }

    float sommeReelle = std::accumulate(res.debitsTurbines.begin(), res.debitsTurbines.end(), 0.f);
    res.debitVanne = std::max(0.f, debitTotal - sommeReelle);

    if (debitTotal > sommeMax) {
        res.repartitionPossible = false;
        res.message =
            "Debit total demande superieur au maximum des turbines. "
            "Turbines saturees, vanne evacuant "
            + std::to_string(res.debitVanne);
    }

    return res;
}
