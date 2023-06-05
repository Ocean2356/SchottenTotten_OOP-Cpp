#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <chrono>

class Joueur;

using namespace std;
using Ordre = array<Joueur*, 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs à la fin d'une partie
using Movement = string; // string représentant une action à effectuer (jouer telle carte sur telle borne, revendiquer telle borne...)

enum class TuileRevendiquee {non_revendiquee, revendiquee_joueur1, revendiquee_joueur2};  // utilisé pour stocker, pour chaque tuile, si elle a été revendiquée et par quel joueur
enum class NumJoueur {joueur1, joueur2};
enum class JoueurGagnant {aucun, joueur1, joueur2};
enum class Combinaison {somme, suite, couleur, brelan, suite_couleur};  // différentes combinaisons de cartes possibles

class PartieException {  // class permettant de lever des exceptions pour les erreurs ayant lieu dans une partie
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


class Carte{ // classe abstraite Carte
public:
    virtual string getInfo() const = 0;
    virtual string getDescription() const = 0;
    virtual Force getForce() const = 0;
    virtual Couleur getCouleur() const = 0;
    virtual ~Carte() = default;
};
std::ostream& operator<<(std::ostream& f, const Carte& c);  // Affichage d'une carte sur un flux ostream

class CarteNormale final : public Carte{  // CarteNormale, classe héritant de la classe abstraite carte
public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force){}
    CarteNormale()=default;
    virtual string getInfo() const override {  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return toString(couleur) + toString(force) + ' ';
    }
    virtual string getDescription() const override {  // méthode utilisée pour informer l'utilisateur des effets d'une carte
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Clan : force = " + toString(force) + " Couleur = " + toString(couleur);
    }
    Couleur getCouleur() const override {return couleur;}
    Force getForce() const override {return force;}
private:
    Couleur couleur;
    Force force;
};

template <class Carte, size_t N>  // La pioche peut être une pioche de cartes normales ou une pioche de cartes tactiques
class Pioche final{  // classe Pioche représentant une des pioches du jeu
public:
    Pioche(): cartes(), nbCartes(N) {}
    bool estVide() const { return nbCartes == 0; } // Méthode indiquant si la pioche est vide
    size_t getNbCartes() const { return nbCartes; }

    void setCarte(size_t n, const Couleur& c, const Force& f){  // Méthode permettant d'allouer une nouvelle carte de la pioche
        cartes[n] = new Carte(c, f);
    }

    void swapCartes(Carte& c1, Carte& c2){  // Méthode permettant d'échanger deux cartes de la pioche
        Carte tmp = c1;
        c1=c2;
        c2=tmp;
    }

    const Carte& piocher() {
        if (estVide())
            throw PartieException("La pioche est vide");
        // Génération d'un nombre aléatoire entre 0 et nbCartes - 1 (pour piocher une des cartes piochable de la pioche)
        std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution <int> distrib {0, (int) nbCartes-1};
        size_t x = distrib(random_eng);

        // On "supprime" la carte piochée, en l'échangeant avec la dernière carte de la pioche et en décrementant le nombre de cartes piochable
        swapCartes(*cartes[x], *cartes[--nbCartes]);
        return *cartes[nbCartes];  // on retourne la carte que l'on vient de "supprimer"
    };

    void placerDessous(const Carte& carte){
        // A REPRENDRE, nécessaire pour implémenter la version tactique, utiliser l'attribut cartesDessous
        cartes[cartes.size()] = carte;
    };

    ~Pioche(){  // destructeur qui désalloue les cartes de la pioche
        for (size_t i = 0; i < N; i++){
            delete cartes[i];
        }
    }
private:
    array<Carte*, N> cartes;  // array de N cartes
    size_t nbCartes;  // nombre de cartes piochables
    // vector<Carte> cartesDessous; TODO pour la version tactique
};


class Tuile
{
public:
    Tuile(): nb_pleine(3), revendiquee(TuileRevendiquee::non_revendiquee){}
    TuileRevendiquee getRevendiquee() const { return revendiquee;}
    void ordonnerCartes(NumJoueur num_joueur);
    void placerCarte(const Carte& c, NumJoueur joueur_num){
        cartes_posees[(int) joueur_num].push_back(&c);
        if (cotePlein(joueur_num))
            rempli_en_premier = joueur_num;
    }
    bool cotePlein(NumJoueur joueur_num) const{ return cartes_posees[(int) joueur_num].size() == nb_pleine;}
    bool estPleine() const{ return cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine;}
    void afficherCote(size_t cote) const{
        size_t iter = 1;
        for(auto c : cartes_posees[cote]){
            cout << *c;
            iter ++;
        }
        while (iter < 5){
            cout << "    ";
            iter ++;
        }
        cout << "|";
    }
    void afficherEtatBorne(size_t num_borne) const{
        cout << "      ";
        if (revendiquee == TuileRevendiquee::revendiquee_joueur1)
            cout << "/R1/";
        else if (revendiquee == TuileRevendiquee::revendiquee_joueur1)
            cout << "/R2/";
        else
            cout << "/B" << num_borne << "/ ";
        cout << "     |";
    }
    bool verifMemeCouleur(NumJoueur num_joueur) const;
    bool verifMemeForce(NumJoueur num_joueur) const;
    bool verifSuite(NumJoueur num_joueur);
    unsigned int getSomme(NumJoueur num_joueur) const;
    bool verifRevendiquable(NumJoueur num_joueur);
    void revendiquer(NumJoueur num_joueur);
    bool estRevendiquee() const;
private:
    array<vector<const Carte*>, 2> cartes_posees;
    //CarteTactique carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_pleine;
    NumJoueur rempli_en_premier;
    TuileRevendiquee revendiquee;
};

class Frontiere{
public:
    void afficherFrontiere() const{
        cout << "\n----------------------------------------------------------------Affichage de la frontiere----------------------------------------------------------------\n";
        for (size_t i=0; i<nb_tuile; i++)
            tuiles[i].afficherCote(0);

        cout << "\n";

        for (size_t i=0; i<nb_tuile; i++)
            tuiles[i].afficherEtatBorne(i + 1);

        cout << "\n";

        for (size_t i=0; i<nb_tuile; i++)
            tuiles[i].afficherCote(1);

        cout << "\n---------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }
    const unsigned int getNbTuile() const { return nb_tuile;};
    unsigned int calculerScore(NumJoueur joueur_num) const;
    JoueurGagnant estFinie() const;
private:
    friend class Agent;
    static const unsigned int nb_tuile = 9;
    array<Tuile, nb_tuile> tuiles;
};

class Main
{
public:
    Main() = default;
    Main(size_t t_max): taille_max(t_max) {}
    bool estVide() const { return nbCartes == 0; }
    size_t getNbCartes() const { return nbCartes; }
    size_t getTailleMax() const { return taille_max; }
    const Carte& getCarte(size_t i) const{ return *cartes[i]; }
    const Carte& jouerCarte(unsigned int i){
        const Carte& carte = *cartes[i];
        cartes.erase(cartes.begin() + i);
        // for (auto it : cartes) cout << *it;
        nbCartes--;
        return carte;
    }
    void piocherCarte(const Carte& carte){
        cartes.push_back(&carte);
        nbCartes++;
    }
private:
    vector<const Carte*> cartes;
    size_t taille_max = 6;  // par défaut, la main contient au maximum 6 cartes. Ce nombre est modifié pour la partie tactique
    size_t nbCartes = 0;  // initialement, la main est vide
};



class Agent
{
public:
    Movement choisirCarteAJouer(const Frontiere& frontiere, NumJoueur joueur_num);
    Movement choisirBornesARevendiquer(Frontiere& frontiere, NumJoueur joueur_num);
    void jouerCarte(Frontiere& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, Force& f, Couleur& coul){
        const Carte& c = main.jouerCarte(pos_carte);
        f = c.getForce();
        coul = c.getCouleur();
        frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);
    }
    void revendiquerBorne(Frontiere& frontiere, unsigned int num_borne, NumJoueur joueur_num){
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }
    void piocher(const Carte& carte);
    Agent() = default;
    Agent(size_t taille_main): main(Main(taille_main)) {}
private:
    Main main;
};


class Joueur
{
public:
    unsigned int getScore() const {return score;}
    void setScore(unsigned int points) {score += points;}
    const string& getNom() const {return nom;}
    const Agent& getAgent() const {return agent;}
    Joueur() = default;
    Joueur(const string& n): nom(n), agent(Agent()) {}
    ~Joueur() = default;
private:
    string nom;
    unsigned int score = 0;
    // bool ia;  // TODO à ajouter lorsque l'on implémentera la possibilité de jouer contre une ia (modifier Jeu::commencer_jeu en conséquence, ainsi que toutes les fonctions qui demandent un choix au joueur
    Agent agent;
};

class Partie
{
public:
    virtual void commencer(Ordre ordre)=0;
    virtual void jouerTour()=0;
    virtual bool testerFin();
    Resultat terminer();
    Partie()=default;
    virtual ~Partie()=default;
protected:
    Frontiere frontiere;
};

class Premiere : public Partie{

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
    ~PremiereNormale() = default;
    void commencer(Ordre ordre);
    void jouerTour();

private:
    static const int NMAIN = 6;

    array <Agent, 2> agents {Agent(NMAIN), Agent(NMAIN)} ;
    int agentActive;

    using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;
    TableauJouee tableauJouee;

    Pioche<CarteNormale, NCARTENORMALE> piocheNormale;
    
    void initierPiocheNormale();
    void initierMains();
    void initierAgents(Ordre ordre);
    void agentSuivant();
};


#endif
