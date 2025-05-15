#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/generator.hpp>
#include <broadcast/utilities/graph.hpp>

#include <algorithm>

using namespace broadcast;

TEST_CASE("Generate graph with 1 vertex", "[graph]") {
    Graph g1{1, 0, {}};
    Graph g2 = generateRandomGraph(1, 0, 42);
    REQUIRE(g1 == g2);
}

TEST_CASE("Generated graphs are simple connected and undirected", "[graph]") {
    int seed = 10202024;
    // Small graphs
    for (int n = 1; n <= 12; ++n) {
        for (int k = 0; k <= n * (n - 1) / 2 - n + 1; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                REQUIRE(isSimpleAndConnected(g));
                seed += 1;
            }
        }
    }
    // Sparse graphs
    for (int n = 1000; n <= 2000; n += 250) {
        for (int k = 0; k <= 3; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                REQUIRE(isSimpleAndConnected(g));
                seed += 1;
            }
        }
    }
    // Dense graphs
    for (int n = 50; n <= 70; n += 10) {
        for (int k = 0; k <= 3; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomGraph(n, n * (n - 1) / 2 - n + 1 - k, seed);
                REQUIRE(isSimpleAndConnected(g));
                seed += 1;
            }
        }
    }
}

TEST_CASE("Generator produces graphs uniformly", "[graph]") {
    std::vector<Graph> possibleGraphs = {
        Graph{4, 0, {{0, 1}, {0, 2}, {0, 3}}},
        Graph{4, 0, {{0, 1}, {0, 2}, {1, 3}}},
        Graph{4, 0, {{0, 1}, {0, 2}, {2, 3}}},
        Graph{4, 0, {{0, 1}, {0, 3}, {1, 2}}},
        Graph{4, 0, {{0, 1}, {0, 3}, {2, 3}}},
        Graph{4, 0, {{0, 1}, {1, 2}, {1, 3}}},
        Graph{4, 0, {{0, 1}, {1, 2}, {2, 3}}},
        Graph{4, 0, {{0, 1}, {1, 3}, {2, 3}}},
        Graph{4, 0, {{0, 2}, {0, 3}, {1, 2}}},
        Graph{4, 0, {{0, 2}, {0, 3}, {1, 3}}},
        Graph{4, 0, {{0, 2}, {1, 2}, {1, 3}}},
        Graph{4, 0, {{0, 2}, {1, 2}, {2, 3}}},
        Graph{4, 0, {{0, 2}, {1, 3}, {2, 3}}},
        Graph{4, 0, {{0, 3}, {1, 2}, {1, 3}}},
        Graph{4, 0, {{0, 3}, {1, 2}, {2, 3}}},
        Graph{4, 0, {{0, 3}, {1, 3}, {2, 3}}}
    };

    std::vector<int> sourceCount(4, 0);
    std::vector<int> graphCount(16, 0);

    for (int cas = 1; cas <= 1000; ++cas) {
        Graph g = generateRandomGraph(4, 0, cas);
        REQUIRE(g.n == 4);
        REQUIRE(isSimpleAndConnected(g));
        REQUIRE((int) g.edges.size() == 3);

        sourceCount[g.s] += 1;

        // Normalize g
        g.s = 0;
        for (auto& [u, v] : g.edges) {
            if (u > v) std::swap(u, v);
        }
        std::sort(g.edges.begin(), g.edges.end());

        bool found = false;
        for (int i = 0; i < 16; ++i) {
            if (g == possibleGraphs[i]) {
                graphCount[i] += 1;
                REQUIRE(!found);
                found = true;
            }
        }
        REQUIRE(found);
    }

    for (int i = 0; i < 4; ++i) {
        REQUIRE(sourceCount[i] >= 100);
    }

    for (int i = 0; i < 16; ++i) {
        REQUIRE(graphCount[i] >= 20);
    }
}
