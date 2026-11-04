#include "cpe/navigation.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <functional>
#include <utility>
#include <algorithm>
#include <chrono>
#include <tuple>

namespace cpe
{
    SearchResult Dijkstra(const Graph& graph, int start, int goal)
    {
        using Clock = std::chrono::steady_clock;
        const auto begin = Clock::now();

        SearchResult result;

        auto finish = [&]()
            {
                result.microseconds =
                    std::chrono::duration<double, std::micro>(Clock::now() - begin).count();

                return result;
            };

        if (start < 0 || goal < 0 ||
            start >= static_cast<int>(graph.size()) ||
            goal >= static_cast<int>(graph.size()))
        {

            result.status = Status::InvalidInput;
            return finish();
        }

        for (const auto& neighbors : graph)
        {
            for (const Edge& edge : neighbors)
            {

                if (edge.to < 0 ||
                    edge.to >= static_cast<int>(graph.size()) ||
                    !std::isfinite(edge.cost) ||
                    edge.cost < 0.0) {

                    result.status = Status::InvalidInput;
                    return finish();
                }
            }
        }

        const double infinity = std::numeric_limits<double>::infinity();

        std::vector<double> g(graph.size(), infinity);
        std::vector<int> parent(graph.size(), -1);

        g[start] = 0.0;

        using Entry = std::pair<double, int>;

        std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> open;


        open.push({ 0.0, start });

        while (!open.empty())
        {
            const auto [savedG, u] = open.top();
            open.pop();

            if (savedG > g[u])
                continue;

            if (u == goal)
            {
                result.status = Status::Found;
                result.cost = g[goal];

                for (int v = goal; v != -1; v = parent[v])
                    result.nodes.push_back(v);

                std::reverse(result.nodes.begin(), result.nodes.end());

                return finish();
            }

            ++result.expanded;

            for (const Edge& edge : graph[u])
            {
                const double candidate = g[u] + edge.cost;

                if (candidate < g[edge.to])
                {
                    g[edge.to] = candidate;
                    parent[edge.to] = u;

                    open.push({ candidate, edge.to });
                }
            }
        }

        result.status = Status::NoPath;
        return finish();

    }


    SearchResult AStar(const Graph& graph, int start, int goal,
        const std::vector<double>& heuristic)
    {
        using Clock = std::chrono::steady_clock;
        const auto begin = Clock::now();

        SearchResult result;

        auto finish = [&]()
            {
                result.microseconds =
                    std::chrono::duration<double, std::micro>(
                        Clock::now() - begin).count();

                return result;
            };

        if (start < 0 || goal < 0 ||
            start >= static_cast<int>(graph.size()) ||
            goal >= static_cast<int>(graph.size()))
        {
            result.status = Status::InvalidInput;
            return finish();
        }

        for (const auto& neighbors : graph)
        {
            for (const Edge& edge : neighbors)
            {
                if (edge.to < 0 ||
                    edge.to >= static_cast<int>(graph.size()) ||
                    !std::isfinite(edge.cost) ||
                    edge.cost < 0.0)
                {
                    result.status = Status::InvalidInput;
                    return finish();
                }
            }
        }

        if (heuristic.size() != graph.size())
        {
            result.status = Status::InvalidInput;
            return finish();
        }

        for (double h : heuristic)
        {
            if (!std::isfinite(h) || h < 0.0)
            {
                result.status = Status::InvalidInput;
                return finish();
            }
        }

        if (std::abs(heuristic[goal]) > 1e-9)
        {
            result.status = Status::InvalidInput;
            return finish();
        }

        const double infinity = std::numeric_limits<double>::infinity();

        std::vector<double> g(graph.size(), infinity);
        std::vector<int> parent(graph.size(), -1);

        using Entry = std::tuple<double, double, int>;

        std::priority_queue<Entry, std::vector<Entry>,
            std::greater<Entry>> open;

        g[start] = 0.0;
        open.push({ heuristic[start], 0.0, start });

        while (!open.empty())
        {
            const auto [priority, savedG, u] = open.top();
            open.pop();

            (void)priority;

            if (savedG > g[u])
                continue;

            if (u == goal)
            {
                result.status = Status::Found;
                result.cost = g[goal];

                for (int v = goal; v != -1; v = parent[v])
                    result.nodes.push_back(v);

                std::reverse(result.nodes.begin(), result.nodes.end());

                return finish();
            }

            ++result.expanded;

            for (const Edge& edge : graph[u])
            {
                const double candidate = g[u] + edge.cost;

                if (candidate < g[edge.to])
                {
                    g[edge.to] = candidate;
                    parent[edge.to] = u;

                    open.push
                    ({
                        candidate + heuristic[edge.to],
                        candidate,
                        edge.to
                        });
                }
            }
        }

        result.status = Status::NoPath;
        return finish();
    }
}

