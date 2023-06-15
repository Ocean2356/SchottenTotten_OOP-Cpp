

#ifndef TACTIQUE_H
#define TACTIQUE_H

#include "../h/partie.h"
#include <iostream>
#include <string>

using namespace std;



// caractéristiques
//enum class Tactique{Troupe, Combat, Ruse};
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
    string getNom() const { return nom;} // méthode utilisée lors de l'affichage d'une carte sur un flux ostream


    // méthode utilisée pour informer l'utilisateur des effets d'une carte
    string getDescription() const override {
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Tactique :  "  + getNom() + "\n";
    }
    string getInfo() const override{  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return nom;
    }

    virtual ~CarteTactique() = default;
private:
    string nom;
};


/*
// Fonction permettant de retourner une string correspondant à un type de carte tactique donné
string toString(Tactique t) {
    switch (t) {
        case Tactique::Troupe:
            return "Troupe";
        case Tactique::Combat:
            return "Combat";
        case Tactique::Ruse:
            return "Ruse";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}

class CarteTactique : public Carte{  // CarteTactique, classe héritant de la classe abstraite carte
public:
    CarteTactique(const Tactique& tac) : tactique(tac){}
    CarteTactique() = default;
    bool estTactique() const override {return true;}
    Tactique getTactique() const {return tactique;}

    // méthode utilisée pour informer l'utilisateur des effets d'une carte
    string getDescription() const override {
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Tactique :  "  + toString(tactique) + "\n";
    }

    virtual ~CarteTactique() = default;
private:
    Tactique tactique;
};*/


class CarteTroupe : public CarteNormale, public CarteTactique{  // classe Troupe, héritant de la classe CarteTactique
public:

    CarteTroupe(const string& nom, Force force, Couleur couleur, Troupe t) : CarteNormale(couleur,force), CarteTactique(nom), troupe(t) {};

    Troupe getTroupe() const { return troupe; }

    CarteTroupe mettre_Joker();
    CarteTroupe mettre_PorteBouclier();
    CarteTroupe mettre_Espion();

    void afficherCarte() const;
    void afficherDosCarte() const { cout << "| Tactique |"; }

private:
    Troupe troupe;

};

class CarteCombat;  // Déclaration avancée de la classe CarteCombat


class Tuile_tactique :  public Tuile{    // classe Tuile_tactique, héritant de la classe Tuile
public:
    Tuile_tactique() : Tuile(), carte_posee_centre(nullptr) {}
    Tuile_tactique(const Tuile& t) = delete;
    Tuile_tactique& operator=(const Tuile& t) = delete;
    const CarteTactique* getCartePoseeCentre() const { return carte_posee_centre; }

    // Méthode vérifiant si le centre de la tuile est plein
    bool centrePlein() const{ return (*carte_posee_centre).getNom() != ""; }

    void placerCarteCentre(const CarteTactique* c){
        if (centrePlein())
            throw PartieException("Le centre de la tuile est deja pleine\n");
        carte_posee_centre = c;
        incr_nb_pleine();
    }
    unsigned int getNbBorne() const { return nb_borne; }

private:
    const CarteTactique* carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_borne;
};

class CarteCombat : public CarteTactique {  // classe Combat, héritant de la classe CarteTactique
public:
    CarteCombat(const string& nom, Combat c) : CarteTactique(nom), combat(c){};

    void mettre_ColinMaillard(Tuile_tactique* t) const {
        const CarteTactique* carte_tactique = dynamic_cast<const CarteTactique*>(this);
        const CarteCombat* carte_combat = dynamic_cast<const CarteCombat*>(carte_tactique);
        t->placerCarteCentre(carte_combat);
    }

    void mettre_CombatdeBoue(Tuile_tactique* t) const {
        const CarteTactique* carte_tactique = dynamic_cast<const CarteTactique*>(this);
        const CarteCombat* carte_combat = dynamic_cast<const CarteCombat*>(carte_tactique);
        t->placerCarteCentre(carte_combat);
    }
    void afficherCarte()const;
    void afficherDosCarte()const {
        cout << "| Tactique |";
    }
    string getNom() const { return nom; }
    Combat getCombat() const { return combat; }

private:
    Combat combat;
    const string nom = toString(combat);
};



class CarteRuse : public CarteTactique {  // classe Ruse, héritant de la classe CarteTactique
public:
    CarteRuse(const string& nom, Ruse r) : CarteTactique(nom), ruse(r) {};

    string getNom() const { return nom; }
    Ruse getRuse() const { return ruse; }


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



//class Defausse : public Pioche {  // classe Defausse, héritant de la classe Pioche
class Defausse {  // classe Defausse, héritant de la classe Pioche
public:
    Defausse() = default;
    Defausse(const Defausse& d) = default;
    Defausse& operator=(const Defausse& d) = default;

    void ajouterCarte(Carte* c) {
        if (cartes.size() >= cartes.max_size()) {
            cout << "La defausse est pleine" << endl;
            return;
        }
        if (c->estTactique()) {
            CarteNormale* carteNormale = static_cast<CarteNormale*>(c);
            cartes[cartes.size() - 1] = new CarteNormale(carteNormale->getCouleur(), carteNormale->getForce());
        } else {
            CarteTactique* carteTactique = static_cast<CarteTactique*>(c);
            cartes[cartes.size() - 1] = new CarteTactique(carteTactique->getNom());
        }
    }

    void afficherDefausse() const {
        for (const auto& carte : cartes) {
            cout << carte->getInfo() << endl;
        }
    }

    void setCarte(size_t n, Carte* c) {
        if (c->estTactique()) {
            CarteNormale* carteNormale = static_cast<CarteNormale*>(c);
            cartes[n] = new CarteNormale(carteNormale->getCouleur(), carteNormale->getForce());
        } else {
            CarteTactique* carteTactique = static_cast<CarteTactique*>(c);
            cartes[n] = new CarteTactique(carteTactique->getNom());
        }
    }

    ~Defausse() {
        for (auto carte : cartes) {
            delete carte;
        }
    }

private:
    array<Carte*, 6> cartes;
};

class Controleur {
public:
    virtual void afficherEtatBorne(size_t num_borne) const {
        // Cette méthode est appelée par Frontiere::afficherFrontiere()
        cout << "      ";  // utilisé pour centrer l'affichage
        Tuile_tactique tuile; // Création d'un objet Tuile_tactique
        const CarteCombat* carte_combat = dynamic_cast<const CarteCombat*>(tuile.getCartePoseeCentre());
        if (carte_combat != nullptr) {
            carte_combat->afficherCarte();
        }
        cout << "     |";  // utilisé pour centrer l'affichage
    }

private:

};

class Affichage: public CarteTactique{ // classe Affichage, héritant de la classe CarteTactique
public:
    void demande_couleur(CarteTroupe& carte);
    void demande_force(CarteTroupe& carte);
private:
};


// classe correspondant à la variante Tactique de la première édition de Schotten-Totten
template<class Carte, size_t N>
class PremiereTactique: public Premiere{
public:
    PremiereTactique();
    // Méthode permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
    void jouerTour() override;
    void initierPiocheTactique();  // initialisation de la pioche tactique
    ~PremiereTactique() = default;
    UI ui = UI();
private:
    Frontiere <Tuile_tactique> frontiere_tactique;
    static const int NMAIN = 7;  // 7 cartes dans la main dans cette variante
    static const int NCARTETACTIQUE = 10;
    Pioche<CarteTactique, NCARTETACTIQUE> piocheTactique;
    array<Agent, 2> agents{Agent(NMAIN), Agent(NMAIN)};  // tableau des agents de la partie

    // Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
    void initierAgents(Ordre ordre) override;

    // Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
    void initierMains() override;
};

class Agent_tactique final:public Agent{
public:
    Agent_tactique() = default;
    Agent_tactique(size_t taille_main) : main(Main(taille_main)){}
    Agent_tactique(const Agent_tactique& a) = default;
    Agent_tactique& operator=(const Agent_tactique& a) = default;

    // Méthode permettant la saisie par l'utilisateur d'une carte à jouer
    Movement choisirCarteAJouer(const Frontiere<class Tuile_tactique>& frontiere, NumJoueur joueur_num) override;

    // Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
    void jouerCarte(Frontiere<class Tuile_tactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, Force& f, Couleur& coul)override;

    // Méthode permettant la saisie par l'utilisateur d'une ou plusieurs bornes à revendiquer
    Movement choisirBornesARevendiquer(Frontiere<class Tuile_tactique>& frontiere, NumJoueur joueur_num)override;

    // Méthode permettant de revendiquer une borne dont le numéro est donné
    void revendiquerBorne(Frontiere<class Tuile_tactique>& frontiere, unsigned int num_borne, NumJoueur joueur_num) override{
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }

    ~Agent_tactique() = default;
private:
    friend class UI;
    Main main;  // un agent a une main

};


#endif //TACTIQUE_H
