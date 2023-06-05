/***********************************************************************************************************************
Nom : schotten_totten.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient les définitions des fonctions
et méthodes déclarées mais non définies dans schotten_totten.h
***********************************************************************************************************************/

#include "../h/schotten_totten.h"

std::initializer_list<Edition> Editions = { Edition::Premiere, Edition::Deuxieme };  // liste des éditions
std::initializer_list<Variante> Variantes = { Variante::Normale, Variante::Tactique, Variante::Experts };  // liste des variantes

// Fonction permettant de retourner une string correspondant à une édition donnée
string toString(Edition e){
    switch (e) {
        case Edition::Premiere: return "Premiere";
        case Edition::Deuxieme: return "Deuxieme";
        default: throw PartieException("Edition inconnue");
    }
}

// Fonction permettant de retourner une string correspondant à une variante donnée
string toString(Variante v){
    switch (v) {
        case Variante::Normale: return "Normale";
        case Variante::Tactique: return "Tactique";
        case Variante::Experts: return "Experts";
        default: throw PartieException("Variante inconnue");
    }
}

// Fonction permettant de saisir l'édition pour une partie
Edition choixEdition(){
    cout<<"Choix de l'edition: ";
    unsigned int i=0;
    // Affichage des éditions
    for (auto e : Editions)
        cout<<"Entrez "<<++i<<" pour jouer a l'edition "<<toString(e)<<"\n";

    // Choix de l'édition
    char choix='a';
    while (!(choix >= '1' && (unsigned int) (choix - '0') <= i)) {
        cout << "Votre choix : ";
        cin >> choix;
        if (choix == '2'){  // test à retirer une fois la deuxième édition implémentée
            cout << "Deuxieme edition pas encore implementee, on ne peut jouer qu'a la premiere\n";
            return (Edition) 0;
        }
    }
    return (Edition) ((unsigned int) (choix - '0') - 1);
}

// Fonction permettant de saisir la variante pour une partie
Variante choixVariante(){
    cout<<"Choix de la variante: ";
    unsigned int i=0;
    // Affichage des variantes
    for (auto& v : Variantes)
        cout<<"Entrez "<<++i<<" pour jouer a la variante "<<toString(v)<<"\n";

    // Choix de la variante
    char choix='a';
    while (!(choix >= '1' && (unsigned int) (choix - '0') <= i)) {
        cout << "Votre choix : ";
        cin >> choix;
    }
    return (Variante) ((unsigned int) (choix - '0') - 1);
}

// Méthode appliquant le design pattern abstract factory pour le choix de la variante de la première édition
Partie* PremiereFactory::getPartie(Variante variante) {
    // Retirer les commentaires une fois les variantes tactique et experte implémentée
    switch (variante)
    {
        case Variante::Normale:
            return new PremiereNormale();
//        case Variante::Tactique:
//            return new PremiereTactique();
//        case Variante::Experts:
//            return new PremiereExperts();
        default:
            return nullptr;
    }
}

// Méthode appliquant le design pattern abstract factory pour le choix de la variante de la deuxième édition
Partie* DeuxiemeFactory::getPartie(Variante variante) {
    // Retirer les commentaires lorsque l'on implémentera la deuxième édition
    switch (variante) {
//        case Variante::Normale:
//            return new PremiereNormale();
//        case Variante::Tactique:
//            return new PremiereTactique();
//        case Variante::Experts:
//            return new PremiereExperts();
        default:
            return nullptr;
    }
}

// Méthode appliquant le design pattern abstract factory pour le choix de l'édition
AbstractEdition* EditionProducer::getEdition(Edition edition)
{
    switch (edition)
    {
        case Edition::Premiere:
            return new PremiereFactory();
        case Edition::Deuxieme:
            return new DeuxiemeFactory();
        default:
            return nullptr;
    }
}


// Méthode permettant de saisir les noms des joueurs  // TODO ajouter choix ia/humain pour chaque joueur
void Jeu::commencerJeu() {
    array <string, 2> nom_preced;
    for (size_t i=0; i<joueurs.size();i++){
        string nom;
        bool test_nom = false;
        while (!test_nom){
            test_nom = true;
            cout<<"Entrez le nom du joueur "<<i+1<<": ";
            cin>>nom;
            for (const auto& n: nom_preced)
                if (n == nom){
                    cout << "Nom deja utilise\n";
                    test_nom = false;
                    break;
                }
        }
        nom_preced[i] = nom;
        joueurs[i] = Joueur(nom);
    }
}

// Méthode permettant de déterminer l'ordre de jeu des joueurs qui retourne un ordre (array<Joueurs*, 2>)
Ordre Jeu::determinerOrdre(){
    Ordre ordre;
    cout << "Choix de l'ordre des joueurs\n";
    cout << "Rappel des noms des joueurs\n";
    for (size_t i=0; i<joueurs.size(); i++)
        cout << "Entrez " << i+1 << " pour que le joueur " << joueurs[i].getNom() << " commence\n";

    // Choix de l'ordre de jeu des joueurs
    char choix='a';
    while (!(choix >= '1' && (unsigned int) (choix - '0') <= joueurs.size())) {
        cout << "Votre choix : ";
        cin >> choix;
    }
    auto ordre_joueur = (unsigned int) (choix - '0');
    ordre[0] = &joueurs[ordre_joueur-1];
    for (size_t i=0; i<joueurs.size(); i++)
        if (ordre_joueur != i+1)
            ordre[ordre_joueur-1+i] = &joueurs[i];
    return ordre;
}


// Méthode permettant la gestion d'une partie (choix édition/variante, initialisation, déroulement, fin)
void Jeu::jouerPartie(Edition edition, Variante variante)
{
    // abstract factory pattern pour choisir la partie et la variante
    AbstractEdition* abstractEdition = editionProducer.getEdition(edition);
    Partie* partie = abstractEdition->getPartie(variante);

    // Détermination de l'ordre de jeu des joueurs et initialisations
    Ordre ordre = determinerOrdre();
    partie->commencer(ordre);

    // déroulement de la partie tour par tour jusqu'à ce que la partie soit terminée
    while (!partie->testerFin())
        partie->jouerTour();

    // affichage du résultat de la partie et mise à jour des scores des joueurs
    traiterResultat(ordre, partie->terminer());
}


// Méthode permettant de mettre à jour et d'afficher les points des joueurs et le joueur qui a remporté la partie
void Jeu::traiterResultat(Ordre ordre, Resultat resultat){
    size_t joueur_gagnant = 0;
    size_t max_points = resultat[0];

    // Mise à jour du score des joueurs à partir du résultat
    for (size_t i = 0; i<resultat.size(); i++) {
        ordre[i]->setScore(resultat[i]);
        if (max_points < resultat[i]) {
            max_points = resultat[i];
            joueur_gagnant = i;
        }
    }

    // Affichage des nouveaux scores des joueurs
    cout << "Le joueur " << ordre[joueur_gagnant]->getNom() << "gagne la partie et remporte "
        << resultat[joueur_gagnant] <<"points !\n";
    cout << "Affichage des scores :";
    for (size_t i = 0; i<resultat.size(); i++) {
        cout << "Nouveau score du joueur " << ordre[joueur_gagnant]->getNom()
            << " = " << ordre[joueur_gagnant]->getScore() <<"\n";
    }
}


// Méthode permettant d'afficher les scores des joueurs et le joueur gagnant une fois toutes les parties disputées
void Jeu::finirJeu() const{
    cout << "Fin du jeu\n";
    // Affichage des scores finaux et du joueur gagnant
    size_t joueur_gagnant = 0;
    unsigned int score_max = 0;
    for (size_t i=0; i<getNbJoueurs(); i++){
        unsigned int score = getScore(i);
        cout << "Le joueur " << getNom(i) << "a obtenu " << score << "points.\n";
        if (score_max < score) {
            score_max = score;
            joueur_gagnant = i;
        }
    }
    cout << "Le joueur " << getNom(joueur_gagnant)  << "remporte le jeu !\n";
}