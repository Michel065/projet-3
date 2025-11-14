#pragma once
#include <vector>
#include <string>

struct LigneDonnees
{
    float elav;
    float qtot;
    float qturb;
    float qvan;
    float nivAmont;
    float q[5];
    float p[5];
};

class SourceDonnees
{
public:
    // mieux : std::string, en minuscule, et const ref
    explicit SourceDonnees(const std::string& source = "DataProjet2025.csv");

    const LigneDonnees& ligneCourante() const;
    bool avancer(); // passe à la ligne suivante, retourne false si fin

private:
    std::vector<LigneDonnees> m_lignes;
    std::size_t m_indexCourant = 0;
};
