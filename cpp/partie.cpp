/***********************************************************************************************************************
Nom : partie.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des fonctions
et méthodes déclarées mais non définies dans partie.h
***********************************************************************************************************************/


#include "../h/partie.h"

std::initializer_list<Couleur> Couleurs = {Couleur::Rouge, Couleur::Marron, Couleur::Jaune, Couleur::Vert,
                                           Couleur::Bleu, Couleur::Violet};  // liste des couleurs
std::initializer_list<Force> Forces = {Force::un, Force::deux, Force::trois, Force::quatre, Force::cinq, Force::six,
                                       Force::sept, Force::huit, Force::neuf};  // liste des forces

// Fonction permettant de retourner une string correspondant à une couleur donnée
string toString(Couleur c) {
    switch (c) {
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
string toString(Force f) {
    switch (f) {
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
    }
}

// Fonction permettant d'afficher une carte sur un flux ostream
std::ostream &operator<<(std::ostream &f, const Carte &c) {
    f << c.getInfo();
    return f;
}


unsigned int Tuile::getSomme(NumJoueur num_joueur) const {
    unsigned int somme = 0;
    for (size_t i = 0; i < nb_pleine; i++) {
        somme += (unsigned int) cartes_posees[(int) num_joueur][i]->getForce();
    }
    return somme;
}

bool Tuile::verifMemeCouleur(NumJoueur num_joueur) const {
    Couleur coul = cartes_posees[(int) num_joueur][0]->getCouleur();
    for (size_t i = 1; i < nb_pleine; i++) {
        if (cartes_posees[(int) num_joueur][i]->getCouleur() != coul)
            return false;
    }
    return true;
}

bool Tuile::verifMemeForce(NumJoueur num_joueur) const {
    Force force = cartes_posees[(int) num_joueur][0]->getForce();
    for (size_t i = 1; i < nb_pleine; i++) {
        if (cartes_posees[(int) num_joueur][i]->getForce() != force)
            return false;
    }
    return true;
}


void Tuile::ordonnerCartes(NumJoueur num_joueur) {
    bool modif;  // on arrête le parcours lorsque plus aucune modification n'a été effectuée
    do {
        modif = false;
        for (size_t i = 0; i < nb_pleine - 1; i++) {
            if (cartes_posees[(int) num_joueur][i]->getForce() > cartes_posees[(int) num_joueur][i + 1]->getForce()) {
                auto tmp = cartes_posees[(int) num_joueur][i];
                cartes_posees[(int) num_joueur][i] = cartes_posees[(int) num_joueur][i + 1];
                cartes_posees[(int) num_joueur][i + 1] = tmp;
                modif = true;
            }
        }
    } while (modif);
}


bool Tuile::verifSuite(NumJoueur num_joueur) {
    cout << "Suite";
    ordonnerCartes(num_joueur);
    cout << "coucou";
    for (size_t i = 0; i < nb_pleine - 1; i++)
        if ((unsigned int) cartes_posees[(int) num_joueur][i]->getForce() + 1 !=
            (unsigned int) cartes_posees[(int) num_joueur][i + 1]->getForce())
            return false; // deux forces qui ne se suivent pas
    cout << "SuiteFin";
    return true;
}

bool Tuile::verifRevendiquable(NumJoueur num_joueur) {
    if (estRevendiquee())
        return false;
    auto autre_joueur = (NumJoueur) (((int) num_joueur + 1) % 2);
    if (!cotePlein(num_joueur))
        return false;
    else {
        // Détermination de la combinaison du joueur qui souhaite revendiquer la tuile
        Combinaison joueur1;
        if (verifMemeForce(num_joueur))
            joueur1 = Combinaison::brelan;
        else if (verifMemeCouleur(num_joueur))
            if (verifSuite(num_joueur))
                joueur1 = Combinaison::suite_couleur;
            else
                joueur1 = Combinaison::couleur;
        else if (verifSuite(num_joueur))
            joueur1 = Combinaison::suite;
        else
            joueur1 = Combinaison::somme;
        if (cartes_posees[(int) autre_joueur].size() != nb_pleine) {
            // TODO, verifier si l'on peut revendiquer une tuile non encore remplie (pour l'instant on considère que non)
            // on utilisera pour cela le tableau des cartes déjà jouées.
            return false;
        } else {
            cout << "OK";
            // Détermination de la combinaison de l'autre joueur
            Combinaison joueur2;
            if (verifMemeForce(autre_joueur))
                joueur2 = Combinaison::brelan;
            else if (verifMemeCouleur(autre_joueur))
                if (verifSuite(autre_joueur))
                    joueur2 = Combinaison::suite_couleur;
                else
                    joueur2 = Combinaison::couleur;
            else if (verifSuite(autre_joueur))
                joueur2 = Combinaison::suite;
            else
                joueur2 = Combinaison::somme;
            cout << (int) joueur1 << (int) joueur2;
            if ((int) joueur1 >
                (int) joueur2)  // le joueur qui souhaite revendiquer la tuile a la meilleure combinaison
                return true;
            else if ((int) joueur1 == (int) joueur2) { // les deux joueurs ont la même combinaison
                unsigned int somme1 = getSomme(num_joueur);
                unsigned int somme2 = getSomme(autre_joueur);
                if (somme1 > somme2)
                    return true;
                else if (somme1 == somme2)  // le joueur qui a rempli la tuile en premier peut la revendiquer
                    return rempli_en_premier == num_joueur;
                else
                    return false;
            } else  // l'autre joueur a la meilleure combinaison
                return false;
        }
    }
}


void Tuile::revendiquer(NumJoueur num_joueur) {
    if (!verifRevendiquable(num_joueur))
        throw PartieException("La tuile n'est pas revendiquable par ce joueur");
    if (num_joueur == NumJoueur::joueur1)
        revendiquee = TuileRevendiquee::revendiquee_joueur1;
    else
        revendiquee = TuileRevendiquee::revendiquee_joueur2;
}

JoueurGagnant Frontiere::estFinie() const {
    unsigned int adjacent = 1;
    unsigned int joueur_preced = 0;
    unsigned int nb_joueur_1 = 0;
    unsigned int nb_joueur_2 = 0;
    for (size_t i = 0; i < nb_tuile; i++) {
        TuileRevendiquee num_joueur = tuiles[i].getRevendiquee();
        if (num_joueur == TuileRevendiquee::revendiquee_joueur1) {
            if (joueur_preced == 1) {
                adjacent++;
                if (adjacent == 3)
                    return JoueurGagnant::joueur1;
            } else
                adjacent = 1;
            nb_joueur_1++;
            if (nb_joueur_1 == 5)
                return JoueurGagnant::joueur1;
            joueur_preced = 1;
        } else if (num_joueur == TuileRevendiquee::revendiquee_joueur2) {
            if (joueur_preced == 2) {
                adjacent++;
                if (adjacent == 3)
                    return JoueurGagnant::joueur2;
            } else
                adjacent = 1;
            nb_joueur_2++;
            if (nb_joueur_2 == 5)
                return JoueurGagnant::joueur2;
            joueur_preced = 2;
        } else {  // Tuile non revendiquee
            joueur_preced = 0;
            adjacent = 1;
        }
    }
    return JoueurGagnant::aucun;
}

void PremiereNormale::jouerTour() {

    Agent &agent = agents[agentActive];
    cout << "\nIl reste " << piocheNormale.getNbCartes() << " cartes dans la pioche.";
    Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere, (NumJoueur) agentActive);

    // Jouer la carte choisie
    size_t pos_carte = carte_a_jouee.find("Carte:");
    auto carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_carte + 6];
    size_t pos_borne = carte_a_jouee.find("Borne:");
    auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_borne + 6];
    Force force;
    Couleur couleur;
    agent.jouerCarte(frontiere, carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, force, couleur);
    tableauJouee[(int) force][(int) couleur] = true;

    Movement bornes_a_revendiquer = agent.choisirBornesARevendiquer(frontiere, (NumJoueur) agentActive);
    // Revendiquer les bornes
    unsigned int pos_nb_revendiquer = bornes_a_revendiquer.find("nb:");
    auto nb_bornes_a_revendiquer = (unsigned int) (unsigned char) bornes_a_revendiquer[pos_nb_revendiquer + 3];
    //cout << nb_bornes_a_revendiquer;
    size_t pos_recherche = 0;
    for (unsigned int i = 0; i < nb_bornes_a_revendiquer; i++) {
        pos_recherche = bornes_a_revendiquer.find("Revendiquer:", pos_recherche);
        auto borne_a_revendiquer = (unsigned int) (unsigned char) bornes_a_revendiquer[pos_recherche + 12];
        cout << borne_a_revendiquer;
        agent.revendiquerBorne(frontiere, borne_a_revendiquer, (NumJoueur) agentActive);
    }

    // Piocher une nouvelle carte
    const CarteNormale &carte = piocheNormale.piocher();
    agent.piocher(carte);
    agentSuivant();
}

void Premiere::initierPiocheNormale() {
    size_t pos = 0;
    for (auto &c: Couleurs)
        for (auto &f: Forces) {
            piocheNormale.setCarte(pos++, c, f);
        }
}

void PremiereNormale::initierMains() {
    for (auto &agent: agents)
        for (int i = 0; i < NMAIN; ++i) {
            const CarteNormale &carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
}

unsigned int Frontiere::calculerScore(NumJoueur joueur_num) const {
    unsigned int score = 0;
    for (size_t i = 0; i < nb_tuile; i++) {
        if ((int) tuiles[i].getRevendiquee() == (int) joueur_num + 1)
            score++;
    }
    return score;
}

Resultat Partie::terminer() {
    Resultat res;
    JoueurGagnant gagnant = frontiere.estFinie();
    if (gagnant == JoueurGagnant::joueur1) {
        res[0] = 5;
        res[1] = frontiere.calculerScore(NumJoueur::joueur2);
    } else if (gagnant == JoueurGagnant::joueur2) {
        res[0] = frontiere.calculerScore(NumJoueur::joueur1);
        res[1] = 5;
    } else {
        throw PartieException("La partie n'est pas encore terminee");
    }
    return res;
}

Movement Agent::choisirCarteAJouer(const Frontiere &f, NumJoueur joueur_num) {
    Movement mvt;
    f.afficherFrontiere();
    if (main.getNbCartes() > 0) {
        cout << "Choix de la carte a jouer\n";
        for (size_t i = 0; i < main.getNbCartes(); i++)
            cout << "Entrez " << i + 1 << " pour jouer la carte " << main.getCarte(i) << "\n";
        unsigned int choix_carte = -1;
        while (choix_carte < 1 || choix_carte > main.getNbCartes()) {
            cout << "Votre Choix : ";
            cin >> choix_carte;
        }
        --choix_carte;
        mvt += "Carte:";
        mvt += (char) choix_carte;

        cout << "Vous pouvez jouer sur les bornes suivantes: \n";
        unsigned int cpt = 0;
        for (size_t i = 0; i < f.getNbTuile(); i++) {
            cpt++;
            if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()) {
                cout << "Entrez " << cpt << " pour jouer sur la borne numero " << i + 1 << "\n";
            }
        }

        unsigned int choix_borne = 0;
        while (choix_borne < 1 || choix_borne > cpt) {
            cout << "Votre Choix : ";
            cin >> choix_borne;
        }
        mvt += "Borne:";
        mvt += (char) (choix_borne - 1);
    }
    return mvt;
}


Movement Agent::choisirBornesARevendiquer(Frontiere &f, NumJoueur joueur_num) {
    Movement mvt;
    f.afficherFrontiere();

    string choix_revendiquer;
    unsigned int nb_bornes_a_revendiquer = 0;
    while (choix_revendiquer != "oui" && choix_revendiquer != "non") {
        cout << "souhaitez-vous revendiquer une borne ? (oui/non)\n";
        cin >> choix_revendiquer;
        size_t i = 0;
        for (auto &c: choix_revendiquer)
            choix_revendiquer[i++] = (char) tolower(c);
        if (choix_revendiquer == "oui") {
            int continuer = 1;
            while (continuer != 0) {
                unsigned int choix_borne_a_revendiquer = 0;
                while (choix_borne_a_revendiquer < 1 || choix_borne_a_revendiquer > f.getNbTuile()) {
                    cout << "Entrez le numero de la borne a revendiquer  (entre 1 et " << f.getNbTuile() << "): ";
                    cin >> choix_borne_a_revendiquer;
                }
                if (f.tuiles[choix_borne_a_revendiquer - 1].verifRevendiquable(joueur_num)) {
                    mvt += "Revendiquer:";
                    mvt += (char) (choix_borne_a_revendiquer - 1);
                    nb_bornes_a_revendiquer++;
                    cout << "Revendiquer une autre borne ? (entrez 0 si non, un autre entier si oui) :";
                    cin >> continuer;
                } else {
                    cout << "La borne " << choix_borne_a_revendiquer << " ne peut pas etre revendiquee";
                    continuer = 0;
                }
            }

        }
    }
    mvt += "nb:";
    mvt += (char) nb_bornes_a_revendiquer;
    return mvt;
}


void Agent::piocher(const Carte &carte) {
    if (main.getTailleMax() == main.getNbCartes())
        throw PartieException("La main est pleine");
    main.piocherCarte(carte);
}


bool Partie::testerFin() {
    return frontiere.estFinie() != JoueurGagnant::aucun;
}


void Premiere::commencer(Ordre ordre) {
    initierAgents(ordre);
    initierMains();
}

// Constructeur de la classe PremiereNormale
PremiereNormale::PremiereNormale() {
    initierPiocheNormale();  // initialisation de la pioche (54 cartes normales)
    // initialisation du tableau de cartes jouées
    for (size_t i = 0; i < NFORCE; i++) {
        for (size_t j = 0; i < NCOULEUR; i++)
            tableauJouee[i][j] = false;
    }
}

void PremiereNormale::initierAgents(Ordre ordre) {
    agents[0] = ordre[0]->getAgent();
    agents[1] = ordre[1]->getAgent();
    agentActive = 0;
}