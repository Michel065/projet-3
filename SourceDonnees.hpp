#pragma once
#include <iostream>

struct LigneDonnees
{
    float elav;
    float qtot;
    float qvan;
    float nivAmont;
    float q[5];
    float p[5];
};

class SourceDonnees
{
public:
    SourceDonnees(std::String source="DataProjet2025.xlsx");

    const LigneDonnees& ligneCourante() const;
    bool avancer(); // passe à la ligne suivante, retourne false si fin

private:
    std::vector<LigneDonnees> m_lignes;
    std::size_t m_indexCourant = 0;
};
