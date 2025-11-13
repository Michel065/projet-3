#pragma once
#include <memory>

enum class TypeMesure
{
    Elav,
    Qtot,
    Qturb,
    Qvan,
    NivAmont,
    Q1,
    Q2,
    Q3,
    Q4,
    Q5,
    P1,
    P2,
    P3,
    P4,
    P5,
};

class Capteur
{
public:
    Capteur(std::shared_ptr<SourceDonnees> source, TypeMesure type);

    float lire() const;

private:
    std::shared_ptr<SourceDonnees> m_source;
    TypeMesure m_type;
};
