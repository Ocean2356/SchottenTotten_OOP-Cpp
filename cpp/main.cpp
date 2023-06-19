/***********************************************************************************************************************
Nom : main.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp permettant de jouer à Schotten-Totten utilisé dans le cadre du projet P23 de l'UV LO21.
***********************************************************************************************************************/


#include "../h/partie.h"
#include "../h/schotten_totten.h"

int main(){
    try{
        Jeu j;
        j.commencerJeu();  // choix des noms des joueurs
        bool continuer = true;
        while (continuer){  // tant que le joueur souhaite faire une partie
            Edition edition = choixEdition();  // choix de l'édition du jeu
            Variante variante = choixVariante();  // choix de la variante du jeu
            j.jouerPartie(edition, variante);

            string choix;  // on demande à l'utilisateur s'il souhaite jouer une autre partie
            do{
                cout << "Jouer une autre partie ? (oui/non)";
                cin >> choix;
                size_t i = 0;
                for (auto &c: choix)
                    choix[i++] = (char) tolower(c);
            } while (choix != "oui" && choix != "non");
            continuer = choix == "oui";
        }
        j.finirJeu();

    }catch (PartieException &e){
        cout << "Erreur : " << e.getInfo() << "\n";
    }
    return 0;
}
