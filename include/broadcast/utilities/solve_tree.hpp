#ifndef BROADCAST_UTILITIES_SOLVE_TREE_HPP
#define BROADCAST_UTILITIES_SOLVE_TREE_HPP

#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/graph.hpp>

#include <vector>

namespace broadcast {

/**
 * @brief Computes an optimal broadcasting protocol on a tree. 
 *
 * @param g The input graph.
 * @param ignoreEdges A list of edge indices to ignore (default is empty).
 *
 * @return An optimal BroadcastingProtocol
 *
 * @note Assumes the resulting graph (after ignoring edges) is a tree.
 *
 * Time Complexity: O(m), where m is the number of edges of g.
 */
BroadcastingProtocol solveOnTree(const Graph& g, const std::vector<int>& ignoreEdges = {});

} // namespace broadcast

#endif // BROADCAST_UTILITIES_SOLVE_TREE_HPP
