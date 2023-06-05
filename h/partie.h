#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

class Joueur;
class Frontiere;

using Ordre = array<Joueur*, 2>;
using Resultat = array<size_t, 2>;
using Mouvement = string;

using Force = size_t;
enum class Couleur {Rouge, Marron, Jaune, Vert, Bleu, Violet};
enum class NumJoueur {joueur1, joueur2};
enum class TuileRevendiquee {non_revendiquee, revendiquee_joueur1, revendiquee_joueur2};
enum class Combinaison {somme, suite, couleur, brelan, suite_couleur};

// conversion d'une caractéristique en string
string toString(Couleur c);
string toString(Force f)


class PartieException : public exception
{
    public:
        PartieException(const string& message) : info(message){}
        ~PartieException() = default;
        string getInfo() const { return info; }
        //const char* what() const noexcept override {return info.c_str();}
    private:
        string info;
};


class Carte
{
	public:
		virtual string getInfo() const;
		virtual string getDescription() const;

};

class CarteNormale : public Carte
{
	public:
		 CarteNormale(Couleur c, Force f) : couleur(c), force(f){}
		 ~CarteNormale() = default;
         string getInfo() const{ return toString(couleur) + toString(force) + ' ';};
		 string getDescription() const {return description;};

	private:
		 Couleur couleur;
		 Force force;
         string description;
};

class Pioche
{
	public:
		Pioche(Carte** _cartes, size_t nbCartes) : cartes(_cartes){};
		~Pioche() = default;
        size_t getNbCartes() const {return nbCartes;}; // Méthode retournant le nombre de cartes piochables
		bool piocher(Carte** carte);
        void swapCartes(Carte* c1, Carte* c2); // Méthode permettant d'échanger deux cartes de la pioche
	private:
		Carte** cartes;
		size_t nbCartes;
		bool estVide() const{return nbCartes == 0;}; // Méthode indiquant si la pioche est vide

};

class Main  // classe permettant de représenter une main
{
	public:
		Main(size_t taille) : taille_max(taille){}
		~Main() = default;
		void piocherCarte(Carte* c); // Méthode permettant de piocher une carte, c'est-à-dire de l'ajouter à la main
        Carte* jouerCarte(size_t i); // Méthode permettant de supprimer une carte de la main
	private:
		size_t taille_max; // par défaut, la main contient au maximum 6 cartes. Ce nombre est modifié pour la partie tactique
        vector<Carte*> cartes;
};

class Agent
{
	public:
		Mouvement jouer(Frontiere &frontiere, NumJoueur joueur_num);
		void jouerCarte(size_t i);
		void revendiquerBorne(size_t i);
		void piocher(Carte* c);
		void initierMain(Main* m);
	private:
		Main* main;
};

class Joueur
{
    public:
        Joueur(string _nom) : nom(_nom){}
        ~Joueur() = default;
        string getNom() const;
        void setNom(string _nom);
        Agent* getAgent() const;
    private:
        string nom;
        Agent* agent;
};

class Tuile
{
	public:
		Tuile(size_t n) : nb_pleine(n), revendiquee(TuileRevendiquee::non_revendiquee){}
        ~Tuile() = default;
        TuileRevendiquee getRevendiquee() {return revendiquee;}
        void placerCarte(Carte* c, NumJoueur joueur_num);
        bool cote_plein(NumJoueur j);
        bool est_plaine();
        size_t getNbPlein() {return nb_pleine;}
        void setNbPlein(size_t n) {nb_pleine = n;}
    private:
        array<vector<Carte*>, 2> cartes_posees;
        size_t nb_pleine;
        TuileRevendiquee revendiquee;
        NumJoueur rempli_en_premier;
};

class Frontiere{
    public:
        Frontiere();
        ~Frontiere() = default;
        bool estFinie();
        void placerCarte(Carte* c, size_t i, NumJoueur joueur_num);
    private:
        void initierTuiles();
        static const size_t nb_tuiles = 9;
        array<Tuile, nb_tuiles> tuiles;
};

class Partie
{
    public:
        virtual void commencer(Ordre ordre) = 0;
        virtual void jouerTour() = 0;
        virtual void testerFin() = 0;
        virtual Resultat terminer() = 0;
};

class Premiere : public Partie{
    protected:
        static const size_t NCOULEUR = 6;
        static const size_t NFORCEMIN = 1;
        static const size_t NFORCEMAX = 9;
        static const size_t NFORCE = NFORCEMAX - NFORCEMIN + 1;
        static const size_t NCARTENORMALE = NCOULEUR * NFORCE;
};

class PremiereNormale : public Premiere{
    public:
        PremiereNormale();
        ~PremiereNormale();
        void commencer(Ordre ordre);
        void jouerTour();
        void testerFin();
        Resultat terminer();

    private:
        void initierCartes();
        void initierTableauJouee();
        void initierPioche();
        void initierMains();
        void initierAgents();
        void initierFrontiere();
        void initierPartie();

        void agentSuivant();

        void supprimerCartes();
        
        Carte** cartes;
        Pioche pioche;
        array<Agent, 2> agents;
        Frontiere frontiere;

        using TableauJouee = array<array<bool, NFORCE>, NCOULEUR>;
        TableauJouee tableauJouee;
        static const size_t NMAIN = 6;
        size_t agentActive;
};
        








