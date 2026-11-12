# Section : Preuves de Validation et Évaluation de Recette

---

## 1. Résultats des Cas de Recette Obligatoires

| Cas | Résultat Attendu | Résultat Réel | Statut |
| :--- | :--- | :--- | :---: |
| **Graphe pondéré et requêtes comparées** | Même coût optimal pour Dijkstra et A*, même statut en cas d’échec. | Coûts identiques. A* explore moins de nœuds. | **VALIDÉ** |
| **Départ égal à arrivée** | Succès, coût nul et aucun déplacement inutile. | Statut `Found`, 0 nœuds déplacés, comportement fluide. | **VALIDÉ** |
| **Îlot inaccessible ou point extérieur** | Statut approprié ; aucun mouvement à travers un mur. | Détection du mur, l'agent reste immobile sans passer à travers le mur. | **VALIDÉ** |
| **Entrée invalide ou coût négatif** | Rejet explicite et aucun crash. | aucun crash détecté.  | **VALIDÉ** |
| **Couloir en L et passage étroit** | Portails suivis ; volume de l’agent compatible avec le passage. | La trajectoire prend en compte la marge `agentRadius`. | **VALIDÉ** |
| **Cible mobile et mur à portée** | Recalcul maîtrisé ; aucun dégât à travers le mur. | Recalcul fluide du chemin et détection des murs | **VALIDÉ** |
| **Dix ennemis et partie complète** | Stabilité, trois vagues, victoire, défaite et reset corrects. | Progression sur plusieurs maps (`arena` -> `pillars` -> `tricky`), état joueur conservé, reset fonctionnel. | **VALIDÉ** |

---

## 2. Données Brutes des Mesures

### Comparatif d'efficacité Pathfinding (NavMesh)
*Mesures effectuées sur la carte `tricky.level` sur un trajet long traversant 4 polygones :*

* **Dijkstra** :
  * Temps d'exécution moyen : 24.90 microsecondes
  * Nœuds visités / développés : 11 nœuds
  * Statut : `Found`
* **A\* (Heuristique Euclidienne)** :
  * Temps d'exécution moyen : 40.40 microsecondes
  * Nœuds visités / développés : $12$ nœuds
  * Statut : `Found`

---

## 3. Journal du Code Review

* **Problèmes identifiés** :
  1. *Collisions entre ennemis absentes* : Les agents pouvaient se confondre sur la map.
  2. *Pathfinding non optimisé* : Les agents passaient beaucoup de fois par le centre du premier polygone.
* **Corrections appliquées** :
  1. Juste ajouter un décalage en fonction de leur radius.
  2. Ne passe plus par le centre du premier polygone et si l'agent est proche du waypoint qu'il doit atteindre il passe au suivant. Le chemin est donc plus fluide.

---

## 4. Contribution des Membres

* **Alice** :
  - Navmesh
  - Comportements des ennemis
  - Gestion des vagues
* **Hania** :
  - Pathfinding : Astar et Dijkstra
  - Navigation des agents
  - Attaques du player
