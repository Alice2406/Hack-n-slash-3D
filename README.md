# Hack'n Slash 3D

## Prérequis & Versions

| Composant | Version |
| :--- | :--- |
| **OS** | Windows 10 (x64) |
| **Visual Studio** | 2026 |
| **Langage** | C++17 |
| **Include/Libs** | raylib |


## Compilation & Exécution

1. Ouvrez la solution `.sln` dans Visual Studio 2026.
2. Choisir **Release | x64**, définir **campus_path_engine** comme projet de démarrage, puis Générer la solution.
3. Exécuter avec Ctrl+F5, ou F5, ou la flèche verte. 

Les sources de raylib sont incluses et compilées par le projet `raylib`. Aucun téléchargement de bibliothèque n’est nécessaire lors de la génération. Le pilote graphique doit prendre en charge OpenGL 3.3.
Les fichiers CMake restent fournis pour la portabilité. Ils ne sont pas nécessaires au parcours `.sln` de la salle. 

## Commandes

| Commande | Action fournie |
|---|---|
| Flèches, ZQSD ou WASD | Déplacer le joueur avec collisions physiques |
| F1 | Afficher les surfaces sources ou le NavMesh construit par votre code |
| F2 | Afficher les chemins produits par votre code |
| F3 | Afficher ou masquer le HUD |
| Tab | Choisir Dijkstra ou A* pour les requêtes |
| Clic gauche sur le sol | Appeler NavMesh::FindPath depuis le joueur |
| Clic droit sur une entité ou N | Sélectionner un marqueur ennemi |
| F5 | Changer de scène |
| R | Réinitialiser les positions de la scène |
| P | Mettre la simulation en pause |
| Espace | Attaque du joueur |
| Échap | Fermer |

## Limites connues

- portail seulement si 2 polygones partagent le même segment, si un segment est plus petit que l'autre, le code ne reconnait pas le portail créée.
- les ennemis peuvent de temps en temps se retrouver dans `OutsideNavMesh`
