/***********************************************************************************************************************
Nom : tuile.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient des méthodes déclarées mais
non définies dans partie.h pour la classe Tuile
***********************************************************************************************************************/


#include "../h/partie.h"

// Méthode permettant de vérifier que toutes les cartes d'un côté de la tuile ont la même couleur
bool Tuile::verifMemeCouleur(NumJoueur num_joueur) const{
    const Carte* premiereCarte = cartes_posees[static_cast<int>(num_joueur)][0];
    auto premiereCarteNormale = dynamic_cast<const CarteNormale*>(premiereCarte);
    if (premiereCarteNormale == nullptr) {
        // La première carte n'est pas une CarteNormale, on considère donc qu'elles n'ont pas la même couleur
        return false;
    }
    Couleur couleur = premiereCarteNormale->getCouleur();
    for (size_t i = 1; i < nb_pleine; i++) {
        const Carte* carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale*>(carte);
        if (carteNormale == nullptr || carteNormale->getCouleur() != couleur) {
            // La carte n'est pas une CarteNormale ou sa couleur est différente
            return false;
        }
    }
    return true;
}

// Méthode permettant de vérifier que toutes les cartes d'un côté de la tuile ont la même force
bool Tuile::verifMemeForce(NumJoueur num_joueur) const{
    const Carte* premiereCarte = cartes_posees[static_cast<int>(num_joueur)][0];
    auto premiereCarteNormale = dynamic_cast<const CarteNormale*>(premiereCarte);
    if (premiereCarteNormale == nullptr) {
        // La première carte n'est pas une CarteNormale, on considère donc qu'elles n'ont pas la même force
        return false;
    }
    Force force = premiereCarteNormale->getForce();
    for (size_t i = 1; i < nb_pleine; i++) {
        const Carte* carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale*>(carte);
        if (carteNormale == nullptr || carteNormale->getForce() != force) {
            // La carte n'est pas une CarteNormale ou sa force est différente
            return false;
        }
    }
    return true;
}



// Méthode appelée par verifSuite permettant de trier les cartes en fonction de la force d'un côté de la tuile
void Tuile::ordonnerCartes(NumJoueur num_joueur){
    // on utilise ici un tri à bulle. Ce tri n'est pas le plus optimisé, mais il est simple et largement suffisant
    // en effet, dans ce cas, il y a au maximum 4 cartes d'un côté de la frontière. Le coût du tri est donc faible
    bool modif;  // on arrête le parcours lorsque plus aucune modification n'a été effectuée
    do {
        modif = false;
        for (size_t i = 0; i < nb_pleine - 1; i++) {
            const Carte* carte1 = cartes_posees[static_cast<int>(num_joueur)][i];
            const Carte* carte2 = cartes_posees[static_cast<int>(num_joueur)][i + 1];
            auto carteNormale1 = dynamic_cast<const CarteNormale*>(carte1);
            auto carteNormale2 = dynamic_cast<const CarteNormale*>(carte2);
            if (carteNormale1 != nullptr && carteNormale2 != nullptr &&
                carteNormale1->getForce() > carteNormale2->getForce()) {
                std::swap(cartes_posees[static_cast<int>(num_joueur)][i],
                          cartes_posees[static_cast<int>(num_joueur)][i + 1]);
                modif = true;
            }
        }
    } while (modif);
}


// Méthode permettant de vérifier que les cartes d'un côté de la tuile forment une suite
bool Tuile::verifSuite(NumJoueur num_joueur){
    ordonnerCartes(num_joueur);  // on trie les cartes de ce côté de la tuile

    std::vector<const CarteNormale*> cartes_normales;
    cartes_normales.reserve(nb_pleine);

    for (size_t i = 0; i < nb_pleine; i++) {
        const Carte* carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carte_normale = dynamic_cast<const CarteNormale*>(carte);
        if (carte_normale != nullptr) {
            cartes_normales.push_back(carte_normale);
        }
    }

    for (size_t i = 0; i < cartes_normales.size() - 1; i++) {
        if (static_cast<int>(cartes_normales[i]->getForce()) + 1 != static_cast<int>(cartes_normales[i + 1]->getForce())) {
            return false; // on a trouvé deux forces qui ne se suivaient pas
        }
    }
    return true;
}

// Méthode permettant de sommer la force de toutes les cartes d'un côté de la tuile
unsigned int Tuile::getSomme(NumJoueur num_joueur) const {
    unsigned int somme = 0;
    for (size_t i = 0; i < nb_pleine; i++) {
        const Carte* carte = cartes_posees[static_cast<int>(num_joueur)][i];
        auto carteNormale = dynamic_cast<const CarteNormale*>(carte);
        if (carteNormale != nullptr) {
            somme += static_cast<unsigned int>(carteNormale->getForce());
        }
    }
    return somme;
}

int max(int a, int b){
    if (a<b)
        return b;
    return a;
}

bool Tuile::verifSuiteCouleurPossible(bool joueur_actif_suite_couleur, bool meme_couleur, bool suite, int force_max, int force_min, unsigned int somme, unsigned int somme_joueur_actif, int couleur_carte_a_jouer, TableauJouee tab) const{
    if (!meme_couleur || !suite) // l'autre joueur ne pourra jamais faire de suite couleur
        return false;

    int force_possible_1;
    int force_possible_2 = -1;
    if (force_max == force_min + 1){  // au plus deux forces possibles pour compléter la suite
        if (force_min > 1){
            force_possible_1 = force_min - 1;
            if (force_max < 9)
                force_possible_2 = force_max + 1;
        }
        else  // une seule force possible
            force_possible_1 = force_max + 1;
    }
    else
        force_possible_1 = force_min + 1;  // la force restante pour la suite se situe entre les deux premières
    somme += force_possible_1;  // on ne considère pas le cas de force_possible 2 pour simplifier

    if (somme_joueur_actif >= somme && joueur_actif_suite_couleur)  // l'autre joueur ne pourra pas battre la somme
        // Rappel : en cas d'égalité des combinaisons, le joueur qui remplit la borne en premier l'emporte
        return false;

    // Il reste à regarder si l'autre joueur peut faire la combinaison gagnante
    if (!tab[couleur_carte_a_jouer][force_possible_1] || (force_possible_2 != -1 && !tab[couleur_carte_a_jouer][force_possible_2]))
        // la combinaison suite couleur est possible
        return true;
    return false;  // la combinaison suite couleur est impossible
}


bool Tuile::verifBrelanPossible(bool joueur_actif_brelan, bool meme_force, int force_carte_a_jouer, unsigned int somme_joueur_actif, TableauJouee tab) const{
    if (!meme_force) // l'autre joueur ne pourra jamais faire de brelan
        return false;

    if (force_carte_a_jouer * 3 <= somme_joueur_actif && joueur_actif_brelan) // l'autre joueur ne pourra pas battre la somme
        // Rappel : en cas d'égalité des combinaisons, le joueur qui remplit la borne en premier l'emporte
        return false;
    for (Pos i=0; i<6; i++)
        if (!tab[force_carte_a_jouer][i])
            // Il reste au moins une carte jouable pour compléter le brelan
            return true;
    return false;  // l'autre joueur ne pourra pas compléter le brelan
}

bool Tuile::verifMemeCouleurPossible(bool joueur_actif_meme_couleur, bool meme_couleur, unsigned int somme, unsigned int somme_joueur_actif, int couleur_carte_a_jouer, TableauJouee tab) const{
    if (!meme_couleur) // l'autre joueur ne pourra jamais avoir trois fois la même couleur
        return false;
    int max_force_jouable = -1;
    for (Pos i=0; i<9; i++)
        if (!tab[couleur_carte_a_jouer][i]) {
            // Il reste au moins une carte jouable pour compléter les cartes de la même couleur
            max_force_jouable = max(i, max_force_jouable);
        }
    // si max_force_jouable vaut -1, il ne reste aucune carte de la couleur jouable
    // sinon, on regarde si la force de l'autre joueur peut battre la force du joueur actif
    return (max_force_jouable != -1) && (((max_force_jouable + 1 + somme) > somme_joueur_actif) || !joueur_actif_meme_couleur);
}

bool Tuile::verifSuitePossible(bool joueur_actif_suite, bool suite, int force_max, int force_min, unsigned int somme, unsigned int somme_joueur_actif, TableauJouee tab) const{
        int force_carte_a_jouer;
        if (!suite) // l'autre joueur ne pourra jamais faire de suite
            return false;
        int force_possible_1;
        int force_possible_2 = -1;
        if (force_max == force_min + 1){  // au plus deux forces possibles pour compléter la suite
            if (force_min > 1){
                force_possible_1 = force_min - 1;
                if (force_max < 9)
                    force_possible_2 = force_max + 1;
            }
            else  // une seule force possible
                force_possible_1 = force_max + 1;
        }
        else
            force_possible_1 = force_min + 1;  // la force restante pour la suite se situe entre les deux premières
        somme += force_possible_1;  // on ne considère pas le cas de force_possible 2 pour simplifier

        if (somme_joueur_actif >= somme && joueur_actif_suite)  // l'autre joueur ne pourra pas battre la somme
        // Rappel : en cas d'égalité des combinaisons, le joueur qui remplit la borne en premier l'emporte
            return false;

        // Il reste à regarder si l'autre joueur peut faire la combinaison gagnante
        for (Pos i=0; i<6; i++)
            if (!tab[i][force_possible_1] || (force_possible_2 != -1 && !tab[i][force_possible_2])){
                // Il reste au moins une carte jouable pour compléter les cartes de la même suite
                return true;
            }
        // la carte à jouer est déjà jouée, l'autre joueur ne pourra pas l'emporter
        return false;
}


bool Tuile::verifSommePossible(unsigned int somme, unsigned int somme_joueur_actif, TableauJouee tab) const{
    if (somme > somme_joueur_actif)
        return true;
    if (somme + 9 <= somme_joueur_actif)
        return false;
    for (Pos i=8; i>-1; i--)
        for (Pos j=0; j<6; j++)
        if (!tab[j][i]) {
            // Il reste au moins une carte jouable pour compléter les cartes de la même suite
            if (somme + i + 1 > somme_joueur_actif)
                return true;
            else
                return false;
        }
    return false;
}

bool Tuile::verifRevendiquableNonPleine(NumJoueur num_joueur, NumJoueur autre_joueur, Combinaison combinaison, TableauJouee tab) const{
    unsigned int nb_cartes_posees_autre_joueur = cartes_posees[(int) autre_joueur].size();
    unsigned int somme_joueur_actif = getSomme(num_joueur);
    if (nb_cartes_posees_autre_joueur == 0){
        // il faudrait faire de nombreuses vérifications, on ne l'a pas fait par manque de temps
        return false;
    }
    if (nb_cartes_posees_autre_joueur == 1){
        // Il faudrait faire de nombreuses vérifications, on ne l'a pas fait par manque de temps
        return false;
    }
    if (nb_cartes_posees_autre_joueur == 2){
        auto premiereCarte = dynamic_cast<const CarteNormale*>(cartes_posees[(int) autre_joueur][0]);
        if (premiereCarte == nullptr) {
            // La première carte n'est pas une CarteNormale, impossible pour cette variante
            throw PartieException("Erreur, la carte devrait être une carte Normale");
        }
        auto deuxiemeCarte = dynamic_cast<const CarteNormale*>(cartes_posees[(int) autre_joueur][1]);
        if (deuxiemeCarte == nullptr) {
            // La première carte n'est pas une CarteNormale, impossible pour cette variante
            throw PartieException("Erreur, la carte devrait être une carte Normale");
        }
        auto premiereCarteJoueurActif = dynamic_cast<const CarteNormale*>(cartes_posees[(int) num_joueur][0]);
        if (premiereCarteJoueurActif == nullptr) {
            // La première carte n'est pas une CarteNormale, impossible pour cette variante
            throw PartieException("Erreur, la carte devrait être une carte Normale");
        }


        bool meme_couleur = premiereCarte->getCouleur() == deuxiemeCarte->getCouleur();
        bool meme_force = premiereCarte->getForce() == deuxiemeCarte->getForce();
        bool suite = ((int)premiereCarte->getForce() == (int)deuxiemeCarte->getForce() + 1) || ((int) premiereCarte->getForce() + 1 == (int)deuxiemeCarte->getForce());
        suite = suite || ((int)premiereCarte->getForce() == (int)deuxiemeCarte->getForce() + 2) || ((int) premiereCarte->getForce() + 2 == (int)deuxiemeCarte->getForce());
        int force_max = max((int)premiereCarte->getForce(), (int)deuxiemeCarte->getForce());
        int force_min = min((int)premiereCarte->getForce(), (int)deuxiemeCarte->getForce());
        auto somme = (unsigned int)premiereCarte->getForce() + (unsigned int)deuxiemeCarte->getForce();
        int couleur_carte_a_jouer = (int) premiereCarte->getCouleur();
        int force_carte_a_jouer = (int) premiereCarte->getForce();

        if (combinaison == Combinaison::suite_couleur){
            // si l'autre joueur peut faire une meilleure suite couleur, il l'emporte
            return !verifSuiteCouleurPossible(true, meme_couleur, suite, force_max, force_min, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
        }
        if (combinaison == Combinaison::brelan) {
            // si l'autre joueur peut faire une suite couleur ou un meilleur brelan, il l'emporte
            bool suiteCouleurPossible = verifSuiteCouleurPossible(false, meme_couleur, suite, force_max, force_min, somme,
                                                                  somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool brelanPossible = verifBrelanPossible(true, meme_force, force_carte_a_jouer, somme_joueur_actif, tab);
            return !(suiteCouleurPossible || brelanPossible);
        }
        if (combinaison == Combinaison::couleur){
            // si l'autre joueur peut faire une suite couleur ou un brelan ou une meilleure combinaison couleur, il l'emporte
            bool suiteCouleurPossible = verifSuiteCouleurPossible(false, meme_couleur, suite, force_max, force_min, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool brelanPossible = verifBrelanPossible(false, meme_force, force_carte_a_jouer, somme_joueur_actif, tab);
            bool memeCouleurPossible = verifMemeCouleurPossible(true, meme_couleur, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            return !(suiteCouleurPossible || brelanPossible || memeCouleurPossible);
        }
        if (combinaison == Combinaison::suite){
            // si l'autre joueur peut faire une suite couleur ou un brelan ou une combinaison couleur ou une meilleure suite, il l'emporte
            bool suiteCouleurPossible = verifSuiteCouleurPossible(false,meme_couleur, suite, force_max, force_min, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool brelanPossible = verifBrelanPossible(false, meme_force, force_carte_a_jouer, somme_joueur_actif, tab);
            bool memeCouleurPossible = verifMemeCouleurPossible(false, meme_couleur, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool suitePossible = verifSuitePossible(true, suite, force_max, force_min, somme, somme_joueur_actif, tab);
            return !(suiteCouleurPossible || brelanPossible || memeCouleurPossible || suitePossible);
        }
        if (combinaison == Combinaison::somme){
            // si l'autre joueur peut faire une meilleure combinaison que somme ou obtenir une meilleure somme, il l'emporte
            bool suiteCouleurPossible = verifSuiteCouleurPossible(false, meme_couleur, suite, force_max, force_min, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool brelanPossible = verifBrelanPossible(false, meme_force, force_carte_a_jouer, somme_joueur_actif, tab);
            bool memeCouleurPossible = verifMemeCouleurPossible(false, meme_couleur, somme, somme_joueur_actif, couleur_carte_a_jouer, tab);
            bool suitePossible = verifSuitePossible(false, suite, force_max, force_min, somme, somme_joueur_actif, tab);
            bool sommePossible = verifSommePossible(somme, somme_joueur_actif, tab);
            return !(suiteCouleurPossible || brelanPossible || memeCouleurPossible || suitePossible || sommePossible);
        }
    }

    return false;
}

// Méthode permettant de vérifier qu'un joueur peut revendiquer une tuile
bool Tuile::verifRevendiquable(NumJoueur num_joueur, TableauJouee tab) {
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
    if (cartes_posees[(int) autre_joueur].size() != nb_pleine) {
        return verifRevendiquableNonPleine(num_joueur, autre_joueur, joueur1, tab);
    } else{
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
}

Combinaison Tuile::determinerCombinaison(NumJoueur num_joueur){
    if (verifMemeCouleur(num_joueur))
        if (verifSuite(num_joueur))
            // meilleure combinaison possible (trois cartes de la même couleur qui se suivent
            return Combinaison::suite_couleur;
        else
            // trois cartes de la même couleur, troisième meilleure combinaison
            return Combinaison::couleur;
    else if (verifMemeForce(num_joueur))
        // trois cartes de la même force, deuxième meilleure combinaison
        return Combinaison::brelan;
    else if (verifSuite(num_joueur))
        // trois cartes qui se suivent, mais pas de la même couleur, quatrième meilleure combinaison
        return Combinaison::suite;
    else
        // moins bonne combinaison, la somme
        return Combinaison::somme;
}

// Méthode permettant de revendiquer une tuile pour un joueur
void Tuile::revendiquer(NumJoueur num_joueur){
    if (num_joueur == NumJoueur::joueur1)
        revendiquee = TuileRevendiquee::revendiquee_joueur1;
    else
        revendiquee = TuileRevendiquee::revendiquee_joueur2;
}