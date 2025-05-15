#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/protocol.hpp>

#include <sstream>

using namespace broadcast;

TEST_CASE("Valid broadcasting protocol 1", "[broadcast]") {
    Graph g{3, 0, {{0,1}, {1,2}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{0, 1}}, {{1, 2}}};
    REQUIRE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Valid broadcasting protocol 2", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}}};
    REQUIRE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Valid broadcasting protocol 3", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 0}}, {{1, 2}}, {{2, 3}}, {{0, 5}}, {{5, 7}}, {{8, 4}}};
    REQUIRE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Valid broadcasting protocol 4", "[broadcast]") {
    Graph g{1, 0, {}};
    BroadcastingProtocol bp;
    bp.rounds = {};
    REQUIRE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Valid broadcasting protocol 5", "[broadcast]") {
    Graph g{2, 1, {{0, 1}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{1, 0}}};
    REQUIRE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 1 (double inform)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}}, {{0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 2 (invalid vertex)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}, {0, 9}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 3 (empty round)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}}, {}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 4 (two neighbours in one round)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}, {1, 2}}, {{8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 5 (in the end not all informed)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 0}, {8, 7}}, {{0, 2}, {7, 5}, {8, 4}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 6 (uninformed tries to inform)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}, {7, 5}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {8, 4}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 7 (uninformed tries to inform)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}, {7, 5}}, {{1, 0}, {2, 3}, {8, 4}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Invalid broadcasting protocol 8 (edge does not exist)", "[broadcast]") {
    Graph g{9, 6, {{2, 3}, {2, 1}, {7, 8}, {1, 0}, {5, 0}, {2, 0}, {1, 6}, {8, 1}, {7, 5}, {8, 4}}};
    BroadcastingProtocol bp;
    bp.rounds = {{{6, 1}}, {{1, 8}, {6, 5}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {8, 4}}};
    REQUIRE_FALSE(isValidBroadcastingProtocol(bp, g));
}

TEST_CASE("Broadcasting Protocol read and write 1", "[protocol][io]") {
    BroadcastingProtocol bp1, bp2, bp3, bp4;
    bp1.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {7, 5}, {8, 4}}};
    bp2.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {5, 7}, {8, 4}}};
    bp3.rounds = {{{6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {8, 4}, {7, 5}}};
    bp4.rounds = {{{6, 1}, {6, 1}}, {{1, 8}}, {{1, 2}, {8, 7}}, {{1, 0}, {2, 3}, {8, 4}, {7, 5}}};
    std::stringstream ss;
    printBroadcastingProtocol(bp1, ss);
    std::stringstream input_stream(ss.str());
    BroadcastingProtocol bp_read = readBroadcastingProtocol(input_stream);
    REQUIRE(bp1 == bp_read);
    REQUIRE_FALSE(bp2 == bp_read);
    REQUIRE_FALSE(bp3 == bp_read);
    REQUIRE_FALSE(bp4 == bp_read);
}

TEST_CASE("Broadcasting Protocol read and write 2", "[protocol][io]") {
    BroadcastingProtocol bp1, bp2;
    bp1.rounds = {};
    bp2.rounds = {{}};
    std::stringstream ss;
    printBroadcastingProtocol(bp1, ss);
    std::stringstream input_stream(ss.str());
    BroadcastingProtocol bp_read = readBroadcastingProtocol(input_stream);
    REQUIRE(bp1 == bp_read);
    REQUIRE_FALSE(bp2 == bp_read);
}

TEST_CASE("Broadcasting Protocol read and write 3", "[protocol][io]") {
    BroadcastingProtocol bp1, bp2;
    bp1.rounds = {{}};
    bp2.rounds = {};
    std::stringstream ss;
    printBroadcastingProtocol(bp1, ss);
    std::stringstream input_stream(ss.str());
    BroadcastingProtocol bp_read = readBroadcastingProtocol(input_stream);
    REQUIRE(bp1 == bp_read);
    REQUIRE_FALSE(bp2 == bp_read);
}
