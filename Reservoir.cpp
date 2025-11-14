#include "Reservoir.hpp"

Reservoir::Reservoir(int id, std::shared_ptr<Capteur> capteurNiveau)
    : m_id(id),
      m_capteurNiveau(std::move(capteurNiveau)),
      m_niveau(0.f)
{
    mettreAJour();
}

int Reservoir::getId() const
{
    return m_id;
}

float Reservoir::getNiveau()const
{
    return m_niveau;
}

void Reservoir::mettreAJour()
{
    if (m_capteurNiveau) {
        m_niveau = m_capteurNiveau->lire();
    }
}
