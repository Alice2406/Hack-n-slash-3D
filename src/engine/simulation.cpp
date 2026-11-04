#include "cpe/simulation.hpp"
#include <utility>
#include <iostream>

namespace cpe {
Simulation::Simulation(Level source) : level(std::move(source)) { Reset(); }

void Simulation::SetEnemy(Enemy& enemy)
{
    if (enemy.kind == EnemyKind::Light || enemy.kind == EnemyKind::Heavy)
    {
        enemy.hp = 30;
        enemy.damage = 10;
        enemy.detectRange = 8.0;
        enemy.loseRange = 10.0;
        enemy.attackRange = 0.65;
        enemy.windupDuration = 0.5;
        enemy.recoverDuration = 0.8;
        enemy.speed = 1.5;
    }
}

void Simulation::Reset() {
    player = Player{};
    player.position = level.playerSpawn;
    enemies.clear();
    elapsed = 0;
    for (const auto& spawn : level.spawns) {
        Enemy enemy;
        enemy.position = spawn.position;
        enemy.kind = spawn.kind;
        SetEnemy(enemy);
        enemies.push_back(enemy);
    }
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
