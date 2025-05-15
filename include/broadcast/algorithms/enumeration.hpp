#ifndef BROADCAST_ALGORITHM_ENUMERATE_HPP
#define BROADCAST_ALGORITHM_ENUMERATE_HPP

#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/graph.hpp>

namespace broadcast {

/**
 * @brief Solves the broadcasting problem by enumerating all spanning trees and
 *   solving the problem on each tree in linear time.
 *
 * @param g The input graph.
 *
 * @return A BroadcastingProtocol object representing the optimal solution.
 *
 * Time Complexity:
 *  - O(n^k * m), where n is the number of vertices, m is the number of edges
 *    and k is the cyclomatic number
 */
BroadcastingProtocol algorithmEnumerateAllTrees(const Graph& g, bool verbose = false);

} // namespace broadcast

#endif // BROADCAST_ALGORITHM_ENUMERATE_HPP
