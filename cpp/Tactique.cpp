
#include "../h/Tactique.h"
#include <iostream>
#include <string>


std::initializer_list<Troupe> Troupes ={Troupe::Joker, Troupe::Espion, Troupe::Porte_Bouclier};  // liste des cartes troupes
std::initializer_list<Combat> Combats ={Combat::Colin_Maillard, Combat::Combat_de_Boue };  // liste des cartes combats
std::initializer_list<Ruse> Ruses ={Ruse::Chasseur_de_tete, Ruse::Stratege, Ruse::Banshee, Ruse::Traitre };  // liste des cartes combats

Defausse Defausse::instance = Defausse();


// Fonction permettant de retourner une string correspondant à un type de carte tactique troupe donné
string toString(Troupe t) {
    switch (t) {
        case Troupe::Joker:
            return "jok";
        case Troupe::Espion:
            return "esp";
        case Troupe::Porte_Bouclier:
            return "p_b";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}


string toString(Combat c) {
    switch (c) {
        case Combat::Colin_Maillard:
            return "c_m";
        case Combat::Combat_de_Boue:
            return "c_b";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}

string toString(Ruse r) {
    switch (r) {
        case Ruse::Chasseur_de_tete:
            return "cdt";
        case Ruse::Stratege:
            return "str";
        case Ruse::Banshee:
            return "ban";
        case Ruse::Traitre:
            return "tra";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}

Couleur TuileTactique::demandeCouleur() const{
    cout << "Choix de la couleur de la carte\n";
    for (auto couleur : Couleurs) {
        cout << "Entrez " << (int) couleur + 1 << " pour choisir la couleur " << toString(couleur) << "\n";
    }
    int choix_couleur = -1;
    while (choix_couleur < 0 || choix_couleur > 7) {
        cout << "Veuillez choisir une couleur\n";
        cin >> choix_couleur;
    }
    return (Couleur) (choix_couleur - 1);
}

Force TuileTactique::demandeForce(unsigned int contrainte) const{
    cout << "Choix de la force de la carte\n";
    int choix_force = -1;
    while (choix_force < 1 || choix_force > contrainte) {
        cout << "Veuillez choisir une force entre 1 et " << contrainte << "\n";
        cin >> choix_force;
    }
    return static_cast<Force>(choix_force);
}

void PremiereTactique::jouerTour(){
    AgentTactique& agent = agents[agentActive];  // On récupère l'agent dont c'est le tour
    int choix_pioche = 0;
    bool pioche_normale_non_vide = !piocheNormale.estVide();
    bool pioche_tactique_non_vide = !piocheTactique.estVide();
    if(pioche_normale_non_vide)
        cout << "Il reste " << piocheNormale.getNbCartes() << " cartes dans la pioche normale.\n";
    else
        cout << "La pioche Normale est vide !\n";
    if(pioche_tactique_non_vide)
        cout << "Il reste " << piocheTactique.getNbCartes() << " cartes dans la pioche tactique.\n";
    else
        cout << "La pioche Tactique est vide !\n";

    Movement carte_a_jouer = agent.choisirCarteAJouer(frontiere_tactique, (NumJoueur) agentActive, agents[(agentActive + 1)%2].getNbCartesTactiquesJouees());
    size_t pos_carte = carte_a_jouer.find("Carte:");
    if (pos_carte == std::string::npos){
        // pas de carte à jouer, on force le joueur à revendiquer toutes les bornes possibles
        // pour éviter que la partie ne s'arrête jamais
        for (size_t i=0; i<frontiere_tactique.getNbTuile(); i++){
            if (frontiere_tactique.verifRevendiquable(i, (NumJoueur) agentActive))
                agent.revendiquerBorne(frontiere_tactique, i, (NumJoueur) agentActive);
        }
    }else{
        size_t pos_carte_tactique = carte_a_jouer.find("Tactique:");
        if (pos_carte_tactique == std::string::npos){  // on joue une carte normale
            auto pos_carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_carte + 6];
            size_t pos_borne = carte_a_jouer.find("Borne:");
            if (pos_borne != std::string::npos){  // si on a trouvé une position de borne
                // on place la carte sur la borne
                auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_borne + 6];
                string nom_carte;
                agent.jouerCarte(frontiere_tactique, pos_carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, nom_carte);
            }
        }
        else{  // on joue une carte tactique
            auto pos_carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_carte + 6];
            string nom;
            nom.push_back(carte_a_jouer[pos_carte_tactique + 9]);
            nom.push_back(carte_a_jouer[pos_carte_tactique + 10]);
            nom.push_back(carte_a_jouer[pos_carte_tactique + 11]);
            if (nom == toString(Ruse::Chasseur_de_tete)){
                //agent.mettre_ChasseurdeTete();
                cout << "Desole, l'implementation de cette carte ne fonctionne pas\n";
                agent.getMainModifiable().jouerCarte(pos_carte_a_jouer);  // on retire la carte de la main
                Defausse::getInstance().ajouterCarte(&agent.getMain().getCarte(pos_carte_a_jouer));
            }

            else if (nom == toString(Ruse::Banshee) || nom == toString(Ruse::Stratege) || nom == toString(Ruse::Traitre)){
                agent.actionStrategeBansheeTraitre(frontiere_tactique, nom, (NumJoueur) agentActive);
                agent.getMainModifiable().jouerCarte(pos_carte_a_jouer);  // on retire la carte de la main
                Defausse::getInstance().ajouterCarte(&agent.getMain().getCarte(pos_carte_a_jouer));
            }
            else{  // on devrait pouvoir jouer la carte sur une borne ou au centre
                size_t pos_borne = carte_a_jouer.find("Borne:");
                if (pos_borne != std::string::npos){  // si on a trouvé une position de borne
                    // on place la carte sur la borne
                    auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_borne + 6];
                    agent.jouerCarte(frontiere_tactique, pos_carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, nom);
                }
            }
        }
    }

    Movement bornes_a_revendiquer = agent.choisirBornesARevendiquer(frontiere_tactique, (NumJoueur) agentActive);
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
                agent.revendiquerBorne(frontiere_tactique, borne_a_revendiquer, (NumJoueur) agentActive);
            }
        }
    }

    unsigned int nb_choix = 0;
    if (agent.getIa()){
        if (pioche_normale_non_vide || pioche_tactique_non_vide)
            choix_pioche = 1;
        else
            return;
    }
    else {
        if (pioche_normale_non_vide) //il a au moins une carte normale
            cout << "Entrez " << ++nb_choix << " pour piocher une carte normale\n";

        if (pioche_tactique_non_vide) //il a au moins une carte normale
            cout << "Entrez " << ++nb_choix << " pour piocher une carte tactique\n";
        if (nb_choix == 0) {
            cout << "Les deux pioches sont vides, pas de pioche !\n";
            return;
        } else {
            cout << "\nDans quel pioche voulez-vous piochez ? \n";
            bool saisie_ok = false;
            string saisie;
            while (!saisie_ok) {
                getline(cin, saisie);
                if ((unsigned int) saisie[0] - '0' >= 1 && (unsigned int) saisie[0] - '0' <= nb_choix) {
                    saisie_ok = true;
                    choix_pioche = (int) (unsigned char) saisie[0] - '0';
                }
            }
        }
    }

    if (choix_pioche == 1 && pioche_normale_non_vide) {
        // Piocher une nouvelle carte
        const Carte &carte = piocheNormale.piocher();
        agent.piocher(carte);
    } else {
        const Carte &carte = piocheTactique.piocher();
        agent.piocher(carte);
    }
    agentSuivant();
}


//Méthode permettant d'initialiser la pioche normale
void PremiereTactique::initierPiocheTactique(){
    // on alloue une nouvelle carte pour chaque force et couleur (par appel de Pioche::setCarte)
    size_t pos = 0;
    piocheTactique.setCarteTactique(pos++, toString(Troupe::Joker));
    for(auto& t: Troupes)
        piocheTactique.setCarteTactique(pos++, toString(t));
    for(auto& c: Combats)
        piocheTactique.setCarteTactique(pos++, toString(c));
    for(auto& r: Ruses)
        piocheTactique.setCarteTactique(pos++, toString(r));
}

// Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
void PremiereTactique::initierAgents(Ordre ordre){
    agents[0] = ordre[0]->getAgentTactique();
    agents[1] = ordre[1]->getAgentTactique();
    agentActive = 0;
}

// Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
void PremiereTactique::initierMains(){
    for (auto& agent: agents)
        for (int i = 0; i < NMAIN; i++){
            // On pioche des cartes Clans jusqu'à en avoir NMAIN = 7 en main.
            const Carte& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
}

// Constructeur de la classe PremiereTactique
PremiereTactique::PremiereTactique(){
    initierPiocheTactique();  // initialisation de la pioche (10 cartes tactiques)
}


// Méthode permettant la saisie par l'utilisateur d'une carte à jouer
Movement AgentTactique::choisirCarteAJouer(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num, unsigned int nb_cartes_tactiques_jouees_autre_joueur){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    ui_tactique.afficherFrontiere_tactique(f);
    vector <int> pos_cartes_normales;
    vector <int> pos_cartes_tactiques;

    //Comptage du nombre de cartes tactiques du joueur
    for(Pos i= 0 ; i < getMain().getNbCartes(); i++){
        if(getMain().getCarte(i).estTactique()){
            if (getMain().getCarte(i).getInfo() != toString(Troupe::Joker) || nb_jokers_joues == 0)
                pos_cartes_tactiques.push_back((int)i);
        }
        else
            pos_cartes_normales.push_back((int)i);
    }

    unsigned int nb_choix = 1;
    bool a_carte_normale_en_main = !pos_cartes_normales.empty();
    bool a_carte_tactique_en_main = !pos_cartes_tactiques.empty();
    unsigned int choix_carte;
    if (this->getIa()){
        if (a_carte_normale_en_main)
            choix_carte = 1;
        else if (a_carte_tactique_en_main)
            choix_carte = 2;
        else
            return mvt;
    }
    else{
        if(a_carte_normale_en_main) //il a au moins une carte normale
            cout << "Entrez " << nb_choix++ << " pour jouer une carte normale\n";

        if(a_carte_tactique_en_main && nb_cartes_tactiques_jouees <= nb_cartes_tactiques_jouees_autre_joueur)  //il a au moins une carte tactique
            cout << "Entrez " << nb_choix++ << " pour jouer une carte tactique\n";

        if (!a_carte_normale_en_main)
            cout << "Entrez " << nb_choix << " pour ne rien jouer ce tour-ci";
        bool test_saisie = false;
        string choix;
        while (!test_saisie){
            cout << "Votre choix : ";
            cin >> choix;
            if ((unsigned int) (choix[0] - '0') >= 1 && (unsigned int) (choix[0] - '0') <= nb_choix) {
                test_saisie = true;
                choix_carte = (unsigned int) (choix[0] - '0');
            }
        }
    }
    Pos carte_a_jouer = -1;
    if (choix_carte == 1 && a_carte_normale_en_main){
        if (this->getIa())
            carte_a_jouer = ui_tactique.getChoixCarteIa(*this, pos_cartes_normales);
        else
            carte_a_jouer = ui_tactique.getChoixCarte(*this, pos_cartes_normales);
    }
    else if ((choix_carte == 1 && !a_carte_normale_en_main && a_carte_tactique_en_main) || (choix_carte == 2 && a_carte_tactique_en_main)){
        nb_cartes_tactiques_jouees ++;
        if (this->getIa())
            carte_a_jouer = ui_tactique.getChoixCarteIa(*this, pos_cartes_tactiques);
        else
            carte_a_jouer = ui_tactique.getChoixCarte(*this, pos_cartes_tactiques);
    }
    else{
        return mvt;
    }

    mvt += "Carte:";
    mvt += (char) carte_a_jouer;
    Pos choix_borne;
    if ((choix_carte == 1 && !a_carte_normale_en_main && a_carte_tactique_en_main) || (choix_carte == 2 && a_carte_tactique_en_main)){
        string nom = this->getMain().getCarte(carte_a_jouer).getInfo();
        bool carte_troupe = false;
        bool carte_combat = false;
        bool carte_ruse = false;
        for (auto t : Troupes){
            if (nom == toString(t)){
                carte_troupe = true;
                break;
            }
        }
        if (carte_troupe){  // on joue les cartes troupes normalement sur les bornes
            if (this->getIa())
                choix_borne = ui_tactique.getChoixBorneIa(f, joueur_num);
            else{
                choix_borne = ui_tactique.getChoixBorne(f, joueur_num);
            }

        }
        else{
            for (auto c : Combats){
                if (nom == toString(c)){
                    carte_combat = true;
                    break;
                }
            }
            if (carte_combat){
                if (this->getIa())
                    choix_borne = ui_tactique.getChoixBorneCarteCentreIa(f);
                else
                    choix_borne = ui_tactique.getChoixBorneCarteCentre(f);
            }
            else{
                for (auto r : Ruses){
                    if (nom == toString(r)){
                        carte_ruse = true;
                        break;
                    }
                }
                if (!carte_ruse)  // pas de choix de borne pour la carte ruse
                    throw PartieException("Carte tactique inconnue");
                else{
                    mvt += "Tactique:" + nom; // pas de positionnement sur une borne pour les cartes ruses
                    return mvt;
                }

            }
        }
        mvt += "Tactique:" + nom;
    }
    else{
        if (this->getIa())
            choix_borne = ui_tactique.getChoixBorneIa(f, joueur_num);
        else
            choix_borne = ui_tactique.getChoixBorne(f, joueur_num);
    }
    if (choix_borne == ERREUR){
        cout << "Impossible de jouer cette carte, toutes les bornes sont occupees\n";
        return "";
    }
    mvt += "Borne:";
    mvt += (char) (choix_borne);
    return mvt;
}

// Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
void AgentTactique::jouerCarte(Frontiere<class TuileTactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, const string& nom_carte){
    if (frontiere.tuiles[pos_borne].cotePlein(joueur_num))
        throw PartieException("La tuile est deja pleine\n");
    const Carte& c = this->getMainModifiable().jouerCarte(pos_carte);
    auto carte_tactique = dynamic_cast<const CarteTactique*> (&c);
    if (carte_tactique == nullptr){
        frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière
    }
    else{
        nb_cartes_tactiques_jouees ++;
        if (nom_carte == toString(Combat::Colin_Maillard)){
            mettre_ColinMaillard(frontiere.tuiles[pos_borne], carte_tactique);
            return;
        }
        else if (nom_carte == toString(Combat::Combat_de_Boue)){
            mettre_CombatdeBoue(frontiere.tuiles[pos_borne], carte_tactique);
            return;
        }
        if (nom_carte == toString(Troupe::Joker))
            nb_jokers_joues ++;
        frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière
    }

}


bool TuileTactique::verifRevendiquable(NumJoueur num_joueur) {
    // si la tuile est déjà revendiquée, elle n'est pas revendiquable
    if (estRevendiquee())
        return false;
    auto autre_joueur = (NumJoueur) (((int) num_joueur + 1) % 2);
    // si le côté du joueur qui a demandé à revendiquer la borne n'est pas plein, il ne peut pas revendiquer la borne
    if (!cotePlein(num_joueur) || !cotePlein(autre_joueur))
        return false;

    // test si carte combat, choix des caractéristiques
    array<vector <Couleur>, 2> couleurs_cartes;
    array<vector <Force>, 2> forces_cartes;
    for (int i=0; i<2; i++){
        for (auto c: cartes_posees[i]){
            auto carte_tactique = dynamic_cast<const CarteTactique*> (c);
            if (carte_tactique != nullptr){
                if (carte_tactique->getInfo() == toString(Troupe::Joker)){
                    cout << "Choix des caracteristiques du Joker\n";
                    couleurs_cartes[i].push_back(demandeCouleur());
                    forces_cartes[i].push_back(demandeForce(9));
                }
                else if (carte_tactique->getInfo() == toString(Troupe::Espion)){
                    cout << "Choix des caracteristiques de l'Espion\n";
                    couleurs_cartes[i].push_back(demandeCouleur());
                    forces_cartes[i].push_back(Force::sept);
                }
                else if(carte_tactique->getInfo() == toString(Troupe::Porte_Bouclier)){
                    cout << "Choix des caracteristiques du Porte-Bouclier\n";
                    couleurs_cartes[i].push_back(demandeCouleur());
                    forces_cartes[i].push_back(demandeForce(3));
                }
            }
        }
    }

    // Détermination des combinaisons des joueurs
    Combinaison joueur1;
    Combinaison joueur2;

    if (carte_posee_centre != nullptr && carte_posee_centre->getInfo() == toString(Combat::Colin_Maillard)){
        joueur1 = Combinaison::somme;
        joueur2 = Combinaison::somme;
    }
    else{
        joueur1 = determinerCombinaison(num_joueur, couleurs_cartes[0], forces_cartes[0]);
        joueur2 = determinerCombinaison(autre_joueur,  couleurs_cartes[1], forces_cartes[1]);
    }


    if ((int) joueur1 > (int) joueur2)
        // le joueur qui souhaite revendiquer la tuile a la meilleure combinaison
        return true;
    else if ((int) joueur1 == (int) joueur2) { // les deux joueurs ont la même combinaison
        unsigned int somme1 = getSomme(num_joueur, forces_cartes[0]);
        unsigned int somme2 = getSomme(autre_joueur, forces_cartes[1]);
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

Movement AgentTactique::choisirBornesARevendiquer(Frontiere<class TuileTactique> &f, NumJoueur joueur_num) {
    if (this->getIa())
        return ui_tactique.getChoixBornesARevendiquerIa(f, joueur_num);
    else
        return ui_tactique.getChoixBornesARevendiquer(f, joueur_num);
}

void AgentTactique::mettre_ChasseurdeTete(){
    unsigned int nbCartesClanAPiocher = 3;
    unsigned int nbCartesTactiquesAPiocher = 3;
    Pioche<CarteNormale,54> pioche;
    Pioche<CarteTactique,11> piocheTactique;
    unsigned int taillePiocheClan = pioche.getNbCartes();
    unsigned int taillePiocheTactique = piocheTactique.getNbCartes();
    vector<const Carte*> cartesPiochees;

    // Cas où les pioches sont vides
    if(taillePiocheClan==0 && taillePiocheTactique==0) {
        cout << "Vous ne pouvez piocher aucune carte... (pioches vides)\n";
        return;
    }

    // Cas où les pioches ne sont pas vides
    else if(taillePiocheClan>0 && taillePiocheTactique>0 ) {
    // Cas où la taille de la pioche clan est inférieure à trois
        do {
            if(taillePiocheClan<3){
                // On règle la valeur de cartes clan piochables
                nbCartesClanAPiocher = taillePiocheClan;
            }
            cout << "Combien de cartes de la pioche Clan souhaitez-vous piocher ? (max : "<< nbCartesClanAPiocher <<")\n"
                                                                                                                    "Votre choix :" << endl;
            cin >> nbCartesClanAPiocher;

        } while (nbCartesClanAPiocher > min(taillePiocheClan, (unsigned int)3) or nbCartesClanAPiocher < 0);

        unsigned int a = 3 - nbCartesClanAPiocher;
        unsigned int b = piocheTactique.getNbCartes();
        nbCartesTactiquesAPiocher = min(a,b);
    }
    // Cas où seule la pioche clan est vide
    else if(taillePiocheClan==0){
    // Cas où la taille de la pioche tactique est inférieure à trois
        if(taillePiocheTactique<3){
            nbCartesTactiquesAPiocher = taillePiocheTactique;
        }
        cout <<"La pioche Clan est vide. Vous allez piocher "<<nbCartesTactiquesAPiocher<<" cartes tactiques. "<< endl;
        nbCartesClanAPiocher = 0;
    }

    // Cas où seule la pioche tactique est vide
    else{
    // Cas où la taille de la pioche tactique est inférieure à trois
        if(taillePiocheClan<3){
            nbCartesClanAPiocher = taillePiocheClan;
        }
        cout <<"La pioche Tactique est vide. Vous allez piocher "<<nbCartesClanAPiocher<<" cartes clans. "<< endl;
        nbCartesTactiquesAPiocher = 0;
    }
    // On pioche les cartes
    unsigned int nbCartesTotal = nbCartesClanAPiocher+nbCartesTactiquesAPiocher;
    cout<<"Cartes piochees : \n";

    for (unsigned int j = 0; j < nbCartesClanAPiocher; j++) {
        const CarteNormale* c = &pioche.piocher();
        std::cout<<j+1<<". "<<c;
        cartesPiochees.push_back(c);
    }

    for (unsigned int j = 0; j < (3 - nbCartesClanAPiocher); j++) {
        const CarteTactique* c = &piocheTactique.piocher();
        std::cout<<nbCartesClanAPiocher+j+1<<". "<<c;
        cartesPiochees.push_back(c);
    }


    // On demande au joueur quelles cartes il veut remplacer
    vector<const Carte*> to_replace;
    int rep_carte = -1;
    for(unsigned int i =0; i<nbCartesTotal-1;i++){
        rep_carte = -1;
        while (!(1 <= rep_carte && rep_carte <= this->getMain().getNbCartes())) {
            cout <<"Quelle carte souhaitez-vous remplacer ? ("<<i+1<<"/"<<nbCartesTotal-1<<")"<<endl;
            cout <<"Votre choix :" << endl;
            cin >> rep_carte;
        }
        rep_carte--;

        // sauvegarde de l'adresse des cartes à remplacer
        to_replace.push_back(&this->getMain()[rep_carte]);
    }

    // Suppression des cartes de la main du joueur
    unsigned int cpt = 0;
    for(auto carte : to_replace) {
        for (auto carte_main : this->getMain().getCartes()){
            cpt ++;
            if (carte_main == carte){
                // On enlève la carte de la main du joueur
                this->getMain().getCartes().erase(this->getMain().getCartes().begin() + cpt);

                // On l'ajoute à la pioche
                auto carte_normale = dynamic_cast<const CarteNormale *>(carte);
                if (carte_normale) {
                    // ajout à la pioche clan
                    pioche.placerDessous(carte_normale);
                } else {
                    auto carte_tactique = dynamic_cast<const CarteTactique *>(carte);
                    if (carte_tactique != nullptr) {
                        // ajout à la pioche tactique
                        piocheTactique.placerDessous(carte_tactique);
                    }
                }

            }
        }
    }


    // Ajout des cartes dans la main du joueur
    for(auto carte : cartesPiochees){
        this->getMain().getCartes().push_back(carte);
    }
}


void AgentTactique::actionStrategeBansheeTraitre(Frontiere<class TuileTactique>& frontiere, string& nomTactique, NumJoueur num_joueur) {
    vector<int> tuiles_jouables;
    vector<int> indice_carte;
    NumJoueur cote = num_joueur;

    // on ne peut pas récupérer de cartes tactiques pour le traître
    bool tactique_possible = nomTactique != toString(Ruse::Traitre);

    unsigned int numTuile = 0;
    if (nomTactique == toString(Ruse::Banshee) || nomTactique == toString(Ruse::Traitre))
        cote = (NumJoueur) (((int) num_joueur + 1) % 2);

    for (int i = 0; i < frontiere.getNbTuile(); i++)
        if (!frontiere.tuiles[i].estRevendiquee() &&
            !frontiere.tuiles[i].getCartesPosees(cote).empty()) {
            // la tuile n'est pas revendiquée et a au moins une carte
            if (tactique_possible) {  // si on peut récupérer une carte tactique, on peut agir sur cette tuile
                tuiles_jouables.push_back(i);
                cout << "Entrez : " << ++numTuile << " pour agir sur la tuile" << i + 1 << "\n";
            } else { // sinon (cas du traitre), on vérifie qu'il y a au moins une carte clan de posée de ce côté de la tuile
                for (auto carte: frontiere.tuiles[i].getCartesPosees(cote)) {
                    auto carteNormale = dynamic_cast<const CarteNormale *>(carte);
                    if (carteNormale != nullptr) {
                        tuiles_jouables.push_back(i);
                        cout << "Entrez : " << ++numTuile << " pour agir sur la tuile" << i + 1 << "\n";
                        break;
                    }
                }
            }
        }

    if (tuiles_jouables.empty()){
        // on ne peut agir sur aucune borne
        cout << "Vous ne pouvez jouer la carte sur aucune tuile, vous la perdez\n";
        return;
    }
    unsigned int choix_borne = 0;
    bool saisie_correcte = false;
    string recup_choix;
    while (!saisie_correcte){
        cout << "Votre Choix : ";
        getline(cin, recup_choix);
        if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= numTuile){
            choix_borne = tuiles_jouables[(unsigned int) (recup_choix[0] - '0') - 1];
            saisie_correcte = true;
        }
        if (!saisie_correcte)
            cout << "Veuillez entrez un numero d'une borne sur laquelle vous pouvez jouer\n";
    }
    int cpt = 0;
    for (auto carte: frontiere.tuiles[choix_borne].getCartesPosees(cote)) {
        if (nomTactique == toString(Ruse::Traitre)) {
            auto carteTactique = dynamic_cast<const CarteTactique *>(carte);
            // on récupère uniquement les cartes clans si la carte est un Traitre
            if (carteTactique == nullptr) {
                indice_carte.push_back(cpt);
                cout << "Entrez " << cpt + 1 << " pour agir sur la carte " << *carte << "\n";
            }
        } else{
            indice_carte.push_back(cpt);
            cout << "Entrez " << cpt + 1 << " pour agir sur la carte " << *carte << "\n";
        }
        cpt++;
    }

    cout << "Choix de la carte sur laquelle agir\n";
    unsigned int choix_carte = 0;
    while (choix_carte < 1 || choix_carte > cpt){
        cout << "Votre Choix : ";
        getline(cin, recup_choix);
        if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= cpt)
            choix_carte = (unsigned int) (recup_choix[0] - '0');
    }
    --choix_carte;
    const Carte* c = frontiere.tuiles[choix_borne].getCartesPosees(cote)[indice_carte[choix_carte]];
    frontiere.tuiles[choix_borne].removeCarte(c, (int) cote);

    // il reste à déplacer ou défausser la carte
    unsigned int choix_action;
    if (nomTactique == toString(Ruse::Traitre)) {
        choix_action = 1;
        cote = (NumJoueur) (((int)cote + 1) %2);  // le traitre joue de son côté de la borne
    }
    else if (nomTactique == toString(Ruse::Banshee)) {
        choix_action = 0;
    }
    else{  // stratège
        cout << "Quelle action souhaitez-vous faire ? (0:defausser, 1:placer sur tuile)\n";
        saisie_correcte = false;
        while (!saisie_correcte){
            cout << "Votre choix : ";
            getline(cin, recup_choix);
            if ((unsigned int) (recup_choix[0] - '0') == 0 || (unsigned int) (recup_choix[0] - '0') == 1){
                choix_action = (unsigned int) (recup_choix[0] - '0');
                saisie_correcte = true;
            }
        }
    }
    if (choix_action == 0) {
        Defausse::getInstance().ajouterCarte(c);
    } else if (choix_action == 1) {
        auto carte_tactique = dynamic_cast<const CarteTactique*> (c);
        Pos pos_borne;
        if (carte_tactique != nullptr && (carte_tactique->getInfo() == toString(Combat::Colin_Maillard) || carte_tactique->getInfo() == toString(Combat::Combat_de_Boue))){
            if (this->getIa())
                pos_borne = ui_tactique.getChoixBorneCarteCentreIa(frontiere);
            else
                pos_borne = ui_tactique.getChoixBorneCarteCentre(frontiere);
            if (pos_borne == ERREUR){
                cout << "Impossible de jouer la carte, elle est défaussée";
                Defausse::getInstance().ajouterCarte(c);
            }
            else{
                frontiere.tuiles[pos_borne].placerCarteCentre(carte_tactique);
            }
        }else{  // carte jouée comme une carte Clan (on considère que le stratège jouera toujours de son côté de la borne)
            if (this->getIa())
                pos_borne = ui_tactique.getChoixBorneIa(frontiere, cote);
            else
                pos_borne = ui_tactique.getChoixBorne(frontiere, cote);
            if (pos_borne == ERREUR){
                cout << "Impossible de jouer la carte, elle est défaussée";
                Defausse::getInstance().ajouterCarte(c);
            }
            else{
                frontiere.tuiles[pos_borne].placerCarte(*c, cote);
            }
        }
    }
}


// permet de vérifier que toutes les cartes d'un côté de la tuile ont la même couleur
bool TuileTactique::verifMemeCouleur(NumJoueur num_joueur, const vector <Couleur>& coul) const{
    const Carte* premiereCarte = cartes_posees[static_cast<int>(num_joueur)][0];
    unsigned int cpt = 0;
    Couleur couleur;
    auto premiereCarteNormale = dynamic_cast<const CarteNormale*>(premiereCarte);
    if (premiereCarteNormale == nullptr) {
        couleur = coul[cpt++];
    }
    else{
        couleur = premiereCarteNormale->getCouleur();
    }
    for (size_t i = 1; i < nb_pleine; i++) {
        const Carte *carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale *>(carte);
        if (carteNormale == nullptr) {
            if (carteNormale == nullptr) {
                if (couleur != coul[cpt++])
                    return false;
            } else {
                if (couleur != carteNormale->getCouleur())
                    return false;
            }
        }
    }
    return true;
}

// permet de vérifier que toutes les cartes d'un côté de la tuile ont la même force
bool TuileTactique::verifMemeForce(NumJoueur num_joueur, vector <Force>& f) const{
    const Carte* premiereCarte = cartes_posees[static_cast<int>(num_joueur)][0];
    auto premiereCarteNormale = dynamic_cast<const CarteNormale*>(premiereCarte);
    Force force;
    int cpt=0;
    if (premiereCarteNormale == nullptr) {
        force = f[cpt++];
    }
    else
        force = premiereCarteNormale->getForce();
    for (size_t i = 1; i < nb_pleine; i++) {
        const Carte *carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale *>(carte);
        if (carteNormale == nullptr) {
            if (force != f[cpt++])
                return false;

        } else {
            if (carteNormale->getForce() != force) {
                return false;
            }
        }
    }
    return true;
}

// Méthode appelée par verifSuite permettant de trier les cartes en fonction de la force d'un côté de la tuile
void TuileTactique::ordonnerCartes(NumJoueur num_joueur, vector <Force>& f){
    // on utilise ici un tri à bulle. Ce tri n'est pas le plus optimisé, mais il est simple et largement suffisant
    // en effet, dans ce cas, il y a au maximum 4 cartes d'un côté de la frontière. Le coût du tri est donc faible
    bool modif;  // on arrête le parcours lorsque plus aucune modification n'a été effectuée
    do {
        modif = false;
        for (int i = 0; i < nb_pleine - 1; i++) {
            const Carte* carte1 = cartes_posees[static_cast<int>(num_joueur)][i];
            const Carte* carte2 = cartes_posees[static_cast<int>(num_joueur)][i + 1];
            auto carteNormale1 = dynamic_cast<const CarteNormale*>(carte1);
            auto carteNormale2 = dynamic_cast<const CarteNormale*>(carte2);
            if (carteNormale1 == nullptr){
                if (carteNormale2 == nullptr){
                    if (f[i] < f[i + 1]){
                        std::swap(cartes_posees[static_cast<int>(num_joueur)][i],
                                  cartes_posees[static_cast<int>(num_joueur)][i + 1]);
                        Force tmp = f[i];
                        f[i] = f[i+1];
                        f[i+1] = tmp;
                        modif = true;
                    }
                }
                else{
                    if (f[i] < carteNormale2->getForce()) {
                        std::swap(cartes_posees[static_cast<int>(num_joueur)][i],
                                  cartes_posees[static_cast<int>(num_joueur)][i + 1]);
                        modif = true;
                    }
                }
            }else{
                if (carteNormale2 == nullptr){
                    if (carteNormale1->getForce() < f[i+1]) {
                        std::swap(cartes_posees[static_cast<int>(num_joueur)][i],
                                  cartes_posees[static_cast<int>(num_joueur)][i + 1]);
                        modif = true;
                    }
                }
                else{
                    if (carteNormale1->getForce() > carteNormale2->getForce()){
                        std::swap(cartes_posees[static_cast<int>(num_joueur)][i],
                                  cartes_posees[static_cast<int>(num_joueur)][i + 1]);
                        modif = true;
                    }
                }
            }
        }
    } while (modif);
}

// permet de vérifier que les cartes d'un côté de la tuile forment une suite
bool TuileTactique::verifSuite(NumJoueur num_joueur, vector <Force>& f){
    ordonnerCartes(num_joueur, f);  // on trie les cartes de ce côté de la tuile

    for (size_t i = 0; i < nb_pleine - 1; i++) {
        const Carte* carte1 = cartes_posees[static_cast<int>(num_joueur)][i];
        const Carte* carte2 = cartes_posees[static_cast<int>(num_joueur)][i + 1];
        auto carteNormale1 = dynamic_cast<const CarteNormale*>(carte1);
        auto carteNormale2 = dynamic_cast<const CarteNormale*>(carte2);
        if (carteNormale1 == nullptr){
            if (carteNormale2 == nullptr){
                if (static_cast <int>(f[i]) != static_cast <int>(f[i+1]) + 1){
                    return false; // on a trouvé deux forces qui ne se suivaient pas
                }
            }
            else{
                if  (static_cast <int>(f[i]) != static_cast <int> (carteNormale2->getForce())+ 1){
                    return false; // on a trouvé deux forces qui ne se suivaient pas
                }
            }
        }else{
            if (carteNormale2 == nullptr){
                if (static_cast <int> (carteNormale1->getForce()) != static_cast <int>(f[i+1]) + 1){
                    return false; // on a trouvé deux forces qui ne se suivaient pas
                }
            }
            else{
                if (static_cast <int> (carteNormale1->getForce()) != static_cast<int>(carteNormale2->getForce()) + 1){
                    return false; // on a trouvé deux forces qui ne se suivaient pas
                }
            }
        }
    }
    return true;
}

// permet de sommer la force de toutes les cartes d'un côté de la tuile
unsigned int TuileTactique::getSomme(NumJoueur num_joueur, vector <Force>& f) const{
    unsigned int somme = 0;
    int cpt = 0;
    for (size_t i = 0; i < nb_pleine; i++) {
        const Carte* carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale*>(carte);
        if (carteNormale != nullptr) {
            somme += static_cast<unsigned int>(carteNormale->getForce());
        }
        else
            somme += static_cast<unsigned int> (f[cpt++]);
    }
    return somme;
}

Combinaison TuileTactique::determinerCombinaison(NumJoueur num_joueur, const vector <Couleur>& coul, vector <Force>& f){
    if (verifMemeCouleur(num_joueur, coul))
        if (verifSuite(num_joueur, f))
            return Combinaison::suite_couleur;
            // meilleure combinaison possible (trois cartes de la même couleur qui se suivent

        else
            // trois cartes de la même couleur, troisième meilleure combinaison
            return Combinaison::couleur;
    else if (verifMemeForce(num_joueur, f))
        // trois cartes de la même force, deuxième meilleure combinaison
        return Combinaison::brelan;
    else if (verifSuite(num_joueur, f))
        // trois cartes qui se suivent, mais pas de la même couleur, quatrième meilleure combinaison
        return Combinaison::suite;
    else
        // moins bonne combinaison, la somme
        return Combinaison::somme;
}