#ifndef BROADCAST_UTILITIES_GENERATOR_HPP
#define BROADCAST_UTILITIES_GENERATOR_HPP

#include <broadcast/utilities/graph.hpp>

#include <vector>
#include <utility>

namespace broadcast {

/**
 * @brief Generates a random, undirected, connected, and simple graph with n vertices and cyclomatic number k.
 *
 * @param n Number of vertices (must be >= 1)
 * @param k Cyclomatic number (must satisfy 0 <= k <= n * (n - 1) / 2 - n + 1)
 * @param seed Random seed
 *
 * @return Graph object with n vertices and n - 1 + k edges
 *
 * @throws std::invalid_argument if constraints are violated
 */
Graph generateRandomGraph(int n, int k, unsigned long long seed);

/**
 * @brief Generates an undirected, connected, and simple perfect broadcasting graph
 * with n vertices and cyclomatic number k. A perfect broadcasting graph is a graph
 * such that there exists a valid broadcasting protocol that consists of
 * ceil(log_2(n)) rounds.
 *
 * @param n Number of vertices (must be >= 1)
 * @param k Cyclomatic number (must satisfy 0 <= k <= n * (n - 1) / 2 - n + 1)
 * @param seed Random seed
 *
 * @return Graph object with n vertices and n - 1 + k edges
 *
 * @throws std::invalid_argument if constraints are violated
 */
Graph generateRandomPerfectBroadcastingGraph(int n, int k, unsigned long long seed);

} // namespace broadcast

#endif // BROADCAST_UTILITIES_GENERATOR_HPP

