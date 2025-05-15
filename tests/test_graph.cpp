#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/graph.hpp>

#include <sstream>

using namespace broadcast;

TEST_CASE("Simple connected graph is valid 1", "[graph]") {
    Graph g{3, 0, {{0,1}, {1,2}}};
    REQUIRE(isSimpleAndConnected(g));
}

TEST_CASE("Simple connected graph is valid 2", "[graph]") {
    Graph g{5, 0, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}}};
    REQUIRE(isSimpleAndConnected(g));
}

TEST_CASE("Simple connected graph is valid 3", "[graph]") {
    Graph g{1, 0, {}};
    REQUIRE(isSimpleAndConnected(g));
}

TEST_CASE("Graph with self-loop is invalid", "[graph]") {
    Graph g{3, 0, {{0,0}, {1,2}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph with parallel edge is invalid", "[graph]") {
    Graph g{5, 0, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}, {2, 1}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Disconnected Graph is invalid", "[graph]") {
    Graph g{7, 0, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}, {5, 6}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph with invalid vertex is invalid", "[graph]") {
    Graph g{6, 0, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}, {5, 6}, {4, 5}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph with invalid source is invalid 1", "[graph]") {
    Graph g{6, 6, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}, {4, 5}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph with invalid source is invalid 2", "[graph]") {
    Graph g{6, -5, {{1,0}, {1,2}, {1, 3}, {1, 4}, {2, 3}, {4, 5}}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph without vertices is invalid", "[graph]") {
    Graph g{0, 0, {}};
    REQUIRE_FALSE(isSimpleAndConnected(g));
}

TEST_CASE("Graph read and write 1", "[graph][io]") {
    Graph g1{5, 0, {{1, 0}, {1, 2}, {1, 3}, {1, 4}, {2, 3}}};
    Graph g2{5, 0, {{1, 0}, {1, 2}, {1, 3}, {1, 4}, {3, 2}}};
    Graph g3{5, 0, {{1, 0}, {1, 2}, {1, 3}, {2, 3}, {1, 4}}};
    Graph g4{5, 0, {{1, 0}, {1, 0}, {1, 2}, {1, 3}, {1, 4}, {2, 3}}};
    std::stringstream ss;
    printGraph(g1, ss);
    std::stringstream input_stream(ss.str());
    Graph g_read = readGraph(input_stream);
    REQUIRE(g1 == g_read);
    REQUIRE_FALSE(g2 == g_read);
    REQUIRE_FALSE(g3 == g_read);
    REQUIRE_FALSE(g4 == g_read);
}

TEST_CASE("Graph read and write 2", "[graph][io]") {
    Graph g{1, 0, {}};
    std::stringstream ss;
    printGraph(g, ss);
    std::stringstream input_stream(ss.str());
    Graph g_read = readGraph(input_stream);
    REQUIRE(g == g_read);
}
