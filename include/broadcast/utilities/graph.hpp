#ifndef BROADCAST_UTILITIES_GRAPH_HPP
#define BROADCAST_UTILITIES_GRAPH_HPP

#include <vector>
#include <utility>
#include <string>
#include <iostream>

namespace broadcast {

/**
 * @brief Represents an undirected graph with numbered vertices and a source.
 *
 * Vertices are numbered from 0 to n-1.
 * The graph is stored as a list of undirected edges {u, v}.
 */
struct Graph {
    int n; // Number of vertices
    int s; // Source vertex
    std::vector<std::pair<int, int>> edges; // List of undirected edges

    bool operator==(const Graph& other) const;
};

/**
 * @brief Reads a graph from an input stream.
 *
 * The input format is:
 *   n m s
 *   u1 v1
 *   u2 v2
 *   ...
 *   um vm
 *
 * where:
 *   - n is the number of vertices (numbered 0 to n-1)
 *   - s is the source vertex
 *   - m is the number of edges
 *   - each (ui, vi) is an undirected edge
 *
 * @param in The input stream to read from (default is std::cin)
 * @return A Graph object populated with the input data
 */
Graph readGraph(std::istream& in = std::cin);

/**
 * @brief Prints a graph to an output stream.
 *
 * The output format matches the input format of readGraph:
 *   n m s
 *   u1 v1
 *   ...
 *   um vm
 *
 * where:
 *   - n is the number of vertices (numbered 0 to n-1)
 *   - s is the source vertex
 *   - m is the number of edges
 *   - each (ui, vi) is an undirected edge
 *
 * @param g The graph to print
 * @param out The output stream to write to (default is std::cout)
 */
void printGraph(const Graph& g, std::ostream& out = std::cout);

/**
 * @brief Verifies if a graph is simple and connected.
 *
 * @param g The graph to check
 * @return true if the graph is simple and connected, false otherwise
 */
bool isSimpleAndConnected(const Graph& g);

} // namespace broadcast

#endif // BROADCAST_UTILITIES_GRAPH_HPP
