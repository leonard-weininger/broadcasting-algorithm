#include <broadcast/utilities/generator.hpp>
#include <broadcast/utilities/graph.hpp>

#include <algorithm>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

namespace broadcast {

Graph generateRandomGraph(int n, int k, unsigned long long seed) {
    if (n < 1) {
        throw std::invalid_argument("Number of vertices must be at least 1.");
    }

    long long maxEdges = 1LL * n * (n - 1) / 2;
    long long maxCyclomatic = maxEdges - (n - 1);

    if (k < 0 || k > maxCyclomatic) {
        throw std::invalid_argument("Cyclomatic number k must satisfy 0 <= k <= n*(n-1)/2 - n + 1.");
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distr(0, n - 1);

    Graph g;
    g.n = n;
    g.s = distr(rng);

    if (n == 1) {
        return g;
    }

    if (n == 2) {
        if (rng() & 1) {
            g.edges.emplace_back(0, 1);
        }
        else {
            g.edges.emplace_back(1, 0);
        }
        return g;
    }

    // 1. Find a random spanning tree on n vertices using pruefer code
    std::vector<int> pruefer_code(n - 2);
    std::vector<int> deg(n, 1);

    for (int i = 0; i < n - 2; ++i) {
        pruefer_code[i] = distr(rng);
        deg[pruefer_code[i]] += 1;
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> leaves;

    for (int i = 0; i < n; ++i) {
        if (deg[i] == 1) leaves.push(i);
    }

    for (int i = 0; i < n - 2; ++i) {
        int u = leaves.top();
        leaves.pop();
        int v = pruefer_code[i];
        deg[u] -= 1;
        deg[v] -= 1;
        if (deg[v] == 1) leaves.push(v);
        g.edges.emplace_back(u, v);
    }

    g.edges.emplace_back(leaves.top(), -1);
    leaves.pop();
    g.edges.back().second = leaves.top();

    std::set<std::pair<int, int>> edgeSet;
    for (auto [u, v] : g.edges) {
        if (u > v) std::swap(u, v);
        edgeSet.emplace(u, v);
    }

    // 2. Add k random edges that are not yet part of the graph
    while ((int) g.edges.size() < n - 1 + k) {
        int u = distr(rng);
        int v = distr(rng);
        if (u == v) continue;
        if (u > v) std::swap(u, v);
        if (edgeSet.find(std::make_pair(u, v)) != edgeSet.end()) continue;
        g.edges.emplace_back(u, v);
        edgeSet.emplace(u, v);
    }

    // 3. Randomly shuffle the edges
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        if (rng() & 1) {
            std::swap(g.edges[i].first, g.edges[i].second);
        }
    }
    std::shuffle(g.edges.begin(), g.edges.end(), rng);

    return g;
}

Graph generateRandomPerfectBroadcastingGraph(int n, int k, unsigned long long seed) {
    if (n < 1) {
        throw std::invalid_argument("Number of vertices must be at least 1.");
    }

    long long maxEdges = 1LL * n * (n - 1) / 2;
    long long maxCyclomatic = maxEdges - (n - 1);

    if (k < 0 || k > maxCyclomatic) {
        throw std::invalid_argument("Cyclomatic number k must satisfy 0 <= k <= n*(n-1)/2 - n + 1.");
    }

    Graph g;
    g.n = n;

    std::set<std::pair<int, int>> edgeSet;

    // 1. Add edges that ensure a perfect broadcasting graph
    int cur = 1;
    while (cur < n) {
        for (int i = 0; i < cur && cur + i < n; ++i) {
            g.edges.emplace_back(i, cur + i);
            edgeSet.emplace(i, cur + i);
        }
        cur += cur;
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distr(0, n - 1);

    // 2. Add k random edges
    while ((int) g.edges.size() < n - 1 + k) {
        int u = distr(rng);
        int v = distr(rng);
        if (u == v) continue;
        if (u > v) std::swap(u, v);
        if (edgeSet.find(std::make_pair(u, v)) != edgeSet.end()) continue;
        g.edges.emplace_back(u, v);
        edgeSet.emplace(u, v);
    }

    // 3. Permute the vertex indices and shuffle the edges
    std::vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    std::shuffle(p.begin(), p.end(), rng);

    for (auto& [u, v] : g.edges) {
        u = p[u];
        v = p[v];
        if (rng() & 1) std::swap(u, v);
    }
    std::shuffle(g.edges.begin(), g.edges.end(), rng);
    g.s = p[0];

    return g;
}

} // namespace broadcast
