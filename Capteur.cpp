#include "Capteur.hpp"
Capteur::Capteur(std::shared_ptr<SourceDonnees> source, TypeMesure type)
        : m_source(std::move(source)),
          m_type(type)
{}

Capteur::lire()const{

}