#include "cpe/simulation.hpp"
#include "cpe/level.hpp"

namespace cpe 
{
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

        if (!level.navmesh.IsBuilt())
        {
            path = {};
            next = 0;
            mustRecalculate = false;

            return;
        }

        if (requestCooldown > 0.0) return;

        bool targetChanged = !hasTarget || Distance(target, lastTarget) > 0.75;
        bool retry = path.search.status != Status::Found;
        bool stoppedFar = Finished() && Distance(position, target) > 0.1;
        
        if (!targetChanged && !retry && !stoppedFar) return;

        path = level.navmesh.FindPath(position, target, algorithm);

        next = path.points.size() > 1 ? 1 : path.points.size();
        lastTarget = target;
        hasTarget = true;
        requestCooldown = 0.3;

        ++requests;
    }

    Vec2 Navigator::Follow(const Level& level, Vec2 position, double speed, double dt)
    {
        if (path.search.status != Status::Found || dt <= 0)return;
        double budget = speed * dt;
        while (!Finished() && budget > 0)
        {
            Vec2 delta = path.points[next] - position;
            double distance = Length(delta);
            if (distance <= budget)
            {
                position = path.points[next++];
                budget -= distance;
            }
            else
            {
                position = position + delta * (budget / distance);
                budget = 0;

                //position = level.MoveSliding(position, level.displacement, level.agentRadius);
                 
            }
        }
        return position;
    }
}
