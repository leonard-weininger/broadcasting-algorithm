#ifndef BROADCAST_UTILITIES_MATCHING_HPP
#define BROADCAST_UTILITIES_MATCHING_HPP

#include <vector>

namespace broadcast {

/**
 * @brief Bipartite graph maximum matching solver using the Hopcroft-Karp algorithm.
 *
 * This class represents a bipartite graph between two disjoint sets A and B and provides
 * methods to construct the graph and compute the maximum matching.
 */
struct BipartiteMatching {
    const int n, m;
    int matchingSize;
    std::vector<std::vector<int>> adj;
    std::vector<bool> validLeft, validRight;
    std::vector<int> matchLeft, matchRight, que, parent, root;

    /**
     * @brief Constructs a bipartite matcher for a graph with n nodes on the left (A) and m nodes on the right (B).
     *
     * @param n Number of vertices in set A.
     * @param m Number of vertices in set B.
     */
    BipartiteMatching(int _n, int _m);

    /**
     * @brief Adds an undirected edge between vertex a in set A and vertex b in set B.
     *
     * @param a Index of vertex in A (0-based).
     * @param b Index of vertex in B (0-based).
     */
    void addEdge(int a, int b);

    /**
     * @brief Removes vertex a in set A from the graph (disables it for matching).
     *
     * @param a Index of vertex in A.
     */
    void removeVertexA(int a);

    /**
     * @brief Removes vertex b in set B from the graph (disables it for matching).
     *
     * @param b Index of vertex in B.
     */
    void removeVertexB(int b);

    /**
     * @brief Adds vertex a in set A to the graph (enables it for matching).
     *
     * @param a Index of vertex in A.
     */
    void addVertexA(int a);

    /**
     * @brief Adds vertex b in set B to the graph (enables it for matching).
     *
     * @param b Index of vertex in B.
     */
    void addVertexB(int b);


    /**
     * @brief Computes the maximum matching in a bipartite graph using the Hopcroft-Karp algorithm.
     *
     * @return The size of the maximum matching.
     *
     * Time Complexity:
     *  - Initial call: O(sqrt(n) * m), where n is the number of vertices and m is the number of edges.
     *  - Subsequent calls: O(min(sqrt(n), x) * m), where x is the number of times addVertexA,
     *    addVertexB, removeVertexA, or removeVertexB have been called since the last
     *    computeMaximumMatching call.
     */
    int computeMaximumMatching();
};

} // namespace broadcast

#endif // BROADCAST_UTILITIES_MATCHING_HPP

