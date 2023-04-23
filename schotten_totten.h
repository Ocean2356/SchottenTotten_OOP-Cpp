#ifndef SCHOTTEN_TOTTEN_H
#define SCHOTTEN_TOTTEN_H

#include <string>
#include "partie.h"

using namespace std;

using Ordre = array<Joueur &, 2>;
using Joueurs = array<Joueur, 2>;

enum class Edition
{
    Premiere,
    Deuxieme
};
enum class Variante
{
    Normale,
    Tactique,
    Experts
};

class Jeu
{
private:
    // Joueur joueurA;
    // Joueur joueurB;
    Joueurs joueurs;
    Ordre determinerOrdre();
    void traiterResultat(Ordre ordre, Resultat resultat);

public:
    void jouerPartie(Edition edition, Variante variante);
    Jeu() : joueurs(){};
    ~Jeu() = default;
};

void Jeu::jouerPartie(Edition edition, Variante variante)
{
    Ordre ordre = determinerOrdre();

    // abstract factory pattern
    EditionProducer editionProducer;
    AbstractEdition abstractEdition = editionProducer.getEdition(edition);
    Partie partie = abstractEdition.getPartie(variante);

    partie.commencer();
    while (!partie.testerFin())
        partie.jouerTour();
    Resultat resultat = partie.terminer();
    traiterResultat(ordre, resultat);
}

class EditionProducer
{
public:
    AbstractEdition getEdition(Edition edition);
};

class AbstractEdition
{
public:
    virtual Partie getPartie(Variante variante);
};

class PremiereFactory : AbstractEdition
{
};

class DeuxiemeFactory : AbstractEdition
{
};

class Joueur
{
private:
public:
    string nom;
    bool ia;
    int score;
    Joueur();
    ~Joueur() = default;
};

#endif