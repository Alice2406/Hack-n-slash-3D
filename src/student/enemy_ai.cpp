#include "cpe/simulation.hpp"
#include <iostream>

namespace cpe {
    void Simulation::UpdateEnemy(Enemy& enemy, double dt) {
        enemy.navigator.Tick(dt);

        if (enemy.hp <= 0)
        {
            enemy.state = State::Dead;
            enemy.navigator.Clear();
            return;
        }
        if (enemy.state == State::Dead)
            return;
        const double distance = Distance(enemy.position, player.position);
        if (player.hp <= 0 || distance > enemy.loseRange)
        {
            enemy.state = State::Idle;
            enemy.navigator.Clear();
            enemy.timer = 0.0;
            return;
        }

        switch (enemy.state) {
        case State::Idle:
            if (distance <= enemy.detectRange) enemy.state = State::Chase;
            break;
        case State::Chase:
            if (enemy.kind == EnemyKind::Light && distance < 3.0)
            {
                Vec2 fleeDir = Normalized(enemy.position - player.position);
                Vec2 newPos = enemy.position + fleeDir * (enemy.speed * dt);

                if (level.navmesh.SegmentInside(enemy.position, newPos)) {
                    enemy.position = newPos;
                }
                enemy.navigator.path.search.status = cpe::Status::NoPath;
            }
            else if (distance <= enemy.attackRange && level.navmesh.SegmentInside(enemy.position, player.position)) {
                enemy.state = State::Windup;
                enemy.timer = enemy.windupDuration;
                if (enemy.kind == EnemyKind::Heavy) {
                    enemy.navigator.Clear();
                }
            }
            else {
                enemy.navigator.Plan(level, enemy.position, player.position, algorithm);
                enemy.position = enemy.navigator.Follow(level, enemy.position, enemy.speed, dt);
            }
            break;
        case State::Windup:
            enemy.timer -= dt;
            if (enemy.timer <= 0) {
                if (distance <= enemy.attackRange && level.navmesh.SegmentInside(enemy.position, player.position))
                    player.hp = std::max(0, player.hp - enemy.damage);
                enemy.state = State::Recover;
                enemy.timer = enemy.recoverDuration;
            }
            break;
        case State::Recover:
            enemy.timer -= dt;
            if (enemy.timer <= 0) enemy.state = State::Chase;
            break;
        case State::Dead: break;
        }
    }
}