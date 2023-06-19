#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <QTextEdit>
#include <iostream>
#include <streambuf>

#include "qt_PremiereNormale.h"
#include "h/partie.h"
#include "h/schotten_totten.h"


using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create the main window widget
    QWidget window;

    // Create the output label
    QLabel label;
    label.setFixedSize(1500, 500);
    QLabelStreamBuf streamBuf(&label);
    // Create the input text field
    QLineEdit lineEdit;
    InputStreamBuf inBuf(&lineEdit);

    QPushButton button_start("START");
    QPushButton button_submit("Submit");

    // Redirect stdout to the text field
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::cout.rdbuf(&streamBuf);
    // Redirect stdin to the input text field
    std::streambuf* oldCinBuf = std::cin.rdbuf();
    std::cin.rdbuf(&inBuf);

    // Set the layout for the window
    QVBoxLayout layout;
    layout.addWidget(&label);
    layout.addWidget(&lineEdit);
    layout.addWidget(&button_start);
    layout.addWidget(&button_submit);

    window.setLayout(&layout);

    QObject::connect(&button_start, &QPushButton::clicked, [&]() {

        while(true){
            cout<<"hello\n";
            string s;
            cin >> s;
            cout << s;
        }

    // try{
    //     Jeu j;
    //     j.commencerJeu();  // choix des noms des joueurs
    //     bool continuer = true;
    //     while (continuer){  // tant que le joueur souhaite faire une partie
    //         Edition edition = choixEdition();  // choix de l'édition du jeu
    //         Variante variante = choixVariante();  // choix de la variante du jeu
    //         j.jouerPartie(edition, variante);

    //         string choix;  // on demande à l'utilisateur s'il souhaite jouer une autre partie
    //         do{
    //             cout << "Jouer une autre partie ? (oui/non)";
    //             cin >> choix;
    //             size_t i = 0;
    //             for (auto &c: choix)
    //                 choix[i++] = (char) tolower(c);
    //         } while (choix != "oui" && choix != "non");
    //         continuer = choix == "oui";
    //     }
    //     j.finirJeu();
    // }catch (PartieException &e){
    //     cout << "Erreur : " << e.getInfo() << "\n";
    // }

    });

    // Show the window
    window.show();

    // Run the application event loop
    int result = app.exec();

    // Restore the original stdout stream buffer
    std::cout.rdbuf(oldCoutBuf);
    // Restore the original stdin stream buffer
    std::cin.rdbuf(oldCinBuf);


    return result;
}



