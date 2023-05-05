#ifndef PARTIE_H
#define PARTIE_H

#include <string>
#include <array>
#include <vector>

class Joueur;



using namespace std;
using Force = size_t;
using Ordre = array<Joueur&, 2>;
using Resultat = array<int, 2>;
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
    const Couleur& getCouleur() const {return couleur;}
    const Force& getForce() const {return force;}

private:
    Couleur couleur;
    Force force;
};

template <class Carte, size_t N>
class Pioche
{
public:
    bool estVide() const { return nbCartes == 0; }
    size_t getNbCartes() const { return nbCartes; }
    bool piocher(const Carte& carte) {
        if (estVide())
            return false;
        size_t x = rand()%nbCartes;
        carte = cartes[x]; // creation d'une référence sur la carte piochee
        cartes[x] = cartes[--nbCartes];  // déplacement de la dernière carte à l'emplacement de la carte piochée
        return true;
    }
    Pioche(): cartes(), nbCartes(N){};
    Carte &operator[](size_t i) const { return &cartes[i]; }

private:
    array<Carte *, N> cartes;
    size_t nbCartes = 0;
};

/*
class PiochePartieTactique : public Pioche{
public:
     void placerDessous(Carte &carte);
private:
     vector<Carte> cartesDessous;
};
*/


class Tuile
{
public:
    Tuile(): nb_pleine(0){}
    bool est_pleine(){
        if (cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine)
            return 1;
        return 0;
    }
private:
    array<vector<Carte>, 2> cartes_posees;
    unsigned int nb_pleine;
};

class Frontiere{

private:
    static const int nb_tuile = 9;
    array<Tuile, nb_tuile> tuiles;
};

class Main
{
public:
    Main(size_t taille):cartes(taille){}
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    Carte &jouerCarte(size_t i);
    void piocherCarte(Carte &carte);

private:
    vector<Carte*> cartes;
    size_t nbCartes = 0;
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


class Joueur
{
public:
    string nom;
    // bool ia;
    Agent agent;
    int score;
    Joueur();
    ~Joueur() = default;
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

class Premiere : public Partie{
public:
    void commencer(Ordre ordre);
    void jouerTour();
    bool testerFin();
    Resultat terminer();
protected:
    static const int NCOULEUR = 6;
    static const int FORCEMIN = 1;
    static const int FORCEMAX = 9;
    static const int NFORCE = FORCEMAX - FORCEMIN + 1;
    static const int NCARTENORMALE = NCOULEUR * NFORCE;
};

class PremiereNormale : public Premiere
{
public:
    PremiereNormale();
    ~PremiereNormale();

private:
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


#endif
