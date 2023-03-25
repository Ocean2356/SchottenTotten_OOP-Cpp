# Classes

Carte : représente une carte du jeu, avec un attribut jouée de type booléen indiquant si la carte est placée sur une tuile ou défaussée ou non.

Carte basique : représente une carte Clan du jeu, avec un attribut couleur et un attribut valeur.

Carte Tactique : représente une carte avec une capacité spéciale. Une méthode appliquer(Partie p) qui modifie l’état de la partie selon la capacité de la carte.

Main : représente l’ensemble des cartes dans la main d’un joueur. Une méthode piocher(Pioche p) qui ajoute une carte de la pioche à la main. Une méthode jouer(Carte c, Tuile t) qui retire une carte de la main et la place sur une tuile.

Pioche : représente l’ensemble des cartes à piocher. Une méthode mélanger() qui mélange les cartes de la pioche. Une méthode estVide() qui renvoie un booléen indiquant si la pioche est vide ou non.

Tuile : représente une tuile où les joueurs placent les cartes pour la revendiquer. Une méthode estPleine() qui renvoie un booléen indiquant si la tuile a trois cartes de chaque côté ou non. Une méthode estRevendiquée() qui renvoie un booléen indiquant si la tuile a été revendiquée par un joueur ou non. Une méthode revendiquer(Joueur j) qui attribue la tuile au joueur j. Une méthode comparer() qui renvoie un entier négatif, nul ou positif selon que les cartes du joueur A sont plus faibles, égales ou plus fortes que les cartes du joueur B.

Frontière : représente l’ensemble des tuiles pour lesquelles les joueurs s’affrontent. Une méthode estFinie() qui renvoie un booléen indiquant si la frontière a satisfait la condition d'arrêt ou non. Une méthode gagnant() qui renvoie le joueur qui a revendiqué 5 tuiles ou 3 tuiles adjacentes.

Agent : représente un joueur qui joue le jeu. Une méthode choisirCarte(Main m, Frontière f) qui renvoie une carte à jouer parmi celles de la main m en fonction de l’état de la frontière f.

Partie : représente l’ensemble des éléments du jeu, donc les cartes, la frontière, et les deux joueurs. Un attribut édition qui est une chaîne de caractères indiquant l’édition du jeu (classique ou avec cartes tactiques). Un attribut revendiquerAutomatique qui est un booléen indiquant si une borne est revendiquée automatiquement dès que les conditions sont satisfaites. Une méthode commencer() qui initialise les éléments du jeu (mélange et distribution des cartes, placement des tuiles). Une méthode jouerTour() qui fait jouer le joueur courant en lui demandant de choisir une carte et une tuile, puis vérifie si il peut revendiquer une ou plusieurs tuiles. Une méthode testerFin() qui vérifie si la partie est terminée et affiche le résultat.

Joueur : représente une personne ou une IA en dehors d’une partie. Un attribut nom qui est une chaîne de caractères. Un attribut score qui est un entier.

Jeu : représente l’ensemble de 2 joueurs et plusieurs parties. Il est possible de jouer chaque partie dans une édition ou une variante différente, et accumuler le score.
