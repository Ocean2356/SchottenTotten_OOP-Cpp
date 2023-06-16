

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
        Couleur couleur = static_cast<Couleur>(i);
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
        Force force = static_cast<Force>(i);
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
    return CarteTroupe("Joker", getForce(), getCouleur(), Troupe::Joker);
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

template<>
void PremiereTactique<Carte,64>::jouerTour(){
    Agent_tactique& agent = dynamic_cast<Agent_tactique&>(agents[agentActive]);  // On récupère l'agent dont c'est le tour
    unsigned int choix_pioche = 0;
    unsigned int nb_carte_tactique = 0;
    if(piocheNormale.getNbCartes() > 0)
        cout << "\nIl reste " << piocheNormale.getNbCartes() << " cartes dans la pioche.";
    else
        cout << "`\nLa pioche Normale est vide !";
    if(piocheTactique.getNbCartes() > 0)
        cout << "\nIl reste " << piocheTactique.getNbCartes() << " cartes dans la pioche.";
    else
        cout << "\nLa pioche Tactique est vide !";

    //Vérification de si le joueur a combien de cartes tactiques
    for(size_t j = 0 ; j <= agent.getMain().getTailleMax(); j++){
        for(auto& i : agent.getMain().getCarte(j)){
            if(i.estTactique()){
                nb_carte_tactique++;
            }
        }
    }
    if(nb_carte_tactique != agent.getMain().getNbCartes())
        //il a au moins une carte normale
        Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere_tactique, (NumJoueur) agentActive);
        // on verifie combien il a jouer de carte tactique
        if(carte_a_jouee.find("Carte:") != std::string::npos){

        }

    Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere_tactique, (NumJoueur) agentActive);
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
            agent.jouerCarte(frontiere_tactique, carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, force, couleur);
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
                agent.revendiquerBorne(frontiere_tactique, borne_a_revendiquer, (NumJoueur) agentActive);
            }
        }
    }

    cout << "\nDans quel pioche voulez-vous piochez ? "<< endl;
    cout << "1: Pioche normale" << endl;
    cout << "2: Pioche tactique" << endl;
    cin >> choix_pioche;
    if (choix_pioche == 1)
    // Piocher une nouvelle carte
        if (piocheNormale.getNbCartes() > 0){
            const CarteNormale& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
    else
        if(piocheTactique.getNbCartes() > 0){
            const CarteTactique& carte = piocheTactique.piocher();
            agent.piocher(carte);
        }
    agentSuivant();
}


//Méthode permettant d'initialiser la pioche normale
template<>
void PremiereTactique<Carte,64>::initierPiocheTactique(){
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
template<>
void PremiereTactique<Carte,64>::initierAgents(Ordre ordre){
    agents[0] = ordre[0]->getAgent();
    agents[1] = ordre[1]->getAgent();
    agentActive = 0;
}

// Méthode permettant d'initialiser les mains (appelée par la méthode Premiere::commencer)
template<>
void PremiereTactique<Carte,64>::initierMains(){
    for (auto& agent: agents)
        for (int i = 0; i < NMAIN; ++i){
            // On pioche des cartes Clans jusqu'à en avoir NMAIN = 6 en main.
            const CarteNormale& carte = piocheNormale.piocher();
            agent.piocher(carte);
        }
}

// Constructeur de la classe PremiereTactique
template<>
PremiereTactique<Carte,64>::PremiereTactique(){
    initierPiocheTactique();  // initialisation de la pioche (54 cartes tactiques)
}


// Méthode permettant la saisie par l'utilisateur d'une carte à jouer
Movement Agent_tactique::choisirCarteAJouer(const Frontiere<class Tuile_tactique>& f, NumJoueur joueur_num){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    PremiereTactique<Carte,64>().ui_tactique.afficherFrontiere_tactique(f);
    string recup_choix;
    if (main.getNbCartes() > 0){  // s'il reste encore des cartes au joueur
        Pos choix_carte = PremiereNormale().ui.getChoixCarte(main);
        mvt += "Carte:";
        mvt += (char) choix_carte;

        Pos choix_borne = PremiereTactique<Carte,64>().ui_tactique.getChoixBorne(f, joueur_num);
        if (choix_borne == ERREUR) return "";
        mvt += "Borne:";
        mvt += (char) (choix_borne);
    }
    return mvt;
}

// Méthode permettant de jouer une carte dont la position dans la main est donnée sur une frontière
void Agent_tactique::jouerCarte(Frontiere<class Tuile_tactique>& frontiere, unsigned int pos_carte, size_t pos_borne, NumJoueur joueur_num, string nom_carte){
    if (frontiere.tuiles[pos_borne].cotePlein(joueur_num))
        throw PartieException("La tuile est deja pleine\n");
    const Carte& c = main.jouerCarte(pos_carte);  // on récupère la carte à jouer
    const CarteTactique& carteTactique = dynamic_cast<const CarteTactique&>(c);
    frontiere.tuiles[pos_borne].placerCarte(c, joueur_num);  // on place la carte sur la frontière

}

















