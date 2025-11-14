#pragma once

#include <memory>
#include "Capteur.hpp"

class Reservoir
{
public:
    Reservoir(int id, std::shared_ptr<Capteur> capteurNiveau);

    int   getId() const;
    float getNiveau();        
    void  mettreAJour();

private:
    int m_id;
    std::shared_ptr<Capteur> m_capteurNiveau;
    float m_niveau = 0.f;
};
