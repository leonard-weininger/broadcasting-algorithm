#include <catch2/catch_test_macros.hpp>

#include <broadcast/algorithms/cyclomatic.hpp>
#include <broadcast/algorithms/dp.hpp>
#include <broadcast/algorithms/enumeration.hpp>
#include <broadcast/utilities/generator.hpp>
#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/protocol.hpp>

#include <algorithm>
#include <functional>
#include <random>
#include <set>
#include <vector>

using namespace broadcast;

TEST_CASE("Broadcasting on single vertex", "[algorithm]") {
    Graph g{1, 0, {}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 0);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 0);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 0);
}

TEST_CASE("Broadcasting on line 1", "[algorithm]") {
    Graph g{5, 0, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 4);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 4);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 4);
}

TEST_CASE("Broadcasting on line 2", "[algorithm]") {
    Graph g{10, 1, {{1, 8}, {8, 2}, {2, 5}, {5, 4}, {4, 3}, {3, 9}, {9, 7}, {7, 0}, {0, 6}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 9);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 9);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 9);
}

TEST_CASE("Broadcasting on circle 1", "[algorithm]") {
    Graph g{4, 0, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 2);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 2);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 2);
}

TEST_CASE("Broadcasting on circle 2", "[algorithm]") {
    Graph g{7, 3, {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 0}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 4);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 4);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 4);
}

TEST_CASE("Broadcasting on small graph 1", "[algorithm]") {
    Graph g{5, 1, {{1, 0}, {0, 3}, {4, 1}, {4, 2}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 3);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 3);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 3);
}

TEST_CASE("Broadcasting on small graph 2", "[algorithm]") {
    Graph g{10, 4, {
        {4, 5},
        {4, 2},
        {5, 2},
        {2, 1},
        {4, 0},
        {1, 6},
        {0, 7},
        {0, 6},
        {6, 7},
        {7, 3},
        {3, 9},
        {8, 9}
    }};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 5);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 5);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 5);
}

TEST_CASE("Broadcasting on small graph 3", "[algorithm]") {
    Graph g{9, 6, {{4, 8}, {8, 7}, {8, 1}, {7, 5}, {1, 6}, {1, 0}, {1, 2}, {3, 2}, {2, 0}, {0, 5}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 4);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 4);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 4);
}

TEST_CASE("Broadcasting on small graph 4", "[algorithm]") {
    Graph g{7, 0, {{0, 1}, {0, 4}, {4, 6}, {6, 5}, {5, 3}, {3, 2}, {3, 1}, {5, 1}}};
    REQUIRE(isSimpleAndConnected(g));

    BroadcastingProtocol bp1 = algorithmCyclomatic(g);
    REQUIRE(isValidBroadcastingProtocol(bp1, g));
    REQUIRE((int) bp1.rounds.size() == 3);

    BroadcastingProtocol bp2 = algorithmDP(g);
    REQUIRE(isValidBroadcastingProtocol(bp2, g));
    REQUIRE((int) bp2.rounds.size() == 3);

    BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
    REQUIRE(isValidBroadcastingProtocol(bp3, g));
    REQUIRE((int) bp3.rounds.size() == 3);
}

TEST_CASE("Broadcasting on perfect broadcasting graphs", "[algorithm]") {
    int seed = 345678903;

    std::function<int(int)> log2_ceil = [&](int n) {
        int ans = 0;
        while ((1 << ans) < n) ans += 1;
        return ans;
    };

    for (int n = 4; n <= 16; ++n) {
        for (int k = 0; k <= std::min(5, n * (n - 1) / 2 - n + 1); ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomPerfectBroadcastingGraph(n, k, seed);
                seed += 1;
                REQUIRE(isSimpleAndConnected(g));

                int perfectRounds = log2_ceil(n);

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));
                REQUIRE((int) bp1.rounds.size() == perfectRounds);

                if (n <= 10) {
                    BroadcastingProtocol bp2 = algorithmDP(g);
                    REQUIRE(isValidBroadcastingProtocol(bp2, g));
                    REQUIRE((int) bp2.rounds.size() == perfectRounds);
                }

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));
                REQUIRE((int) bp3.rounds.size() == perfectRounds);
            }
        }
    }

    for (int n : {64, 128}) {
        for (int k = 0; k <= 3; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomPerfectBroadcastingGraph(n, k, seed);
                seed += 1;
                REQUIRE(isSimpleAndConnected(g));

                int perfectRounds = log2_ceil(n);

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));
                REQUIRE((int) bp1.rounds.size() == perfectRounds);

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));
                REQUIRE((int) bp3.rounds.size() == perfectRounds);
            }
        }
    }

    for (int n : {256, 512}) {
        for (int k = 0; k <= 2; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomPerfectBroadcastingGraph(n, k, seed);
                seed += 1;
                REQUIRE(isSimpleAndConnected(g));

                int perfectRounds = log2_ceil(n);

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));
                REQUIRE((int) bp1.rounds.size() == perfectRounds);

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));
                REQUIRE((int) bp3.rounds.size() == perfectRounds);
            }
        }
    }

    for (int n = 1023; n <= 1025; ++n) {
        for (int k = 0; k <= 4; ++k) {
            for (int cas = 1; cas <= 10; ++cas) {
                Graph g = generateRandomPerfectBroadcastingGraph(n, k, seed);
                seed += 1;
                REQUIRE(isSimpleAndConnected(g));

                int perfectRounds = log2_ceil(n);

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));
                REQUIRE((int) bp1.rounds.size() == perfectRounds);
            }
        }
    }
}

TEST_CASE("Broadcasting on many random small graphs", "[algorithm]") {
    int seed = 22056421;
    for (int n = 3; n <= 10; ++n) {
        for (int k = 0; k <= std::min(5, n * (n - 1) / 2 - n + 1); ++k) {
            for (int cas = 1; cas <= 20; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;

                REQUIRE(isSimpleAndConnected(g));

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));

                BroadcastingProtocol bp2 = algorithmDP(g);
                REQUIRE(isValidBroadcastingProtocol(bp2, g));

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));

                REQUIRE(bp1.rounds.size() == bp2.rounds.size());
                REQUIRE(bp1.rounds.size() == bp3.rounds.size());
            }
        }
    }
}

TEST_CASE("Broadcasting on many random medium size graphs", "[algorithm]") {
    int seed = 2787845;
    for (int n = 20; n <= 40; n += 5) {
        for (int k = 0; k <= 4; ++k) {
            for (int cas = 1; cas <= 20; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;

                REQUIRE(isSimpleAndConnected(g));

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));

                REQUIRE(bp1.rounds.size() == bp3.rounds.size());
            }
        }
    }
}

TEST_CASE("Broadcasting on many random large graphs", "[algorithm]") {
    int seed = 59878450;
    for (int n = 100; n <= 500; n += 100) {
        for (int k = 0; k <= 2; ++k) {
            for (int cas = 1; cas <= 5; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;

                REQUIRE(isSimpleAndConnected(g));

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));

                REQUIRE(bp1.rounds.size() == bp3.rounds.size());
            }
        }
    }
    for (int n = 600; n <= 1000; n += 100) {
        for (int k = 0; k <= 1; ++k) {
            for (int cas = 1; cas <= 5; ++cas) {
                Graph g = generateRandomGraph(n, k, seed);
                seed += 1;

                REQUIRE(isSimpleAndConnected(g));

                BroadcastingProtocol bp1 = algorithmCyclomatic(g);
                REQUIRE(isValidBroadcastingProtocol(bp1, g));

                BroadcastingProtocol bp3 = algorithmEnumerateAllTrees(g);
                REQUIRE(isValidBroadcastingProtocol(bp3, g));

                REQUIRE(bp1.rounds.size() == bp3.rounds.size());
            }
        }
    }
}
