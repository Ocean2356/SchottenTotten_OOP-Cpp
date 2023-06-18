/***********************************************************************************************************************
Nom : user_interface.cpp
Auteurs : Ines Abbache, Sehrish Shakeel, Haiyang Ma et Eliott Sebbagh
Description : Fichier cpp utilisé dans le cadre du projet P23 de l'UV LO21. Il contient des méthodes déclarées mais
non définies dans partie.h et tactique.h pour les classe UI et UITactique
***********************************************************************************************************************/

#include "../h/partie.h"
#include "../h/Tactique.h"


void UI::afficherFrontiere(const Frontiere<class Tuile>& f) const{
    Pos nb_tuile = (char) f.nb_tuile;
    cout
            << "\n----------------------------------------------------------------Affichage de la frontiere----------------------------------------------------------------\n";
    // Affichage des cartes des tuiles du côté du premier joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 0);
    cout << "\n";

    // Affichage de l'état des tuiles (le numéro du joueur qui l'a revendiquée ou le numéro de la tuile si elle n'est pas revendiquée)
    for (size_t i = 0; i < nb_tuile; i++)
//        f.tuiles[i].afficherEtatBorne(i + 1);
        afficherEtatBorne(f.tuiles[i], i+1);
    cout << "\n";

    // Affichage des cartes des tuiles du côté du second joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 1);

    cout
            << "\n---------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

// Méthode permettant d'afficher les cartes d'un côté de la tuile
void UI::afficherCote(const Tuile &t, size_t cote) const{
    size_t iter = 1;
    for (auto c: t.cartes_posees[cote]){
        cout << *c;
        iter++;
    }
    while (iter < 5){
        // il y a au maximum 4 cartes d'un côté d'une tuile. Pour centrer l'affichage, on remplit toujours les positions vides
        cout << "    ";
        iter++;
    }
    cout << "|";
}

// Méthode permettant d'afficher l'état d'une tuile
void UI::afficherEtatBorne(const Tuile &t, size_t num_borne) const{
    cout << "      ";  // utilisé pour centrer l'affichage
    if (t.revendiquee == TuileRevendiquee::revendiquee_joueur1)
        cout << "/R1/ ";
    else if (t.revendiquee == TuileRevendiquee::revendiquee_joueur2)
        cout << "/R2/ ";
    else
        cout << "/B" << num_borne << "/ ";
    cout << "     |";  // utilisé pour centrer l'affichage
}

Pos UI::getChoixCarte(Main& main) {
    string recup_choix;
    cout << "Choix de la carte a jouer\n";
    for (Pos i = 0; i < main.getNbCartes(); i++)
        cout << "Entrez " << i + 1 << " pour jouer la carte " << main.getCarte(i) << "\n";
    Pos choix_carte = -1;
    while (choix_carte < 1 || choix_carte > main.getNbCartes()){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((Pos) (recup_choix[0] - '0') >= 1 && (Pos) (recup_choix[0] - '0') <= main.getNbCartes())
            choix_carte = (Pos) (recup_choix[0] - '0');
    }
    --choix_carte;
    return choix_carte;
}

Pos UI::getChoixCarteIa(Main& main) {
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) main.getNbCartes() - 1};
    for (Pos i = 0; i < main.getNbCartes(); i++)
        cout << "Carte " << i + 1 << " : " << main.getCarte(i) << "\n";
    return (char) distrib(random_eng);
}

Pos UI::getChoixBorne(const Frontiere<class Tuile>& f, NumJoueur joueur_num) {
    string recup_choix;
    cout << "Vous pouvez jouer sur les bornes suivantes: \n";
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i+1);
            cout << "Entrez " << i + 1 << " pour jouer sur la borne numero " << i + 1 << "\n";
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    unsigned int choix_borne = 0;
    bool saisie_correcte = false;
    while (!saisie_correcte){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9){
            choix_borne = (unsigned int) (recup_choix[0] - '0');
            for (auto& b : borne_jouable){
                if (b == choix_borne){
                    saisie_correcte = true;
                    break;
                }
            }
        }
        if (!saisie_correcte)
            cout << "Veuillez entrez un numero d'une borne sur laquelle vous pouvez jouer\n";
    }
    return (char) --choix_borne;
}

Pos UI::getChoixBorneIa(const Frontiere<class Tuile>& f, NumJoueur joueur_num) {
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i);
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) borne_jouable.size() - 1};
    return (char) borne_jouable[distrib(random_eng)];
}

Movement UI::getChoixBornesARevendiquer( Frontiere<class Tuile>& f, NumJoueur joueur_num, TableauJouee tab){
    Movement mvt;
    afficherFrontiere(f);

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
            if (f.verifRevendiquable(choix_borne_a_revendiquer - 1, joueur_num, tab)){
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


Movement UI::getChoixBornesARevendiquerIa( Frontiere<class Tuile>& f, NumJoueur joueur_num, TableauJouee tab){
    Movement mvt;
    Pos nb_bornes_a_revendiquer = 0;

    for (Pos i = 0; i<f.getNbTuile(); i++)
        if (f.verifRevendiquable((size_t) i, joueur_num, tab)){
            // Si la tuile est revendiquable par le joueur
            mvt += "Revendiquer:";
            mvt += i;
            nb_bornes_a_revendiquer++;
        }
    mvt += "nb:";
    mvt += (char) nb_bornes_a_revendiquer;
    return mvt;
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
    Pos nb_tuile = (char) f.nb_tuile;
    cout
            << "\n----------------------------------------------------------------Affichage de la frontiere----------------------------------------------------------------\n";
    // Affichage des cartes des tuiles du côté du premier joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 0);
    cout << "\n";

    // Affichage de l'état des tuiles (le numéro du joueur qui l'a revendiquée ou le numéro de la tuile si elle n'est pas revendiquée)
    for (size_t i = 0; i < nb_tuile; i++)
//        f.tuiles[i].afficherEtatBorne(i + 1);
        afficherEtatBorne(f.tuiles[i], i+1);
    cout << "\n";

    // Affichage des cartes des tuiles du côté du second joueur
    for (size_t i = 0; i < nb_tuile; i++)
        afficherCote(f.tuiles[i], 1);

    cout
            << "\n---------------------------------------------------------------------------------------------------------------------------------------------------------\n";
}

void UITactique::afficherCote(const TuileTactique &t, size_t cote) const{
    size_t iter = 1;
    for (auto c: t.cartes_posees[cote]){
        cout << *c;
        iter++;
    }
    while (iter < 5){
        // il y a au maximum 4 cartes d'un côté d'une tuile. Pour centrer l'affichage, on remplit toujours les positions vides
        cout << "    ";
        iter++;
    }
    cout << "|";
}

// Méthode permettant d'afficher l'état d'une tuile
void UITactique::afficherEtatBorne(const TuileTactique &t, size_t num_borne) const{
    if (t.carte_posee_centre != nullptr)
        t.carte_posee_centre->getInfo();
    else
        cout << "      ";  // utilisé pour centrer l'affichage
    if (t.revendiquee == TuileRevendiquee::revendiquee_joueur1)
        cout << "/R1/ ";
    else if (t.revendiquee == TuileRevendiquee::revendiquee_joueur2)
        cout << "/R2/ ";
    else
        cout << "/B" << num_borne << "/ ";
    cout << "     |";  // utilisé pour centrer l'affichage
}


Pos UITactique::getChoixCarte(const Main& main, const vector <int>& cartes_jouables) const{
    string recup_choix;
    cout << "Choix de la carte a jouer\n";
    Pos cpt = 1;
    for (Pos i = 0; i < main.getNbCartes(); i++){
        for (auto j : cartes_jouables){
            if (j == i)
                cout << "Entrez " << cpt++ << " pour jouer la carte " << main.getCarte(i) << "\n";
        }
    }

    Pos choix_carte = -1;
    while (choix_carte < 1 || choix_carte > cpt){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((Pos) (recup_choix[0] - '0') >= 1 && (Pos) (recup_choix[0] - '0') <= cpt)
            choix_carte = (Pos) (recup_choix[0] - '0');
    }
    --choix_carte;
    return (char) cartes_jouables[choix_carte];
}

Pos UITactique::getChoixCarteIa(const Main& main, const vector<int>& cartes_jouables) const{
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) cartes_jouables.size() - 1};
    for (Pos i = 0; i < main.getNbCartes(); i++)
        cout << "Carte " << i + 1 << " : " << main.getCarte(i) << "\n";
    return (char) cartes_jouables[distrib(random_eng)];
}


Pos UITactique::getChoixBorne(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const{
    string recup_choix;
    cout << "Vous pouvez jouer sur les bornes suivantes: \n";
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i+1);
            cout << "Entrez " << i + 1 << " pour jouer sur la borne numero " << i + 1 << "\n";
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    unsigned int choix_borne = 0;
    bool saisie_correcte = false;
    while (!saisie_correcte){
        cout << "Votre Choix : ";
        cin >> recup_choix;
        if ((unsigned int) (recup_choix[0] - '0') >= 1 && (unsigned int) (recup_choix[0] - '0') <= 9){
            choix_borne = (unsigned int) (recup_choix[0] - '0');
            for (auto& b : borne_jouable){
                if (b == choix_borne){
                    saisie_correcte = true;
                    break;
                }
            }
        }
        if (!saisie_correcte)
            cout << "Veuillez entrez un numero d'une borne sur laquelle vous pouvez jouer\n";
    }
    return (char) --choix_borne;
}

Pos UITactique::getChoixBorneIa(const Frontiere<class TuileTactique>& f, NumJoueur joueur_num) const{
    vector <int> borne_jouable;
    for (size_t i = 0; i < f.getNbTuile(); i++){
        if (!f.tuiles[i].cotePlein(joueur_num) && !f.tuiles[i].estRevendiquee()){
            borne_jouable.push_back((int) i);
        }
    }
    if (borne_jouable.empty())
        // on ne peut jouer sur aucune borne (pourrait arriver en variante tactique)
        return -1;
    // Génération d'un nombre aléatoire entre 0 et nbBornesJouables - 1
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib{0, (int) borne_jouable.size() - 1};
    return (char) borne_jouable[distrib(random_eng)];
}

GUI::GUI(QWidget *parent)
    : QMainWindow(parent),
    m_selectedCardIndex(-1),
    m_selectedBorneIndex(-1)
{
    // Set up the main window
    setWindowTitle("Schotten Totten");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create the grid layout
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(10);

    // Create labels for displaying information
    frontiereLabel = new QLabel("Frontiere:", centralWidget);
    coteLabel = new QLabel("Cote:", centralWidget);
    etatBorneLabel = new QLabel("Etat Borne:", centralWidget);

    // Create buttons for card choices
    for (int i = 0; i < 6; i++)
    {
        cardButtons[i] = new QPushButton("Card " + QString::number(i + 1), centralWidget);
        connect(cardButtons[i], SIGNAL(clicked()), this, SLOT(onCardClicked()));
    }

    // Create buttons for borne choices
    for (int i = 0; i < 9; i++)
    {
        borneButtons[i] = new QPushButton("Borne " + QString::number(i + 1), centralWidget);
        connect(borneButtons[i], SIGNAL(clicked()), this, SLOT(onBorneClicked()));
    }

    // Add the widgets to the grid layout
    gridLayout->addWidget(frontiereLabel, 0, 0);
    gridLayout->addWidget(coteLabel, 0, 1);
    gridLayout->addWidget(etatBorneLabel, 0, 2);

    for (int i = 0; i < 6; i++)
    {
        gridLayout->addWidget(cardButtons[i], i + 1, 0);
    }

    for (int i = 0; i < 9; i++)
    {
        gridLayout->addWidget(borneButtons[i], i + 1, 2);
    }
}

void GUI::onCardClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        // Retrieve the card index from the button's text
        QString buttonText = button->text();
        // The button's text is of the form "Card X", where X is the card index
        // We retrieve the index by removing the "Card " prefix and converting the remaining string to an integer
        m_selectedCardIndex = buttonText.mid(5).toInt() - 1;

        // Handle the card selection
        // TODO: Implement the logic for handling the card selection
    }
}

void GUI::onBorneClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        // Retrieve the borne index from the button's text
        QString buttonText = button->text();
        m_selectedBorneIndex = buttonText.mid(6).toInt() - 1;

        // Handle the borne selection
        // TODO: Implement the logic for handling the borne selection
    }
}

Pos GUI::getChoixCarte(Main& main) 
{
    // Create a signal-slot connection to handle the card selection
    QEventLoop loop;
    connect(this, SIGNAL(cardSelected(int)), &loop, SLOT(quit()));

    // Wait for the card selection signal to be emitted
    loop.exec();

    // Retrieve the selected card index
    int selectedCard = m_selectedCardIndex;

    // Clear the selected card index for future selections
    m_selectedCardIndex = -1;

    // Return the chosen position
    return selectedCard;
}

Pos GUI::getChoixBorne(const Frontiere<class Tuile>& f, NumJoueur joueur_num)
{
    // Create a signal-slot connection to handle the borne selection
    QEventLoop loop;
    connect(this, SIGNAL(borneSelected(int)), &loop, SLOT(quit()));

    // Wait for the borne selection signal to be emitted
    loop.exec();

    // Retrieve the selected borne index
    int selectedBorne = m_selectedBorneIndex;

    // Clear the selected borne index for future selections
    m_selectedBorneIndex = -1;

    // Return the chosen position
    return selectedBorne;
}

Movement GUI::getChoixBornesARevendiquer(Frontiere<class Tuile>& frontiere, NumJoueur joueur_num, TableauJouee tab)
{
    // Create a signal-slot connection to handle the borne claims selection
    QEventLoop loop;
    connect(this, SIGNAL(borneClaimsSelected(Movement)), &loop, SLOT(quit()));

    // Wait for the borne claims selection signal to be emitted
    loop.exec();

    // Retrieve the selected borne claims movement
    Movement selectedMovement = m_selectedMovement;

    // Clear the selected borne claims movement for future selections
    m_selectedMovement = Movement();

    // TODO: Implement the logic to process the selected borne claims movement

    // Return the selected movement
    return selectedMovement;
}
