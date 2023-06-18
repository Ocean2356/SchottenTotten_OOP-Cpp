/***********************************************************************************************************************
Nom : partie.h
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier header utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des classes
représentant les cartes, la pioche, la main, les tuiles, la frontière, les agents et les parties
***********************************************************************************************************************/


#ifndef PARTIE_H
#define PARTIE_H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <chrono>

class Tuile;
class Joueur;


enum class JoueurGagnant{aucun, joueur1, joueur2};
enum class NumJoueur{joueur1, joueur2};
enum class Combinaison{somme, suite, couleur, brelan, suite_couleur};
enum class TuileRevendiquee{non_revendiquee, revendiquee_joueur1, revendiquee_joueur2};



using namespace std;
using Ordre = array<Joueur*  , 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs à la fin d'une partie
using Movement = string; // string représentant une action à effectuer (jouer telle carte sur telle borne, revendiquer telle borne...)
using Pos = char;
const Pos ERREUR = -1;
using TableauJouee = array<array<bool, 9>, 6>;


// classe permettant de lever des exceptions pour les erreurs ayant lieu dans une partie
class PartieException : public std::exception{
public:
    explicit PartieException(const string& i) : info(i){}
    string getInfo() const{ return info; }
private:
    string info;
};

// caractéristiques
enum class Couleur{Rouge, Marron, Jaune, Vert, Bleu, Violet};
enum class Force{
    un = 1, deux = 2, trois = 3, quatre = 4, cinq = 5, six = 6, sept = 7, huit = 8, neuf = 9
};

// listes contenant les valeurs possibles pour chacune des caractéristiques
extern std::initializer_list<Couleur> Couleurs;
extern std::initializer_list<Force> Forces;

// conversion d'une caractéristique en string
string toString(Couleur c);
string toString(Force f);


class Carte{ // classe abstraite Carte
public:
    virtual string getInfo() const = 0;
    virtual bool estTactique() const = 0;
    virtual ~Carte() = default;
};

std::ostream& operator<<(std::ostream& f, const Carte& c);  // Affichage d'une carte sur un flux ostream

class CarteNormale : public Carte{  // CarteNormale, classe héritant de la classe abstraite carte
public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force){}
    CarteNormale(const CarteNormale& c) = default;
    CarteNormale& operator=(const CarteNormale& c) = default;
    const CarteNormale& operator=(const CarteNormale& c) const {
        const CarteNormale& c2 = c;
        return c2;
    }
    string getInfo() const override{  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return toString(couleur) + toString(force) + ' ';
    }
    Couleur getCouleur() const { return couleur; }
    Force getForce() const { return force; }
    void setCouleur(const Couleur& c) {couleur = c;}
    void setForce(const Force& f){force = f;}
    bool estTactique() const override {return false;}
private:
    Couleur couleur;
    Force force;
};

template<class Carte, size_t N>  // La pioche peut être une pioche de cartes normales ou une pioche de cartes tactiques
class Pioche {  // classe Pioche représentant une des pioches du jeu
public:
    Pioche() : nbCartes(N){}
    Pioche(const Pioche& p) = delete;
    Pioche& operator=(const Pioche& p) = delete;
    bool estVide() const{ return nbCartes == 0; } // Méthode indiquant si la pioche est vide
    size_t getNbCartes() const{ return nbCartes; }

    // Méthode permettant d'allouer une nouvelle carte normale de la pioche
    void setCarteNormale(size_t n, const Couleur& c, const Force& f){
        cartes[n] = new Carte(c, f);
    }

    // Méthode permettant d'échanger deux cartes de la pioche
    void swapCartes(const Carte& c1, const Carte& c2) const{
        Carte tmp = c1;
        c1 = c2;
        c2 = tmp;
    }

    // Méthode permettant de piocher une carte
    virtual const Carte& piocher() {
        if (estVide())
            throw PartieException("La pioche est vide");
        try{
            // Génération d'un nombre aléatoire entre 0 et nbCartes - 1 (pour piocher une des cartes piochable de la pioche)
            std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
            std::uniform_int_distribution<size_t> distrib{0, nbCartes - 1};
            size_t x = distrib(random_eng);
            // On "supprime" la carte piochée, en l'échangeant avec la dernière carte de la pioche et en décrementant le nombre de cartes piochable
            toString(cartes[x]->getCouleur());
            toString(cartes[x]->getForce());
            toString(cartes[nbCartes - 1]->getCouleur());
            toString(cartes[nbCartes - 1]->getForce());
            swapCartes((*cartes[x]), *cartes[--nbCartes]);
            return *cartes[nbCartes];  // on retourne la carte que l'on vient de "supprimer"
        }catch(PartieException& e){
            cout << "Erreur "<< e.getInfo() << " lors de la pioche";
            try{
                toString(cartes[nbCartes - 1]->getCouleur());
                toString(cartes[nbCartes - 1]->getForce());
                return *cartes[--nbCartes];
            }catch(PartieException& e){
                nbCartes --;
                return *cartes[0];
            }

        }

    };
    void placerDessous(const Carte* carte){  // Méthode permettant de placer une carte en dessous de la pioche
        cartes[nbCartes++] = carte;
    };
    ~Pioche(){  // destructeur qui désalloue les cartes de la pioche
        for (size_t i = 0; i < N; i++){
            delete cartes[i];
        }
    }
protected:
    array<const Carte*, N> cartes;  // array de N cartes
    size_t nbCartes;  // nombre de cartes piochables
    // vector<Carte> cartesDessous; TODO pour la version tactique (+ mettre une carte aléatoire en dessous de la pioche initialement)
};


class Main final{  // classe permettant de représenter une main
public:
    Main() = default;
    Main(size_t t_max) : taille_max(t_max){}
    Main(const Main& m) = default;
    Main& operator=(const Main& m) = default;
    bool estVide() const{ return nbCartes == 0; }  // TODO à utiliser dans la version tactique
    size_t getNbCartes() const{ return nbCartes; }
    size_t getTailleMax() const{ return taille_max; }
    const Carte& getCarte(size_t i) const{ return *cartes[i]; }
    const Carte& operator[](size_t i) const{ return *cartes[i]; }
    // Méthode permettant de jouer une carte de la main
    const Carte& jouerCarte(unsigned int i){
        const Carte& carte = *cartes[i];
        cartes.erase(cartes.begin() + i);  // on efface la carte de la main
        nbCartes--;  // on décrémente le nombre de cartes de la main
        return carte;  // on retourne la carte à jouer
    }

    // Méthode permettant de piocher une carte, c'est-à-dire de l'ajouter à la main
    void piocherCarte(const Carte& carte){
        cartes.push_back(&carte);  // ajout de la carte dans la main
        nbCartes++;  // on incrémente le nombre de cartes de la main
    }

    vector<const Carte*>& getCartes() {
        return cartes;
    }

    ~Main() = default;
private:
    vector<const Carte* > cartes;  // vector représentant les cartes de la main d'un joueur
    size_t taille_max;  // par défaut, la main contient au maximum 6 cartes. Ce nombre est modifié pour la partie tactique
    size_t nbCartes = 0;  // initialement, la main est vide
};


class Tuile{  // classe permettant de gérer une Tuile du jeu (Borne ou Muraille en fonction de l'édition)
public:
    Tuile() : nb_pleine(3), revendiquee(TuileRevendiquee::non_revendiquee){}
    Tuile(const Tuile& t) = delete;
    Tuile& operator=(const Tuile& t) = delete;
    TuileRevendiquee getRevendiquee() const{ return revendiquee; }

    // Méthode vérifiant si le côté d'un joueur est plein
    bool cotePlein(NumJoueur joueur_num) const{ return cartes_posees[(int) joueur_num].size() == nb_pleine; }

    // Méthode vérifiant si la tuile est pleine des deux côtés
    virtual bool estPleine() const{
        return cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine;
    }

    // Méthode permettant de vérifier si une tuile est revendiquée ou non
    virtual bool estRevendiquee() const{
        return revendiquee == TuileRevendiquee::revendiquee_joueur1 ||
               revendiquee == TuileRevendiquee::revendiquee_joueur2;
    }

    // Méthode permettant de placer une carte d'un côté de la tuile
    virtual void placerCarte(const Carte& c, NumJoueur joueur_num){
        if (cotePlein(joueur_num))
            throw PartieException("La tuile est deja pleine\n");
        cartes_posees[(int) joueur_num].push_back(&c);
        if (cotePlein(joueur_num) && !estPleine())
            // le joueur vient de remplir la tuile de son côté et elle n'est pas pleine du côté de son adversaire
            rempli_en_premier = joueur_num;  // utilisé dans verif_revendiquable en cas d'égalité parfaite de combinaison
    }

    // permet de vérifier que toutes les cartes d'un côté de la tuile ont la même couleur
    bool verifMemeCouleur(NumJoueur num_joueur) const;

    // permet de vérifier que toutes les cartes d'un côté de la tuile ont la même force
    bool verifMemeForce(NumJoueur num_joueur) const;

    // Méthode appelée par verifSuite permettant de trier les cartes en fonction de la force d'un côté de la tuile
    void ordonnerCartes(NumJoueur num_joueur);

    // permet de vérifier que les cartes d'un côté de la tuile forment une suite
    bool verifSuite(NumJoueur num_joueur);

    // permet de sommer la force de toutes les cartes d'un côté de la tuile
    unsigned int getSomme(NumJoueur num_joueur) const;

    // permet de vérifier qu'un joueur peut revendiquer une tuile
    virtual bool verifRevendiquable(NumJoueur num_joueur, TableauJouee tab);

    virtual Combinaison determinerCombinaison(NumJoueur num_joueur);

    // revendique une tuile pour un joueur
    virtual void revendiquer(NumJoueur num_joueur);

    virtual ~Tuile() = default;
protected:
    friend class UI;
    array<vector<const Carte*>, 2> cartes_posees;  // représente les cartes posées de part et d'autre de la tuile
    //CarteTactique carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_pleine;  // nombre de cartes pour que la tuile soit pleine d'un côté (3 en général, 4 si la carte combat de Boue est posée au centre)
    NumJoueur rempli_en_premier;  // utilisé dans verif_revendiquable en cas d'égalité parfaite de combinaison
    TuileRevendiquee revendiquee;  // indique si la tuile est revendiquée (et par quel joueur) ou non
    bool verifRevendiquableNonPleine(NumJoueur num_joueur, NumJoueur autre_joueur, Combinaison combinaison, TableauJouee tab) const;
    bool verifSuiteCouleurPossible(bool joueur_actif_suite_couleur, bool meme_couleur, bool suite, int force_max, int force_min, unsigned int somme, unsigned int somme_joueur_actif, int couleur_carte_a_jouer, TableauJouee tab) const;
    bool verifBrelanPossible(bool joueur_actif_brelan, bool meme_force, int force_carte_a_jouer, unsigned int somme_joueur_actif, TableauJouee tab) const;
    bool verifMemeCouleurPossible(bool joueur_actif_meme_couleur, bool meme_couleur, unsigned int somme, unsigned int somme_joueur_actif, int couleur_carte_a_jouer, TableauJouee tab) const;
    bool verifSuitePossible(bool joueur_actif_suite, bool suite, int force_max, int force_min, unsigned int somme, unsigned int somme_joueur_actif, TableauJouee tab) const;
    bool verifSommePossible(unsigned int somme, unsigned int somme_joueur_actif, TableauJouee tab) const;
};


template<class T>
class Frontiere final{  // classe permettant de représenter une frontière
public:
    Frontiere() = default;
    Frontiere(const Frontiere& f) = delete;
    Frontiere& operator=(const Frontiere& f) = delete;
    const unsigned int getNbTuile() const{ return nb_tuile; };
    bool verifRevendiquable(size_t i, NumJoueur num_joueur, TableauJouee tab) { return tuiles[i].verifRevendiquable(num_joueur, tab);}
    bool verifRevendiquable(size_t i, NumJoueur num_joueur) { return tuiles[i].verifRevendiquable(num_joueur);}
    // Méthode permettant de calculer le score du joueur qui a perdu la partie
    unsigned int calculerScore(NumJoueur joueur_num) const;

    // Méthode permettant de retourner le joueur gagnant ou JoueurGagnant::aucun si aucun joueur n'a gagné à ce stade
    JoueurGagnant estFinie() const;

    // Méthode permettant d'afficher la frontière
    void afficherFrontiere() const;
    ~Frontiere() = default;

private:
    friend class UI;
    friend class UITactique;
    friend class Agent;  // un agent peut accéder aux attributs de la classe frontiere, en particulier tuiles
    friend class AgentTactique; // un agent tactique peut accéder aux attributs de la classe frontier, en particulier tuiles
    static const unsigned int nb_tuile = 9; // le nombre de tuiles vaut 9 pour la première édition // TODO changer pour pouvoir gérer la deuxième édition (7 tuiles)
    array<T, nb_tuile> tuiles;  // représente l'ensemble des tuiles
};


// Méthode permettant de calculer le score du joueur qui a perdu la partie
template <class T>
unsigned int Frontiere<T>::calculerScore(NumJoueur joueur_num) const{
    // Pour cela, on compte puis retourne le nombre de bornes revendiquéess par ce joueur.
    unsigned int score = 0;
    for (size_t i = 0; i < nb_tuile; i++){
        if ((int) tuiles[i].getRevendiquee() == (int) joueur_num + 1)
            score++;
    }
    return score;
}


// Méthode permettant de retourner le joueur gagnant ou JoueurGagnant::aucun si aucun joueur n'a gagné à ce stade
template <class T>
JoueurGagnant Frontiere<T>::estFinie() const{
    unsigned int adjacent = 1;  // compte le nombre de tuiles adjacentes revendiquées par un même joueur
    unsigned int joueur_preced = 0;  // joueur ayant revendiqué la borne précédente
    unsigned int nb_joueur_1 = 0;  // nombre de bornes revendiquées par le joueur 1
    unsigned int nb_joueur_2 = 0;  // nombre de bornes revendiquées par le joueur 2

    for (size_t i = 0; i < nb_tuile; i++){
        TuileRevendiquee num_joueur = tuiles[i].getRevendiquee();
        if (num_joueur == TuileRevendiquee::revendiquee_joueur1){  // le joueur 1 a revendiqué la tuile
            if (joueur_preced == 1){  // le joueur 1 a également revendiqué la tuile précédente
                adjacent++;
                if (adjacent == 3)
                    // 3 tuiles adjacentes revendiquées par le joueur 1, il l'emporte
                    return JoueurGagnant::joueur1;
            } else
                // le joueur 1 n'a pas revendiqué la tuile précédente
                adjacent = 1;
            nb_joueur_1++;

            if (nb_joueur_1 == 5)
                // 5 tuiles revendiquées par le joueur 1, il l'emporte
                return JoueurGagnant::joueur1;
            joueur_preced = 1;
        } else if (num_joueur == TuileRevendiquee::revendiquee_joueur2){  // le joueur 2 a revendiqué la tuile
            if (joueur_preced == 2){  // le joueur 2 a également revendiqué la tuile précédente
                adjacent++;
                if (adjacent == 3)
                    // 3 tuiles adjacentes revendiquées par le joueur 2, il l'emporte
                    return JoueurGagnant::joueur2;
            } else
                // le joueur 2 n'a pas revendiqué la tuile précédente
                adjacent = 1;
            nb_joueur_2++;

            if (nb_joueur_2 == 5)
                // 5 tuiles revendiquées par le joueur 2, il l'emporte
                return JoueurGagnant::joueur2;
            joueur_preced = 2;
        } else{  // Tuile non revendiquee
            joueur_preced = 0;
            adjacent = 1;
        }
    }
    // On a parcouru toutes les tuiles sans trouver trois tuiles adjacentes revendiquées par un même joueur
    // ni 5 tuiles revendiquées par un même joueur. La partie n'est donc pas encore terminée
    return JoueurGagnant::aucun;
}



class UI
{
public:
    virtual void afficherFrontiere(const  Frontiere<class Tuile> &f) const;
    virtual void afficherCote(const Tuile &t, size_t cote) const;
    virtual void afficherEtatBorne(const Tuile &t, size_t num_borne) const;
    virtual Pos getChoixCarte(Main& main);
    virtual Pos getChoixCarteIa(Main& main);
    virtual Pos getChoixBorne(const Frontiere<class Tuile>& f, NumJoueur joueur_num);
    virtual Pos getChoixBorneIa(const Frontiere<class Tuile>& f, NumJoueur joueur_num);

    virtual Movement getChoixBornesARevendiquer(Frontiere<class Tuile>& frontiere, NumJoueur joueur_num, TableauJouee tab);
    virtual Movement getChoixBornesARevendiquerIa(Frontiere<class Tuile>& frontiere, NumJoueur joueur_num, TableauJouee tab);
};


class Agent{  // classe représentant un agent. L'agent peut agir sur la partie (jouer et piocher des cartes, revendiquer des bornes...)
public:
    Agent() = default;
    Agent(size_t taille_main) : main(Main(taille_main)){}
    Agent(const Agent& a) = default;
    Agent& operator=(const Agent& a) = default;
    const bool& getIa() const { return ia;}
    void setIa (const bool& est_ia) {ia = est_ia;}
    // Méthode permettant de piocher une carte
    void piocher(const Carte& carte){
        if (main.getTailleMax() == main.getNbCartes())
            throw PartieException("La main est pleine");
        main.piocherCarte(carte);
    }

    // Méthode permettant la saisie par l'utilisateur d'une carte à jouer
    virtual Movement choisirCarteAJouer(const Frontiere<class Tuile>& f, NumJoueur joueur_num);

    // Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
    virtual void jouerCarte(Frontiere<class Tuile>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, Force& f, Couleur& coul);

    // Méthode permettant la saisie par l'utilisateur d'une ou plusieurs bornes à revendiquer
    virtual Movement choisirBornesARevendiquer(Frontiere<class Tuile>& frontiere, NumJoueur joueur_num, TableauJouee tab);

    // Méthode permettant de revendiquer une borne dont le numéro est donné
    virtual void revendiquerBorne(Frontiere<class Tuile>& frontiere, unsigned int num_borne, NumJoueur joueur_num){
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }
    Main getMain() const{ return main; }

    virtual ~Agent() = default;
private:
    UI ui = UI();
    bool ia;
    // un agent a une main
protected:
    Main main;
};


class Partie{  // classe abstraite permettant de représenter une partie
public:
    Partie() = default;
    Partie(const Partie& p) = delete;
    Partie& operator=(const Partie& p) = delete;
    virtual void commencer(Ordre ordre) = 0;
    virtual void jouerTour() = 0;

    // Méthode permettant de tester, par appel à Frontiere::estFinie(), si la partie est terminée
    virtual bool testerFin(){
        return frontiere.estFinie() != JoueurGagnant::aucun;
    }
    virtual Resultat terminer();  // Méthode permettant de calculer et de retourner le score des joueurs à la fin d'une partie
    virtual ~Partie() = default;
protected:
    Frontiere<class Tuile> frontiere;
};


// classe abstraite permettant de spécifier certaines méthodes et caractéristiques communes aux variantes d'une partie de la première édition
class Premiere : public Partie{
public:
    virtual void initierAgents(Ordre ordre) = 0;
    virtual void initierMains() = 0;

    // Méthode permettant d'initialiser les agents et les mains pour commencer une partie
    void commencer(Ordre ordre) final{
        initierAgents(ordre);
        initierPiocheNormale();
        initierMains();
    }
    void initierPiocheNormale();  // initialisation de la pioche normale

    // Méthode permettant de mettre à jour l'agent actif
    void agentSuivant(){
        agentActive++;
        agentActive %= 2;
    }

    virtual ~Premiere() = default;
protected:
    static const int NCOULEUR = 6;  // 6 couleurs pour la première édition
    static const int FORCEMIN = 1;
    static const int FORCEMAX = 9;
    static const int NFORCE = FORCEMAX - FORCEMIN + 1;  // 9 forces pour la première édition
    // 9*6 = 54 cartes normales pour la première édition (auxquelles s'ajoutent d'éventuelles cartes tactiques)
    static const int NCARTENORMALE = NCOULEUR * NFORCE;
    int agentActive;  // représente l'agent actif, c'est-à-dire le joueur dont c'est le tour
    Pioche<CarteNormale, NCARTENORMALE> piocheNormale; // pioche de 54 cartes normales (utilisée par les variantes normales et tactique)
};


// classe correspondant à la variante Normale de la première édition de Schotten-Totten
class PremiereNormale final : public Premiere{
public:
    PremiereNormale();
    // Méthode permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
    void jouerTour() override;
    ~PremiereNormale() = default;
private:
    static const int NMAIN = 6;  // 6 cartes dans la main dans cette variante
    array<Agent, 2> agents{Agent(NMAIN), Agent(NMAIN)};  // tableau des agents de la partie
    using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;
    TableauJouee tableauJouee;  // tableau utilisé pour stocker les cartes déjà jouées
    // Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
    void initierAgents(Ordre ordre) override;

    // Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
    void initierMains() override;
};

#endif