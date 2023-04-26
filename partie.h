#ifndef PARTIE_H
#define PARTIE_H

#include <string>
#include "schotten_totten.h"

using namespace std;

using Resultat = array<int, 2>;
using Force = size_t;
using Movement = string;

enum class Couleur
{
    Rouge,
    Marron,
    Jaune,
    Vert,
    Bleu,
    Violet
};

class Partie
{
public:
    void commencer(Ordre ordre);
    void jouerTour();
    bool testerFin();
    Resultat terminer();
    Partie();
    ~Partie();
};

class Premiere : public Partie
{
};

class PremiereNormale : public Premiere
{
public:
    void commencer(Ordre ordre);
    void jouerTour();
    bool testerFin();
    Resultat terminer();
    PremiereNormale();
    ~PremiereNormale();

private:
    static const int NCOULEUR = 6;
    static const int FORCEMIN = 1;
    static const int FORCEMAX = 9;
    static const int NFORCE = FORCEMAX - FORCEMIN + 1;
    static const int NCARTENORMALE = NCOULEUR * NFORCE;
    static const int NMAIN = 6;

    Agent agents[2];
    int agentActive;

    using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;
    TableauJouee tableauJouee;
    bool jouee(Couleur couleur, Force force);

    Pioche<CarteNormale, NCARTENORMALE> piocheNormale;
    Frontiere frontiere;

    void initierPiocheNormale();
    void initierMains();
    void initierAgents(Ordre ordre);
    void agentSuivant();
};

template <class Carte, size_t N>
class Pioche
{
public:
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    bool piocher(Carte &carte) { if (estVide()) return false; carte = getCarte(); return true; };
    void placerDessous(Carte &carte);
    Pioche() : cartes(), nbCartes(N){};
    Carte &operator[](size_t i) const { return &cartes[i]; };

private:
    array<Carte &, N> cartes;
    size_t nbCartes = 0;
    vector<Carte> cartesDessous;
    Carte &getCarte();
};

class Carte
{
public:
    string getInfo() const;
    string getDescription() const;
};

class CarteNormale : public Carte
{
public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force){};
    CarteNormale() = default;

private:
    Couleur couleur;
    Force force;
};

class Tuile
{
};

class Frontiere
{
};

class Agent
{
public:
    Movement jouer(const Frontiere &frontiere);
    void piocher(const Carte &carte);
    Agent();

private:
    Main main;
};

class Main
{
public:
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    Carte &jouerCarte(size_t i);
    void piocherCarte(Carte &carte);

private:
    vector<Carte &> cartes;
    size_t nbCartes = 0;
};

#endif