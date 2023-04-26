#include "partie.h"

PremiereNormale::PremiereNormale()
{
    initierPiocheNormale();
}

void PremiereNormale::commencer(Ordre ordre)
{
    initierMains();
    initierAgents(ordre);
}

void PremiereNormale::jouerTour()
{
    Agent &agent = agents[agentActive];
    Movement movement = agent.jouer(frontiere);
    CarteNormale carte;
    if(piocheNormale.piocher(carte));
        agent.piocher(carte);
    agentSuivant();
}

void PremiereNormale::initierPiocheNormale()
{
    for (int i = 0; i < NCOULEUR; ++i)
        for (int j = 0; j < NFORCE; ++j)
            piocheNormale[i * NCOULEUR + j] = *new CarteNormale((Couleur)i, (Force)j + FORCEMIN);
}

void PremiereNormale::initierMains()
{
    for (auto &agent : agents)
        for (int i = 0; i < NMAIN; ++i){
            CarteNormale carte;
            if(piocheNormale.piocher(carte));
                agent.piocher(carte);
        }
}

void PremiereNormale::agentSuivant()
{
    agentActive++;
    agentActive %= 2;
}