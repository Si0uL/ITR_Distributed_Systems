# Systèmes distribués

Tous les scripts s'utilisent de la même manière:
```
./script_name.out [machine_idx] [n_machines]
```
On veut donc lancer `n_machines` scripts dans autant de terminaux séparés.

## Star

Premier modèle en étoile. Si l'on a `n` machines, la machine `0` est maître, les `n-1` autres sont des esclaves. Il existe un lien entre le maître et chaque esclave. Le maître récupère les `n-1` valeurs des esclaves, puis calcule le min et le renvoit.

### Star1

C'est le modèle le plus simple: le maître se met en attente de la machine `1`, puis de la machine `2`, ...

Limites: l'ordre d'éxécution est donc fixé: `0`, puis `1`, ... puis `n-1`.

### Star 2

Ici, c'est les maître qui demande aux machines leur valeurs à tour de rôle. Les esclaves sont tous en attente.

Limites: l'ordre d'éxécution est plus souple, mais le maître doit être lancé quand tous les esclaves le sont.

### Star3

Ici, on reprend l'architecture `star2`, en améliorant le maître: si il ne peut pas se connecter à un esclave, il attend (1 sec), puis réessaye. L'ordre d'éxécution des machines est donc libre.

Limites: le maître parcourt les machines par ordre d'indice. Ainsi, si la première n'est pas prête, toutes les autres doivent donc l'attendre.


## Ring

Ici, on implémente une structure en anneau: chaque machine est connectée à deux voisins précédent et suivant, et possède une valeur.

### Ring1

Ici, toutes les machines, sauf la `0` commencent par attendre. La machine `0` envoit sa valeur à la machine `1` dès que celle-ci est prête, puis se met en attente. Toutes les suivantes agissent ensuite de la même façon: elles actualisent le minimum propagé avec leur valeur, puis le transmettent à leur voisin dès qu'il est prêt. La dernière machine le renvoit à la machine `0` qui l'affiche, c'est le minimum global car on a bien parcouru toutes les machines.

Remarque: on aurait pu rajouter un 2e tour de l'anneau pour propager le résultat à toutes les machines si besoin est.

Limites: on a un code légèrement différent d'une machine à l'autre, car la `0` a encore un rôle particulier.
On pourrait changer celà en envoyant deux entiers au lieu d'un: le minimum temporaire, et le nombre de machines consultées pour ce minimum. Chaque machine commence alors par tenter un envoi, sinon se met en écoute. Dès que l'on reçoit une valeur avec un nombre de machines consultées égal à `n`, c'est le minimum global.
