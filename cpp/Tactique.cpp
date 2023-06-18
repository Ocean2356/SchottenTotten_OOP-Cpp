

#include "../h/Tactique.h"
#include <iostream>
#include <string>


std::initializer_list<Troupe> Troupes ={Troupe::Joker, Troupe::Espion, Troupe::Porte_Bouclier};  // liste des cartes troupes
std::initializer_list<Combat> Combats ={Combat::Colin_Maillard, Combat::Combat_de_Boue };  // liste des cartes combats
std::initializer_list<Ruse> Ruses ={Ruse::Chasseur_de_tete, Ruse::Stratege, Ruse::Banshee, Ruse::Traitre };  // liste des cartes combats



// Fonction permettant de retourner une string correspondant à un type de carte tactique troupe donné
string toString(Troupe t) {
    switch (t) {
        case Troupe::Joker:
            return "joker";
        case Troupe::Espion:
            return "espion";
        case Troupe::Porte_Bouclier:
            return "porte_bouclier";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}


string toString(Combat c) {
    switch (c) {
        case Combat::Colin_Maillard:
            return "colin_maillard";
        case Combat::Combat_de_Boue:
            return "combat_boue";
        default:
            throw PartieException("Type de carte tactique inconnue");

    }
}

string toString(Ruse r) {
    switch (r) {
        case Ruse::Chasseur_de_tete:
            return "chasseur_de_tete";
        case Ruse::Stratege:
            return "stratege";
        case Ruse::Banshee:
            return "banshee";
        case Ruse::Traitre:
            return "traitre";
        default:
            throw PartieException("Type de carte tactique inconnue");
    }
}

void demande_couleur(CarteTroupe& carte){
    for (int i = 0; i < static_cast<int>(Couleur::Violet) + 1; i++) {
        auto couleur = static_cast<Couleur>(i);
        std::cout << i << ": " << static_cast<int>(couleur) << std::endl;
    }
    int choix_couleur = -1;
    while (choix_couleur < 0 || choix_couleur > 7) {
        cout << "Veuillez choisir une couleur :" << endl;
        cin >> choix_couleur;
    }
    carte.setCouleur(static_cast<Couleur>(choix_couleur));
}

void demande_force(CarteTroupe& carte, unsigned int max){
    for (int i = 0; i < max  + 1; i++) {
        auto force = static_cast<Force>(i);
        std::cout << i << ": " << static_cast<int>(force) << std::endl;
    }
    int choix_force = -1;
    while (choix_force < 0 || choix_force > max) {
        cout << "Veuillez choisir une force entre 1 et" << max-1 << ":" << endl;
        cin >> choix_force;
    }
    carte.setForce(static_cast<Force>(choix_force));
}

// Carte Joker se transforme en n'importe quelle carte clan
CarteTroupe CarteTroupe::mettre_Joker() {
    cout << "Carte joker pose : " << endl;
    demande_couleur(*this);
    demande_force(*this, 10);
    return {"Joker", this->CarteNormale::getForce(), this->CarteNormale::getCouleur(), Troupe::Joker};
}

//Carte clan de valeur 7 dont on choisit la couleur
CarteTroupe CarteTroupe::mettre_Espion() {
    cout << "Carte espion pose : " << endl;
    demande_couleur(*this);
    setForce(Force::sept);
    return *this;
}

//carte porte_bouclier permet de se transformer en une valeur de 1,2 ou 3 et de choisir n'importe quelle couleur
CarteTroupe CarteTroupe::mettre_PorteBouclier() {
    cout << "Carte porte bouclier pose : " << endl;
    demande_couleur(*this);
    demande_force(*this,4);
    return *this;
}

//affichage d'une carte troupe
void CarteTroupe::afficherCarte() const {
    if (getNom() == "Joker") {
        cout << "|    Joker |";
    } else if (getNom() == "Porte Bouclier") {
        cout << "| Porte bouclier |";
    } else if (getNom() == "Espion") {
        cout << "|   Espion |";
    } else {
        cout << "|    " << getNom() << "    |";
    }
}

//affichage d'une carte combat
void CarteCombat::afficherCarte()const{
    if(getNom()=="Colin Maillard"){
        cout<< "| Colin Maillard |";
    }
    else if(getNom()=="Combat de Boue"){
        cout<< "| Combat Boue |";
    }
}

void CarteRuse::afficherCarte()const{
    if(getNom()=="Chasseur de Tete"){
        cout<< "| chasseur_tete |";
    }
    else if(getNom()=="Stratege"){
        cout<< "| stratege |";
    }
    else if(getNom()=="Banshee"){
        cout<< "|  banshee |";
    }
    else if(getNom()=="Traitre"){
        cout<< "|  traitre |";
    }
}

void CarteRuse::mettre_ChasseurdeTete() {
    cout << "Vous avez joué la carte Chasseur de Tête" << endl;
    cout  << "Veuillez choisir le nombre de carte Clan que vous voulez piocher :" << endl;
    int nbCarteClan;
    cin >> nbCarteClan;
    while (nbCarteClan>3 || nbCarteClan<0){
        cout  << "Veuillez choisir le nombre de carte Clan que vous voulez piocher (entre 1 et 3 cartes) :" << endl;
        cin >> nbCarteClan;
    }
    switch(nbCarteClan){
        case 0 : {
            cout << "Vous voulez donc piocher 3 cartes tactiques" << endl;

            break;
        }

        case 1:{
            cout << "Vous voulez donc piocher 1 carte clan et 2 cartes tactiques" << endl;

            break;
        }

        case 2: {
            cout << "Vous voulez donc piocher 2 cartes clans et 1 carte tactique" << endl;

            break;
        }

        case 3: {
            cout << "Vous voulez donc piocher 3 cartes clans" << endl;
            for (int k=0;k<3;k++){

            }

            break;

        }

    }
}


void PremiereTactique::jouerTour(){
    AgentTactique& agent = agents[agentActive];  // On récupère l'agent dont c'est le tour
    unsigned int choix_pioche = 0;
    if(piocheNormale.getNbCartes() > 0)
        cout << "Il reste " << piocheNormale.getNbCartes() << " cartes dans la pioche normale.\n";
    else
        cout << "La pioche Normale est vide !\n";
    if(piocheTactique.getNbCartes() > 0)
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
        auto pos_carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_carte + 6];
        size_t pos_borne = carte_a_jouer.find("Borne:");
        if (pos_borne != std::string::npos){  // si on a trouvé une position de borne
            // on place la carte sur la borne
            auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouer[pos_borne + 6];
            string nom_carte;
            agent.jouerCarte(frontiere_tactique, pos_carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, nom_carte);
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

    cout << "\nDans quel pioche voulez-vous piochez ? \n";
    cout << "1: Pioche normale\n";
    cout << "2: Pioche tactique\n";
    bool saisie_ok = false;
    string saisie;
    while (!saisie_ok){
        getline(cin, saisie);
        if ((unsigned int) saisie[0] - '0' == 1 || (unsigned int) saisie[0] - '0' == 2){
            saisie_ok = true;
            choix_pioche = (unsigned int) (unsigned char) saisie[0];
        }
    }

    if (choix_pioche == 1){
        // Piocher une nouvelle carte
        if (piocheNormale.getNbCartes() > 0){
            const CarteNormale& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
    }
    else{
        if(piocheTactique.getNbCartes() > 0){
            const CarteTactique& carte = piocheTactique.piocher();
            agent.piocher(carte);
        }
    }
    agentSuivant();
}


//Méthode permettant d'initialiser la pioche normale
void PremiereTactique::initierPiocheTactique(){
    // on alloue une nouvelle carte pour chaque force et couleur (par appel de Pioche::setCarte)
    size_t pos = 0;
    for(auto& t: Troupes)
        piocheTactique.setCarteTactique(pos++, toString(t));
    for(auto& c: Combats)
        piocheTactique.setCarteTactique(pos++, toString(c));
    for(auto& r: Ruses)
        piocheTactique.setCarteTactique(pos++, toString(r));
}

// Méthode permettant d'initialiser les agents (appelée par la méthode Premiere::commencer)
void PremiereTactique::initierAgents(Ordre ordre){
    agents[0] = AgentTactique(NCARTETACTIQUE);
    agents[1] = AgentTactique(NCARTETACTIQUE);
    agentActive = 0;
}

// Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
void PremiereTactique::initierMains(){
    for (auto& agent: agents)
        for (int i = 0; i < NMAIN; ++i){
            // On pioche des cartes Clans jusqu'à en avoir NMAIN = 6 en main.
            const CarteNormale& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
}

// Constructeur de la classe PremiereTactique
PremiereTactique::PremiereTactique(){
    initierPiocheTactique();  // initialisation de la pioche (54 cartes tactiques)
}


// Méthode permettant la saisie par l'utilisateur d'une carte à jouer
Movement AgentTactique::choisirCarteAJouer(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num, unsigned int nb_cartes_tactiques_jouees_autre_joueur){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    ui_tactique.afficherFrontiere_tactique(f);
    vector <int> pos_cartes_normales;
    vector <int> pos_cartes_tactiques;

    //Comptage du nombre de cartes tactiques du joueur
    for(Pos i= 0 ; i <= getMain().getNbCartes(); i++){
        if(getMain().getCarte(i).estTactique()){
            pos_cartes_tactiques.push_back(i);
        }
        else
            pos_cartes_normales.push_back(i);
    }
    unsigned int nb_choix = 1;
    bool a_carte_normale_en_main = !pos_cartes_normales.empty();
    bool a_carte_tactique_en_main = !pos_cartes_tactiques.empty();
    if(a_carte_normale_en_main) //il a au moins une carte normale
        cout << "Entrez " << nb_choix++ << " pour jouer une carte normale\n";

    if(a_carte_tactique_en_main && nb_cartes_tactiques_jouees <= nb_cartes_tactiques_jouees_autre_joueur)  //il a au moins une carte tactique
        cout << "Entrez " << nb_choix++ << " pour jouer une carte normale\n";

    if (!a_carte_normale_en_main)
        cout << "Entrez " << nb_choix << " pour ne rien jouer ce tour-ci\n";
    bool test_saisie = false;
    string choix;
    while (!test_saisie){
        cout << "Votre choix : ";
        getline(cin, choix);
        if ((unsigned int) (choix[0] - '0') >= 1 && (unsigned int) (choix[0] - '0') <= nb_choix) {
            test_saisie = true;
        }
    }
    Pos carte_a_jouer = -1;
    if (choix[0] == '1' && a_carte_normale_en_main){
        if (this->getIa())
            carte_a_jouer = ui_tactique.getChoixCarteIa(main, pos_cartes_normales);
        else
            carte_a_jouer = ui_tactique.getChoixCarte(main, pos_cartes_normales);
    }

    else if ((choix[0] == '1' && !a_carte_normale_en_main && a_carte_tactique_en_main) || (choix[0] == '2' && !a_carte_normale_en_main && a_carte_tactique_en_main)){
        nb_cartes_tactiques_jouees ++;
        if (this->getIa())
            carte_a_jouer = ui_tactique.getChoixCarteIa(main, pos_cartes_tactiques);
        else
            carte_a_jouer = ui_tactique.getChoixCarte(main, pos_cartes_tactiques);
    }
    else{
        return mvt;
    }

    mvt += "Carte:";
    mvt += (char) carte_a_jouer;

    Pos choix_borne = ui_tactique.getChoixBorne(f, joueur_num);
    if (choix_borne == ERREUR) return "";
    mvt += "Borne:";
    mvt += (char) (choix_borne);
    return mvt;
}

// Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
void AgentTactique::jouerCarte(Frontiere<class TuileTactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, string nom_carte){
    if (frontiere.tuiles[pos_borne].cotePlein(joueur_num))
        throw PartieException("La tuile est deja pleine\n");
    const Carte& c = main.jouerCarte(pos_carte);  // on récupère la carte à jouer
    const CarteTactique& carteTactique = dynamic_cast<const CarteTactique&>(c);
    frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière

}


bool TuileTactique::verifRevendiquable(NumJoueur num_joueur) {
    // si la tuile est déjà revendiquée, elle n'est pas revendiquable
    if (estRevendiquee())
        return false;

    // si le côté du joueur qui a demandé à revendiquer la borne n'est pas plein, il ne peut pas revendiquer la borne
    if (!cotePlein(num_joueur))
        return false;

    // Détermination de la combinaison du joueur qui souhaite revendiquer la tuile
    Combinaison joueur1 = determinerCombinaison(num_joueur);

    // on récupère le numéro de l'autre joueur (celui qui n'a pas demandé à revendiquer la borne)
    auto autre_joueur = (NumJoueur) (((int) num_joueur + 1) % 2);
    // Détermination de la combinaison de l'autre joueur
    Combinaison joueur2 = determinerCombinaison(autre_joueur);

    if ((int) joueur1 > (int) joueur2)
        // le joueur qui souhaite revendiquer la tuile a la meilleure combinaison
        return true;
    else if ((int) joueur1 == (int) joueur2) { // les deux joueurs ont la même combinaison
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

Movement AgentTactique::choisirBornesARevendiquer(Frontiere<class TuileTactique> &f, NumJoueur joueur_num) {
    if (this->getIa())
        return ui_tactique.getChoixBornesARevendiquerIa(f, joueur_num);
    else
        return ui_tactique.getChoixBornesARevendiquer(f, joueur_num);
}