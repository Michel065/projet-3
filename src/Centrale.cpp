#include "Centrale.hpp"

Centrale::Centrale(int id,
                   Status statusInitial,
                   std::shared_ptr<Reservoir> reservoirAmont,
                   std::shared_ptr<Capteur> capteurQturb,
                   std::unique_ptr<ModuleRepartitionDebit>& moduleRepartition)
    : m_id(id),
      m_status(statusInitial),
      m_reservoirAmont(std::move(reservoirAmont)),
      m_capteurQturb(std::move(capteurQturb)),
      m_moduleRepartition(std::move(moduleRepartition))
{
}

int Centrale::getId() const{return m_id;}

Status Centrale::getStatus() const{return m_status;}

void Centrale::setStatus(Status s){m_status = s;}

void Centrale::ajouterTurbine(std::unique_ptr<Turbine> turbine)
{
    if (turbine) {
        m_turbines.push_back(std::move(turbine));
    }
}

int Centrale::getNbTurbines() const {return m_turbines.size();}

float Centrale::calculerHauteurChute() const
{
    if (!m_reservoirAmont) {
        return 0.f;
    }

    float nivAmont = m_reservoirAmont->getNiveau();

    float Qtot = 0.f;
    for (const auto& t : m_turbines) {
        if (t) {
            Qtot += t->getDebit();
        }
    }

    const float p1 = -1.453e-6f;
    const float p2 =  0.007022f;
    const float p3 = 99.98f;

    float elav_calc = p1 * Qtot * Qtot + p2 * Qtot + p3;

    return nivAmont - elav_calc;
}

float Centrale::getProductionInstantanee() const
{
    if (m_status != Status::Marche) {
        return 0.f;
    }
    auto details = getProductionInstantaneeDetail();
    float somme = 0.f;
    for (const auto& [id, prod] : details) {
        somme += prod;
    }

    return somme;
}

std::vector<std::pair<int, float>> Centrale::getProductionInstantaneeDetail() const
{
    std::vector<std::pair<int, float>> details;

    if (m_status != Status::Marche) {
        for (const auto& t : m_turbines) {
            if (!t) continue;
            details.emplace_back(t->getId(), 0.f);
        }
        return details;
    }
    for (const auto& t : m_turbines) {
        if (!t) continue;
        float p = t->getProductionInstantanee();
        details.emplace_back(t->getId(), p);
    }

    return details;
}

void Centrale::print_Production_centrale() const
{
    float H = calculerHauteurChute();
    float productionTotale = getProductionInstantanee();

    std::cout << "===== PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "===============================\n";
}

void Centrale::print_Production_centrale_detail() const
{
    float H = calculerHauteurChute();
    float productionTotale = getProductionInstantanee();

    std::cout << "===== DETAIL PRODUCTION CENTRALE " << m_id << " =====\n";
    std::cout << "Hauteur de chute calculee : " << H << "\n";
    std::cout << "Production totale : " << productionTotale << " MW\n";
    std::cout << "---------------------------------------------\n";

    for (const auto& t : m_turbines)
    {
        if (!t) continue;
        t->setHauteurChute(H);
        float prod = t->getProductionInstantanee();

        std::cout << " - Turbine " << t->getId()
                  << " | debit = " << t->getDebit()
                  << " | prod = " << prod << " MW\n";
    }

    std::cout << "===============================\n";
}

const Turbine* Centrale::getTurbine(int index) const
{
    if (index < 0 || index >= getNbTurbines())
        return nullptr;

    return m_turbines[index].get();
}

Turbine* Centrale::getTurbine(int index)// uniquement utilise dans le cas ou on veux simuler depuis l'exterieur
{
    if (index < 0 || index >= getNbTurbines())
        return nullptr;

    return m_turbines[index].get();
}

//test repartition
std::vector<EtatTurbine> Centrale::construireEtatsTurbines() const
{
    std::vector<EtatTurbine> etats;
    etats.reserve(m_turbines.size());

    for (const auto& t : m_turbines) {
        if (!t) continue;
        EtatTurbine e;
        e.id           = t->getId();
        e.status       = t->getStatus();
        e.debitMin     = t->getDebitMin();
        e.debitMax     = t->getDebitMax();
        e.debitActuel  = t->getDebit();
        etats.push_back(e);
    }

    return etats;
}

void Centrale::setCommandeTurbine(int idTurbine, const CommandeTurbine& cmd)
{
    // Si aucune contrainte n'est active, on supp
    if (!cmd.forceDebit && !cmd.forceStatus) {
        m_commandesTurbines.erase(idTurbine);
        return;
    }

    m_commandesTurbines[idTurbine] = cmd;
}

const CommandeTurbine& Centrale::getCommandeTurbine(int idTurbine) const
{
    static const CommandeTurbine cmdNeutre{};
    auto it = m_commandesTurbines.find(idTurbine);
    if (it != m_commandesTurbines.end())
        return it->second;
    return cmdNeutre;
}

void Centrale::clearCommandeTurbine(int idTurbine)
{
    m_commandesTurbines.erase(idTurbine);
}

void Centrale::clearToutesCommandes()
{
    m_commandesTurbines.clear();
}

ResultatRepartition Centrale::repartirDebit(float debitTotal)
{
    if (!m_moduleRepartition) {
        ResultatRepartition res;
        res.repartitionPossible = false;
        res.debitVanne = debitTotal;
        res.message = "Aucun module de repartition de debit n'est defini.";
        return res;
    }

    auto etats = construireEtatsTurbines();
    auto res = m_moduleRepartition->calculer(etats, debitTotal, m_commandesTurbines);

    std::size_t idx = 0;
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (idx < res.debitsTurbines.size()) {
            t->setDebit(res.debitsTurbines[idx]);
        }
        ++idx;
    }

    return res;
}

ResultatRepartition Centrale::mettreAJour() {
    ResultatRepartition res;
    float hauteurChute = calculerHauteurChute();

    if (!m_moduleRepartition || !m_capteurQturb) {
        for (const auto& t : m_turbines) {
            if (!t) continue;
            t->mettreAJourDepuisCapteur();
            t->setHauteurChute(hauteurChute);
        }
        if (m_reservoirAmont) m_reservoirAmont->mettreAJour();

        if (!m_moduleRepartition)
            res.message = "Aucun module de repartition defini, mise a jour simple.";
        else if (!m_capteurQturb)
            res.message = "Capteur Qturb non defini, mise a jour simple.";

        res.repartitionPossible = false;
        return res;
    }

    for (const auto& t : m_turbines) {
        if (!t) continue;
        t->mettreAJourDepuisCapteur();
    }

    float debitTotal = 0.f;
    if (m_capteurQturb) {
        debitTotal = m_capteurQturb->lire();
    }
    else{std::cout << "Centrale " << m_id<< " : capteur Qturb non defini\n";}

    res = repartirDebit(debitTotal);

    std::size_t idx = 0;
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (idx < res.debitsTurbines.size())
            t->setDebit(res.debitsTurbines[idx]);
        t->setHauteurChute(hauteurChute);
        ++idx;
    }

    if (!res.repartitionPossible && !res.message.empty()) {
        std::cout << "Centrale " << m_id << " : " << res.message << '\n';
    }

    if (m_reservoirAmont) m_reservoirAmont->mettreAJour();
    return res;
}

void Centrale::setDebitMinTurbine(int idTurbine, float newMin)
{
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (t->getId() == idTurbine) {
            t->setDebitMin(newMin);
            return;
        }
    }
    std::cout << "Centrale " << m_id << " : turbine " << idTurbine << " introuvable (setDebitMin)\n";
}

void Centrale::setDebitMaxTurbine(int idTurbine, float newMax)
{
    for (auto& t : m_turbines) {
        if (!t) continue;
        if (t->getId() == idTurbine) {
            t->setDebitMax(newMax);
            return;
        }
    }
    std::cout << "Centrale " << m_id << " : turbine " << idTurbine << " introuvable (setDebitMax)\n";
}
