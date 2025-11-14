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
    explicit SourceDonnees(const std::string& source = "DataProjet2025.csv");

    const LigneDonnees& ligneCourante() const;
    bool avancer(); // passe à la ligne suivante ou retourne false si fin
    void print_ligneCourante() const;

    int get_index_courrant()const;
private:
    std::vector<LigneDonnees> m_lignes;
    std::size_t m_indexCourant = 0;
};
