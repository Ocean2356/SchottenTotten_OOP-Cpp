

#ifndef TACTIQUE_H
#define TACTIQUE_H

#include "../h/partie.h"
#include <iostream>
#include <string>

using namespace std;



// caractéristiques
enum class Troupe{Joker,Espion, Porte_Bouclier};
enum class Combat{Colin_Maillard, Combat_de_Boue};
enum class Ruse{Chasseur_de_tete, Stratege, Banshee, Traitre};

// listes contenant les valeurs possibles pour chacune des caractéristiques
extern std::initializer_list<Troupe> Troupes;
extern std::initializer_list<Combat> Combats;
extern std::initializer_list<Ruse> Ruses;

// conversion d'une caractéristique en string
string toString(Troupe t);
string toString(Combat c);
string toString(Ruse r);

class CarteTactique : public Carte{  // CarteTactique, classe héritant de la classe abstraite carte
public:
    CarteTactique(const string& n) : nom(n){}
    CarteTactique() = default;
    bool estTactique() const override {return true;}
    bool estNormale() const override {return false;}
    string getInfo() const override{  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return nom;
    }

    // méthode utilisée pour informer l'utilisateur des effets d'une carte
    virtual string getDescription() const = 0;
    virtual ~CarteTactique() = default;
private:
    const string& nom;
};

class CarteTroupe : public CarteNormale, public CarteTactique{  // classe Troupe, héritant de la classe CarteTactique
public:
    CarteTroupe(const string &nom, Force force, Couleur couleur) : CarteNormale(couleur,force), nom(nom) {};
    string getNom() const { return nom; }

    CarteTroupe mettre_Joker();
    CarteTroupe mettre_PorteBouclier();
    CarteTroupe mettre_Espion();

    void afficherCarte() const;
    void afficherDosCarte() const { cout << "| Tactique |"; }

    Troupe getTroupe() const { return troupe; }
    string getDescription() const override{
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Tactique : Troupe = " + toString(troupe);
    }
private:
    Troupe troupe;
    const string nom = toString(troupe);
};


class CarteCombat : public CarteTactique {  // classe Combat, héritant de la classe CarteTactique
public:
    CarteCombat(const string& nom) : CarteTactique(nom){};

    void mettre_ColinMaillard(Tuile* t) const {
        t->placerCarteCentre(*this);
    }
    void mettre_CombatdeBoue(Tuile* t) const {
        t->placerCarteCentre(*this);
    }
    void afficherCarte()const;
    void afficherDosCarte()const {
        cout << "| Tactique |";
    }
    string getNom() const { return nom; }
    Combat getCombat() const { return combat; }
    string getDescription() const override{
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Tactique : Combat = " + toString(combat);
    }
private:
    Combat combat;
    const string nom = toString(combat);
};



class CarteRuse : public CarteTactique {  // classe Ruse, héritant de la classe CarteTactique
public:
    CarteRuse(const string& nom) : CarteTactique(nom)  {};

    string getNom() const { return nom; }
    Ruse getRuse() const { return ruse; }
    string getDescription() const override{
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Tactique : Ruse = " + toString(ruse);
    }

    static void mettre_ChasseurdeTete() ;
    static void mettre_Stratege() ;
    static void mettre_Banshee() ;
    static void mettre_Traitre() ;

    void afficherCarte()const;
    void afficherDosCarte()const { cout << "| Tactique |";}

private:
    Ruse ruse;
    const string nom = toString(ruse);
};

class Defausse{
public:
    Defausse() = default;
    Defausse(const Defausse& d) = default;
    Defausse& operator=(const Defausse& d) = default;
    void ajouterCarte(Carte* c){
        if (cartes.size() >= cartes.max_size()) {
            cout << "La defausse est pleine" << endl;
            return;
        }
        setCarte(cartes.size(), c);
        cartes[cartes.size()] = c; // Ajouter le pointeur à la fin de l'array
    }
    void afficherDefausse() const;// TO DO
    void setCarte(size_t n, Carte* c){// pour alloué une nouvelle carte dans la defausse
        if (c->estNormale()) {
            CarteNormale* carteNormale = static_cast<CarteNormale*>(c);
            cartes[n] = new CarteNormale(carteNormale->getCouleur(), carteNormale->getForce());
        } else {
            CarteTactique* carteTactique = static_cast<CarteTactique*>(c);
            cartes[n] = new CarteTactique(carteTactique->getNom());
        }
    ~Defausse(){  // destructeur qui désalloue les cartes de la defausse
        for (size_t i = 0; i < 6; i++){
            delete cartes[i];
        }
    }

private :
    array<Carte*,6> cartes;
};





#endif //TACTIQUE_H
