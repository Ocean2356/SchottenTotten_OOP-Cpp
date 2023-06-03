#pragma once

#include <string>
#include <vector>
#include <array>

using namespace std;

using Ordre = array<Joueur*, 2>;
using Resultat = array<size_t, 2>;
using Mouvement = string;

using Force = size_t;
enum class Couleur {Rouge, Marron, Jaune, Vert, Bleu, Violet};
enum class NumJoueur {joueur1, joueur2};
enum class TuileRevendiquee {non_revendiquee, revendiquee_joueur1, revendiquee_joueur2};
enum class Combinaison {somme, suite, couleur, brelan, suite_couleur};

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
		 ~CarteNormale() : default;
		 string getInfo() const;
		 string getDescription() const;
	private:
		 Couleur couleur;
		 Force force;
};

class Pioche
{
	public:
		Pioche(Carte** _cartes, size_t nbCartes) : cartes(_cartes)
		~Pioche() : default;
		bool piocher(Carte** carte);
	private:
		Carte** cartes;
		size_t nbCartes;
		bool estVide() const;
		size_t getNbCartes() const;
};

class Main
{
	public:
		Main(size_t taille) : taille_max(taille){}
		~Main() : default;
		void piocherCarte(Carte* c);
		Carte* getCarte(size_t i);
	private:
		size_t taille_max;
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

class Tuile
{
	public:
		Tuile(size_t n) : nb_pleine(n), revendiquee(TuileRevendiquee::non_revendiquee){}
        ~Tuile() : default;
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
        Frontiere() : initierTuiles();
        ~Frontiere() : default;
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
        Resultat terminer() = 0;
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
        PremiereNormale() : initierCartes();
        ~PremiereNormale() : supprimerCartes();
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
        








