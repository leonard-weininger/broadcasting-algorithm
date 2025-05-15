#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/solve_tree.hpp>
#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/graph.hpp>

using namespace broadcast;

TEST_CASE("Tree dp on single vertex", "[tree_dp]") {
    Graph g{1, 0, {}};
    BroadcastingProtocol bp = solveOnTree(g);
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 0);
}

TEST_CASE("Tree dp on line 1", "[tree_dp]") {
    Graph g{5, 0, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}};
    BroadcastingProtocol bp = solveOnTree(g);
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 4);
}

TEST_CASE("Tree dp on line 2", "[tree_dp]") {
    Graph g{10, 1, {{1, 8}, {8, 2}, {2, 5}, {5, 4}, {4, 3}, {3, 9}, {9, 7}, {7, 0}, {0, 6}}};
    BroadcastingProtocol bp = solveOnTree(g);
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 9);
}

TEST_CASE("Tree dp on small tree 1", "[tree_dp]") {
    Graph g{5, 1, {{1, 0}, {0, 3}, {4, 1}, {4, 2}}};
    BroadcastingProtocol bp = solveOnTree(g);
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 3);
}

TEST_CASE("Broadcasting on small graph 2", "[tree_dp]") {
    Graph g{10, 4, {
        {4, 5},
        {4, 2},
        {2, 1},
        {4, 0},
        {0, 7},
        {0, 6},
        {7, 3},
        {3, 9},
        {8, 9}
    }};
    BroadcastingProtocol bp = solveOnTree(g);
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 5);
}


TEST_CASE("Tree dp with ignored edges 1", "[tree_dp]") {
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
    BroadcastingProtocol bp = solveOnTree(g, {2, 5, 8});
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 5);
}

TEST_CASE("Tree dp with ignored edges 2", "[tree_dp]") {
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
    BroadcastingProtocol bp = solveOnTree(g, {2, 6, 7});
    REQUIRE(isValidBroadcastingProtocol(bp, g));
    REQUIRE((int) bp.rounds.size() == 7);
}

TEST_CASE("Tree dp on graph with cycle", "[tree_dp]") {
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
    REQUIRE_THROWS(solveOnTree(g));
}

TEST_CASE("Tree dp on disconnected graph 1", "[tree_dp]") {
    Graph g{2, 0, {}};
    REQUIRE_THROWS(solveOnTree(g));
}

TEST_CASE("Tree dp on disconnected graph 2", "[tree_dp]") {
    Graph g{10, 4, {
        {4, 5},
        {4, 2},
        {2, 1},
        {4, 0},
        {0, 7},
        {0, 6},
        {7, 3},
        {8, 9}
    }};
    REQUIRE_THROWS(solveOnTree(g));
}
