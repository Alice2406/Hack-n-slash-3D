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
        if (distance <= enemy.attackRange && level.navmesh.SegmentInside(enemy.position, player.position)) {
            enemy.state = State::Windup;
            enemy.timer = enemy.windupDuration;
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























//if (enemy.kind == EnemyKind::Light)
//{
//    enemy.hp = 20;
//    enemy.damage = 8;
//    enemy.speed = 3.5;
//    enemy.radius = 0.15;
//    enemy.detectRange = 8.0;
//    enemy.loseRange = 12.0;
//    enemy.attackRange = 4.0;
//    enemy.windupDuration = 0.2;
//    enemy.recoverDuration = 0.4;
//}
//else if (enemy.kind == EnemyKind::Heavy)
//{
//    enemy.hp = 80;
//    enemy.damage = 25;
//    enemy.speed = 1.2;
//    enemy.radius = 0.35;
//    enemy.detectRange = 6.0;
//    enemy.loseRange = 10.0;
//    enemy.attackRange = 1.0;
//    enemy.windupDuration = 1.0;
//    enemy.recoverDuration = 0.8;
//}
//enemy.state = State::Idle;


//if (distance < 2.5 && enemy.kind == EnemyKind::Light) {
//    Vec2 awayDir = Normalized(enemy.position - player.position);
//    Vec2 fleeTarget = enemy.position + awayDir * 3.0;
//    enemy.navigator.Plan(level, enemy.position, fleeTarget, algorithm);
//    enemy.position = enemy.navigator.Follow(level, enemy.position, enemy.speed, dt);
//}