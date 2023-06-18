

#ifndef TACTIQUE_H
#define TACTIQUE_H

#include "partie.h"
#include <iostream>
#include <string>

using namespace std;
class AgentTactique;


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

using Ordre = array<Joueur*  , 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs à la fin d'une partie
using Movement = string; // string représentant une action à effectuer (jouer telle carte sur telle borne, revendiquer telle borne...)
using Pos = char;




class CarteTactique : public Carte{  // CarteTactique, classe héritant de la classe abstraite carte
public:
    explicit CarteTactique(const string& n) : nom(n){}
    CarteTactique() = default;
    CarteTactique& operator=(const CarteTactique&) = default;
    const CarteTactique& operator=(const CarteTactique& c) const {
        const CarteTactique& c2 = c;
        return c2;
    }
    bool estTactique() const override {return true;}
    string getNom() const { return nom;} // méthode utilisée lors de l'affichage d'une carte sur un flux ostream


    string getInfo() const override{  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return nom;
    }
    virtual Couleur getCouleur() const { throw PartieException("Erreur lors de la pioche");}
    virtual Force getForce() const { throw PartieException("Erreur lors de la pioche");}
    virtual ~CarteTactique() = default;
private:
    string nom;
};


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

    Couleur demandeCouleur() const;
    Force demandeForce(unsigned int contrainte) const;
    void placerCarteCentre(const CarteTactique* c){
        if (centrePlein())
            throw PartieException("Le centre de la tuile est deja rempli\n");
        carte_posee_centre = c;
    }
    void incr_nb_pleines() { this->nb_pleine++;}
    void removeCarte(const Carte* carte, int cote) {
        for (auto it = cartes_posees[cote].begin(); it != cartes_posees[cote].end(); ++it) {
            if (*it == carte) {
                cartes_posees[cote].erase(it);
                break;
            }
        }
    }

    vector<const Carte*> getCartesPosees(NumJoueur num_joueur) const{
        return cartes_posees[(int) num_joueur];
    }
private:
    // permet de vérifier que toutes les cartes d'un côté de la tuile ont la même couleur
    bool verifMemeCouleur(NumJoueur num_joueur, const vector <Couleur>& coul) const;

    // permet de vérifier que toutes les cartes d'un côté de la tuile ont la même force
    bool verifMemeForce(NumJoueur num_joueur, vector <Force>& f) const;

    // Méthode appelée par verifSuite permettant de trier les cartes en fonction de la force d'un côté de la tuile
    void ordonnerCartes(NumJoueur num_joueur, vector <Force>& f);

    // permet de vérifier que les cartes d'un côté de la tuile forment une suite
    bool verifSuite(NumJoueur num_joueur, vector <Force>& f);

    // permet de sommer la force de toutes les cartes d'un côté de la tuile
    unsigned int getSomme(NumJoueur num_joueur, vector <Force>& f) const;

    Combinaison determinerCombinaison(NumJoueur num_joueur, const vector <Couleur>& coul, vector <Force>& f);
    friend class UITactique;
    const CarteTactique* carte_posee_centre;
};

class Defausse {
public:
    Defausse() = default;
    Defausse(const Defausse& d) = default;
    Defausse& operator=(const Defausse& d) = default;

    void ajouterCarte(const Carte* c) {
        cartes.push_back(c);
    }

    void afficherDefausse() const {
        for (const auto& carte : cartes) {
            cout << carte->getInfo() << endl;
        }
    }

    static Defausse& getInstance() {
        return instance;
    }

    ~Defausse() {
        for (auto carte : cartes) {
            delete carte;
        }
    }

private:
    vector<const Carte*> cartes;
    static Defausse instance;
};

class UITactique final{
public:
    void afficherFrontiere_tactique(const Frontiere<class TuileTactique> &f) const;
    void afficherCote(const TuileTactique &t, size_t cote) const;
    void afficherEtatBorne(const TuileTactique &t, size_t num_borne) const;
    Pos getChoixCarte(const AgentTactique& agent, const vector <int>& cartes_jouables) const;
    Pos getChoixCarteIa(const AgentTactique& agent, const vector <int>& cartes_jouables) const;
    Pos getChoixBorneIa(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const;
    Pos getChoixBorne(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const;
    Pos getChoixBorneCarteCentreIa(const Frontiere<class TuileTactique>& f) const;
    Pos getChoixBorneCarteCentre(const Frontiere<class TuileTactique>& f) const;
    Movement getChoixBornesARevendiquerIa(Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const;
    Movement getChoixBornesARevendiquer(Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const;
};

class AgentTactique final: public Agent{
public:
    AgentTactique() = default;
    AgentTactique(size_t taille_main) : Agent(taille_main), nb_cartes_tactiques_jouees(0), nb_jokers_joues(0){}
    AgentTactique(const AgentTactique&) = default;
    AgentTactique& operator=(const AgentTactique&) = default;
    Main& getMainModifiable() { return this->main;}
    unsigned int getNbCartesTactiquesJouees() const { return nb_cartes_tactiques_jouees;}

    void mettre_ChasseurdeTete();
    void actionStrategeBansheeTraitre(Frontiere<class TuileTactique>& frontiere, string& nomTactique, NumJoueur num_joueur);

    void mettre_ColinMaillard(TuileTactique& t, const CarteTactique* c) const {
        t.placerCarteCentre(c);
    }

    void mettre_CombatdeBoue(TuileTactique& t, const CarteTactique* c) const {
        t.placerCarteCentre(c);
        t.incr_nb_pleines();
    }
    // Méthode permettant la saisie par l'utilisateur d'une carte à jouer
    Movement choisirCarteAJouer(const Frontiere<class TuileTactique>& frontiere, NumJoueur joueur_num, unsigned int nb_cartes_tactiques_jouees_autre_joueur);

    // Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
    void jouerCarte(Frontiere<class TuileTactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, const string& nom_carte);

    // Méthode permettant la saisie par l'utilisateur d'une ou plusieurs bornes à revendiquer
    Movement choisirBornesARevendiquer(Frontiere<class TuileTactique>& f, NumJoueur joueur_num);

    // Méthode permettant de revendiquer une borne dont le numéro est donné
    void revendiquerBorne(Frontiere<class TuileTactique>& frontiere, unsigned int num_borne, NumJoueur joueur_num){
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }

    ~AgentTactique() = default;
private:
    UITactique ui_tactique;
    unsigned int nb_cartes_tactiques_jouees;
    unsigned int nb_jokers_joues;
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
        this->nbCartes --;
        this->swapCartes(*this->cartes[x], *this->cartes[this->nbCartes]);
        return *this->cartes[this->nbCartes];  // on retourne la carte que l'on vient de "supprimer"
    }

    void setCarteTactique(size_t n, const string& nom) {
        this->cartes[n] = new CarteTactique(nom);
    }
};

class Joueur final{  // classe représentant un joueur du jeu
public:
    Joueur() = default;
    Joueur(const string& n, const bool& est_ia) : nom(n), ia(est_ia){}
    Joueur(const Joueur& j) = default;
    Joueur& operator=(const Joueur& j) = default;
    unsigned int getScore() const{ return score; }
    void setScore(unsigned int points){ score += points; }
    const string& getNom() const{ return nom; }
    Agent& getAgent() { return agent; }
    AgentTactique& getAgentTactique() { return agent_tactique; }
    ~Joueur() = default;

private:
    string nom;
    unsigned int score = 0;
    bool ia;
    Agent agent;  // un joueur dispose d'un agent
    AgentTactique agent_tactique;
};

// classe correspondant à la variante Tactique de la première édition de Schotten-Totten
class PremiereTactique final: public Premiere{
public:
    PremiereTactique();
    // Méthode permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
    void jouerTour() override;
    void initierPiocheTactique();  // initialisation de la pioche tactique
    ~PremiereTactique() = default;

    static const int NCARTETACTIQUE = 10;
private:
    Frontiere<TuileTactique> frontiere_tactique;
    static const int NMAIN = 7;  // 7 cartes dans la main dans cette variante
    PiocheTactique<CarteTactique, NCARTETACTIQUE> piocheTactique;
    array<AgentTactique, 2> agents{AgentTactique(NMAIN), AgentTactique(NMAIN)};  // tableau des agents de la partie
    // Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
    void initierAgents(Ordre ordre) override;
    // Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
    void initierMains() override;
};



#endif //TACTIQUE_H
