#include "cpe/simulation.hpp"

namespace cpe {
void Navigator::Tick(double dt)
{
    requestCooldown = std::max(0.0, requestCooldown - dt);
}

void Navigator::Clear()
{
    path = Path{};
    next = 0;
    hasTarget = false;
}

void Navigator::Plan(const Level& level, Vec2 position,
                     Vec2 target, Algorithm algorithm) {
    // TODO J4 : gérer la requête, sa temporisation et les échecs.
    (void)level;
    (void)position;
    (void)target;
    (void)algorithm;
}
Vec2 Navigator::Follow(const Level& level, Vec2 position, double speed, double dt) {
    // TODO J4 : consommer les waypoints et appliquer un déplacement valide.
    (void)level;
    (void)speed;
    (void)dt;
    return position;
}
}
