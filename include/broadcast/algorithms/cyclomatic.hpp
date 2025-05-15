#ifndef BROADCAST_ALGORITHM_CYCLOMATIC_HPP
#define BROADCAST_ALGORITHM_CYCLOMATIC_HPP

#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/graph.hpp>

namespace broadcast {

/**
 * @brief Solves the broadcasting problem on graphs with small cyclomatic number.
 *
 * It is an implementation of the algorithm presented in the paper:
 *
 * "Parameterized complexity of broadcasting in graphs" by Fedor V. Fomin, Pierre Fraigniaud,
 * and Petr A. Golovach (2024).
 *
 * @param g The input graph.
 * @param verbose Whether the algorithm should print extra information and performance diagnostics
 *
 * @return A BroadcastingProtocol object representing the optimal solution.
 *
 * Time Complexity:
 *  - O(kn) * 2^{O(k log k)}, where n is the number of vertices and k is the cyclomatic number.
 */
BroadcastingProtocol algorithmCyclomatic(const Graph& g, bool verbose = false);

} // namespace broadcast

#endif // BROADCAST_ALGORITHM_CYCLOMATIC_HPP
