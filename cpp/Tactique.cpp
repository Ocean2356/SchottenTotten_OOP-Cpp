

#include "../h/tactique.h"
#include <iostream>
#include <string>


using namespace std;


// Fonction permettant de retourner une string correspondant à un type de carte tactique donné
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


// Carte Joker se transforme en n'importe quelle carte clan
CarteTroupe CarteTroupe::mettre_Joker() {
    cout << "Carte joker pose : " << endl;
    for (int i = 0; i < static_cast<int>(Couleur::Violet) + 1; i++) {
        Couleur couleur = static_cast<Couleur>(i);
        std::cout << i << ": " << static_cast<int>(couleur) << std::endl;
    }
    int choix_couleur = -1;
    while (choix_couleur < 0 || choix_couleur > 7) {
        cout << "Veuillez choisir une couleur :" << endl;
        cin >> choix_couleur;
    }
    setCouleur(static_cast<Couleur>(choix_couleur));

    int choix_force = -1;
    while (choix_force < 0 || choix_force > 10) {
        cout << "Veuillez choisir une force entre 1 et 9 :" << endl;
        cin >> choix_force;
    }
    setForce(static_cast<Force>(choix_force));
    return *this;
}

//Carte clan de valeur 7 dont on choisit la couleur
CarteTroupe CarteTroupe::mettre_Espion() {
    cout << "Carte joker pose : " << endl;
    for (int i = 0; i < static_cast<int>(Couleur::Violet) + 1; i++) {
        Couleur couleur = static_cast<Couleur>(i);
        std::cout << i << ": " << static_cast<int>(couleur) << std::endl;
    }
    int choix_couleur = -1;
    while (choix_couleur < 0 || choix_couleur > 7) {
        cout << "Veuillez choisir une couleur :" << endl;
        cin >> choix_couleur;
    }
    setCouleur(static_cast<Couleur>(choix_couleur));
    setForce(Force::sept);
    return *this;
}

//carte porte_bouclier permet de se transformer en une valeur de 1,2 ou 3 et de choisir n'importe quelle couleur
CarteTroupe CarteTroupe::mettre_PorteBouclier() {
    cout << "Carte joker pose : " << endl;
    for (int i = 0; i < static_cast<int>(Couleur::Violet) + 1; i++) {
        Couleur couleur = static_cast<Couleur>(i);
        std::cout << i << ": " << static_cast<int>(couleur) << std::endl;
    }
    int choix_couleur = -1;
    while (choix_couleur < 0 || choix_couleur > 7) {
        cout << "Veuillez choisir une couleur :" << endl;
        cin >> choix_couleur;
    }
    setCouleur(static_cast<Couleur>(choix_couleur));

    int choix_force = -1;
    while (choix_force < 0 || choix_force > 4) {
        cout << "Veuillez choisir une force entre 1 et 3 :" << endl;
        cin >> choix_force;
    }
    setForce(static_cast<Force>(choix_force));
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




