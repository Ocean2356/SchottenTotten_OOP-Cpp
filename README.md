# Schotten Totten

## Liens

Rapport : <https://docs.google.com/document/d/1Q0RfS8SuFi3RkuXBjJI0D8JvEX-5lzekX-ZBeGrtu4k/>

Répertoire du projet : <https://github.com/Ocean2356/SchottenTotten_OOP-Cpp>

Répartition du temps : <https://lite.framacalc.org/projet_lo21-a009>

Protocole du message de commit : <https://gitmoji.dev/>

## Présentation du rendue 3

Le rendu 3 est un prototype du jeu Schotten Totten. Il permet de jouer la variante normale de la 1ère édition en mode console contre un joueur humain. Il n'y a pas de joueur artificiel.

Après le débogage, le rendu 3 est fonctionnel. Il est possible de jouer une partie complète et de gagner ou perdre.

Toute fois, certains points restent à améliorer :

- le code n'est pas très propre, il y a des répétitions et des fonctions trop longues ;
- le code n'est pas très modulaire, il est difficile de rajouter des fonctionnalités ;
- il n'y a pas de joueur artificiel ;
- la revandication d'une tuile lorsque l'autre côté n'est pas rempli n'est pas terminée ;

Dans le rendu 4, nous allons avancer dans ces points suivant :

- améliorer la propreté du code : transformer les répétitions dans appels des fonctions, découper les fonctions trop longues ;
- renforcer la modularité du code : isoler la vue et le modèle pour faciliter l'implémentation de la vue graphique ;
- implémenter les fonctionnalités manquantes : joueur artificiel, revandication d'une tuile lorsque l'autre côté n'est pas rempli ;
- implémenter la vue graphique ;
- implémenter la variante tactique ;

## Tutoriel pour Git

Un fichier README a été ajouté dans la branche principale (Main), ce qui représente une avancée par rapport aux autres branches. La procédure de travail pour Haiyang est alors :

- sur son ordinateur local, il bascule sur la branche principale (Checkout Main), il récupère les dernières modifications (Pull de Main distant vers Main local), il bascule sur sa branche personnelle (Checkout Haiyang), il fusionne les modifications de la branche principale avec sa branche personnelle (Pull Rebase de Main local vers Haiyang), les travaux des autres sur la branche principale sont alors synchronisés localement ;

- il travaille, il enregistre ses modifications (Commit), il envoie ses modifications sur le serveur distant (Push de Haiyang local vers Haiyang distant) ;

- puis sur le serveur distant, il demande à fusionner sa branche personnelle avec la branche principale (Pull Request de Haiyang distant vers Main distant) et il accepte la fusion (Merge), les travaux de Haiyang sont alors synchronisés sur le serveur.
