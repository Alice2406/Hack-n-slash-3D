#include "cpe/navigation.hpp"
#include "cpe/Level.hpp"
#include <stdexcept>

namespace cpe {
bool NavMesh::Build() {

    if (source_.empty())
        return false;

    for (std::vector<Vec2> vertices : source_) {
        if (vertices.size() < 3) continue;
        Vec2 somme{};
        for (auto p : vertices) somme = somme + p;
        polygons_.push_back({ vertices, somme / vertices.size() });
    }

    if (polygons_.empty())
        return false;

    for (int i = 0; i < polygons_.size(); i++)
    {
        std::vector<Vec2> pointspi = polygons_[i].vertices;
        for (int edgepi = 0; edgepi < pointspi.size(); edgepi++)
        {
            Vec2 pointleftpi = pointspi[edgepi];
            Vec2 pointrightpi = pointspi[(edgepi + 1) % pointspi.size()];
            for (int j = i + 1; j < polygons_.size(); j++)
            {
                std::vector<Vec2> pointspj = polygons_[j].vertices;
                for (int edgepj = 0; edgepj < pointspj.size(); edgepj++)
                {
                    Vec2 pointleftpj = pointspj[edgepj];
                    Vec2 pointrightpj = pointspj[(edgepj + 1) % pointspj.size()];

                    if (SamePoint(pointleftpi, pointleftpj) && SamePoint(pointrightpi, pointrightpj))
                    {
                        portals_.push_back({ i, j, pointleftpi, pointrightpi });
                        break ;
                    }
                }

            }
        }
    }

    graph_.resize(polygons_.size());

    for (Portal p : portals_)
    {
        if (p.a < 0 || p.b < 0 || p.a >= polygons_.size() || p.b >= polygons_.size() || p.a == p.b)
            throw std::invalid_argument("indice de portail invalides");
        if (Distance(p.p, p.q) < 1e-8)
            throw std::invalid_argument("longueur nulle");
        if (!Contains(p.a, p.p) || !Contains(p.b, p.q))
            throw std::invalid_argument("extremite n'est pas dans la bonne region");
        Vec2 middle = (p.p + p.q) * 0.5;
        double cost = Distance(polygons_[p.a].center, middle) + Distance(middle, polygons_[p.b].center);
        graph_[p.a].push_back({ p.b,cost });
        graph_[p.b].push_back({ p.a,cost });
    }

    built_ = true;

    return true;
}

int NavMesh::Locate(Vec2 point) const {
    if (!Finite(point))
        return -1;

    for (int i = 0; i < polygons_.size(); i++)
    {
        if (Contains(i, point))
            return i;
    }
    return -1;
}

bool NavMesh::Contains(int polygon, Vec2 point) const {
    double side;
    Polygon pol;
    pol = polygons_[polygon];
    bool positif = false;
    bool negatif = false;
    if (!Finite(point))
        throw std::invalid_argument("point non fini");
    for (int i = 0; i < pol.vertices.size(); i++)
    {
        side = Cross((pol.vertices[(i + 1) % pol.vertices.size()]) - pol.vertices[i], point - pol.vertices[i]);
        if (side < -1e-8)
            negatif = true;
        else if (side > 1e-8)
            positif = true;
    }
    return !(positif && negatif);
}

bool NavMesh::SegmentInside(Vec2 from, Vec2 to) const {
    if (!Finite(from) || !Finite(to)) return false;
    std::vector<std::pair<double, double>> intervals;
    const Vec2 direction = from - to;
    for (const Polygon& polygon : polygons_) {
        const auto& v = polygon.vertices;
        double orientation = Cross(v[1] - v[0], v[2] - v[1]) > 0 ? 1.0 : -1.0;
        double low = 0.0, high = 1.0;
        bool valid = true;
        for (std::size_t i = 0; i < v.size(); ++i) {
            Vec2 edge = v[(i + 1) % v.size()] - v[i];
            double base = orientation * Cross(edge, from - v[i]);
            double slope = orientation * Cross(edge, direction);
            if (std::abs(slope) < 1e-12) {
                if (base < -1e-9) { valid = false; break; }
            }
            else {
                double bound = (-1e-9 - base) / slope;
                if (slope > 0) low = std::max(low, bound);
                else high = std::min(high, bound);
            }
        }
        if (valid && low <= high) intervals.push_back({ low,high });
    }
    std::sort(intervals.begin(), intervals.end());
    double covered = 0.0;
    for (auto [low, high] : intervals) {
        if (low > covered + 1e-8) return false;
        covered = std::max(covered, high);
        if (covered >= 1.0 - 1e-8) return true;
    }
    return false;
}

Portal* NavMesh::Between(int a, int b) const
{
    for (Portal p : portals_)
    {
        if ((p.a == a && p.b == b) || (p.a == b && p.b == a))
            return &p;
    }
    return nullptr;
}

Path NavMesh::FindPath(Vec2 from, Vec2 to, Algorithm algorithm) const {
    Path result;

    if (!Finite(from) || !Finite(to)) {
        return result;
    }

    const int startRegion = Locate(from);
    const int goalRegion = Locate(to);

    if (startRegion < 0 || goalRegion < 0) {
        result.search.status = Status::OutsideNavMesh;
        return result;
    }

    auto h = [&](int n) -> double {
        return Distance(polygons_[n].center, polygons_[goalRegion].center);
        };

    if (algorithm == Algorithm::AStar) {
        std::vector<double> heuristic(polygons_.size());
        for (std::size_t i = 0; i < polygons_.size(); ++i) {
            heuristic[i] = h(static_cast<int>(i));
        }
        result.search = AStar(graph_, startRegion, goalRegion, heuristic);
    }
    else {
        result.search = Dijkstra(graph_, startRegion, goalRegion);
    }

    if (result.search.status != Status::Found) {
        result.search.status = Status::NoPath;
        return result;
    }

    auto append = [&](Vec2 point) {
        if (result.points.empty() || !SamePoint(result.points.back(), point)) {
            result.points.push_back(point);
        }
        };

    append(from);

    if (startRegion != goalRegion) {
        append(polygons_[startRegion].center);

        for (std::size_t i = 1; i < result.search.nodes.size(); ++i) {
            const int previous = result.search.nodes[i - 1];
            const int next = result.search.nodes[i];

            const Portal* portal = Between(previous, next);

            if (portal == nullptr) {
                return Path{};
            }

            double lengthPortal = Distance(portal->p, portal->q);
            double lengthEnemy = 2 * (0.25 + 0.05);

            if (lengthEnemy > lengthPortal)
            {
                result.search.status = Status::NoPath;
                return result;
            }

            append((portal->p + portal->q) * 0.5);
            append(polygons_[next].center);
        }
    }

    append(to);

    result.search.cost = 0.0;
    for (std::size_t i = 1; i < result.points.size(); ++i) {
        result.search.cost += Distance(result.points[i - 1], result.points[i]);
    }

    return result;
}
}
