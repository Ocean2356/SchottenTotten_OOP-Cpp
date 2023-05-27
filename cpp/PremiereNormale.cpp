#include "partie.h"
#include "algorithm"
#include "cctype"

std::initializer_list<Couleur> Couleurs = { Couleur::Rouge, Couleur::Marron, Couleur::Jaune, Couleur::Vert, Couleur::Bleu, Couleur::Violet };
std::initializer_list<Force> Forces = { Force::un, Force::deux, Force::trois, Force::quatre, Force::cinq, Force::six, Force::sept, Force::huit, Force::neuf };

string toString(Couleur c){
    switch (c) {
        case Couleur::Rouge: return "R";
        case Couleur::Marron: return "M";
        case Couleur::Jaune: return "J";
        case Couleur::Vert: return "Ve";
        case Couleur::Bleu: return "B";
        case Couleur::Violet: return "Vi";
        default: throw PartieException("Couleur inconnue");
    }
}

string toString(Force f){
    switch (f) {
        case Force::un: return "1";
        case Force::deux: return "2";
        case Force::trois: return "3";
        case Force::quatre: return "4";
        case Force::cinq: return "5";
        case Force::six: return "6";
        case Force::sept: return "7";
        case Force::huit: return "8";
        case Force::neuf: return "9";
        default: throw PartieException("Force inconnue");
    }
}

string Carte::getInfo() const{
    return "TODO";
}

string Carte::getDescription() const{
    return "TODO";
}


std::ostream& operator<<(std::ostream& f, const Carte& c){ // REVOIR POUR CARTE
    f<<toString(c.getCouleur())<<"|"<<toString(c.getForce());
    return f;
}

std::ostream& operator<<(std::ostream& f, const CarteNormale& c){ // REVOIR POUR CARTE
    f<<toString(c.getCouleur())<<"|"<<toString(c.getForce());
    return f;
}


PremiereNormale::PremiereNormale()
{
    initierPiocheNormale();
    for (size_t i = 0; i<NFORCE; i++){
        for (size_t j = 0; i< NCOULEUR; i++)
            tableauJouee[i][j] = false;
    }

}

void PremiereNormale::initierAgents(Ordre ordre){
    agents[0] = ordre[0]->getAgent();
    agents[1] = ordre[1]->getAgent();
    agentActive = 0;
}

void PremiereNormale::commencer(Ordre ordre)
{
    initierAgents(ordre);
    initierMains();
}

Movement Agent::jouer(const Frontiere& f, NumJoueur joueur_num){
    Movement mvt;
    f.afficherFrontiere();
    cout << "Choisissez votre action\n";
    if (main.getNbCartes() > 0){
        cout << "Choix de la carte a jouer\n";
        for (size_t i=0; i<main.getNbCartes(); i++)
            cout << "Entrez " << i+1 << " pour jouer la carte " <<main.getCarte(i) <<"\n";
        unsigned int choix_carte = -1;
        while (choix_carte < 1 || choix_carte > main.getNbCartes()){
            cout << "Votre Choix : ";
            cin >> choix_carte;
        }
        mvt.append("Carte:%c; ", choix_carte);

        cout << "Vous pouvez jouer sur les bornes suivantes: \n";
        unsigned int cpt = 0;
        for (size_t i=0; i<f.getNbTuile(); i++){
            cpt ++;
            if (!f.tuiles[i].cote_plein(joueur_num) && !f.tuiles[i].estRevendiquee()) {
                cout << "Entrez " << cpt << " pour jouer sur la borne numero " << i + 1 << "\n";
            }
        }

        unsigned int choix_borne = 0;
        while (choix_borne < 1 || choix_borne > cpt){
            cout << "Votre Choix : ";
            cin >> choix_borne;
        }

        mvt.append("Borne:%c; ", choix_borne);
    }
    string choix_revendiquer="";
    while (choix_revendiquer != "oui" && choix_revendiquer != "non") {
        cout << "souhaitez-vous revendiquer une borne ? (oui/non)\n";
        cin >> choix_revendiquer;
        for (size_t i = 0; i < choix_revendiquer.length(); i++)
            tolower(choix_revendiquer[i]);
        if (choix_revendiquer == "oui") {
            int continuer = 1;
            unsigned int nb_bornes_a_revendiquer = 0;
            while (continuer != 0) {
                unsigned int choix_borne_a_revendiquer = 0;
                while (choix_borne_a_revendiquer < 1 || choix_borne_a_revendiquer > f.getNbTuile()) {
                    cout << "Entrez le numero de la borne a revendiquer  (entre 1 et " << f.getNbTuile() << " : ";
                    cin >> choix_borne_a_revendiquer;
                }
                if (f.tuiles[choix_borne_a_revendiquer].verif_revendiquable(joueur_num)) {
                    mvt.append("Revendiquer:%c; ", choix_borne_a_revendiquer);
                    nb_bornes_a_revendiquer ++;
                    cout << "Revendiquer une autre borne ? (entrez 0 si non, un autre entier si oui) :";
                    cin >> continuer;
                } else{
                    cout << "La borne " << choix_borne_a_revendiquer << " ne peut pas etre revendiquee";
                    continuer = 0;
                }


            }
            mvt.append("nb: %c", nb_bornes_a_revendiquer);
        }
    }
    return mvt;

}

void Agent::piocher(const Carte &carte){
    if (main.getTailleMax() == main.getNbCartes())
        throw PartieException("La main est pleine");
    main.piocherCarte(carte);
    // cout << main.getCarte(main.getNbCartes() - 1);
}

unsigned int Tuile::get_somme(NumJoueur num_joueur) const{
    unsigned int somme = 0;
    for (size_t i=0; i<nb_pleine; i++){
        somme += (unsigned int )cartes_posees[(int) num_joueur][i].getForce();
    }
    return somme;
}

bool Tuile::verif_meme_couleur(NumJoueur num_joueur) const{
    Couleur coul = cartes_posees[(int) num_joueur][0].getCouleur();
    for (size_t i=1; i<nb_pleine; i++){
        if (cartes_posees[(int) num_joueur][i].getCouleur() != coul)
            return false;
    }
    return true;
}

bool Tuile::verif_meme_force(NumJoueur num_joueur) const{
    Force force = cartes_posees[(int) num_joueur][0].getForce();
    for (size_t i=1; i<nb_pleine; i++){
        if (cartes_posees[(int) num_joueur][i].getForce() != force)
            return false;
    }
    return true;
}


bool Tuile::verif_suite(NumJoueur num_joueur) const{
    vector <int> tableau_force;
    for (size_t i=0; i<nb_pleine; i++)
        tableau_force[i] = (int) cartes_posees[(int) num_joueur][0].getForce();
    sort(tableau_force.begin(), tableau_force.end());
    for (size_t i=0; i<nb_pleine-1;i++)
        if (tableau_force[i] + 1 != tableau_force[i+1])
            return false; // deux forces qui ne se suivent pas
    return true;
}

bool Tuile::verif_revendiquable(NumJoueur num_joueur) const{
    if (estRevendiquee())
        return false;
    NumJoueur autre_joueur = (NumJoueur) (((int) num_joueur + 1) % 2);
    if (cartes_posees[(int) num_joueur].size() != nb_pleine)
        return false;
    else{
        // Détermination de la combinaison du joueur qui souhaite revendiquer la tuile
        Combinaison joueur1;
        if (verif_meme_force(num_joueur))
            joueur1 = Combinaison::brelan;
        else if (verif_meme_couleur(num_joueur))
            if (verif_suite(num_joueur))
                joueur1 = Combinaison::suite_couleur;
            else
                joueur1 = Combinaison::couleur;
        else if (verif_suite(num_joueur))
            joueur1 = Combinaison::suite;
        else
            joueur1 = Combinaison::somme;
        if (cartes_posees[(int) autre_joueur].size() != nb_pleine){
            // TODO, verifier si l'on peut revendiquer une tuile non encore remplie (pour l'instant on considère que non)
            // on utilisera pour cela le tableau des cartes déjà jouées.
            return false;
        }
        else{
            // Détermination de la combinaison de l'autre joueur
            Combinaison joueur2;
            if (verif_meme_force(autre_joueur))
                joueur2 = Combinaison::brelan;
            else if (verif_meme_couleur(autre_joueur))
                if (verif_suite(autre_joueur))
                    joueur2 = Combinaison::suite_couleur;
                else
                    joueur2 = Combinaison::couleur;
            else if (verif_suite(autre_joueur))
                joueur2 = Combinaison::suite;
            else
                joueur2 = Combinaison::somme;

            if (int(joueur1) > int (joueur2))  // le joueur qui souhaite revendiquer la tuile a la meilleure combinaison
                return true;
            else if (int(joueur1) == int (joueur2)){ // les deux joueurs ont la même combinaison
                unsigned int somme1 = get_somme(num_joueur);
                unsigned int somme2 = get_somme(autre_joueur);
                if (somme1 > somme2)
                    return true;
                else if (somme1 == somme2)  // le joueur qui a rempli la tuile en premier peut la revendiquer
                    return rempli_en_premier == num_joueur;
                else
                    return false;
            }
            else  // l'autre joueur a la meilleure combinaison
                return false;
        }
    }
}


void Tuile::revendiquer(NumJoueur num_joueur){
    if (!verif_revendiquable(num_joueur))
        throw PartieException("La tuile n'est pas revendiquable par ce joueur");
    if (num_joueur == NumJoueur::joueur1)
        revendiquee = TuileRevendiquee::revendiquee_joueur1;
    else
        revendiquee = TuileRevendiquee::revendiquee_joueur2;
}

bool Tuile::estRevendiquee() const{
    return revendiquee == TuileRevendiquee::revendiquee_joueur1 || revendiquee == TuileRevendiquee::revendiquee_joueur2;
}

JoueurGagnant Frontiere::estFinie() const{
    unsigned int adjacent = 1;
    unsigned int joueur_preced = 0;
    unsigned int nb_joueur_1 = 0;
    unsigned int nb_joueur_2 = 0;
    for (size_t i=0; i<nb_tuile; i++){
        TuileRevendiquee num_joueur = tuiles[i].getRevendiquee();
        if (num_joueur == TuileRevendiquee::revendiquee_joueur1){
            if (joueur_preced == 1){
                adjacent ++;
                if (adjacent == 3)
                    return JoueurGagnant::joueur1;
            }
            else
                adjacent = 1;
            nb_joueur_1 ++;
            if (nb_joueur_1 == 5)
                return JoueurGagnant::joueur1;
            joueur_preced = 1;
        }
        else if (num_joueur == TuileRevendiquee::revendiquee_joueur2){
            if (joueur_preced == 2){
                adjacent ++;
                if (adjacent == 3)
                    return JoueurGagnant::joueur2;
            }
            else
                adjacent = 1;
            nb_joueur_2 ++;
            if (nb_joueur_2 == 5)
                return JoueurGagnant::joueur2;
            joueur_preced = 2;
        }
        else{  // Tuile non revendiquee
            joueur_preced = 0;
            adjacent = 1;
        }
    }
    return JoueurGagnant::aucun;
}

void PremiereNormale::jouerTour()
{
    Agent& agent = agents[agentActive];
    Movement movement = agent.jouer(frontiere, (NumJoueur) agentActive);

    // Jouer la carte choisie
    size_t pos_carte = movement.find("Carte:");
    unsigned int carte_a_jouer = (unsigned int) movement[pos_carte+6] - 1;
    size_t pos_borne = movement.find("Borne:");
    unsigned int borne_sur_laquelle_jouer = (unsigned int) movement[pos_carte+6] - 1;
    Force force;
    Couleur couleur;
    agent.jouerCarte(frontiere, carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, force, couleur);
    tableauJouee[(int) force][(int) couleur] = true;
    // Revendiquer les bornes
    size_t pos_nb_revendiquer = movement.find("nb:");
    unsigned int nb_bornes_a_revendiquer = (unsigned int) movement[pos_nb_revendiquer];
    size_t pos_recherche = 0;
    for (unsigned int i=0; i<nb_bornes_a_revendiquer; i++){
        pos_recherche = movement.find("Revendiquer:", pos_recherche);
        unsigned int borne_a_revendiquer = (unsigned int) movement[pos_recherche+13] - 1;
        agent.revendiquerBorne(frontiere, borne_a_revendiquer, (NumJoueur) agentActive);
    }
    // Piocher une nouvelle carte
    CarteNormale carte;
    if(piocheNormale.piocher(carte))
        agent.piocher(carte);
    else
        throw PartieException("la pioche est vide, on ne peut pas piocher");
    agentSuivant();
}

void PremiereNormale::initierPiocheNormale() {
    for (int i = 0; i < NCOULEUR; ++i)
        for (int j = 0; j < NFORCE; ++j){
            piocheNormale.setCarte(i * NFORCE + j, (Couleur) i, (Force) (j+FORCEMIN));
            //cout << piocheNormale[i * NFORCE + j] << " position =" << i * NFORCE + j << "\n";
        }
    /*
    for (int i = 0; i < NCOULEUR; ++i)
        for (int j = 0; j < NFORCE; ++j){
            cout << piocheNormale[i * NFORCE + j] << "\n";
        }
    */
}

void PremiereNormale::initierMains()
{
    for (auto &agent : agents)
        for (int i = 0; i < NMAIN; ++i){
            CarteNormale carte;
            if(piocheNormale.piocher(carte))
                agent.piocher(carte);

            else
                throw PartieException("la pioche est vide, on ne peut pas piocher");
        }
}

void PremiereNormale::agentSuivant()
{
    agentActive++;
    agentActive %= 2;
}


unsigned int Frontiere::calculerScore(NumJoueur joueur_num) const{
    unsigned int score = 0;
    for (size_t i=0; i<nb_tuile; i++){
        if ((int) tuiles[i].getRevendiquee() == (int) joueur_num + 1)
            score ++;
    }
    return score;
}

Resultat Partie::terminer(){
    Resultat res;
    JoueurGagnant gagnant = frontiere.estFinie();
    if (gagnant == JoueurGagnant::joueur1){
        res[0] = 5;
        res[1] = frontiere.calculerScore(NumJoueur::joueur2);
    }
    else if(gagnant == JoueurGagnant::joueur2){
        res[0] = frontiere.calculerScore(NumJoueur::joueur1);
        res[1] = 5;
    }
    else{
        throw PartieException("La partie n'est pas encore terminee");
    }
    return res;
}

bool Partie::testerFin(){
    return frontiere.estFinie() != JoueurGagnant::aucun;
}