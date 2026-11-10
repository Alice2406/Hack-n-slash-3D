#include "cpe/simulation.hpp"

namespace cpe 
{
    void Simulation::UpdateGameplay(bool actionPressed, double dt) 
    {
        const double attackRange = 1.5;
        if (actionPressed)
        {
            for (int i = 0; i < enemies.size(); i++)
            {
                if (enemies[i].hp <= 0)
                    continue ;

                double distance = Distance(player.position, enemies[i].position);
                if (distance < attackRange)
                {
                    enemies[i].hp -= 10;
                    if (enemies[i].hp < 0)
                    {
                        enemies[i].hp = 0;
                        enemies[i].state = State::Dead;
                        continue ;
                    }
                }
            }
        }
    }
}
