#include <broadcast/algorithms/enumeration.hpp>
#include <broadcast/utilities/spanning_trees.hpp>
#include <broadcast/utilities/solve_tree.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>

namespace broadcast {

BroadcastingProtocol algorithmEnumerateAllTrees(const Graph& g, bool verbose) {
    Timer totalTimer;

    if (verbose) {
        printHeader("Running the enumeration broadcasting algorithm");
    }

    BroadcastingProtocol best;
    best.rounds.resize(g.n);

    long long numberOfSpanningTrees = 0;

    SpanningTreeIterator iter(g);

    // enumerate all spanning trees
    while (iter.hasNext()) {
        std::vector<int> T = iter.next();
        numberOfSpanningTrees += 1;

        std::vector<bool> edgeInT(g.edges.size(), false);
        for (int eid : T) {
            edgeInT[eid] = true;
        }

        // find all edges that are not contained in current spanning tree
        std::vector<int> ignoreEdges;
        for (int i = 0; i < (int) g.edges.size(); ++i) {
            if (!edgeInT[i]) {
                ignoreEdges.push_back(i);
            }
        }

        // solve broadcasting problem on current spanning tree
        BroadcastingProtocol cur = solveOnTree(g, ignoreEdges);

        // update the best solution
        if (cur.rounds.size() < best.rounds.size()) {
            best = cur;
        }
    }

    if (verbose) {
        double totalTime = totalTimer.elapsed();
        printInfo("Spanning trees enumerated", numberOfSpanningTrees);
        printInfo("Optimal broadcasting rounds", static_cast<unsigned long long>(best.rounds.size()));
        printEmptyLine();
        printInfo("Total algorithm time", totalTime);
    }

    return best;
}

} // namespace broadcast
