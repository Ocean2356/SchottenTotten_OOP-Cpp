/***********************************************************************************************************************
Nom : partie.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des fonctions
et méthodes déclarées mais non définies dans partie.h (sauf pour les classe Tuile et UI)
***********************************************************************************************************************/


#include "../h/partie.h"
#include "../h/tactique.h"

std::initializer_list<Couleur> Couleurs ={Couleur::Rouge, Couleur::Marron, Couleur::Jaune, Couleur::Vert,
                                           Couleur::Bleu, Couleur::Violet};  // liste des couleurs
std::initializer_list<Force> Forces ={Force::un, Force::deux, Force::trois, Force::quatre, Force::cinq, Force::six,
                                       Force::sept, Force::huit, Force::neuf};  // liste des forces

// Fonction permettant de retourner une string correspondant à une couleur donnée
string toString(Couleur c){
    switch (c){
        case Couleur::Rouge:
            return "Ro";
        case Couleur::Marron:
            return "Ma";
        case Couleur::Jaune:
            return "Ja";
        case Couleur::Vert:
            return "Ve";
        case Couleur::Bleu:
            return "Bl";
        case Couleur::Violet:
            return "Vi";
        default:
            throw PartieException("Couleur inconnue");
    }
}

// Fonction permettant de retourner une string correspondant à une force donnée
string toString(Force f){
    switch (f){
        case Force::un:
            return "1";
        case Force::deux:
            return "2";
        case Force::trois:
            return "3";
        case Force::quatre:
            return "4";
        case Force::cinq:
            return "5";
        case Force::six:
            return "6";
        case Force::sept:
            return "7";
        case Force::huit:
            return "8";
        case Force::neuf:
            return "9";
        default:
            throw PartieException("Force inconnue");
    }
}

// Fonction permettant d'afficher une carte sur un flux ostream
std::ostream& operator<<(std::ostream& f, const Carte& c){
    f << c.getInfo();
    return f;
}

// Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
void Agent::jouerCarte(Frontiere<class Tuile>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, Force& f,
                Couleur& coul){
    if (frontiere.tuiles[pos_borne].cotePlein(joueur_num))
        throw PartieException("La tuile est deja pleine\n");
    const Carte& c = main.jouerCarte(pos_carte);  // on récupère la carte à jouer
    auto carteNormale = dynamic_cast<const CarteNormale&>(c);
    f = carteNormale.getForce();// f est utilisée pour mettre à jour le tableau des cartes jouées
    coul = carteNormale.getCouleur();// coul est utilisée pour mettre à jour le tableau des cartes jouées
    frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière

}

// Méthode permettant la saisie par l'utilisateur d'une carte à jouer
Movement Agent::choisirCarteAJouer(const  Frontiere<class Tuile>& f, NumJoueur joueur_num){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    ui.afficherFrontiere(f);
    if (main.getNbCartes() > 0){  // s'il reste encore des cartes au joueur
        Pos choix_carte;
        if (this->getIa())
            choix_carte = ui.getChoixCarteIa(main);
        else
            choix_carte = ui.getChoixCarte(main);
        mvt += "Carte:";
        mvt += (char) choix_carte;
        Pos choix_borne;
        if (this->getIa())
            choix_borne = ui.getChoixBorneIa(f, joueur_num);
        else
            choix_borne = ui.getChoixBorne(f, joueur_num);
        if (choix_borne == ERREUR) return "";
        mvt += "Borne:";
        mvt += (char) (choix_borne);
    }
    return mvt;
}


Movement Agent::choisirBornesARevendiquer( Frontiere<class Tuile>& f, NumJoueur joueur_num, TableauJouee tab){
    if (this->getIa())
        return ui.getChoixBornesARevendiquerIa(f, joueur_num, tab);
    else
        return ui.getChoixBornesARevendiquer(f, joueur_num, tab);
}


// Méthode permettant de calculer et de retourner le score des joueurs à la fin d'une partie
Resultat Partie::terminer(){
    Resultat res;
    JoueurGagnant gagnant = frontiere.estFinie();  // on récupère le joueur gagnant
    if (gagnant == JoueurGagnant::joueur1){
        // le joueur 1 gagne et reçoit 5 points, on cacule le score du joueur 2
        res[0] = 5;
        res[1] = frontiere.calculerScore(NumJoueur::joueur2);
    } else if (gagnant == JoueurGagnant::joueur2){
        // le joueur 2 gagne et reçoit 5 points, on cacule le score du joueur 1
        res[0] = frontiere.calculerScore(NumJoueur::joueur1);
        res[1] = 5;
    } else{
        throw PartieException("La partie n'est pas encore terminee");
    }
    return res;
}

// Méthode permettant d'initialiser la pioche normale
void Premiere::initierPiocheNormale(){
    // on alloue une nouvelle carte pour chaque force et couleur (par appel de Pioche::setCarte)
    size_t pos = 0;
    for (auto& c: Couleurs)
        for (auto& f: Forces)
            piocheNormale.setCarteNormale(pos++, c, f);

}

// Constructeur de la classe PremiereNormale
PremiereNormale::PremiereNormale(){
    // initialisation du tableau de cartes jouées
    for (size_t i = 0; i < NFORCE; i++){
        for (size_t j = 0; i < NCOULEUR; i++)
            tableauJouee[i][j] = false;
    }
}

// Méthode permettant de jouer un tour dans son intégralité (choix de la carte à jouer, revendiquer une ou plusieurs bornes, piocher)
void PremiereNormale::jouerTour(){
    Agent& agent = agents[agentActive];  // On récupère l'agent dont c'est le tour
    if (piocheNormale.getNbCartes() > 0)
        cout << "\nIl reste " << piocheNormale.getNbCartes() << " cartes dans la pioche.";
    else
        cout << "\nLa pioche est vide !";
    Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere, (NumJoueur) agentActive);
    // Jouer la carte choisie
    size_t pos_carte = carte_a_jouee.find("Carte:");
    if (pos_carte == std::string::npos){
        // pas de carte à jouer, on force le joueur à revendiquer toutes les bornes possibles
        // pour éviter que la partie ne s'arrête jamais
        for (size_t i=0; i<frontiere.getNbTuile(); i++){
            if (frontiere.verifRevendiquable(i, (NumJoueur) agentActive, tableauJouee))
                agent.revendiquerBorne(frontiere, i, (NumJoueur) agentActive);
        }

    }else{
        auto carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_carte + 6];
        size_t pos_borne = carte_a_jouee.find("Borne:");
        if (pos_borne != std::string::npos){  // si on a trouvé une position de borne
            // on place la carte sur la borne
            auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_borne + 6];
            Force force;
            Couleur couleur;
            agent.jouerCarte(frontiere, carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, force, couleur);
            tableauJouee[(int) force][(int) couleur] = true;
        }
    }


    Movement bornes_a_revendiquer = agent.choisirBornesARevendiquer(frontiere, (NumJoueur) agentActive, tableauJouee);
    // Revendiquer les bornes
    size_t pos_nb_revendiquer = bornes_a_revendiquer.find("nb:");
    if (pos_nb_revendiquer != std::string::npos){  // si on a trouvé le nombre de bornes
        auto nb_bornes_a_revendiquer = (unsigned int) (unsigned char) bornes_a_revendiquer[pos_nb_revendiquer + 3];
        size_t pos_recherche = 0;
        for (unsigned int i = 0; i < nb_bornes_a_revendiquer; i++){
            // on cherche à partir de la dernière position
            pos_recherche = bornes_a_revendiquer.find("Revendiquer:", pos_recherche);
            if (pos_recherche != std::string::npos){  // si on a trouvé la borne
                // on incrémente pos_recherche pour ne pas considérer la borne précédente, puis on revendique la borne
                auto borne_a_revendiquer = (unsigned int) (unsigned char) bornes_a_revendiquer[pos_recherche++ + 12];
                agent.revendiquerBorne(frontiere, borne_a_revendiquer, (NumJoueur) agentActive);
            }
        }
    }

    // Piocher une nouvelle carte
    if (piocheNormale.getNbCartes() > 0){
        const CarteNormale& carte = piocheNormale.piocher();
        agent.piocher(carte);
    }
    agentSuivant();
}

// Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
void PremiereNormale::initierAgents(Ordre ordre){
    agents[0] = ordre[0]->getAgent();
    agents[1] = ordre[1]->getAgent();
    agentActive = 0;
}

// Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
void PremiereNormale::initierMains(){
    for (auto& agent: agents)
        for (int i = 0; i < NMAIN; ++i){
            // On pioche des cartes Clans jusqu'à en avoir NMAIN = 6 en main.
            const CarteNormale& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
}

