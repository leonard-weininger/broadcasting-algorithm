#ifndef BROADCAST_ALGORITHM_DP_HPP
#define BROADCAST_ALGORITHM_DP_HPP

#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/graph.hpp>

namespace broadcast {

/**
 * @brief Solves the broadcasting problem using dynamic programming.
 *
 * It is based on the work presented in the paper:
 *
 * "Parameterized complexity of broadcasting in graphs" by Fedor V. Fomin, Pierre Fraigniaud,
 * and Petr A. Golovach (2024).
 *
 * @param g The input graph.
 * @return A BroadcastingProtocol object representing the optimal solution.
 *
 * Time Complexity:
 *  - O(3^n * m), where n is the number of vertices and m is the number of edges.
 */
BroadcastingProtocol algorithmDP(const Graph& g, bool verbose = false);

} // namespace broadcast

#endif // BROADCAST_ALGORITHM_DP_HPP
