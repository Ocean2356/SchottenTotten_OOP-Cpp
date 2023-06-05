/***********************************************************************************************************************
Nom : partie.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des fonctions
et méthodes déclarées mais non définies dans partie.h
***********************************************************************************************************************/


#include "../h/partie.h"

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

// Méthode permettant de vérifier que toutes les cartes d'un côté de la tuile ont la même couleur
bool Tuile::verifMemeCouleur(NumJoueur num_joueur) const{
    Couleur coul = cartes_posees[(int) num_joueur][0]->getCouleur();
    for (size_t i = 1; i < nb_pleine; i++){
        if (cartes_posees[(int) num_joueur][i]->getCouleur() != coul)
            return false;
    }
    return true;
}

// Méthode permettant de vérifier que toutes les cartes d'un côté de la tuile ont la même force
bool Tuile::verifMemeForce(NumJoueur num_joueur) const{
    Force force = cartes_posees[(int) num_joueur][0]->getForce();
    for (size_t i = 1; i < nb_pleine; i++){
        if (cartes_posees[(int) num_joueur][i]->getForce() != force)
            return false;
    }
    return true;
}

// Méthode appelée par verifSuite permettant de trier les cartes en fonction de la force d'un côté de la tuile
void Tuile::ordonnerCartes(NumJoueur num_joueur){
    // on utilise ici un tri à bulle. Ce tri n'est pas le plus optimisé, mais il est simple et largement suffisant
    // en effet, dans ce cas, il y a au maximum 4 cartes d'un côté de la frontière. Le coût du tri est donc faible
    bool modif;  // on arrête le parcours lorsque plus aucune modification n'a été effectuée
    do{
        modif = false;
        for (size_t i = 0; i < nb_pleine - 1; i++){
            if (cartes_posees[(int) num_joueur][i]->getForce() > cartes_posees[(int) num_joueur][i + 1]->getForce()){
                // On échange les deux cartes
                auto tmp = cartes_posees[(int) num_joueur][i];
                cartes_posees[(int) num_joueur][i] = cartes_posees[(int) num_joueur][i + 1];
                cartes_posees[(int) num_joueur][i + 1] = tmp;
                modif = true;
            }
        }
    } while (modif);
}


// Méthode permettant de vérifier que les cartes d'un côté de la tuile forment une suite
bool Tuile::verifSuite(NumJoueur num_joueur){
    ordonnerCartes(num_joueur);  // on tri les cartes de ce côté de la tuile
    for (size_t i = 0; i < nb_pleine - 1; i++)
        if ((unsigned int) cartes_posees[(int) num_joueur][i]->getForce() + 1 !=
            (unsigned int) cartes_posees[(int) num_joueur][i + 1]->getForce())
            return false; // on a trouvé deux forces qui ne se suivaient pas
    return true;
}


// Méthode permettant de sommer la force de toutes les cartes d'un côté de la tuile
unsigned int Tuile::getSomme(NumJoueur num_joueur) const{
    unsigned int somme = 0;
    for (size_t i = 0; i < nb_pleine; i++){
        somme += (unsigned int) cartes_posees[(int) num_joueur][i]->getForce();
    }
    return somme;
}

// Méthode permettant de vérifier qu'un joueur peut revendiquer une tuile
bool Tuile::verifRevendiquable(NumJoueur num_joueur){
    // si la tuile est déjà revendiquée, elle n'est pas revendiquable
    if (estRevendiquee())
        return false;

    // si le côté du joueur qui a demandé à revendiquer la borne n'est pas plein, il ne peut pas revendiquer la borne
    if (!cotePlein(num_joueur))
        return false;
    else{
        // Détermination de la combinaison du joueur qui souhaite revendiquer la tuile
        Combinaison joueur1;
        if (verifMemeCouleur(num_joueur))
            if (verifSuite(num_joueur))
                // meilleure combinaison possible (trois cartes de la même couleur qui se suivent
                joueur1 = Combinaison::suite_couleur;
            else
                // trois cartes de la même couleur, troisième meilleure combinaison
                joueur1 = Combinaison::couleur;
        else if (verifMemeForce(num_joueur))
            // trois cartes de la même force, deuxième meilleure combinaison
            joueur1 = Combinaison::brelan;
        else if (verifSuite(num_joueur))
            // trois cartes qui se suivent, mais pas de la même couleur, quatrième meilleure combinaison
            joueur1 = Combinaison::suite;
        else
            // moins bonne combinaison, la somme
            joueur1 = Combinaison::somme;

        // on récupère le numéro de l'autre joueur (celui qui n'a pas demandé à revendiquer la borne)
        auto autre_joueur = (NumJoueur) (((int) num_joueur + 1) % 2);
        if (cartes_posees[(int) autre_joueur].size() != nb_pleine){
            // TODO, verifier si l'on peut revendiquer une tuile non encore remplie (pour l'instant on considère que non)
            // on utilisera pour cela le tableau des cartes déjà jouées.
            return false;
        } else{
            // Détermination de la combinaison de l'autre joueur
            Combinaison joueur2;
            if (verifMemeCouleur(autre_joueur))
                if (verifSuite(autre_joueur))
                    // meilleure combinaison possible (trois cartes de la même couleur qui se suivent
                    joueur2 = Combinaison::suite_couleur;
                else
                    // trois cartes de la même couleur, troisième meilleure combinaison
                    joueur2 = Combinaison::couleur;
            else if (verifMemeForce(autre_joueur))
                // trois cartes de la même force, deuxième meilleure combinaison
                joueur2 = Combinaison::brelan;
            else if (verifSuite(autre_joueur))
                // trois cartes qui se suivent, mais pas de la même couleur, quatrième meilleure combinaison
                joueur2 = Combinaison::suite;
            else
                // moins bonne combinaison, la somme
                joueur2 = Combinaison::somme;

            if ((int) joueur1 >  (int) joueur2)
                // le joueur qui souhaite revendiquer la tuile a la meilleure combinaison
                return true;
            else if ((int) joueur1 == (int) joueur2){ // les deux joueurs ont la même combinaison
                unsigned int somme1 = getSomme(num_joueur);
                unsigned int somme2 = getSomme(autre_joueur);
                if (somme1 > somme2)
                    // le joueur qui souhaite revendiquer la tuile peut le faire
                    return true;
                else if (somme1 == somme2)  // le joueur qui a rempli la tuile en premier peut la revendiquer
                    return rempli_en_premier == num_joueur;
                else
                    // le joueur qui souhaite revendiquer la tuile ne peut pas le faire
                    return false;
            } else  // l'autre joueur a la meilleure combinaison
                return false;
        }
    }
}

// Méthode permettant de revendiquer une tuile pour un joueur
void Tuile::revendiquer(NumJoueur num_joueur){
    if (!verifRevendiquable(num_joueur))
        throw PartieException("La tuile n'est pas revendiquable par ce joueur");
    if (num_joueur == NumJoueur::joueur1)
        revendiquee = TuileRevendiquee::revendiquee_joueur1;
    else
        revendiquee = TuileRevendiquee::revendiquee_joueur2;
}

// Méthode permettant de calculer le score du joueur qui a perdu la partie
unsigned int Frontiere::calculerScore(NumJoueur joueur_num) const{
    // Pour cela, on compte puis retourne le nombre de bornes revendiquéess par ce joueur.
    unsigned int score = 0;
    for (size_t i = 0; i < nb_tuile; i++){
        if ((int) tuiles[i].getRevendiquee() == (int) joueur_num + 1)
            score++;
    }
    return score;
}


// Méthode permettant de retourner le joueur gagnant ou JoueurGagnant::aucun si aucun joueur n'a gagné à ce stade
JoueurGagnant Frontiere::estFinie() const{
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

// Méthode permettant la saisie par l'utilisateur d'une carte à jouer
Movement Agent::choisirCarteAJouer(const Frontiere& f, NumJoueur joueur_num){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    f.afficherFrontiere();
    string recup_choix;
    if (main.getNbCartes() > 0){  // s'il reste encore des cartes au joueur
        cout << "Choix de la carte a jouer\n";
        for (size_t i = 0; i < main.getNbCartes(); i++)
            cout << "Entrez " << i + 1 << " pour jouer la carte " << main.getCarte(i) << "\n";
        unsigned int choix_carte = -1;
        while (choix_carte < 1 || choix_carte > main.getNbCartes()){
            cout << "Votre Choix : ";
            cin >> recup_choix;
            if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9)
                choix_carte = (unsigned int) (recup_choix[0] - '0');
        }
        --choix_carte;
        mvt += "Carte:";
        mvt += (char) choix_carte;

        cout << "Vous pouvez jouer sur les bornes suivantes: \n";
        vector <int> borne_jouable;
        for (size_t i = 0; i < f.getNbTuile(); i++){
            if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
                borne_jouable.push_back((int) i);
                cout << "Entrez " << i + 1 << " pour jouer sur la borne numero " << i + 1 << "\n";
            }
        }
        if (borne_jouable.empty())
            // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
            return "";
        unsigned int choix_borne = 0;
        bool saisie_incorrecte = true;
        while (saisie_incorrecte){
            cout << "Votre Choix : ";
            cin >> recup_choix;
            if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9){
                choix_borne = (unsigned int) (recup_choix[0] - '0');
                for (auto& b : borne_jouable){
                    if (b == choix_borne)
                        saisie_incorrecte = false;
                }
            }
            if (saisie_incorrecte)
                cout << "Veuillez entrez un numero d'une borne sur laquelle vous pouvez jouer\n";
        }
        mvt += "Borne:";
        mvt += (char) (choix_borne - 1);
    }
    return mvt;
}


Movement Agent::choisirBornesARevendiquer(Frontiere& f, NumJoueur joueur_num){
    Movement mvt;
    f.afficherFrontiere();

    string choix_revendiquer;
    unsigned int nb_bornes_a_revendiquer = 0;
    while (choix_revendiquer != "oui" && choix_revendiquer != "non"){
        cout << "souhaitez-vous revendiquer une borne ? (oui/non)\n";
        cin >> choix_revendiquer;
        size_t i = 0;
        for (auto& c: choix_revendiquer)
            choix_revendiquer[i++] = (char) tolower(c);
        if (choix_revendiquer == "oui"){
            int continuer = 1;
            vector <unsigned int> bornes_a_revendiquer;
            string recup_choix;
            while (continuer != 0){
                unsigned int choix_borne_a_revendiquer = 0;
                while (choix_borne_a_revendiquer < 1 || choix_borne_a_revendiquer > f.getNbTuile()){
                    cout << "Entrez le numero de la borne a revendiquer  (entre 1 et " << f.getNbTuile() << "): ";
                    cin >> recup_choix;
                    if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9)
                        choix_borne_a_revendiquer = (unsigned int) (recup_choix[0] - '0');
                }
                if (f.verifRevendiquable(choix_borne_a_revendiquer - 1, joueur_num)){
                    // Si la tuile est revendiquable par le joueur
                    bool saisie_incorrecte = true;
                    do{
                        bornes_a_revendiquer.push_back(choix_borne_a_revendiquer);
                        mvt += "Revendiquer:";
                        mvt += (char) (choix_borne_a_revendiquer - 1);
                        nb_bornes_a_revendiquer++;
                        cout << "Revendiquer une autre borne ? (entrez 0 si non, un autre entier si oui): ";
                        cin >> recup_choix;
                        if ((int) recup_choix[0] >= 1 && (int) recup_choix[0] <= 9)
                            choix_borne_a_revendiquer = (int) recup_choix[0];
                        else
                            for (auto& b : bornes_a_revendiquer){
                                if (b == choix_borne_a_revendiquer)
                                    saisie_incorrecte = false;
                            }
                        if (saisie_incorrecte)
                            cout << "Veuillez entrez un numéro d'une AUTRE borne à revendiquer\n";
                        continuer = choix_borne_a_revendiquer == 0;
                    }while (continuer && saisie_incorrecte);
                } else{
                    // sinon, on considère que le joueur s'est trompé
                    // et qu'il n'essayera pas de revendiquer d'autres bornes pour ce tour
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
        for (auto& f: Forces){
            piocheNormale.setCarte(pos++, c, f);
        }
}

// Constructeur de la classe PremiereNormale
PremiereNormale::PremiereNormale(){
    initierPiocheNormale();  // initialisation de la pioche (54 cartes normales)
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
        cout << "`\nLa pioche est vide !";

    Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere, (NumJoueur) agentActive);
    // Jouer la carte choisie
    size_t pos_carte = carte_a_jouee.find("Carte:");
    if (pos_carte == std::string::npos){
        // pas de carte à jouer, on force le joueur à revendiquer toutes les bornes possibles
        // pour éviter que la partie ne s'arrête jamais
        for (size_t i=0; i<frontiere.getNbTuile(); i++){
            if (frontiere.verifRevendiquable(i, (NumJoueur) agentActive))
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


    Movement bornes_a_revendiquer = agent.choisirBornesARevendiquer(frontiere, (NumJoueur) agentActive);
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

