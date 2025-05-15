#ifndef BROADCAST_UTILITIES_PROTOCOL_HPP
#define BROADCAST_UTILITIES_PROTOCOL_HPP

#include <broadcast/utilities/graph.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace broadcast {

/**
 * @brief Represents a broadcasting protocol on a graph.
 */
struct BroadcastingProtocol {
    // rounds[i] is list of (u, v) pairs, meaning u informs v in round i+1
    std::vector<std::vector<std::pair<int, int>>> rounds;

    bool operator==(const BroadcastingProtocol& other) const;
};

/**
 * @brief Reads a broadcasting protocol from an input stream.
 *
 * The input format is:
 *   r
 *   ROUND_1
 *   ROUND_2
 *   ...
 *   ROUND_r
 *
 * where r is the number of rounds, and each ROUND_i is formatted as:
 *   k
 *   u1 v1
 *   u2 v2
 *   ...
 *   uk vk
 *
 * where k is the number of transmissions in round i, and each line "uj vj"
 * means vertex uj informs vertex vj in that round.
 *
 * @param in Input stream (default: std::cin)
 * @return Parsed BroadcastingProtocol object
 */
BroadcastingProtocol readBroadcastingProtocol(std::istream& in = std::cin);

/**
 * @brief Prints a broadcasting protocol to an output stream.
 *
 * The output format is:
 *   r
 *   ROUND_1
 *   ROUND_2
 *   ...
 *   ROUND_r
 *
 * where r is the number of rounds, and each ROUND_i is formatted as:
 *   k
 *   u1 v1
 *   u2 v2
 *   ...
 *   uk vk
 *
 * where k is the number of transmissions in round i, and each line "uj vj"
 * means vertex uj informs vertex vj in that round.
 *
 * @param protocol The broadcasting protocol to print
 * @param out The output stream to write to (default is std::cout)
 */
void printBroadcastingProtocol(const BroadcastingProtocol& bp, std::ostream& out = std::cout);

/**
 * @brief Verifies if the broadcasting protocol is valid for a given graph.
 *
 * Valid means:
 *   - If u informs v, then {u, v} is an edge in the graph.
 *   - If u informs v, then u must be either the source or have already been informed in a previous round.
 *   - The source is never informed; all other vertices must be informed exactly once.
 *   - Each vertex may inform at most one other vertex per round.
 *   - Each round at least one vertex is informed.
 *
 * @param bp The broadcasting protocol
 * @param g The underlying graph
 * @return true if the protocol is valid for the graph, false otherwise
 */
bool isValidBroadcastingProtocol(const BroadcastingProtocol& bp, const Graph& g);

} // namespace broadcast

#endif // BROADCAST_UTILITIES_PROTOCOL_HPP
