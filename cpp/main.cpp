#include "../h/partie.h"
#include "../h/schotten_totten.h"



int main(){
    /*
    Pioche<CarteNormale*, 54> cartes;

    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 9; ++j){
            cartes[i * 6 + j] = new CarteNormale((Couleur)i, j + 1);
            cout<<"Couleur = "<<toString( cartes[i * 6 + j]->getCouleur())
                << " Force = "<<toString( cartes[i * 6 + j]->getForce())<<"\n";
        }
    */
    try{
        Jeu j;
        j.commencer_jeu(6);
        bool continuer = true;
        while (continuer){
            Edition edition = choixEdition();
            Variante variante = choixVariante();

            j.jouerPartie(edition, variante);

            string choix;
            do{
                cout << "Jouer une autre partie ? (Oui/Non)";
                cin >> choix;
                for (size_t i = 0; i < choix.length(); i++)
                    tolower(choix[i]);
            }while (choix != "oui" && choix != "non");
            continuer = choix == "oui";
        }
        cout << "Fin du jeu\n";
        size_t joueur_gagnant = 0;
        unsigned  int score_max = 0;
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
