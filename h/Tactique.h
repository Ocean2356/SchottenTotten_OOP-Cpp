

#ifndef TACTIQUE_H
#define TACTIQUE_H

#include "partie.h"
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

using Ordre = array<Joueur*  , 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs à la fin d'une partie
using Movement = string; // string représentant une action à effectuer (jouer telle carte sur telle borne, revendiquer telle borne...)
using Pos = char;




class CarteTactique : public Carte{  // CarteTactique, classe héritant de la classe abstraite carte
public:
    explicit CarteTactique(const string& n) : nom(n){}
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
    virtual Couleur getCouleur() const { throw PartieException("Erreur lors de la pioche");}
    virtual Force getForce() const { throw PartieException("Erreur lors de la pioche");}
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


class TuileTactique :  public Tuile{    // classe TuileTactique, héritant de la classe Tuile
public:
    TuileTactique() : Tuile(), carte_posee_centre(nullptr) {}
    TuileTactique(const Tuile& t) = delete;
    TuileTactique& operator=(const Tuile& t) = delete;
    const CarteTactique* getCartePoseeCentre() const { return carte_posee_centre; }

    // Méthode vérifiant si le centre de la tuile est plein
    bool centrePlein() const{ return (*carte_posee_centre).getNom().empty(); }

    // permet de vérifier qu'un joueur peut revendiquer une tuile
    bool verifRevendiquable(NumJoueur num_joueur);

    void placerCarteCentre(const CarteTactique* c){
        if (centrePlein())
            throw PartieException("Le centre de la tuile est deja pleine\n");
        carte_posee_centre = c;
        incr_nb_pleine();
    }
    unsigned int getNbBorne() const { return nb_borne; }
    void incr_nb_pleine(){ nb_pleine++; }
private:
    friend class UITactique;
    static const unsigned int nb_tuile = 9; // le nombre de tuiles vaut 9 pour la première édition // TODO changer pour pouvoir gérer la deuxième édition (7 tuiles)
    array<Tuile, nb_tuile> tuiles;  // représente l'ensemble des tuiles
    const CarteTactique* carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_borne;
};

class CarteCombat : public CarteTactique {  // classe Combat, héritant de la classe CarteTactique
public:
    CarteCombat(const string& nom, Combat c) : CarteTactique(nom), combat(c){};

    void mettre_ColinMaillard(TuileTactique* t) const {
        const CarteTactique* carte_tactique = dynamic_cast<const CarteTactique*>(this);
        const CarteCombat* carte_combat = dynamic_cast<const CarteCombat*>(carte_tactique);
        t->placerCarteCentre(carte_combat);
    }

    void mettre_CombatdeBoue(TuileTactique* t) const {
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
        TuileTactique tuile; // Création d'un objet TuileTactique
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
class UITactique final{
public:
    void afficherFrontiere_tactique(const Frontiere<class TuileTactique> &f) const;
    void afficherCote(const TuileTactique &t, size_t cote) const;
    void afficherEtatBorne(const TuileTactique &t, size_t num_borne) const;
    Pos getChoixCarte(Main& main);
    Pos getChoixBorne(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num);
    Movement getChoixBornesARevendiquerIa(Frontiere<class TuileTactique>& f, NumJoueur joueur_num);
    Movement getChoixBornesARevendiquer( Frontiere<class TuileTactique>& f, NumJoueur joueur_num);
};

class AgentTactique final: public Agent{
public:
    AgentTactique() = default;
    AgentTactique(size_t taille_main) : main(Main(taille_main)){}
    AgentTactique(const AgentTactique&) = default;
    AgentTactique& operator=(const AgentTactique&) = default;


    // Méthode permettant la saisie par l'utilisateur d'une carte à jouer
    Movement choisirCarteAJouer(const Frontiere<class TuileTactique>& frontiere, NumJoueur joueur_num);

    // Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
    void jouerCarte(Frontiere<class TuileTactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, string nom_carte);

    // Méthode permettant la saisie par l'utilisateur d'une ou plusieurs bornes à revendiquer
    Movement choisirBornesARevendiquer(Frontiere<class TuileTactique>& f, NumJoueur joueur_num);

    // Méthode permettant de revendiquer une borne dont le numéro est donné
    void revendiquerBorne(Frontiere<class TuileTactique>& frontiere, unsigned int num_borne, NumJoueur joueur_num){
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }

    ~AgentTactique() = default;
private:
    UITactique ui_tactique;
    Main main;  // un agent a une main
};

template<class Carte, size_t N>
class PiocheTactique final: public Pioche<class CarteTactique, N>{
public:
    PiocheTactique(): Pioche<class CarteTactique, N>() {}

    // Méthode permettant de piocher une carte
    const CarteTactique& piocher() override{
        if (this->estVide())
            throw PartieException("La pioche est vide");
        // Génération d'un nombre aléatoire entre 0 et nbCartes - 1 (pour piocher une des cartes piochable de la pioche)
        std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<size_t> distrib{0, this->nbCartes - 1};
        size_t x = distrib(random_eng);
        // On "supprime" la carte piochée, en l'échangeant avec la dernière carte de la pioche et en décrementant le nombre de cartes piochable
        this->swapCartes(*this->cartes[x], *this->cartes[--this->nbCartes]);
        return *this->cartes[this->nbCartes];  // on retourne la carte que l'on vient de "supprimer"
    }

    void setCarteTactique(size_t n,string nom) {
        this->cartes[n] = new CarteTactique(nom);
    }
};


// classe correspondant à la variante Tactique de la première édition de Schotten-Totten
class PremiereTactique final: public Premiere{
public:
    PremiereTactique();
    // Méthode permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
    void jouerTour() override;
    void initierPiocheTactique();  // initialisation de la pioche tactique
    ~PremiereTactique() = default;
    void setCarteTactique(size_t n, string& nom) {
        piocheTactique.setCarteTactique(n, nom);
    }
private:
    Frontiere<TuileTactique> frontiere_tactique;
    static const int NMAIN = 7;  // 7 cartes dans la main dans cette variante
    static const int NCARTETACTIQUE = 10;
    PiocheTactique<CarteTactique, NCARTETACTIQUE> piocheTactique;
    array<AgentTactique, 2> agents{AgentTactique(NMAIN), AgentTactique(NMAIN)};  // tableau des agents de la partie
    // Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
    void initierAgents(Ordre ordre) override;
    // Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
    void initierMains() override;
};

#endif //TACTIQUE_H
