#include "cpe/simulation.hpp"

namespace cpe 
{
    void Navigator::Plan(const Level& level, Vec2 position,
                         Vec2 target, Algorithm algorithm) 
    {

        if (!Finite(position) || !Finite(target))
        {
            path = {};
            next = 0;
            hasTarget = false;
            mustRecalculate = false;

            return;
        }
        // TODO J4 : gérer la requête, sa temporisation et les échecs.
        //(void)level;
        //(void)position;
        //(void)target;
        //(void)algorithm;
    }
    Vec2 Navigator::Follow(const Level& level, Vec2 position, double speed, double dt) {
        // TODO J4 : consommer les waypoints et appliquer un déplacement valide.
        (void)level;
        (void)speed;
        (void)dt;
        return position;
    }
}
