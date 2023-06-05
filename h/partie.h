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

class Joueur;

using namespace std;
using Ordre = array<Joueur *, 2>; // ordre des joueurs qui jouent
using Resultat = array<unsigned int, 2>; // score des joueurs à la fin d'une partie
using Movement = string; // string représentant une action à effectuer (jouer telle carte sur telle borne, revendiquer telle borne...)

enum class TuileRevendiquee {
    non_revendiquee, revendiquee_joueur1, revendiquee_joueur2
};  // utilisé pour stocker, pour chaque tuile, si elle a été revendiquée et par quel joueur
enum class NumJoueur {
    joueur1, joueur2
};
enum class JoueurGagnant {
    aucun, joueur1, joueur2
};
enum class Combinaison {
    somme, suite, couleur, brelan, suite_couleur
};  // différentes combinaisons de cartes possibles

class PartieException
        : public std::exception {  // classe permettant de lever des exceptions pour les erreurs ayant lieu dans une partie
public:
    explicit PartieException(const string &i) : info(i) {}

    string getInfo() const { return info; }

private:
    string info;
};

// caractéristiques
enum class Couleur {
    Rouge, Marron, Jaune, Vert, Bleu, Violet
};
enum class Force {
    un = 1, deux = 2, trois = 3, quatre = 4, cinq = 5, six = 6, sept = 7, huit = 8, neuf = 9
};

// listes contenant les valeurs possibles pour chacune des caractéristiques
extern std::initializer_list<Couleur> Couleurs;
extern std::initializer_list<Force> Forces;

// conversion d'une caractéristique en string
string toString(Couleur c);

string toString(Force f);


class Carte { // classe abstraite Carte
public:
    virtual string getInfo() const = 0;

    virtual string getDescription() const = 0;

    virtual Force getForce() const = 0;

    virtual Couleur getCouleur() const = 0;

    virtual ~Carte() = default;
};

std::ostream &operator<<(std::ostream &f, const Carte &c);  // Affichage d'une carte sur un flux ostream

class CarteNormale final : public Carte {  // CarteNormale, classe héritant de la classe abstraite carte
public:
    CarteNormale(Couleur _couleur, Force _force) : couleur(_couleur), force(_force) {}

    CarteNormale() = default;

    CarteNormale(const CarteNormale &c) = default;

    CarteNormale &operator=(const CarteNormale &c) = default;

    virtual string getInfo() const override {  // méthode utilisée lors de l'affichage d'une carte sur un flux ostream
        return toString(couleur) + toString(force) + ' ';
    }

    virtual string
    getDescription() const override {  // méthode utilisée pour informer l'utilisateur des effets d'une carte
        // TODO appel de la méthode dans un tour de jeu si l'utilisateur souhaite se renseigner sur une carte
        return "Clan : force = " + toString(force) + " Couleur = " + toString(couleur);
    }

    Couleur getCouleur() const override { return couleur; }

    Force getForce() const override { return force; }

private:
    Couleur couleur;
    Force force;
};

template<class Carte, size_t N>  // La pioche peut être une pioche de cartes normales ou une pioche de cartes tactiques
class Pioche final {  // classe Pioche représentant une des pioches du jeu
public:
    Pioche() : nbCartes(N) {}

    Pioche(const Pioche &p) = delete;

    Pioche &operator=(const Pioche &p) = delete;

    bool estVide() const { return nbCartes == 0; } // Méthode indiquant si la pioche est vide
    size_t getNbCartes() const { return nbCartes; }

    void setCarte(size_t n, const Couleur &c,
                  const Force &f) {  // Méthode permettant d'allouer une nouvelle carte de la pioche
        cartes[n] = new Carte(c, f);
    }

    void swapCartes(Carte &c1, Carte &c2) {  // Méthode permettant d'échanger deux cartes de la pioche
        Carte tmp = c1;
        c1 = c2;
        c2 = tmp;
    }

    // Méthode permettant de piocher une carte
    const Carte &piocher() {
        if (estVide())
            throw PartieException("La pioche est vide");
        // Génération d'un nombre aléatoire entre 0 et nbCartes - 1 (pour piocher une des cartes piochable de la pioche)
        std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distrib{0, (int) nbCartes - 1};
        size_t x = distrib(random_eng);

        // On "supprime" la carte piochée, en l'échangeant avec la dernière carte de la pioche et en décrementant le nombre de cartes piochable
        swapCartes(*cartes[x], *cartes[--nbCartes]);
        return *cartes[nbCartes];  // on retourne la carte que l'on vient de "supprimer"
    };

    void placerDessous(const Carte &carte) {  // Méthode permettant de placer une carte en dessous de la pioche
        // TODO A REPRENDRE, nécessaire pour implémenter la version tactique, utiliser l'attribut cartesDessous
        cartes[cartes.size()] = carte;
    };

    ~Pioche() {  // destructeur qui désalloue les cartes de la pioche
        for (size_t i = 0; i < N; i++) {
            delete cartes[i];
        }
    }

private:
    array<Carte *, N> cartes;  // array de N cartes
    size_t nbCartes;  // nombre de cartes piochables
    // vector<Carte> cartesDessous; TODO pour la version tactique (+ mettre une carte aléatoire en dessous de la pioche initialement)
};


class Tuile {  // classe permettant de gérer une Tuile du jeu (Borne ou Muraille en fonction de l'édition)
public:
    Tuile() : nb_pleine(3), revendiquee(TuileRevendiquee::non_revendiquee) {}

    Tuile(const Tuile &t) = delete;

    Tuile &operator=(const Tuile &t) = delete;

    TuileRevendiquee getRevendiquee() const { return revendiquee; }

    // Méthode vérifiant si le côté d'un joueur est plein
    bool cotePlein(NumJoueur joueur_num) const { return cartes_posees[(int) joueur_num].size() == nb_pleine; }

    // Méthode vérifiant si la tuile est pleine des deux côtés
    virtual bool estPleine() const {
        return cartes_posees[0].size() == nb_pleine && cartes_posees[1].size() == nb_pleine;
    }

    // Méthode permettant de vérifier si une tuile est revendiquée ou non
    virtual bool estRevendiquee() const {
        return revendiquee == TuileRevendiquee::revendiquee_joueur1 ||
               revendiquee == TuileRevendiquee::revendiquee_joueur2;
    }

    // Méthode permettant de placer une carte d'un côté de la tuile
    virtual void placerCarte(const Carte &c, NumJoueur joueur_num) {
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
    virtual bool verifRevendiquable(NumJoueur num_joueur);

    // revendique une tuile pour un joueur
    virtual void revendiquer(NumJoueur num_joueur);

    // Méthode permettant d'afficher les cartes d'un côté de la tuile
    virtual void afficherCote(size_t cote) const {
        // Cette méthode est appelée par Frontiere::afficherFrontiere()
        size_t iter = 1;
        for (auto c: cartes_posees[cote]) {
            cout << *c;
            iter++;
        }
        while (iter < 5) {
            // il y a au maximum 4 cartes d'un côté d'une tuile. Pour centrer l'affichage, on remplit toujours les positions vides
            cout << "    ";
            iter++;
        }
        cout << "|";
    }

    // Méthode permettant d'afficher l'état d'une tuile
    virtual void afficherEtatBorne(size_t num_borne) const {
        // Cette méthode est appelée par Frontiere::afficherFrontiere()
        cout << "      ";  // utilisé pour centrer l'affichage
        if (revendiquee == TuileRevendiquee::revendiquee_joueur1)
            cout << "/R1/";
        else if (revendiquee == TuileRevendiquee::revendiquee_joueur1)
            cout << "/R2/";
        else
            cout << "/B" << num_borne << "/ ";
        cout << "     |";  // utilisé pour centrer l'affichage
    }

    virtual ~Tuile() = default;

private:
    array<vector<const Carte *>, 2> cartes_posees;  // représente les cartes posées de part et d'autre de la tuile
    //CarteTactique carte_posee_centre;  // A implémenter pour la version tactique
    unsigned int nb_pleine;  // nombre de cartes pour que la tuile soit pleine d'un côté (3 en général, 4 si la carte combat de Boue est posée au centre)
    NumJoueur rempli_en_premier;  // utilisé dans verif_revendiquable en cas d'égalité parfaite de combinaison
    TuileRevendiquee revendiquee;  // indique si la tuile est revendiquée (et par quel joueur) ou non
};


class Frontiere final {  // classe permettant de représenter une frontière
public:
    Frontiere() = default;

    Frontiere(const Frontiere &f) = delete;

    Frontiere &operator=(const Frontiere &f) = delete;

    // Méthode permettant d'afficher la frontière
    void afficherFrontiere() const {
        cout
                << "\n----------------------------------------------------------------Affichage de la frontiere----------------------------------------------------------------\n";
        // Affichage des cartes des tuiles du côté du premier joueur
        for (size_t i = 0; i < nb_tuile; i++)
            tuiles[i].afficherCote(0);
        cout << "\n";

        // Affichage de l'état des tuiles (le numéro du joueur qui l'a revendiquée ou le numéro de la tuile si elle n'est pas revendiquée)
        for (size_t i = 0; i < nb_tuile; i++)
            tuiles[i].afficherEtatBorne(i + 1);
        cout << "\n";

        // Affichage des cartes des tuiles du côté du second joueur
        for (size_t i = 0; i < nb_tuile; i++)
            tuiles[i].afficherCote(1);
        cout
                << "\n---------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    const unsigned int getNbTuile() const { return nb_tuile; };

    unsigned int calculerScore(
            NumJoueur joueur_num) const;  // Méthode permettant de calculer le score qu'un joueur a obtenu en fin de partie
    JoueurGagnant
    estFinie() const;  // Méthode permettant de retourner le joueur gagnant ou JoueurGagnant::aucun si aucun joueur n'a gagné à ce stade
    ~Frontiere() = default;

private:
    friend class Agent;  // un agent peut accéder aux attributs de la classe tuile, en particulier tuiles
    static const unsigned int nb_tuile = 9; // le nombre de tuiles vaut 9 pour la première édition // TODO changer pour pouvoir gérer la deuxième édition (7 tuiles)
    array<Tuile, nb_tuile> tuiles;  // représente l'ensemble des tuiles
};


class Main final {  // classe permettant de représenter une main
public:
    Main() = default;

    Main(size_t t_max) : taille_max(t_max) {}

    Main(const Main &m) = default;

    Main &operator=(const Main &m) = default;

    bool estVide() const { return nbCartes == 0; }  // TODO à utiliser dans la version tactique
    size_t getNbCartes() const { return nbCartes; }

    size_t getTailleMax() const { return taille_max; }

    const Carte &getCarte(size_t i) const { return *cartes[i]; }

    // Méthode permettant de jouer une carte de la main
    const Carte &jouerCarte(unsigned int i) {
        const Carte &carte = *cartes[i];
        cartes.erase(cartes.begin() + i);  // on efface la carte de la main
        nbCartes--;  // on décrémente le nombre de cartes de la main
        return carte;  // on retourne la carte à jouer
    }

    // Méthode permettant de piocher une carte, c'est-à-dire de l'ajouter à la main
    void piocherCarte(const Carte &carte) {
        cartes.push_back(&carte);  // ajout de la carte dans la main
        nbCartes++;  // on incrémente le nombre de cartes de la main
    }

    ~Main() = default;

private:
    vector<const Carte *> cartes;  // vector représentant les cartes de la main d'un joueur
    size_t taille_max = 6;  // par défaut, la main contient au maximum 6 cartes. Ce nombre est modifié pour la partie tactique
    size_t nbCartes = 0;  // initialement, la main est vide
};


class Agent {  // classe représentant un agent. L'agent peut agir sur la partie (jouer et piocher des cartes, revendiquer des bornes...)
public:
    Agent() = default;

    Agent(size_t taille_main) : main(Main(taille_main)) {}

    Agent(const Agent &a) = default;

    Agent &operator=(const Agent &a) = default;

    Movement choisirCarteAJouer(const Frontiere &frontiere,
                                NumJoueur joueur_num);  // Méthode permettant la saisie par l'utilisateur d'une carte à jouer
    Movement choisirBornesARevendiquer(Frontiere &frontiere,
                                       NumJoueur joueur_num);  // Méthode permettant la saisie par l'utilisateur d'une ou plusieurs bornes à revendiquer

    // Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
    void jouerCarte(Frontiere &frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, Force &f,
                    Couleur &coul) {
        const Carte &c = main.jouerCarte(pos_carte);  // on récupère la carte à jouer
        f = c.getForce(); // f est utilisée pour mettre à jour le tableau des cartes jouées
        coul = c.getCouleur();  // coul est utilisée pour mettre à jour le tableau des cartes jouées
        frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière
    }

    // Méthode permettant de revendiquer une borne dont le numéro est donné
    void revendiquerBorne(Frontiere &frontiere, unsigned int num_borne, NumJoueur joueur_num) {
        frontiere.tuiles[num_borne].revendiquer(joueur_num);
    }

    void piocher(const Carte &carte);  // Méthode permettant de piocher une carte
    ~Agent() = default;

private:
    Main main;  // un agent a une main
};


class Joueur {  // classe représentant un joueur du jeu
public:
    Joueur() = default;

    Joueur(const string &n) : nom(n) {}

    Joueur(const Joueur &j) = default;

    Joueur &operator=(const Joueur &j) = default;

    unsigned int getScore() const { return score; }

    void setScore(unsigned int points) { score += points; }

    const string &getNom() const { return nom; }

    const Agent &getAgent() const { return agent; }

    ~Joueur() = default;

private:
    string nom;
    unsigned int score = 0;
    // bool ia;  // TODO à ajouter lorsque l'on implémentera la possibilité de jouer contre une ia (modifier Jeu::commencer_jeu en conséquence, ainsi que toutes les fonctions qui demandent un choix au joueur
    Agent agent;  // un joueur dispose d'un agent
};

class Partie {  // classe abstraite permettant de représenter une partie
public:
    Partie() = default;

    Partie(const Partie &p) = delete;

    Partie &operator=(const Partie &p) = delete;

    virtual void commencer(Ordre ordre) = 0;

    virtual void jouerTour() = 0;

    virtual bool testerFin();

    Resultat terminer();  // Méthode permettant de calculer et de retourner le score des joueurs à la fin d'une partie
    virtual ~Partie() = default;

protected:
    Frontiere frontiere;
};

class Premiere
        : public Partie {  // classe abstraite permettant de spécifier certaines méthodes et caractéristiques communes aux variantes d'une partie de la première édition
public:
    void commencer(Ordre ordre) final;

    // Méthode permettant de mettre à jour l'agent actif
    void agentSuivant() {
        agentActive++;
        agentActive %= 2;
    }

    virtual void initierAgents(Ordre ordre) = 0;

    virtual void initierMains() = 0;

    void initierPiocheNormale();  // initialisation de la pioche normale
protected:
    static const int NCOULEUR = 6;  // 6 couleurs pour la première édition
    static const int FORCEMIN = 1;
    static const int FORCEMAX = 9;
    static const int NFORCE = FORCEMAX - FORCEMIN + 1;  // 9 forces pour la première édition
    static const int NCARTENORMALE = NCOULEUR *
                                     NFORCE; // 9*6 = 54 cartes normales pour la première édition (auxquelles s'ajoutent d'éventuelles cartes tactiques)
    int agentActive;  // représente l'agent actif, c'est-à-dire le joueur dont c'est le tour
    Pioche<CarteNormale, NCARTENORMALE> piocheNormale; // pioche de 54 cartes normales (utilisée par les variantes normales et tactique)
};

class PremiereNormale final
        : public Premiere {  // classe correspondant à la variante Normale de la première édition de Schotten-Totten
public:
    PremiereNormale();

    void
    jouerTour() override;  // fonction permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
    ~PremiereNormale() = default;

private:
    static const int NMAIN = 6;  // 6 cartes dans la main dans cette variante
    array<Agent, 2> agents{Agent(NMAIN), Agent(NMAIN)};  // tableau des agents de la partie
    using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;  // TODO attention, ne sert que pour PremiereNormale, faire attention en impélmentant la version tactique
    TableauJouee tableauJouee;  // tableau utilisé pour stocker les cartes déjà jouées. // TODO s'en servir pour vérifier si une borne est revendiquable ou non avant qu'elle ne soit pleine de l'autre côté
    void initierAgents(
            Ordre ordre) override;  // Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
    void
    initierMains() override;  // Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
};

#endif
