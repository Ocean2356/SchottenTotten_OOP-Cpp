/***********************************************************************************************************************
Nom : schotten_totten.h
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier header utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des classes
permettant le choix des éditions et de la variante du jeu ainsi que la classe jeu, qui permet de gérer une succession
de parties.
***********************************************************************************************************************/


#ifndef SCHOTTEN_TOTTEN_H
#define SCHOTTEN_TOTTEN_H

#include <string>
#include <array>
#include "../h/partie.h"

using Joueurs = array<Joueur, 2>;

enum class Edition{ Premiere, Deuxieme};
enum class Variante{Normale, Tactique, Experts};

extern std::initializer_list<Edition> Editions;
extern std::initializer_list<Variante> Variantes;

string toString(Edition e);
string toString(Variante v);

Edition choixEdition();  // saisie par l'utilisateur de l'édition du jeu
Variante choixVariante();  // saisie par l'utilisateur de la variante du jeu

class AbstractEdition{
public:
    virtual Partie* getPartie(Variante variante) = 0;
    virtual ~AbstractEdition() = default;
};

class PremiereFactory final : public AbstractEdition{
    Partie* getPartie(Variante variante) override;
};

class DeuxiemeFactory final : public AbstractEdition{
    Partie* getPartie(Variante variante) override;
};

class EditionProducer final{
public:
    AbstractEdition *getEdition(Edition edition);
};

// la classe jeu permet de jouer une succession de parties (potentiellement de différentes éditions et variantes) de Schotten-Totten
class Jeu final{
public:
    Jeu() = default;
    Jeu(const Jeu &j) = delete;
    Jeu &operator=(const Jeu &j) = delete;
    unsigned int getNbJoueurs() const{ return joueurs.size(); }
    string getNom(size_t i) const{ return joueurs[i].getNom(); }
    unsigned int getScore(size_t i) const{ return joueurs[i].getScore(); }

    void commencerJeu();  // permet de saisir les noms des joueurs
    Ordre determinerOrdre(); // permet de choisir l'ordre de jeu des joueurs pour une partie
    void jouerPartie(Edition edition, Variante variante);  // gestion d'une partie (initialisation, déroulement, fin)

    // affiche les points des joueurs et le joueur qui a remporté la partie
    void traiterResultat(Ordre ordre,Resultat resultat);

    // afficher les scores des joueurs et le joueur gagnant une fois toutes les parties disputées
    void finirJeu() const;
    ~Jeu() = default;
private:
    void setIaPlayers(const bool& ia) { only_ia_players = ia;}
    Joueurs joueurs;  // array de deux joueurs
    bool only_ia_players; // est vrai si il y a deux joueurs ia, sert à ralentir le jeu pour que l'utilisateur humain puisse regarder la partie
    EditionProducer editionProducer;  // utilisé pour le choix de l'édition et de la variante
};

#endif