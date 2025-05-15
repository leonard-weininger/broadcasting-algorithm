#include <catch2/catch_test_macros.hpp>

#include <broadcast/utilities/matching.hpp>

#include <algorithm>
#include <random>
#include <set>
#include <vector>

using namespace broadcast;

bool isValidMatchingForEdges(const BipartiteMatching& bm, const std::vector<std::pair<int, int>>& edges) {
    // matchLeft and matchRight are synchronized
    for (int i = 0; i < bm.n; ++i) {
        if (bm.matchLeft[i] != -1) {
            if (bm.matchLeft[i] < 0 || bm.matchLeft[i] >= bm.m) return false;
            if (bm.matchRight[bm.matchLeft[i]] != i) return false;
        }
    }
    for (int i = 0; i < bm.m; ++i) {
        if (bm.matchRight[i] != -1) {
            if (bm.matchRight[i] < 0 || bm.matchRight[i] >= bm.n) return false;
            if (bm.matchLeft[bm.matchRight[i]] != i) return false;
        }
    }

    // if matched then the vertex must be valid
    for (int i = 0; i < bm.n; ++i) {
        if (bm.matchLeft[i] != -1) {
            if (!bm.validLeft[i]) return false;
        }
    }
    for (int i = 0; i < bm.m; ++i) {
        if (bm.matchRight[i] != -1) {
            if (!bm.validRight[i]) return false;
        }
    }

    // check if matchingSize is correct
    int countMatches = 0;
    for (int i = 0; i < bm.n; ++i) {
        if (bm.matchLeft[i] != -1) {
            countMatches += 1;
        }
    }
    if (countMatches != bm.matchingSize) return false;

    // if matched then edge exists
    std::set<std::pair<int, int>> edgeSet(edges.begin(), edges.end());
    for (int i = 0; i < bm.n; ++i) {
        if (bm.matchLeft[i] != -1) {
            if (edgeSet.find(std::make_pair(i, bm.matchLeft[i])) == edgeSet.end()) return false;
        }
    }

    // matching is valid
    return true;
}

TEST_CASE("Empty Matching 1", "[matching]") {
    int n = 0;
    int m = 0;
    std::vector<std::pair<int, int>> edges = {};
    BipartiteMatching bm(n, m);
    REQUIRE(bm.computeMaximumMatching() == 0);
    REQUIRE(bm.matchingSize == 0);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Empty Matching 2", "[matching]") {
    int n = 1;
    int m = 0;
    std::vector<std::pair<int, int>> edges = {};
    BipartiteMatching bm(n, m);
    REQUIRE(bm.computeMaximumMatching() == 0);
    REQUIRE(bm.matchingSize == 0);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Empty Matching 3", "[matching]") {
    int n = 0;
    int m = 1;
    std::vector<std::pair<int, int>> edges = {};
    BipartiteMatching bm(n, m);
    REQUIRE(bm.computeMaximumMatching() == 0);
    REQUIRE(bm.matchingSize == 0);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Empty Matching 4", "[matching]") {
    int n = 10;
    int m = 5;
    std::vector<std::pair<int, int>> edges = {};
    BipartiteMatching bm(n, m);
    REQUIRE(bm.computeMaximumMatching() == 0);
    REQUIRE(bm.matchingSize == 0);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Small matching graph 1", "[matching]") {
    int n = 3;
    int m = 2;
    std::vector<std::pair<int, int>> edges = {{0, 0}, {0, 1}, {1, 0}, {2, 0}};
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 2);
    REQUIRE(bm.matchingSize == 2);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Small matching graph 2", "[matching]") {
    int n = 4;
    int m = 3;
    std::vector<std::pair<int, int>> edges = {
        {3, 2},
        {0, 2},
        {1, 1},
        {2, 0},
        {3, 0},
        {0, 1},
        {1, 2},
        {2, 0},
        {3, 0},
        {0, 2},
        {1, 1},
        {2, 1}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 3);
    REQUIRE(bm.matchingSize == 3);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Perfect matching small graph", "[matching]") {
    int n = 3;
    int m = 3;
    std::vector<std::pair<int, int>> edges = {
        {0, 0},
        {1, 1},
        {2, 2}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 3);
    REQUIRE(bm.matchingSize == 3);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Matching smaller than min(n, m) 1", "[matching]") {
    int n = 4;
    int m = 4;
    std::vector<std::pair<int, int>> edges = {
        {0, 1},
        {1, 0},
        {3, 3}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 3);
    REQUIRE(bm.matchingSize == 3);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Matching smaller than min(n, m) 2", "[matching]") {
    int n = 4;
    int m = 3;
    std::vector<std::pair<int, int>> edges = {
        {0, 0},
        {1, 0},
        {2, 1},
        {3, 1}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 2);
    REQUIRE(bm.matchingSize == 2);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Matching smaller than min(n, m) 3", "[matching]") {
    int n = 6;
    int m = 6;
    std::vector<std::pair<int, int>> edges = {
        {0, 0},
        {1, 1},
        {1, 3},
        {2, 1},
        {2, 3},
        {3, 1},
        {4, 2},
        {4, 4},
        {5, 4},
        {5, 5}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Large perfect matching", "[matching]") {
    int n = 1000;
    int m = 1000;
    BipartiteMatching bm(n, m);
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
        bm.addEdge(i, i);
        edges.emplace_back(i, i);
    }
    REQUIRE(bm.computeMaximumMatching() == 1000);
    REQUIRE(bm.matchingSize == 1000);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Unique perfect matchings in dense graph", "[matching]") {
    std::mt19937 rng(42);
    for (int n = 100; n <= 200; n += 10) {
        BipartiteMatching bm(n, n);
        std::vector<std::pair<int, int>> edges;
        for (int i = 0; i < n; ++i) {
            edges.emplace_back(i, i);
            for (int j = i + 1; j < n; ++j) {
                if (rng() % 4 >= 1) {
                    edges.emplace_back(i, j);
                }
            }
        }
        std::vector<int> pa(n);
        std::vector<int> pb(n);
        for (int i = 0; i < n; ++i) {
            pa[i] = i;
            pb[i] = i;
        }
        std::shuffle(pa.begin(), pa.end(), rng);
        std::shuffle(pb.begin(), pb.end(), rng);
        for (auto& [a, b] : edges) {
            a = pa[a];
            b = pb[b];
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        for (const auto& [a, b] : edges) {
            bm.addEdge(a, b);
        }
        REQUIRE(bm.computeMaximumMatching() == n);
        REQUIRE(bm.matchingSize == n);
        REQUIRE(isValidMatchingForEdges(bm, edges));
    }
}

TEST_CASE("Matching with removing and adding", "[matching]") {
    int n = 6;
    int m = 7;
    std::vector<std::pair<int, int>> edges = {
        {0, 0},
        {1, 1},
        {1, 3},
        {2, 1},
        {2, 3},
        {3, 1},
        {4, 2},
        {4, 4},
        {5, 4},
        {5, 5},
        {3, 6}
    };
    BipartiteMatching bm(n, m);
    for (auto [a, b] : edges) {
        bm.addEdge(a, b);
    }
    REQUIRE(bm.computeMaximumMatching() == 6);
    REQUIRE(bm.matchingSize == 6);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.removeVertexB(6);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.removeVertexB(6);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.removeVertexB(5);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.removeVertexA(0);
    REQUIRE(bm.computeMaximumMatching() == 4);
    REQUIRE(bm.matchingSize == 4);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.addVertexA(2);
    REQUIRE(bm.computeMaximumMatching() == 4);
    REQUIRE(bm.matchingSize == 4);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.addVertexB(6);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.removeVertexB(0);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.addVertexA(0);
    REQUIRE(bm.computeMaximumMatching() == 5);
    REQUIRE(bm.matchingSize == 5);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    bm.addVertexB(0);
    REQUIRE(bm.computeMaximumMatching() == 6);
    REQUIRE(bm.matchingSize == 6);
    REQUIRE(isValidMatchingForEdges(bm, edges));
    for (int i = 0; i < n; ++i) {
        bm.removeVertexA(i);
        REQUIRE(bm.computeMaximumMatching() == n - i - 1);
        REQUIRE(bm.matchingSize == n - i - 1);
        REQUIRE(isValidMatchingForEdges(bm, edges));
    }
    for (int i = 0; i < n; ++i) {
        bm.addVertexA(i);
        REQUIRE(bm.computeMaximumMatching() == i + 1);
        REQUIRE(bm.matchingSize == i + 1);
        REQUIRE(isValidMatchingForEdges(bm, edges));
    }
    bm.addVertexB(5);
    REQUIRE(bm.computeMaximumMatching() == 6);
    REQUIRE(bm.matchingSize == 6);
    REQUIRE(isValidMatchingForEdges(bm, edges));
}

TEST_CASE("Matching with removing and adding on many random graphs", "[matching]") {
    std::mt19937 rng(327285451);
    std::uniform_int_distribution<int> distrSz(1, 5);
    int n = 10;
    std::vector<int> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = i;
    }
    std::vector<bool> validLeft(n), validRight(n);
    for (int cas = 1; cas <= 5; ++cas) {
        std::vector<std::pair<int, int>> edges;
        for (int i = 0; i < n; ++i) {
            int sz = distrSz(rng);
            std::shuffle(vec.begin(), vec.end(), rng);
            for (int j = 0; j < sz; ++j) {
                edges.emplace_back(i, vec[j]);
            }
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        BipartiteMatching bm(n, n);
        for (const auto& [a, b] : edges) {
            bm.addEdge(a, b);
        }
        bm.computeMaximumMatching();
        REQUIRE(isValidMatchingForEdges(bm, edges));
        for (int mask = 1; mask < (1 << n); ++mask) {
            for (int submask = mask; submask >= 1; submask = ((submask - 1) & mask)) {
                std::vector<std::pair<int, int>> curEdges;
                for (int i = 0; i < n; ++i) {
                    if (!(mask & (1 << i))) {
                        validLeft[i] = false;
                        validRight[i] = false;
                        bm.removeVertexA(i);
                        bm.removeVertexB(i);
                    }
                    else if (!(submask & (1 << i))) {
                        validLeft[i] = true;
                        validRight[i] = false;
                        bm.addVertexA(i);
                        bm.removeVertexB(i);
                    }
                    else {
                        validLeft[i] = false;
                        validRight[i] = true;
                        bm.removeVertexA(i);
                        bm.addVertexB(i);
                    }
                }
                for (const auto& [a, b] : edges) {
                    if (validLeft[a] && validRight[b]) {
                        curEdges.emplace_back(a, b);
                    }
                }
                BipartiteMatching curBM(n, n);
                for (const auto& [a, b] : curEdges) {
                    curBM.addEdge(a, b);
                }
                REQUIRE(bm.computeMaximumMatching() == curBM.computeMaximumMatching());
                REQUIRE(isValidMatchingForEdges(bm, curEdges));
                REQUIRE(isValidMatchingForEdges(curBM, curEdges));
                REQUIRE(validLeft == bm.validLeft);
                REQUIRE(validRight == bm.validRight);
            }
        }
    }

}
