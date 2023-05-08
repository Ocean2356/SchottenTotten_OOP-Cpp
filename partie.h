#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include <string>
#include <array>
#include <vector>

class Joueur;



using namespace std;
using Force = size_t; //numéro de la carte
using Ordre = array<Joueur&, 2>; // ordre des joueurs qui jouent
using Resultat = array<int, 2>; // score des joueurs
using Movement = string; // "piocher" ou "jouer"



class PartieException {
    public:
        PartieException(const string& i) :info(i) {}
        string getInfo() const { return info; }
    private:
        string info;
    };

    // caractéristiques
    enum class Couleur {Rouge, Marron,Jaune,Vert,Bleu,Violet };
    enum class Force { un=1, deux=2, trois=3, quatre=4, cinq=5, six=6, sept=7, huit=8, neuf=9 };
    

    // conversion d'une caractéristique en string
    string toString(Couleur c);
    string toString(Force f);
   

    // écriture d'une caractéristique sur un flux ostream
    ostream& operator<<(ostream& f, Couleur c);
    ostream& operator<<(ostream& f, Force x);

    // listes contenant les valeurs possibles pour chacune des caractéristiques
    extern std::initializer_list<Couleur> Couleurs;
    extern std::initializer_list<Force> Forces;

    // affichage des valeurs possibles pour chaque caractéristiques
    void printCouleurs(std::ostream& f = cout);
    void printForces(std::ostream& f = cout);



template <class Carte, size_t N> 
class Pioche
{
public:
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    bool piocher(Carte &carte) {
        if (estVide())
            return false;
        carte = getCarte();
        return true;
    };
    void placerDessous(Carte &carte){
        cartes[cartes.size()] = carte;
    };
    Pioche(): cartes(), nbCartes(N){};
    Carte &operator[](size_t i) const { return &cartes[i]; };

private:
    array<Carte *, N> cartes;
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

std::ostream& operator<<(std::ostream& f, const Carte& c);


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

class Tuile
{
public:
    Tuile(): nb_pleine(0){}
    bool est_pleine() const{
        if (cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine)
            return 1;
        return 0;
    }
    bool estRevendiquée() const {}
private:
    array<vector<Carte>, 2> cartes_posees;
    unsigned int nb_pleine;
};

class Frontiere{
    public:
    bool estFinie() const { 
        if(tuiles[0].est_pleine() && tuiles[1].est_pleine() && tuiles[2].est_pleine())
            return 1;
        return 0;
     };
    Joueur gagnant() const{};
private:
    static const int nb_tuile = 9;
    array<Tuile, nb_tuile> tuiles;
};

class Main
{
public:
    Main(size_t taille):cartes(taille){};
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    Carte &jouerCarte(size_t i){return *cartes[i];};
    void piocherCarte(Carte &carte){cartes.push_back(&carte); nbCartes++;};

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
