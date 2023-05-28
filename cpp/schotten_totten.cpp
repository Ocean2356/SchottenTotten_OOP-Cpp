#include "schotten_totten.h"

std::initializer_list<Edition> Editions = { Edition::Premiere, Edition::Deuxieme };
std::initializer_list<Variante> Variantes = { Variante::Normale, Variante::Tactique, Variante::Experts };

string toString(Edition e){
    switch (e) {
        case Edition::Premiere: return "Premiere";
        case Edition::Deuxieme: return "Deuxieme";
        default: throw PartieException("Edition inconnue");
    }
}

string toString(Variante v){
    switch (v) {
        case Variante::Normale: return "Normale";
        case Variante::Tactique: return "Tactique";
        case Variante::Experts: return "Experts";
        default: throw PartieException("Variante inconnue");
    }
}


Edition choixEdition(){
    cout<<"Choix de l'edition: ";
    unsigned int i=0;
    for (auto e : Editions){
        i++;
        cout<<"Entrez "<<i<<" pour jouer a l'edition "<<toString(e)<<"\n";
    }
    unsigned int choix;
    do{
        cout<<"Votre choix: ";
        cin>>choix;
    }while(choix<1 || choix>i);
    return (Edition) (choix - 1);
}

Variante choixVariante(){
    cout<<"Choix de la variante: ";
    unsigned int i=0;
    for (auto v : Variantes){
        i++;
        cout<<"Entrez "<<i<<" pour jouer a la variante "<<toString(v)<<"\n";
    }
    unsigned int choix;
    do{
        cout<<"Votre choix: ";
        cin>>choix;
    }while(choix<1 || choix>i);
    return (Variante) (choix - 1);
}

void Jeu::commencer_jeu(size_t taille_main) {
    array <string, 2> nom_preced;
    for (size_t i=0; i<joueurs.size();i++){
        cout<<"Entrez le nom du joueur "<<i+1<<": ";
        string nom;
        cin>>nom;
        for (auto n: nom_preced)
            if (n == nom)
                cout << "Nom deja utilise\n";
        nom_preced[i] = nom;
        joueurs[i] = Joueur(nom, taille_main);
    }
}

Ordre Jeu::determinerOrdre(){
    Ordre ordre;
    cout << "Choix de l'ordre des joueurs\n";
    cout << "Rappel des noms des joueurs\n";
    for (size_t i=0; i<joueurs.size(); i++)
        cout << "Entrez " << i+1 << " pour que le joueur " << joueurs[i].getNom() << " commence\n";

    int choix;
    do{
        cout<<"Votre choix : ";
        cin>>choix;
    }while (choix < 1 || choix >joueurs.size());
    ordre[0] = &joueurs[choix-1];
    for (size_t i=0; i<joueurs.size(); i++)
        if (choix != i+1)
            ordre[choix-1+i] = &joueurs[i];
    return ordre;
}


void Jeu::jouerPartie(Edition edition, Variante variante)
{
    // abstract factory pattern

    AbstractEdition* abstractEdition = editionProducer.getEdition(edition);

    Partie* partie = abstractEdition->getPartie(variante);
  
    Ordre ordre = determinerOrdre();

    partie->commencer(ordre);

    while (!partie->testerFin())
        partie->jouerTour();

    Resultat resultat = partie->terminer();
    traiterResultat(ordre, resultat);
}

void Jeu::traiterResultat(Ordre ordre, Resultat resultat){
    size_t joueur_gagnant = 0;
    size_t max_points = resultat[0];
    for (size_t i = 0; i<resultat.size(); i++) {
        ordre[i]->setScore(resultat[i]);
        if (max_points < resultat[i]) {
            max_points = resultat[i];
            joueur_gagnant = i;
        }
    }
    cout << "Le joueur " << ordre[joueur_gagnant]->getNom() << "gagne et remporte " << resultat[joueur_gagnant]
        <<"points !\n";
    cout << "Affichage des scores :";
    for (size_t i = 0; i<resultat.size(); i++) {
        cout << "Nouveau score du joueur " << ordre[joueur_gagnant]->getNom()
            << " = " << ordre[joueur_gagnant]->getScore() <<"\n";
    }

}

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

Partie* AbstractEdition::getPartie(Variante variante) {
    // Ajouté pour que cela compile, à voir ce que l'on en fait
    switch (variante) {
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


Partie* DeuxiemeFactory::getPartie(Variante variante) {
    switch (variante) {
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

Partie* PremiereFactory::getPartie(Variante variante) {
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