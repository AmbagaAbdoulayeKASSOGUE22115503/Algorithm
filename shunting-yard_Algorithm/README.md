Lobjectif de ce tuto est decrire un programme permettant devaluer un ensemble dexpressions arithmetiques fournies sous la forme de  de chaîne de caractères en notation infixe. Les expressions arithmetiques pouvant etre evaluées, sont construites avec les élements suivants :
-Valeurs numeriques réelles ne contenant que des nombres positifs.
-Operateurs binaires +,- ,* ,/ ,^ representant respectivement laddition, la soustraction,
la multiplication et la fonction puissance de deux nombres reels. Ces operateurs respectent les propriétés de priorité et dassociativité telles que définies en arithmetique classique.
-Parentheses permettant de modi er la priorite des operateurs.
-Les elements dune expression peuvent être ou ne pas être séparés par des espaces.Les expression suivantes, contenues dans le fichier de test arithmeticExpression.txt sont des expressions arithmetiques exploitables par le programme que lon souhaite écrire, dont l'évaluation donne des valeurs bien différentes.

On va utiliser les implantations des types abstraits de donnees (Token, Stack, Queue). Les types abstraits Stack et Queue correspondent a l'implantation statique de la pile et dynamique de la file. Ces implantations sont cependant rendues independantes du type d'information stockée, en proposant une gestion de collection de pointeurs non typés (void *).

L'algorithme de Shunting Yard est un algorithme utilisé pour convertir une expression arithmétique en notation infixée en une expression en notation postfixée (ou polonaise inversée), ce qui facilite son évaluation. Voici comment fonctionne l'algorithme :

## 1. Initialisation :
   - On utilise une file d'attente (queue) pour stocker les opérandes et les opérateurs pendant le traitement de l'expression.
   - On utilise une pile (stack) pour stocker les opérateurs temporaires et les parenthèses pendant le traitement.

## 2. Parcours de l'expression :
   * On parcourt l'expression de gauche à droite.
   * À chaque caractère rencontré :
      * Si c'est un opérande (chiffre ou variable), on l'ajoute à la file d'attente.
      * Si c'est un opérateur :
         * Si la pile est vide, ou si le haut de la pile est une parenthèse ou un opérateur de priorité inférieure, on ajoute l'opérateur à la pile.
         * Sinon, tant que le haut de la pile est un opérateur de priorité supérieure ou égale à celui rencontré, on dépile l'opérateur et on l'ajoute à la file d'attente, puis on ajoute le nouvel opérateur à la pile.
      - Si c'est une parenthèse ouvrante, on l'ajoute à la pile.
      - Si c'est une parenthèse fermante, on dépile tous les opérateurs de la pile jusqu'à ce qu'on rencontre la parenthèse ouvrante correspondante. On ajoute ces opérateurs à la file d'attente.

## 3. Fin du parcours :
   - Une fois que toute l'expression est traitée, on dépile tous les opérateurs restants de la pile et on les ajoute à la file d'attente.

## 4. Expression postfixée :
   - La file d'attente résultante contient l'expression postfixée équivalente à l'expression initiale.

## 5. Évaluation de l'expression postfixée :
   - Pour évaluer l'expression postfixée, on parcourt la file d'attente de gauche à droite.
   - Si on rencontre un opérande, on le place sur la pile.
   - Si on rencontre un opérateur, on dépile les deux derniers opérandes de la pile, on applique l'opérateur à ces opérandes, puis on place le résultat sur la pile.
   - Une fois que la file d'attente est parcourue, le résultat final se trouve au sommet de la pile.
