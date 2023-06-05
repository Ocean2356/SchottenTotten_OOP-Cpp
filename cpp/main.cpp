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
                for (auto& c : choix)
                    choix[i++] = (char) tolower(c);
            }while (choix != "oui" && choix != "non");
            continuer = choix == "oui";
        }
        cout << "Fin du jeu\n";

        // Affichage des scores et du joueur gagnant
        size_t joueur_gagnant = 0;
        unsigned int score_max = 0;
        for (size_t i=0; i<j.getNbJoueurs(); i++){
            unsigned int score = j.getScore(i);
            cout << "Le joueur " << j.getNom(i) << "a obtenu " << score << "points.\n";
            if (score_max < score) {
                score_max = score;
                joueur_gagnant = i;
            }
        }
        cout << "Le joueur " << j.getNom(joueur_gagnant)  << "remporte la partie !\n";

    }catch(PartieException& e){
        cout << "Erreur : " << e.getInfo() << "\n";
    }
    return 0;
}
