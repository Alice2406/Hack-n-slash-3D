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

    bool Navigator::Finished() const 
    { 
        return next >= path.points.size(); 
    }

    void Navigator::Plan(const Level& level, Vec2 position,
                         Vec2 target, Algorithm algorithm) 
    {
        if (requestCooldown > 0.0) return;

        bool targetChanged = !hasTarget || Distance(target, lastTarget) > 0.2;
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
        if (path.search.status != Status::Found || dt <= 0) return position;
        double budget = speed * dt;
        while (!Finished() && budget > 0)
        {
            Vec2 delta = path.points[next] - position;
            double distance = Length(delta);
            if (distance < 1e-8) { ++next; continue; }
            if (distance <= budget)
            {
                position = path.points[next++];
                budget -= distance;
            }
            else
            {
                position = position + delta * (budget / distance);
                budget = 0;
            }
        }
        return position;
    }
}
