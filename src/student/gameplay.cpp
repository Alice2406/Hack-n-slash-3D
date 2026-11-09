//#include "cpe/simulation.hpp"
//
//namespace cpe 
//{
//    void Simulation::UpdateGameplay(bool actionPressed, double dt) 
//    {
//        if (!actionPressed)
//            return;
//
//        const double attackRange = 1.5;
//        int closestEnemy = -1;
//        double closestDistance = attackRange;
//
//        for (int i = 0; i < static_cast<int>(enemies.size()); i++)
//        {
//            if (enemies[i].hp <= 0)
//                continue;
//
//            double distance = Distance(player.position, enemies[i].position);
//
//            if (distance < closestDistance)
//            {
//                closestDistance = distance;
//                closestEnemy = i;
//            }
//        }
//
//        if (closestEnemy != -1)
//        {
//            enemies[closestEnemy].hp -= 10;
//        }
//
//        if (closestEnemy != -1)
//        {
//            enemies[closestEnemy].hp -= 10;
//
//            if (enemies[closestEnemy].hp < 0)
//                enemies[closestEnemy].hp = 0;
//        }
//        // TODO PROJET : attaques, dégâts, vie, mort, vagues, victoire et défaite.
//        // Espace alimente actionPressed ; aucun effet de jeu n'est fourni.
//        (void)actionPressed;
//        (void)dt;
//    }
//}
