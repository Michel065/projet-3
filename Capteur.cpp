#include "Capteur.hpp"
#include "SourceDonnees.hpp"

Capteur::Capteur(std::shared_ptr<SourceDonnees> source, TypeMesure type)
    : m_source(std::move(source)),
      m_type(type)
{}

float Capteur::lire() const
{
    const LigneDonnees& ligne = m_source->ligneCourante();

    switch (m_type)
    {
    case TypeMesure::Elav:      return ligne.elav;
    case TypeMesure::Qtot:      return ligne.qtot;
    case TypeMesure::Qturb:      return ligne.qturb;
    case TypeMesure::Qvan:      return ligne.qvan;
    case TypeMesure::NivAmont:  return ligne.nivAmont;

    case TypeMesure::Q1: return ligne.q[0];
    case TypeMesure::Q2: return ligne.q[1];
    case TypeMesure::Q3: return ligne.q[2];
    case TypeMesure::Q4: return ligne.q[3];
    case TypeMesure::Q5: return ligne.q[4];

    case TypeMesure::P1: return ligne.p[0];
    case TypeMesure::P2: return ligne.p[1];
    case TypeMesure::P3: return ligne.p[2];
    case TypeMesure::P4: return ligne.p[3];
    case TypeMesure::P5: return ligne.p[4];
    }
    //sinon
    return 0.f;
}
