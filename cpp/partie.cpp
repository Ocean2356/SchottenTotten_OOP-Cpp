/***********************************************************************************************************************
Nom : partie.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des fonctions
et méthodes déclarées mais non définies dans partie.h (sauf pour la classe Tuile)
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

Movement UI::getChoixBornesARevendiquer( Frontiere<class Tuile>& f, NumJoueur joueur_num, TableauJouee tab){
    Movement mvt;
    afficherFrontiere(f);

    string choix_revendiquer;
    Pos nb_bornes_a_revendiquer = 0;
    while (choix_revendiquer != "oui" && choix_revendiquer != "non"){
        cout << "souhaitez-vous revendiquer une borne ? (oui/non)\n";
        cin >> choix_revendiquer;
        size_t i = 0;
        for (auto& c: choix_revendiquer)
            choix_revendiquer[i++] = (char) tolower(c);
        if (choix_revendiquer != "oui") continue;

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
            if (f.verifRevendiquable(choix_borne_a_revendiquer - 1, joueur_num, tab)){
                // Si la tuile est revendiquable par le joueur
                bool saisie_incorrecte = false;
                for (auto& b : bornes_a_revendiquer)
                    if (b == choix_borne_a_revendiquer){
                        saisie_incorrecte = true;
                        cout << "Veuillez entrer un numero d'une AUTRE borne\n";
                    }
                if (! saisie_incorrecte){
                    bornes_a_revendiquer.push_back(choix_borne_a_revendiquer);
                    mvt += "Revendiquer:";
                    mvt += (char) (choix_borne_a_revendiquer - 1);
                    nb_bornes_a_revendiquer++;
                    do{
                        saisie_incorrecte = true;
                        cout << "Revendiquer une autre borne ? (entrez 0 si non, un entier strictement positif si oui): ";
                        cin >> recup_choix;
                        if ((unsigned int) (recup_choix[0] - '0') >= 0 && (unsigned int) (recup_choix[0] - '0') <= 9){
                            saisie_incorrecte = false;
                            choix_borne_a_revendiquer = (unsigned int) (recup_choix[0] - '0');
                            continuer = choix_borne_a_revendiquer != 0;
                        }
                    }while (continuer && saisie_incorrecte);
                }
            } else{
                // sinon, on considère que le joueur s'est trompé
                // et qu'il n'essayera pas de revendiquer d'autres bornes pour ce tour
                cout << "La borne " << choix_borne_a_revendiquer << " ne peut pas etre revendiquee";
                continuer = 0;
            }

        }
    }
    mvt += "nb:";
    mvt += (char) nb_bornes_a_revendiquer;
    return mvt;
}


Movement UI::getChoixBornesARevendiquerIa( Frontiere<class Tuile>& f, NumJoueur joueur_num, TableauJouee tab){
    Movement mvt;
    Pos nb_bornes_a_revendiquer = 0;

    for (Pos i = 0; i<f.getNbTuile(); i++)
        if (f.verifRevendiquable((size_t) i, joueur_num, tab)){
            // Si la tuile est revendiquable par le joueur
            mvt += "Revendiquer:";
            mvt += i;
            nb_bornes_a_revendiquer++;
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
void PremiereNormale::initierPiocheNormale(){
    // on alloue une nouvelle carte pour chaque force et couleur (par appel de Pioche::setCarte)
    size_t pos = 0;
    for (auto& c: Couleurs)
        for (auto& f: Forces)
            piocheNormale.setCarteNormale(pos++, c, f);

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

void UI::afficherFrontiere(const Frontiere<class Tuile>& f) const{
    Pos nb_tuile = f.nb_tuile;
    cout
            << "\n----------------------------------------------------------------Affichage de la frontiere----------------------------------------------------------------\n";
    // Affichage des cartes des tuiles du côté du premier joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 0);
    cout << "\n";

    // Affichage de l'état des tuiles (le numéro du joueur qui l'a revendiquée ou le numéro de la tuile si elle n'est pas revendiquée)
    for (size_t i = 0; i < nb_tuile; i++)
//        f.tuiles[i].afficherEtatBorne(i + 1);
        afficherEtatBorne(f.tuiles[i], i+1);
    cout << "\n";

    // Affichage des cartes des tuiles du côté du second joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 1);

    cout
            << "\n---------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

// Méthode permettant d'afficher les cartes d'un côté de la tuile
void UI::afficherCote(const Tuile &t, size_t cote) const{
    size_t iter = 1;
    for (auto c: t.cartes_posees[cote]){
        cout << *c;
        iter++;
    }
    while (iter < 5){
        // il y a au maximum 4 cartes d'un côté d'une tuile. Pour centrer l'affichage, on remplit toujours les positions vides
        cout << "    ";
        iter++;
    }
    cout << "|";
}

// Méthode permettant d'afficher l'état d'une tuile
void UI::afficherEtatBorne(const Tuile &t, size_t num_borne) const{
    cout << "      ";  // utilisé pour centrer l'affichage
    if (t.revendiquee == TuileRevendiquee::revendiquee_joueur1)
        cout << "/R1/ ";
    else if (t.revendiquee == TuileRevendiquee::revendiquee_joueur2)
        cout << "/R2/ ";
    else
        cout << "/B" << num_borne << "/ ";
    cout << "     |";  // utilisé pour centrer l'affichage
}

Pos UI::getChoixCarte(Main& main) {
    string recup_choix;
    cout << "Choix de la carte a jouer\n";
    for (Pos i = 0; i < main.getNbCartes(); i++)
        cout << "Entrez " << i + 1 << " pour jouer la carte " << main.getCarte(i) << "\n";
    Pos choix_carte = -1;
    while (choix_carte < 1 || choix_carte > main.getNbCartes()){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((Pos) (recup_choix[0] - '0') >= 1 && (Pos) (recup_choix[0] - '0') <= main.getNbCartes())
            choix_carte = (Pos) (recup_choix[0] - '0');
    }
    --choix_carte;
    return choix_carte;
}

Pos UI::getChoixCarteIa(Main& main) {
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) main.getNbCartes() - 1};
    auto debug = distrib(random_eng);
    for (Pos i = 0; i < main.getNbCartes(); i++)
        cout << "Carte " << i << " : " << main.getCarte(i) << "\n";
    cout << "Choix carte ia = " << debug << "\n";
    return (char) debug;
}

Pos UI::getChoixBorne(const Frontiere<class Tuile>& f, NumJoueur joueur_num) {
    string recup_choix;
    cout << "Vous pouvez jouer sur les bornes suivantes: \n";
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i+1);
            cout << "Entrez " << i + 1 << " pour jouer sur la borne numero " << i + 1 << "\n";
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    unsigned int choix_borne = 0;
    bool saisie_correcte = false;
    while (!saisie_correcte){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9){
            choix_borne = (unsigned int) (recup_choix[0] - '0');
            for (auto& b : borne_jouable){
                if (b == choix_borne){
                    saisie_correcte = true;
                    break;
                }
            }
        }
        if (!saisie_correcte)
            cout << "Veuillez entrez un numero d'une borne sur laquelle vous pouvez jouer\n";
    }
    return (char) --choix_borne;
}

Pos UI::getChoixBorneIa(const Frontiere<class Tuile>& f, NumJoueur joueur_num) {
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i);
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) borne_jouable.size() - 1};
    auto debug = borne_jouable[distrib(random_eng)];
    cout << "Choix borne ia = " << debug << "\n";
    return (char) debug;
}