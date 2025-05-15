#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/spanning_trees.hpp>
#include <broadcast/utilities/generator.hpp>
#include <broadcast/utilities/graph.hpp>

#include <algorithm>
#include <queue>
#include <random>
#include <vector>

using namespace broadcast;

std::vector<std::vector<int>> getAllSpanningTrees(const Graph& g) {
    std::vector<std::vector<int>> ans;
    SpanningTreeIterator iter(g);
    while (iter.hasNext()) {
        std::vector<int> T = iter.next();
        std::sort(T.begin(), T.end());
        ans.push_back(T);
    }
    std::sort(ans.begin(), ans.end());
    return ans;
}

TEST_CASE("Spanning trees on single vertex graph", "[spanning_trees]") {
    Graph g{1, 0, {}};
    std::vector<std::vector<int>> correct_trees = {{}};
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on single vertex graph with loop", "[spanning_trees]") {
    Graph g{1, 0, {{0, 0}}};
    std::vector<std::vector<int>> correct_trees = {{}};
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on graph with two vertices and 5 parallel edges", "[spanning_trees]") {
    Graph g{2, 0, {{0, 1}, {0, 1}, {1, 0}, {0, 1}, {1, 0}}};
    std::vector<std::vector<int>> correct_trees = {{0}, {1}, {2}, {3}, {4}};
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on tree", "[spanning_trees]") {
    Graph g{9, 5, {{0, 3}, {5, 0}, {5, 1}, {3, 8}, {3, 6}, {4, 3}, {1, 2}, {1, 7}}};
    std::vector<std::vector<int>> correct_trees = {{0, 1, 2, 3, 4, 5, 6, 7}};
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on complete graph on 4 vertices", "[spanning_trees]") {
    Graph g{4, 0, {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}, {1, 3}}};
    std::vector<std::vector<int>> correct_trees = {
        {0, 1, 2},
        {0, 1, 3},
        {0, 1, 5},
        {0, 2, 3},
        {0, 2, 4},
        {0, 2, 5},
        {0, 3, 4},
        {0, 4, 5},
        {1, 2, 3},
        {1, 2, 4},
        {1, 3, 4},
        {1, 3, 5},
        {1, 4, 5},
        {2, 3, 5},
        {2, 4, 5},
        {3, 4, 5},
    };
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on graph with 13 vertices and 15 edges", "[spanning_trees]") {
    Graph g{13, 9, {
        {8, 6},
        {4, 0},
        {4, 3},
        {0, 3},
        {0, 1},
        {1, 8},
        {10, 8},
        {9, 8},
        {2, 1},
        {2, 11},
        {7, 2},
        {7, 11},
        {11, 5},
        {5, 7},
        {12, 7}
    }};
    std::vector<std::vector<int>> correct_trees = {
        {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 12, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 13, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 9, 11, 12, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 9, 11, 13, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 9, 12, 13, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 13, 14},
        {0, 1, 2, 4, 5, 6, 7, 8, 10, 12, 13, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 9, 11, 13, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 10, 11, 13, 14},
        {0, 1, 3, 4, 5, 6, 7, 8, 10, 12, 13, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 10, 11, 13, 14},
        {0, 2, 3, 4, 5, 6, 7, 8, 10, 12, 13, 14},
    };
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

TEST_CASE("Spanning trees on small multigraph", "[spanning_trees]") {
    Graph g{4, 3, {{0, 1}, {1, 2}, {1, 3}, {2, 3}, {2, 3}, {0, 0}}};
    std::vector<std::vector<int>> correct_trees = {
        {0, 1, 2},
        {0, 1, 3},
        {0, 1, 4},
        {0, 2, 3},
        {0, 2, 4}
    };
    REQUIRE(getAllSpanningTrees(g) == correct_trees);
}

std::vector<std::vector<int>> bruteForceAllSpanningTrees(const Graph& g) {
    std::vector<std::vector<int>> ans;
    if ((int) g.edges.size() < g.n - 1) return ans;

    std::vector<std::vector<std::pair<int, int>>> adj(g.n);
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        auto [u, v] = g.edges[i];
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }

    // iterate over all subsets of n-1 edges
    std::vector<int> mask(g.edges.size(), 0);
    std::fill(mask.end() - g.n + 1, mask.end(), 1);
    do {
        // check if the current subset of edges spans the whole graph using BFS
        std::vector<bool> visited(g.n, false);
        visited[0] = true;

        std::queue<int> q;
        q.push(0);

        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto [u, eid] : adj[v]) {
                if (mask[eid] && !visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }
            }
        }

        if (std::all_of(visited.begin(), visited.end(), [](bool v) { return v; })) {
            ans.push_back(std::vector<int>());
            for (int i = 0; i < (int) g.edges.size(); ++i) {
                if (mask[i]) ans.back().push_back(i);
            }
        }
    }
    while (std::next_permutation(mask.begin(), mask.end()));

    std::sort(ans.begin(), ans.end());
    return ans;
}

TEST_CASE("Spanning trees on many random small simple graphs", "[spanning_trees]") {
    int seed = 80760465;
    for (int n = 3; n <= 6; ++n) {
        for (int k = 0; k <= n * (n - 1) / 2 - n + 1; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
}

TEST_CASE("Spanning trees on many random medium size simple graphs", "[spanning_trees]") {
    int seed = 33754445;
    for (int n = 12; n <= 15; ++n) {
        for (int k = 0; k <= 5; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
}

TEST_CASE("Spanning trees on many random large sparse simple graphs", "[spanning_trees]") {
    int seed = 132123;
    for (int n = 20; n <= 100; n += 10) {
        for (int k = 0; k <= 2; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
    for (int n = 100; n <= 500; n += 100) {
        for (int k = 0; k <= 1; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
}

Graph generateRandomMultiGraph(int n, int k, int seed) {
    Graph g = generateRandomGraph(n, 0, seed);
    std::mt19937 rng(seed);
    std::uniform_int_distribution distr(0, n - 1);
    for (int i = 1; i <= k; ++i) {
        int u = distr(rng);
        int v = distr(rng);
        g.edges.emplace_back(u, v);
    }
    return g;
}

TEST_CASE("Spanning trees on many random small multi-graphs", "[spanning_trees]") {
    int seed = 5007008;
    for (int n = 3; n <= 7; ++n) {
        for (int k = 0; k <= 8; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomMultiGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
}

TEST_CASE("Spanning trees on many random medium size multi-graphs", "[spanning_trees]") {
    int seed = 666654964;
    for (int n = 12; n <= 15; ++n) {
        for (int k = 0; k <= 5; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomMultiGraph(n, k, seed);
                seed += 1;
                REQUIRE(getAllSpanningTrees(g) == bruteForceAllSpanningTrees(g));
            }
        }
    }
}
