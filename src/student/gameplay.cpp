#include "cpe/simulation.hpp"


namespace cpe {

    void Simulation::SpawnWave(int waveNumber) {
        enemies.clear();

        int enemyCount = 0;
        if (waveNumber == 1) enemyCount = 3;
        else if (waveNumber == 2) enemyCount = 6;
        else if (waveNumber == 3) enemyCount = 10;

        for (int i = 0; i < enemyCount; ++i) {
            Enemy e;

            Vec2 randomPos;
            bool validPosition = false;

            double width = level.bounds.max.x - level.bounds.min.x;
            double height = level.bounds.max.z - level.bounds.min.z;

            while (!validPosition) {
                double rx = level.bounds.min.x + (static_cast<double>(rand()) / RAND_MAX) * width;
                double rz = level.bounds.min.z + (static_cast<double>(rand()) / RAND_MAX) * height;
                randomPos = Vec2{ rx, rz };

                if (level.navmesh.Locate(randomPos) >= 0 && level.CanStand(randomPos, level.agentRadius)) {
                    validPosition = true;
                }
            }

            if (!validPosition) {
                randomPos = level.playerSpawn;
            }

            e.position = randomPos;

            if (waveNumber >= 2 && rand() % 2 == 0) {
                e.kind = EnemyKind::Heavy;
            }
            else {
                e.kind = EnemyKind::Light;
            }

            SetEnemy(e);
            enemies.push_back(e);
        }
    }

void Simulation::UpdateGameplay(bool actionPressed, double dt) {
    if (player.hp <= 0) return;

    const double attackRange = 1.5;
    if (actionPressed)
    {
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].hp <= 0)
                continue;

            double distance = Distance(player.position, enemies[i].position);
            if (distance < attackRange)
            {
                enemies[i].hp -= 10;
                if (enemies[i].hp < 0)
                {
                    enemies[i].hp = 0;
                    enemies[i].state = State::Dead;
                    continue;
                }
            }
        }
    }

    bool allDead = true;
    for (const auto& enemy : enemies) {
        if (enemy.state != State::Dead) {
            allDead = false;
            break;
        }
    }

    if (allDead && !enemies.empty()) {
        currentWave++;
        if (currentWave <= 3)
            SpawnWave(currentWave);
        else
            return;
    }
}
}

//dessiner la map 
//audio pour les ennemis