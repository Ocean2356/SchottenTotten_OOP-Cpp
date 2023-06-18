#include "../h/partie.h"
#include "../h/schotten_totten.h"

int main(){
    PremiereNormale p = PremiereNormale();
    Joueur j1("Ines", 0);
    Joueur j2("Sehrish", 0);
    Ordre o = {&j1, &j2};
    p.commencer(o);
    while(!p.testerFin()){
        p.jouerTour();
    }
}