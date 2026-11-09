#include "cpe/simulation.hpp"
#include <utility>
#include <iostream>

namespace cpe {
Simulation::Simulation(Level source) : level(std::move(source)) { Reset(); }

void Simulation::SetEnemy(Enemy& enemy)
{
    if (enemy.kind == EnemyKind::Light)
    {
        enemy.hp = 20;
        enemy.damage = 8;
        enemy.speed = 2.5;
        enemy.radius = 0.15;
        enemy.detectRange = 8.0;
        enemy.loseRange = 9.0;
        enemy.attackRange = 2.5;
        enemy.windupDuration = 0.2;
        enemy.recoverDuration = 1.0;
    }
    else if (enemy.kind == EnemyKind::Heavy)
    {
        enemy.hp = 80;
        enemy.damage = 25;
        enemy.speed = 1.2;
        enemy.radius = 0.35;
        enemy.detectRange = 6.0;
        enemy.loseRange = 10.0;
        enemy.attackRange = 1.0;
        enemy.windupDuration = 1.0;
        enemy.recoverDuration = 1.0;
    }
}

void Simulation::Reset() {
    player = Player{};
    player.position = level.playerSpawn;
    enemies.clear();
    elapsed = 0;
    currentWave = 1;
    SpawnWave(currentWave);

}

void Simulation::SetAlgorithm(Algorithm value) {
    algorithm = value;
    for (auto& enemy : enemies) enemy.navigator.Invalidate();
}

std::size_t Simulation::RequestCount() const {
    std::size_t count = 0;
    for (const auto& enemy : enemies) count += enemy.navigator.requests;
    return count;
}

void Simulation::Update(Vec2 movement, bool actionPressed, double dt) {
    if (!std::isfinite(dt) || dt <= 0) return;
    dt = std::min(dt, 0.05);
    elapsed += dt;

    if (Finite(movement) && Length(movement) > 1e-8) {
        Vec2 direction = Normalized(movement);
        player.facing = direction;
        player.position = level.MoveSliding(
            player.position, direction * (3.5 * dt), level.agentRadius);
    }

    UpdateGameplay(actionPressed, dt);
    for (auto& enemy : enemies) UpdateEnemy(enemy, dt);
}
}
