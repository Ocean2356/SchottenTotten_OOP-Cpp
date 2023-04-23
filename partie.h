#ifndef PARTIE_H
#define PARTIE_H

#include <string>

using namespace std;

using Resultat = array<int, 2>;
using Force = size_t;

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
    // private:
    // CarteNormale cartes[];
    // Frontiere frontiere;
    // Agent agent[2];
    // bool revendiquerAutomatique;
public:
    virtual void commencer();
    virtual void jouerTour();
    virtual bool testerFin();
    virtual Resultat terminer();
    Partie();
    ~Partie();
};

class Premiere : public Partie
{
public:
    void commencer() override;
    void jouerTour() override;
    bool testerFin() override;
    Resultat terminer() override;
};

class PremiereNormale : public Premiere
{
private:
    static const int NCOULEUR = 6;
    static const int FORCEMIN = 1;
    static const int FORCEMAX = 9;
    static const int NFORCE = FORCEMAX - FORCEMIN + 1;
    static const int NCARTENORMALE = NCOULEUR * NFORCE;
    static const int NMAIN = 6;
    using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;
    TableauJouee tableauJouee;
    using CartesNormales = array<Carte *, NCOULEUR * NFORCE>;
    CartesNormales cartesNormales;
    void initierPiocheNormale();
    void initierMain();
    void piocher();

public:
    void commencer() final;
    void jouerTour() final;
    bool testerFin() final;
    Resultat terminer() final;
    PremiereNormale();
    ~PremiereNormale();
};

PremiereNormale::PremiereNormale(){

}

void PremiereNormale::commencer()
{
    initierPiocheNormale();
    initierMain();
}

void PremiereNormale::initierPiocheNormale()
{
    Pioche<CarteNormale*, NCARTENORMALE> piocheNormale();
    for (int i = 0; i < NCOULEUR; ++i)
        for (int j = 0; j < NFORCE; ++j)
            piocheNormale[i * NCOULEUR + j] = (CarteNormale*) new CarteNormale((Couleur)i, (Force)j + FORCEMIN);
}

void PremiereNormale::piocher()
{
}

template <class Carte, size_t N>
class Pioche : public array<Carte, N>
{
private:
    array<Carte, N> cartes;
    size_t nbCartes = 0;
    vector<Carte> cartesDessous;

public:
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    Carte& piocher() ;
    void placerDessous(Carte& carte) ;
    Pioche() : cartes(), nbCartes(N) {};
    Carte& operator[] (size_t i) const { return cartes[i]; };
};

class Carte{};

class CarteNormale : Carte
{
private:
    // bool jouee;
    Couleur couleur;
    Force force;

public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force){};
};

class Tuile
{
};

class Frontiere
{
};

class Agent
{
};

#endif