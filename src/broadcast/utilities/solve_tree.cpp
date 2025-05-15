#include <broadcast/utilities/solve_tree.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace broadcast {

BroadcastingProtocol solveOnTree(const Graph& g, const std::vector<int>& ignoreEdges) {
    if (g.n == 1) {
        BroadcastingProtocol ans;
        return ans;
    }

    std::vector<bool> edgeOk(g.edges.size(), true);
    for (int eid : ignoreEdges) edgeOk[eid] = false;

    std::vector<std::vector<int>> adj(g.n);
    std::vector<int> deg(g.n, 0);

    // build adjacency lists and list of degrees of all vertices
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        if (edgeOk[i]) {
            auto [u, v] = g.edges[i];
            adj[u].push_back(v);
            adj[v].push_back(u);
            deg[u] += 1;
            deg[v] += 1;
        }
    }

    for (int v = 0; v < g.n; ++v) {
        if (deg[v] == 0) {
            throw std::runtime_error("The given graph is not connected.");
        }
    }

    // dp[v] = time needed to inform subtree rooted at v
    std::vector<int> dp(g.n, -1);

    // leavesByTime[t] = list of all vertices v, dp[v] = t 
    std::vector<std::vector<int>> leavesByTime(g.n);

    // children[v] is a list of all children of v. The children are
    // initially inserted in increasing number of dp[v], but later
    // their order is reversed
    std::vector<std::vector<int>> children(g.n);

    for (int v = 0; v < g.n; ++v) {
        if (deg[v] == 1 && v != g.s) {
            dp[v] = 0;
            leavesByTime[dp[v]].push_back(v);
        }
    }

    // iterate over all leaves v in increasing dp[v]
    for (int t = 0; t < g.n; ++t) {
        for (int v : leavesByTime[t]) {
            if (deg[v] != 1) {
                throw std::runtime_error("The given graph is not connected.");
            }
            for (int u : adj[v]) {
                if (deg[u] == 0) continue;
                // add v to the list of children of u
                children[u].push_back(v);

                // delete the leaf v
                deg[u] -= 1;
                deg[v] -= 1;

                // check if u becomes a leaf after the deletion of v
                if (deg[u] == 1 && u != g.s) {
                    // compute dp[u]
                    std::reverse(children[u].begin(), children[u].end());
                    dp[u] = 0;
                    for (int i = 0; i < (int) children[u].size(); ++i) {
                        dp[u] = std::max(dp[u], dp[children[u][i]] + 1 + i);
                    }
                    leavesByTime[dp[u]].push_back(u);
                }
            }
        }
    }
    
    for (int v = 0; v < g.n; ++v) {
        if (deg[v] >= 1) {
            throw std::runtime_error("The given graph contains cycles.");
        }
    }

    // compute dp[source]
    std::reverse(children[g.s].begin(), children[g.s].end());
    dp[g.s] = 0;
    for (int i = 0; i < (int) children[g.s].size(); ++i) {
        dp[g.s] = std::max(dp[g.s], dp[children[g.s][i]] + 1 + i);
    }

    // t[v] = time that v is informed in an optimal broadcasting protocol
    std::vector<int> t(g.n);
    t[g.s] = 0;

    // stack of currently informed vertices
    std::vector<int> st;
    st.push_back(g.s);

    BroadcastingProtocol ans;
    ans.rounds.resize(dp[g.s]);

    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        // v informs its children u in decreasing order of dp[u]
        for (int i = 0; i < (int) children[v].size(); ++i) {
            int u = children[v][i];
            ans.rounds[t[v] + i].emplace_back(v, u);
            t[u] = t[v] + 1 + i;
            st.push_back(u);
        }
    }

    return ans;
}

} // namespace broadcast
