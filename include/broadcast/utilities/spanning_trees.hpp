#ifndef BROADCAST_UTILITIES_SPANNING_TREES_HPP
#define BROADCAST_UTILITIES_SPANNING_TREES_HPP

#include <broadcast/utilities/graph.hpp>

#include <vector>

namespace broadcast {

/**
 * @brief Iterator over all spanning trees of an undirected graph.
 *
 * This class enumerates all possible spanning trees of an undirected graph
 * using the Gabow-Myers algorithm, as described in their paper:
 * "Finding All Spanning Trees of Directed and Undirected Graphs" (1978).
 *
 * Usage:
 * SpanningTreeIterator iter(graph);
 * while (iter.hasNext()) {
 *     std::vector<int> tree = iter.next();
 *     // process tree
 * }
 *
 * @param g The input undirected graph.
 * @complexity Time: O(V + E + EN), where V = number of vertices,
 * E = number of edges, and N = number of spanning trees.
 * Space: O(V + E).
 */
class SpanningTreeIterator {
public:
    SpanningTreeIterator(const Graph& _g);

    bool hasNext() const;
    std::vector<int> next();

private:
    int n, m, r;
    std::vector<std::pair<int, int>> edges;

    std::vector<std::vector<std::pair<int, int>>> outgoing;
    std::vector<std::vector<std::pair<int, int>>> incoming;

    std::vector<bool> edgeInG;

    std::vector<int> T;
    std::vector<bool> vertexInT;
    std::vector<std::vector<int>> tree;

    std::vector<int> F;
    std::vector<bool> edgeInF;
    std::vector<bool> edgeActiveInF;

    std::vector<int> lastTreeWalkStartTime;
    std::vector<int> lastTreeWalkEndTime;

    std::vector<std::pair<int, std::vector<std::pair<int, bool>>>> recursionStack;

    bool isWDescendentOfV(int w, int v);
    void performNewTreeWalk();

    bool done;
    void advance();
};

} // namespace broadcast

#endif // BROADCAST_UTILITIES_SPANNING_TREES_HPP
