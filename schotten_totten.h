#ifndef SCHOTTEN_TOTTEN_H
#define SCHOTTEN_TOTTEN_H

#include <string>
#include <array>
#include "partie.h"


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

string toString(Edition e);
string toString(Variante v);

extern std::initializer_list<Edition> Editions;
extern std::initializer_list<Variante> Variantes;

class AbstractEdition
{
public:
    virtual Partie* getPartie(Variante variante);
};

class PremiereFactory : public AbstractEdition
{
    Partie* getPartie(Variante variante);
};

class DeuxiemeFactory : public AbstractEdition
{
    Partie* getPartie(Variante variante);
};

class EditionProducer
{
public:
    AbstractEdition* getEdition(Edition edition);
};


class Jeu
{
public:
    void commencer_jeu(size_t taille_main);
    Ordre determinerOrdre();
    void traiterResultat(Ordre ordre, Resultat resultat);
    void jouerPartie(Edition edition, Variante variante);
    Jeu():joueurs{Joueur(6), Joueur(6)}{}  // reprendre, 6 doit être remplacé par la taille de la main
    ~Jeu() = default;
    Joueurs joueurs;
private:
    // Joueur joueurA;
    // Joueur joueurB;

    EditionProducer editionProducer;
};


#endif

