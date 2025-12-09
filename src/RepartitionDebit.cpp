#include "RepartitionDebit.hpp"
#include <algorithm>
#include <numeric>
#include <cmath> // pour std::fabs

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

    // 1) Construire les intervalles autorises [minAut, maxAut]
    for (std::size_t i = 0; i < n; ++i) {
        const auto& e   = etats[i];
        auto        it  = commandes.find(e.id);

        Status statusEffectif = e.status;
        bool   statusForce    = false;

        if (it != commandes.end() && it->second.forceStatus) {
            statusEffectif = it->second.statusImpose;
            statusForce    = true;
        }

        // Cas bloques :
        if ((statusForce && statusEffectif != Status::Marche) ||
            (!statusForce && statusEffectif == Status::Maintenance))
        {
            minAut[i] = 0.f;
            maxAut[i] = 0.f;
            continue;
        }

        float dmin = e.debitMin;
        float dmax = e.debitMax;

        // Debit impose
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

    // 2) Base = debits actuels clampés dans [minAut, maxAut]
    std::vector<float> base(n, 0.f);
    for (std::size_t i = 0; i < n; ++i) {
        if (maxAut[i] <= 0.f) {
            base[i] = 0.f;
            continue;
        }
        const auto& e = etats[i];
        float d = e.debitActuel;          // valeur mesuree actuelle
        d = clamp(d, minAut[i], maxAut[i]);
        base[i] = d;
    }

    float sommeBase = std::accumulate(base.begin(), base.end(), 0.f);

    // 3) Si la somme colle deja au debitTotal, on ne touche a rien
    const float EPS = 1e-3f;
    if (std::fabs(sommeBase - debitTotal) <= EPS) {
        res.debitsTurbines = base;
        res.debitVanne = std::max(0.f, debitTotal - sommeBase);
        return res;
    }

    // 4) Sinon, on ajuste uniquement ce qui est necessaire
    res.debitsTurbines = base;
    float delta = debitTotal - sommeBase;

    if (delta > 0.f) {
        // Il faut augmenter la somme -> marge vers le haut
        std::vector<float> margeUp(n, 0.f);
        for (std::size_t i = 0; i < n; ++i) {
            margeUp[i] = std::max(0.f, maxAut[i] - base[i]);
        }
        float sommeMargeUp = std::accumulate(margeUp.begin(), margeUp.end(), 0.f);

        if (sommeMargeUp <= 0.f) {
            res.repartitionPossible = false;
            res.message = "Debit total demande superieur au maximum des turbines. Turbines saturees.";
        } else {
            float incr = std::min(delta, sommeMargeUp);
            for (std::size_t i = 0; i < n; ++i) {
                if (margeUp[i] <= 0.f) continue;
                float add = incr * (margeUp[i] / sommeMargeUp);
                res.debitsTurbines[i] = base[i] + add;
            }
        }
    } else if (delta < 0.f) {
        // Il faut diminuer la somme -> marge vers le bas
        std::vector<float> margeDown(n, 0.f);
        for (std::size_t i = 0; i < n; ++i) {
            margeDown[i] = std::max(0.f, base[i] - minAut[i]);
        }
        float sommeMargeDown = std::accumulate(margeDown.begin(), margeDown.end(), 0.f);

        if (sommeMargeDown <= 0.f) {
            res.repartitionPossible = false;
            res.message = "Impossible de reduire le debit des turbines sans violer les debits_min.";
        } else {
            float dec = std::min(-delta, sommeMargeDown);
            for (std::size_t i = 0; i < n; ++i) {
                if (margeDown[i] <= 0.f) continue;
                float sub = dec * (margeDown[i] / sommeMargeDown);
                res.debitsTurbines[i] = base[i] - sub;
            }
        }
    }

    float sommeFinale = std::accumulate(res.debitsTurbines.begin(), res.debitsTurbines.end(), 0.f);
    res.debitVanne = std::max(0.f, debitTotal - sommeFinale);

    return res;
}
