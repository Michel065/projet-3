#include "SourceDonnees.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace
{
    std::string nettoyerNombre(const std::string& s)
    {
        std::string res = s;
        for (char& c : res)
            if (c == ',')
                c = '.';
        return res;
    }

    bool convertirFloat(const std::string& s, float& out)
    {
        try
        {
            out = std::stof(nettoyerNombre(s));
            return true;
        }
        catch (...)
        {
            out = 0.f;
            return false;
        }
    }
}

SourceDonnees::SourceDonnees(const std::string& cheminFichier)
{
    std::ifstream file(cheminFichier);
    if (!file)
    {
        std::cout << "erreur : impossible d'ouvrir le fichier " << cheminFichier << std::endl;
        return;
    }

    std::string ligne;
    if (!std::getline(file, ligne))
        return; // fichier vide

    while (std::getline(file, ligne))
    {
        if (ligne.empty())
            continue;

        std::stringstream ss(ligne);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';'))
            tokens.push_back(token);

        if (tokens.size() < 15)
            continue;

        LigneDonnees ld{};

        std::size_t i = 0;
        convertirFloat(tokens[i++], ld.elav);      // Elav
        convertirFloat(tokens[i++], ld.qtot);      // Qtot
        convertirFloat(tokens[i++], ld.qturb);     // Qturb
        convertirFloat(tokens[i++], ld.qvan);      // Qvan
        convertirFloat(tokens[i++], ld.nivAmont);  // Niv Amont

        convertirFloat(tokens[i++], ld.q[0]);      // Q1
        convertirFloat(tokens[i++], ld.p[0]);      // P1
        convertirFloat(tokens[i++], ld.q[1]);      // Q2
        convertirFloat(tokens[i++], ld.p[1]);      // P2
        convertirFloat(tokens[i++], ld.q[2]);      // Q3
        convertirFloat(tokens[i++], ld.p[2]);      // P3
        convertirFloat(tokens[i++], ld.q[3]);      // Q4
        convertirFloat(tokens[i++], ld.p[3]);      // P4
        convertirFloat(tokens[i++], ld.q[4]);      // Q5
        convertirFloat(tokens[i++], ld.p[4]);      // P5

        m_lignes.push_back(ld);
    }

    m_indexCourant = 0;
}

const LigneDonnees& SourceDonnees::ligneCourante() const
{
    return m_lignes[m_indexCourant];
}

bool SourceDonnees::avancer()
{
    if (m_indexCourant + 1 >= m_lignes.size())
        return false;
    ++m_indexCourant;
    return true;
}

void SourceDonnees::print_ligneCourante() const
{
    if (m_lignes.empty()) {
        std::cout << "Aucune ligne chargee dans SourceDonnees.\n";
        return;
    }

    const LigneDonnees& ld = ligneCourante();

    std::cout << "Ligne " << m_indexCourant << " :\n";
    std::cout << "  Elav      = " << ld.elav      << "\n";
    std::cout << "  Qtot      = " << ld.qtot      << "\n";
    std::cout << "  Qturb     = " << ld.qturb     << "\n";
    std::cout << "  Qvan      = " << ld.qvan      << "\n";
    std::cout << "  NivAmont  = " << ld.nivAmont  << "\n";

    std::cout << "  Q  : [";
    for (int i = 0; i < 5; ++i) {
        std::cout << ld.q[i] << (i < 4 ? ", " : "]\n");
    }

    std::cout << "  P  : [";
    for (int i = 0; i < 5; ++i) {
        std::cout << ld.p[i] << (i < 4 ? ", " : "]\n");
    }
}

int SourceDonnees::get_index_courrant()const{
    return m_indexCourant;
}
