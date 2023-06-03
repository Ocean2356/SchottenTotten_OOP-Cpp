#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include <string>
#include <array>
#include <vector>

class Joueur;


using namespace std;
using Ordre = array<Joueur*, 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs
using Movement = string; // "piocher" ou "jouer"

enum class TuileRevendiquee {non_revendiquee, revendiquee_joueur1, revendiquee_joueur2};
enum class NumJoueur {joueur1, joueur2};
enum class JoueurGagnant {aucun, joueur1, joueur2};
enum class Combinaison {somme, suite, couleur, brelan, suite_couleur};

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

class Carte
{
public:
    virtual string getInfo() const;
    string getDescription() const; // TODO
    virtual Force getForce() const { return Force::un;} // REPRENDRE
    virtual Couleur getCouleur() const { return Couleur::Vert;} // REPRENDRE
};

std::ostream& operator<<(std::ostream& f, const Carte& c);


class CarteNormale : public Carte
{
public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force){}
    CarteNormale()=default;
    Couleur getCouleur() const {return couleur;}
    Force getForce() const {return force;}
private:
    Couleur couleur;
    Force force;
};
std::ostream& operator<<(std::ostream& f, const CarteNormale& c);

template <class Carte, size_t N>
class Pioche
{
public:
    bool estVide() const { return nbCartes == 0; };
    size_t getNbCartes() const { return nbCartes; };
    void setCarte(size_t n, const Couleur& c, const Force& f){
        cartes[n] = new Carte(c, f);
    }

    void swapCartes(Carte& c1, Carte& c2){
        Carte tmp = c1;
        c1=c2;
        c2=tmp;
    }

    bool piocher(Carte& carte) {
        if (estVide())
            return false;
        carte = piocherCarte();
        return true;
    };
    void placerDessous(const Carte& carte){
        // A REPRENDRE, nécessaire pour implémenter la version tactique
        cartes[cartes.size()] = carte;
    };
    Pioche(): cartes(), nbCartes(N) {
        for (size_t i =0; i<N; i++){
            cartes[i] = new Carte;
        }
    }
    ~Pioche() {
        for (size_t i =0; i<N; i++){
            delete cartes[i];
        }
    }
    Carte& operator[](size_t i) const { return *cartes[i]; }
private:
    array<Carte*, N> cartes;
    size_t nbCartes = 0;
    // vector<Carte> cartesDessous; TODO pour la version tactique
    const Carte& piocherCarte(){
        size_t x = rand()%nbCartes;
        swapCartes(*cartes[x], *cartes[--nbCartes]);
        return *cartes[nbCartes];
    }
};




class Tuile
{
public:
    Tuile(): nb_pleine(3), revendiquee(TuileRevendiquee::non_revendiquee){}
    TuileRevendiquee getRevendiquee() const { return revendiquee;}
    void placerCarte(const Carte& c, NumJoueur joueur_num){
        cartes_posees[(int) joueur_num].push_back(c);
        if (cote_plein(joueur_num))
            rempli_en_premier = joueur_num;
    }
    bool cote_plein(NumJoueur joueur_num) const{
        if (cartes_posees[(int) joueur_num].size() == nb_pleine)
            return 1;
        return 0;
    }
    bool est_pleine() const{
        if (cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine)
            return 1;
        return 0;
    }
    void afficher() const{
        for(auto it=cartes_posees[0].begin(); it != cartes_posees[0].end(); it++){
            cout << *it;
        }
        cout << " //// ";
        for(auto it=cartes_posees[1].begin(); it != cartes_posees[1].end(); it++){
            cout << *it;
        }
    }
    bool verif_meme_couleur(NumJoueur num_joueur) const;
    bool verif_meme_force(NumJoueur num_joueur) const;
    bool verif_suite(NumJoueur num_joueur) const;
    unsigned int get_somme(NumJoueur num_joueur) const;
    bool verif_revendiquable(NumJoueur num_joueur) const;
    void revendiquer(NumJoueur num_joueur);
    bool estRevendiquee() const;
private:
    array<vector<Carte>, 2> cartes_posees;
    //CarteTactique carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_pleine;
    NumJoueur rempli_en_premier;
    TuileRevendiquee revendiquee;
};

class Frontiere{
public:
    void afficherFrontiere() const{
        cout << "\n----------------------Affichage de la frontiere---------------------------";
        for (size_t i=0; i<nb_tuile; i++){
            cout<< "\nBorne " << i+1 <<"\n";
            tuiles[i].afficher();
        }
        cout << "\n";
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
    Main(size_t taille):cartes(), taille_max(taille){}
    bool estVide() const { return nbCartes == 0; }
    size_t getNbCartes() const { return nbCartes; }
    size_t getTailleMax() const { return taille_max; }
    const Carte& getCarte(size_t i) const{ return *cartes[i]; }
    const Carte& jouerCarte(size_t i){ cartes.erase(cartes.begin() + i); nbCartes--; return *cartes[i]; }
    void piocherCarte(const Carte& carte){ cartes.push_back(&carte);  nbCartes++;}
private:
    vector<const Carte*> cartes;
    size_t taille_max;
    size_t nbCartes = 0;
};



class Agent
{
public:
    Movement jouer(const Frontiere &frontiere, NumJoueur joueur_num);
    void jouerCarte(Frontiere& frontiere, size_t pos_carte, size_t pos_borne, NumJoueur joueur_num, Force& f, Couleur& coul){
        const Carte& c = main.jouerCarte(pos_carte);
        f = c.getForce();
        coul = c.getCouleur();
        frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);
    }
    void revendiquerBorne(Frontiere& frontiere, unsigned int num_borne, NumJoueur joueur_num){
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }
    void piocher(const Carte &carte);
    Agent(size_t taille): main(Main(taille)) {}
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
    Joueur(size_t taille): score(0), agent(Agent(taille)) {}
    Joueur(const string& n, size_t taille): nom(n), score(0), agent(Agent(taille)) {}
    ~Joueur() = default;
private:
    string nom;
    unsigned int score;
    // bool ia;  // à ajouter lorsque l'on implémentera la possibilité de jouer contre une ia
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
    ~PremiereNormale(){
        for (size_t i=0; i<NCARTENORMALE; i++){
            delete &piocheNormale[i];
        }
    }
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
