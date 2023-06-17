

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
    vector <int> pos_cartes_normales;
    vector <int> pos_cartes_tactiques;
    if(piocheNormale.getNbCartes() > 0)
        cout << "\nIl reste " << piocheNormale.getNbCartes() << " cartes dans la pioche.";
    else
        cout << "`\nLa pioche Normale est vide !";
    if(piocheTactique.getNbCartes() > 0)
        cout << "\nIl reste " << piocheTactique.getNbCartes() << " cartes dans la pioche.";
    else
        cout << "\nLa pioche Tactique est vide !";

    //Comptage du nombre de cartes tactiques du joueur
    for(Pos i= 0 ; i <= agent.getMain().getNbCartes(); i++){
        if(agent.getMain().getCarte(i).estTactique()){
            pos_cartes_tactiques.push_back(i);
        }
        else
            pos_cartes_normales.push_back(i);
    }
    if(!pos_cartes_normales.empty()){
        //il a au moins une carte normale
        Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere_tactique, (NumJoueur) agentActive);
        // on verifie combien il a jouer de carte tactique
        if(carte_a_jouee.find("Carte:") != std::string::npos){

        }
    }


    Movement carte_a_jouee = agent.choisirCarteAJouer(frontiere_tactique, (NumJoueur) agentActive);
    // Jouer la carte choisie
    size_t pos_carte = carte_a_jouee.find("Carte:");
    if (pos_carte == std::string::npos){
        // pas de carte à jouer, on force le joueur à revendiquer toutes les bornes possibles
        // pour éviter que la partie ne s'arrête jamais
        for (size_t i=0; i<frontiere_tactique.getNbTuile(); i++){
            if (frontiere_tactique.verifRevendiquable(i, (NumJoueur) agentActive))
                agent.revendiquerBorne(frontiere_tactique, i, (NumJoueur) agentActive);
        }

    }else{
        auto carte_a_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_carte + 6];
        size_t pos_borne = carte_a_jouee.find("Borne:");
        if (pos_borne != std::string::npos){  // si on a trouvé une position de borne
            // on place la carte sur la borne
            auto borne_sur_laquelle_jouer = (unsigned int) (unsigned char) carte_a_jouee[pos_borne + 6];
            string nom_carte;
            agent.jouerCarte(frontiere_tactique, carte_a_jouer, borne_sur_laquelle_jouer, (NumJoueur) agentActive, nom_carte);
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

    cout << "\nDans quel pioche voulez-vous piochez ? "<< endl;
    cout << "1: Pioche normale" << endl;
    cout << "2: Pioche tactique" << endl;
    cin >> choix_pioche;
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
    auto agent = dynamic_cast<const AgentTactique*> (&ordre[0]->getAgent());
    if (agent == nullptr)
        throw PartieException("Erreur, on devrait avoir un agent tactique pour une partie tactique");
    agents[0] = *agent;
    agent = dynamic_cast<const AgentTactique*> (&ordre[1]->getAgent());
    if (agent == nullptr)
        throw PartieException("Erreur, on devrait avoir un agent tactique pour une partie tactique");
    agents[1] = *agent;
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
Movement AgentTactique::choisirCarteAJouer(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num){
    Movement mvt;  // string permettant d'indiquer les actions à effectuer
    ui_tactique.afficherFrontiere_tactique(f);
    string recup_choix;
    if (main.getNbCartes() > 0){  // s'il reste encore des cartes au joueur
        Pos choix_carte = ui_tactique.getChoixCarte(main);
        mvt += "Carte:";
        mvt += (char) choix_carte;

        Pos choix_borne = ui_tactique.getChoixBorne(f, joueur_num);
        if (choix_borne == ERREUR) return "";
        mvt += "Borne:";
        mvt += (char) (choix_borne);
    }
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

Movement UITactique::getChoixBornesARevendiquer(Frontiere<class TuileTactique>& f, NumJoueur joueur_num){
    Movement mvt;
    afficherFrontiere_tactique(f);

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
            if (f.verifRevendiquable(choix_borne_a_revendiquer - 1, joueur_num)){
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


Movement UITactique::getChoixBornesARevendiquerIa(Frontiere<class TuileTactique>& f, NumJoueur joueur_num){
    Movement mvt;
    Pos nb_bornes_a_revendiquer = 0;

    for (Pos i = 0; i<f.getNbTuile(); i++)
        if (f.verifRevendiquable((size_t) i, joueur_num)){
            // Si la tuile est revendiquable par le joueur
            mvt += "Revendiquer:";
            mvt += i;
            nb_bornes_a_revendiquer++;
        }
    mvt += "nb:";
    mvt += (char) nb_bornes_a_revendiquer;
    return mvt;
}

void UITactique::afficherFrontiere_tactique(const Frontiere<class TuileTactique> &f) const{

}

Pos UITactique::getChoixCarte(Main& main){

}

Pos UITactique::getChoixBorne(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num){

}










