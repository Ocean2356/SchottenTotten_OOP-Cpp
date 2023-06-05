#include "../h/partie.h"
#include <random>
#include <chrono>

// Fonction permettant de retourner une string correspondant à une couleur donnée
string toString(Couleur c) {
    switch (c) {
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
string toString(Force f) {
    return to_string(f);
}

// Méthode permettant d'échanger deux cartes de la pioche
void Pioche::swapCartes(Carte* c1, Carte* c2){
    Carte* tmp = c1;
    c1 = c2;
    c2 = tmp;
};

// Méthode permettant de piocher une carte
bool Pioche::piocher(Carte **carte) {
    if (estVide())
        throw PartieException("La pioche est vide");
    else {
        // Génération d'un nombre aléatoire entre 0 et nbCartes - 1 (pour piocher une des cartes piochable de la pioche)
        std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution <int> distrib {0, (int) nbCartes-1};
        size_t x = distrib(random_eng);

        // On "supprime" la carte piochée, en l'échangeant avec la dernière carte de la pioche et en décrementant le nombre de cartes piochable
        swapCartes(carte[x], cartes[--nbCartes]);
        return carte[nbCartes];  // on retourne la carte que l'on vient de "supprimer"
    }
}


void Main::piocherCarte(Carte* c){
    cartes.push_back(c);
}

Carte* Main::jouerCarte(size_t i) {
    Carte* c = cartes[i];
    cartes.erase(cartes.begin() + i);
    return c;
}


