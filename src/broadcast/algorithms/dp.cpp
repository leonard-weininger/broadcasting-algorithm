#include <broadcast/algorithms/dp.hpp>
#include <broadcast/utilities/matching.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

namespace broadcast {

BroadcastingProtocol algorithmDP(const Graph& g, bool verbose) {
    Timer totalTimer;

    if (verbose) {
        printHeader("Running the dp broadcasting algorithm");
    }

    // dp[subset] is min time to broadcast the message to all of vertices of the subset
    std::vector<int> dp(1 << g.n, g.n);
    dp[1 << g.s] = 0;

    BipartiteMatching bm(g.n, g.n);

    for (const auto& [u, v] : g.edges) {
        bm.addEdge(u, v);
        bm.addEdge(v, u);
    }

    // iterate over all subsets of the vertices, a subset is represented as a bitmask
    for (int mask = 1; mask < (1 << g.n); ++mask) {
        // if the subset does not contain the source you can continue
        if (!(mask & (1 << g.s))) continue;

        // iterate over all subsubsets of the current subset
        for (int submask = mask; submask >= 1; submask = ((submask - 1) & mask)) {
            // submask can not improve the current best time of mask
            if (dp[submask] + 1 >= dp[mask]) continue;

            // sizeA = size of the set of already informed vertices
            int sizeA = 0;

            // sizeB = size of the set of vertices that need to be informed
            int sizeB = 0;

            // find the sets of already informed vertices A
            // and set of vertices that need to be informed B
            for (int i = 0; i < g.n; ++i) {
                if ((mask ^ submask) & (1 << i)) {
                    sizeB += 1;
                    bm.removeVertexA(i);
                    bm.addVertexB(i);
                }
                else if (submask & (1 << i)) {
                    sizeA += 1;
                    bm.addVertexA(i);
                    bm.removeVertexB(i);
                }
                else {
                    bm.removeVertexA(i);
                    bm.removeVertexB(i);
                }
            }

            // impossible to inform all B from A in one round
            if (sizeA < sizeB) continue;

            // is it possible to inform all vertices of B in one round
            if (bm.computeMaximumMatching() == sizeB) {
                dp[mask] = dp[submask] + 1;
            }
        }
    }

    int mask = (1 << g.n) - 1;
    BroadcastingProtocol ans;

    // reconstruct the optimal broadcasting protocol by doing the above process in reverse
    while (mask != (1 << g.s)) {
        for (int submask = mask; submask >= 1; submask = ((submask - 1) & mask)) {
            if (dp[submask] >= dp[mask]) continue;
            int sizeA = 0;
            int sizeB = 0;
            for (int i = 0; i < g.n; ++i) {
                if ((mask ^ submask) & (1 << i)) {
                    sizeB += 1;
                    bm.removeVertexA(i);
                    bm.addVertexB(i);
                }
                else if (submask & (1 << i)) {
                    sizeA += 1;
                    bm.addVertexA(i);
                    bm.removeVertexB(i);
                }
                else {
                    bm.removeVertexA(i);
                    bm.removeVertexB(i);
                }
            }
            if (sizeA < sizeB) continue;
            if (bm.computeMaximumMatching() == sizeB) {
                ans.rounds.push_back(std::vector<std::pair<int, int>>());
                for (int i = 0; i < g.n; ++i) {
                    if ((mask ^ submask) & (1 << i)) {
                        ans.rounds.back().emplace_back(bm.matchRight[i], i);
                    }
                }
                mask = submask;
                break;
            }
        }
    }
    std::reverse(ans.rounds.begin(), ans.rounds.end());

    if (verbose) {
        double totalTime = totalTimer.elapsed();
        printInfo("Optimal broadcasting rounds", static_cast<unsigned long long>(ans.rounds.size()));
        printEmptyLine();
        printInfo("Total algorithm time", totalTime);
    }

    return ans;
}

} // namespace broadcast
