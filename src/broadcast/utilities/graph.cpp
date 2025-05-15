#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/hash.hpp>

#include <algorithm>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace broadcast {

bool Graph::operator==(const Graph& other) const {
    return n == other.n && s == other.s && edges == other.edges;
}

Graph readGraph(std::istream& in) {
    Graph g;
    int m;

    // Read number of vertices
    if (!(in >> g.n)) {
        throw std::runtime_error("Failed to read number of vertices (n).");
    }

    // Read number of edges
    if (!(in >> m)) {
        throw std::runtime_error("Failed to read number of edges (m).");
    }

    // Read source vertex
    if (!(in >> g.s)) {
        throw std::runtime_error("Failed to read source vertex (s).");
    }

    if (g.n < 0 || m < 0 || g.s < 0 || g.s >= g.n) {
        throw std::runtime_error("Invalid graph parameters: negative or out-of-bounds values.");
    }

    g.edges.resize(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(in >> u >> v)) {
            throw std::runtime_error("Failed to read edge " + std::to_string(i) + ".");
        }
        if (u < 0 || v < 0 || u >= g.n || v >= g.n) {
            throw std::runtime_error("Edge vertices out of bounds at edge " + std::to_string(i) + ".");
        }
        g.edges[i] = {u, v};
    }

    return g;
}

void printGraph(const Graph& g, std::ostream& out) {
    out << g.n << ' ' << g.edges.size() << ' ' << g.s << '\n';
    for (const auto& [u, v] : g.edges) {
        out << u << ' ' << v << '\n';
    }
}

bool isSimpleAndConnected(const Graph& g) {
    if (g.n <= 0) return false; // Need at least one vertex
    if (g.s < 0 || g.s >= g.n) return false; // Invalid source

    for (const auto& [u, v] : g.edges) {
        if (u < 0 || u >= g.n) return false; // Invalid vertex
        if (v < 0 || v >= g.n) return false; // Invalid vertex
        if (u == v) return false; // Self-loop
    }

    // Check for duplicates
    std::unordered_set<std::pair<int, int>, customHash> edgeSet;
    for (auto [u, v] : g.edges) {
        if (u > v) std::swap(u, v); // Normalize edge
        edgeSet.emplace(u, v);
    }
    if (edgeSet.size() != g.edges.size()) {
        return false; // Duplicate must exist
    }

    // Build adjacency list
    std::vector<std::vector<int>> adj(g.n);
    for (const auto& [u, v] : g.edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS to check connectivity
    std::vector<bool> visited(g.n, false);
    std::queue<int> q;
    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    return std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

} // namespace broadcast
