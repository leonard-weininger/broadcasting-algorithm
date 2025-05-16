#include <broadcast/algorithms/cyclomatic.hpp>
#include <broadcast/utilities/spanning_trees.hpp>
#include <broadcast/utilities/solve_tree.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

namespace broadcast {

// ============= Main Algorithm starts at line 672 ==================

/**
 * For a given graph, it computes a minimal feedback edge set.
 * That is a smallest set of edge indices, such that after
 * removing them from the graph, the graph becomes acyclic.
 *
 * It runs a BFS and all edges that are not part of the BFS-tree,
 * will be included in the minimal feedback edge set.
 */
std::vector<int> getMinimalFeedbackEdgeSet(const Graph& g,
        const std::vector<std::vector<std::pair<int, int>>>& adj) {
    std::vector<bool> isEdgeInBFSTree(g.edges.size(), false);

    std::vector<bool> visited(g.n, false);
    visited[0] = true;

    std::queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto [u, eid] : adj[v]) {
            if (!visited[u]) {
                isEdgeInBFSTree[eid] = true;
                visited[u] = true;
                q.push(u);
            }
        }
    }

    std::vector<int> feedbackEdgeSet;
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        if (!isEdgeInBFSTree[i]) feedbackEdgeSet.push_back(i);
    }
    return feedbackEdgeSet;
}

/**
 * For a given graph g, and a given list of edge indices S, where
 * S is a feedback edge set of g, it computes a list of vertices U
 * that satisfies the following constraints:
 *  - the source g.s is in U
 *  - all vertices v that are incident to some edge in S, are in U
 *  - all vertices v that have three or more distinct paths to other
 *    vertices in U, are also in U
 */
std::vector<int> constructU(const Graph& g, 
        const std::vector<std::vector<std::pair<int, int>>>& adj,
        const std::vector<int>& S) {
    std::vector<bool> inU(g.n, false);
    inU[g.s] = true;

    for (int i : S) {
        inU[g.edges[i].first] = true;
        inU[g.edges[i].second] = true;
    }

    std::vector<int> deg(g.n, 0);
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        const auto& [u, v] = g.edges[i];
        if (!inU[u] || !inU[v]) {
            deg[u] += 1;
            deg[v] += 1;
        }
    }

    std::vector<int> leavesNotInU;
    for (int v = 0; v < g.n; ++v) {
        if (!inU[v]) {
            assert(deg[v] >= 1);
            if (deg[v] == 1) {
                leavesNotInU.push_back(v);
            }
        }
    }

    while (!leavesNotInU.empty()) {
        int v = leavesNotInU.back();
        leavesNotInU.pop_back();
        for (const auto& [u, eid] : adj[v]) {
            if (deg[u] == 0) continue;
            deg[u] -= 1;
            deg[v] -= 1;
            if (!inU[u]) {
                assert(deg[u] >= 1);
                if (deg[u] == 1) {
                    leavesNotInU.push_back(u);
                }
            }
        }
    }

    std::vector<int> U;
    for (int v = 0; v < g.n; ++v) {
        if (deg[v] >= 3) inU[v] = true;
        if (inU[v]) U.push_back(v);
    }
    return U;
}

/**
 * Represents an xy-Tree from x to y, where vx and vy are the neighbours
 * of x and y respectively. The number of vertices in the tree is 'size'.
 */
struct XYTree {
    int x, vx, y, vy, size;
};

/**
 * For a given xyTree it computes in linear time a vector bh, where
 * - bh[0][t] = min time to inform y from x when all vertices in the tree must be informed in at most t rounds
 * - bh[1][t] = same but inform x from y
 * - if it is impossible then bh[0/1][t] = g.n
 */
std::vector<std::vector<int>> computeBH(const Graph& g,
        const XYTree& xyTree,
        std::vector<int> xyPath,
        const std::vector<std::vector<int>>& children,
        const std::vector<int>& dp) {
    std::vector<std::vector<int>> bh(2, std::vector<int>(xyTree.size, g.n));

    for (int j = 0; j < 2; ++j) {
        int curT = 0;

        for (int k = (int) xyPath.size() - 2; k >= 1; --k) {
            assert(k < (int) xyPath.size() - 1);
            int addT = 0;
            int nxtT = 0;
            bool inserted = false;
            for (int c : children[xyPath[k]]) {
                if (curT > dp[c] && !inserted) {
                    nxtT = std::max(nxtT, 1 + curT + addT);
                    addT += 1;
                    inserted = true;
                }
                nxtT = std::max(nxtT, 1 + dp[c] + addT);
                addT += 1;
            }
            if (!inserted) {
                nxtT = std::max(nxtT, 1 + curT + addT);
                addT += 1;
            }
            curT = nxtT;
        }

        int critT = curT + 1;

        for (int t = 0; t < critT; ++t) {
            bh[j][t] = g.n;
        }

        curT = 1;
        for (int k = 1; k < (int) xyPath.size(); ++k) {
            if (k == (int) xyPath.size() - 1) {
                bh[j][critT] = curT;
                break;
            }
            std::vector<int> vt;
            for (int c : children[xyPath[k]]) {
                vt.push_back(dp[c] + 1);
            }
            int timeNeed = 0;
            for (int l = 0; l < (int) vt.size(); ++l) {
                timeNeed = std::max(timeNeed, vt[l] + l);
            }
            timeNeed += 1;
            int firstFree = g.n;
            for (int l = (int) vt.size() - 1; l >= 0; --l) {
                while (curT + timeNeed + vt[l] > critT && timeNeed >= 0) {
                    firstFree = timeNeed;
                    timeNeed -= 1;
                }
                if (timeNeed < 0) {
                    firstFree = g.n;
                    break;
                }
                timeNeed -= 1;
            }
            if (timeNeed >= 0) firstFree = 0;
            if (curT + firstFree + 1 > critT) {
                bh[j][critT] = g.n;
                break;
            }
            curT += firstFree + 1;
        }

        for (int t = critT + 1; t < xyTree.size; ++t) {
            bh[j][t] = (int) xyPath.size() - 1;
        }

        std::reverse(xyPath.begin(), xyPath.end());
    }

    return bh;
}

/**
 * For a given xyTree it computes in linear time the vectors dh and dhRemove, where
 * - dh[0][t] = min time to inform the xyTree when x starts sending at time 0 and y at time t
 * - dh[1][t] = min time to inform the xyTree when y starts sending at time 0 and x at time t
 * - dhRemoveEdge[0/1][t] is the index of an edge on the path from x to y such that the optimal
 *   time dh[0/1][t] is achieved by removing this edge and informing the two resulting subtrees
 *   from x and y in optimal time
 */
void computeDH(const XYTree& xyTree,
        std::vector<int> xyPath,
        const std::vector<int>& pathEdges,
        const std::vector<std::vector<int>>& children,
        const std::vector<int>& dp,
        std::vector<std::vector<int>>& dh,
        std::vector<std::vector<int>>& dhRemoveEdge) {
    std::vector<int> tx(pathEdges.size()), ty(pathEdges.size());
    tx[0] = ty[0] = 0;

    for (int l = 0; l < 2; ++l) {
        std::vector<std::vector<int>> ct(pathEdges.size());
        std::vector<int> pre(pathEdges.size());
        std::vector<int> nxt(pathEdges.size());
        std::vector<int> pos(pathEdges.size());

        pre[0] = -1;
        nxt[0] = -1;
        pos[0] = -1;

        for (int j = 1; j < (int) pathEdges.size(); ++j) {
            pre[j] = j - 1;
            nxt[j - 1] = j;
            nxt[j] = -1;
            ct[j - 1].push_back(0);
            pos[j] = (int) ct[j - 1].size() - 1;
            for (int i = 0; i < (int) children[xyPath[j]].size(); ++i) {
                ct[j - 1].back() = std::max(ct[j - 1].back(), dp[children[xyPath[j]][i]] + 1 + i);
                ct[j].push_back(dp[children[xyPath[j]][i]]);
            }

            int curJ = j - 1;
            int curPos = pos[j];

            while (curJ >= 1) {
                int preJ = pre[curJ];
                int prePos = pos[curJ];
                int preVal = ct[preJ][prePos];
                int addT = curJ - preJ;
                bool curPosChange = false;

                while (curPos >= 1 && ct[curJ][curPos - 1] <= ct[curJ][curPos]) {
                    std::swap(ct[curJ][curPos - 1], ct[curJ][curPos]);
                    ct[preJ][prePos] = std::max(ct[preJ][prePos],  ct[curJ][curPos] + curPos + addT);
                    assert(pos[nxt[curJ]] == curPos);
                    pos[nxt[curJ]] -= 1;
                    curPos -= 1;
                    curPosChange = true;
                }
                ct[preJ][prePos] = std::max(ct[preJ][prePos], ct[curJ][curPos] + curPos + addT);

                if (preJ == 0) break;
                if (ct[preJ][prePos] == preVal) break;
                if (prePos >= 1 && ct[preJ][prePos] < ct[preJ][prePos - 1]) break;

                if (curPos == 0 && !curPosChange) {
                    nxt[preJ] = nxt[curJ];
                    pre[nxt[curJ]] = pre[curJ];
                    pos[nxt[curJ]] = pos[curJ];
                }

                curJ = preJ;
                curPos = prePos;
            }

            tx[j] = 1 + ct[0][0];
        }

        std::reverse(xyPath.begin(), xyPath.end());
        std::swap(tx, ty);
    }
    std::reverse(ty.begin(), ty.end());

    dh.assign(2, std::vector<int>(xyTree.size));
    dhRemoveEdge.assign(2, std::vector<int>(xyTree.size));

    for (int t = 0, j0 = 0, j1 = (int) pathEdges.size() - 1; t < xyTree.size; ++t) {
        dh[0][t] = std::max(tx[j0], ty[j0] + t);
        dhRemoveEdge[0][t] = pathEdges[j0];

        while (j0 + 1 < (int) pathEdges.size() && dh[0][t] >= std::max(tx[j0 + 1], ty[j0 + 1] + t)) {
            j0 += 1;
            dh[0][t] = std::max(tx[j0], ty[j0] + t);
            dhRemoveEdge[0][t] = pathEdges[j0];
        }

        if (dh[0][t] > tx.back()) {
            dh[0][t] = tx.back();
            dhRemoveEdge[0][t] = pathEdges.back();
        }

        dh[1][t] = std::max(ty[j1], tx[j1] + t);
        dhRemoveEdge[1][t] = pathEdges[j1];

        while (j1 - 1 >= 0 && dh[1][t] >= std::max(ty[j1 - 1], tx[j1 - 1] + t)) {
            j1 -= 1;
            dh[1][t] = std::max(ty[j1], tx[j1] + t);
            dhRemoveEdge[1][t] = pathEdges[j1];
        }

        if (dh[1][t] > ty[0]) {
            dh[1][t] = ty[0];
            dhRemoveEdge[1][t] = pathEdges[0];
        }
    }
}

/**
 * Finds all xTrees and xyTrees and:
 *
 *  - For all x in U it:
 *     - computes the time needed to inform all xTrees rooted at x,
 *       it is safed in timeNeededForXTrees[x]
 *     - let BP be an optimal broadcasting protocol for the subgraph
 *       that includes x and all incident xTrees, such that all neighbours
 *       of x get informed as late as possible (while still guaranteeing an
 *       optimal broadcasting time). Then freeChildrenPositions[x] is
 *       a list of all times at which x does not inform any vertex and the
 *       last entry of freeChildrenPositions[x] is 1 plus the last time
 *       that x informs any of its xTree neighbours
 *
 * - For all xyTrees it computes the vectors bh and dh which are described
 *   in their own function
 */
void preProcessXTreesAndXYTrees(const Graph& g,
        const std::vector<std::vector<std::pair<int, int>>>& adj,
        const std::vector<int>& U,
        const std::vector<int>& positionInU,
        std::vector<int>& timeNeededForXTrees,
        std::vector<std::vector<int>>& freeChildrenPositions,
        std::vector<XYTree>& xyTrees,
        std::vector<std::vector<std::vector<int>>>& bh, 
        std::vector<std::vector<std::vector<int>>>& dh,
        std::vector<std::vector<std::vector<int>>>& dhRemoveEdge) {
    std::vector<int> deg(g.n, 0);

    for (int i = 0; i < (int) g.edges.size(); ++i) {
        const auto& [u, v] = g.edges[i];
        if (positionInU[u] == -1 || positionInU[v] == -1) {
            deg[u] += 1;
            deg[v] += 1;
        }
    }

    std::vector<std::vector<int>> leavesByTime(g.n);
    std::vector<std::vector<int>> children(g.n);
    std::vector<int> dp(g.n, -1);
    std::vector<int> subtreeSize(g.n, 1);

    for (int v = 0; v < g.n; ++v) {
        if (deg[v] == 1 && positionInU[v] == -1) {
            dp[v] = 0;
            leavesByTime[dp[v]].push_back(v);
        }
    }

    for (int t = 0; t < g.n; ++t) {
        for (int v : leavesByTime[t]) {
            for (auto [u, eid] : adj[v]) {
                if (deg[u] == 0) continue;
                children[u].push_back(v);
                subtreeSize[u] += subtreeSize[v];
                deg[u] -= 1;
                deg[v] -= 1;
                if (deg[u] == 1 && positionInU[u] == -1) {
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

    timeNeededForXTrees.resize(U.size());
    freeChildrenPositions.resize(U.size());

    for (int i = 0; i < (int) U.size(); ++i) {
        int u = U[i];
        dp[u] = 0;

        if (children[u].empty()) {
            freeChildrenPositions[i].push_back(0);
        }
        else {
            std::reverse(children[u].begin(), children[u].end());

            for (int j = 0; j < (int) children[u].size(); ++j) {
                dp[u] = std::max(dp[u], dp[children[u][j]] + 1 + j);
            }
            timeNeededForXTrees[i] = dp[u];

            int curT = timeNeededForXTrees[i] + 1;
            for (int j = (int) children[u].size() - 1; j >= 0; --j) {
                while (dp[children[u][j]] + curT + 1 > timeNeededForXTrees[i]) {
                    freeChildrenPositions[i].push_back(curT);
                    curT -= 1;
                }
                assert(curT >= 0);
                curT -= 1;
            }
            assert(curT == -1);
            std::reverse(freeChildrenPositions[i].begin(), freeChildrenPositions[i].end());
        }
    }

    int sumXYTreeSizes = 0;

    for (int u : U) {
        for (const auto& [v, eid] : adj[u]) {
            if (deg[v] == 0) continue;
            if (positionInU[v] != -1) continue;
            std::vector<int> xyPath;
            std::vector<int> pathEdges;
            xyPath.push_back(u);
            xyPath.push_back(v);
            pathEdges.push_back(eid);
            deg[u] -= 1;
            deg[v] -= 1;
            int xyTreeSize = 2;
            while (positionInU[xyPath.back()] == -1) {
                int w = xyPath.back();
                assert(deg[w] == 1);
                xyTreeSize += subtreeSize[w];
                for (auto [w2, eid2] : adj[w]) {
                    if (deg[w2] == 0) continue;
                    if (w2 == u) continue;
                    xyPath.push_back(w2);
                    pathEdges.push_back(eid2);
                    deg[w] -= 1;
                    deg[w2] -= 1;
                }
                assert(deg[w] <= 0);
            }
            int sz = (int) xyPath.size();
            xyTrees.push_back({xyPath[0], xyPath[1], xyPath[sz - 1], xyPath[sz - 2], xyTreeSize});

            for (int i = 1; i < sz - 1; ++i) {
                std::reverse(children[xyPath[i]].begin(), children[xyPath[i]].end());
            }

            bh.push_back(computeBH(g, xyTrees.back(), xyPath, children, dp));

            dh.push_back(std::vector<std::vector<int>>());
            dhRemoveEdge.push_back(std::vector<std::vector<int>>());
            computeDH(xyTrees.back(), xyPath, pathEdges, children, dp, dh.back(), dhRemoveEdge.back());

            sumXYTreeSizes += xyTreeSize;
        }
    }

    assert(sumXYTreeSizes <= g.n + 2 * (int) xyTrees.size());
}

// from a given graph g and a list of vertices U that satisfies the above constraints
// this function constructs a multigraph G2, such that
//  - V(G2) = U
//  - for every edge {u, v} in E(g) between two vertices in U add the same edge in G2
//  - for every xyTree in g add an edge {x, y} in G2
void constructG2(const Graph& g, const std::vector<int>& U,
        const std::vector<int>& positionInU,
        const std::vector<XYTree>& xyTrees,
        Graph& G2, std::vector<int>& correspondingEdgeInG) {
    G2.n = (int) U.size();
    G2.s = positionInU[g.s];

    for (int i = 0; i < (int) g.edges.size(); ++i) {
        auto [u, v] = g.edges[i];
        if (positionInU[u] != -1 && positionInU[v] != -1) {
            G2.edges.emplace_back(positionInU[u], positionInU[v]);
            correspondingEdgeInG.push_back(i);
        }
    }
    for (int i = 0; i < (int) xyTrees.size(); ++i) {
        int x = xyTrees[i].x;
        int y = xyTrees[i].y;
        G2.edges.emplace_back(positionInU[x], positionInU[y]);
    }
}

void computeInitialCAndUsedXYTrees(const Graph& G2,
        const std::vector<int>& T2,
        const std::vector<int>& U,
        const std::vector<XYTree>& xyTrees,
        const std::vector<int>& positionInU,
        std::vector<std::vector<std::pair<int, int>>>& C,
        std::vector<bool>& usedXYTrees) {

    usedXYTrees.assign(xyTrees.size(), false);
    C.resize(U.size());

    for (int e2id : T2) {
        int u = -1;
        int v = -1;
        if (e2id < int(G2.edges.size() - xyTrees.size())) {
            u = U[G2.edges[e2id].first];
            v = U[G2.edges[e2id].second];
        }
        else {
            u = xyTrees[e2id - (G2.edges.size() - xyTrees.size())].x;
            v = xyTrees[e2id - (G2.edges.size() - xyTrees.size())].y;
            usedXYTrees[e2id - (G2.edges.size() - xyTrees.size())] = true;
        }
        assert(u != -1);
        assert(v != -1);
        assert(positionInU[u] != -1);
        assert(positionInU[v] != -1);
        C[positionInU[u]].emplace_back(positionInU[v], e2id);
        C[positionInU[v]].emplace_back(positionInU[u], e2id);
    }

    std::function<void(int, int)> dfsBuildC = [&](int v, int par) {
        for (int i = 0; i < (int) C[v].size(); ++i) {
            if (C[v][i].first == par) {
                std::swap(C[v][i], C[v].back());
                C[v].pop_back();
                break;
            }
        }
        for (auto [u, eid] : C[v]) {
            assert(u != par);
            dfsBuildC(u, v);
        }
    };

    dfsBuildC(G2.s, -1);

    for (int i = 0; i < (int) xyTrees.size(); ++i) {
        if (!usedXYTrees[i]) {
            int u = positionInU[xyTrees[i].x];
            int v = positionInU[xyTrees[i].y];
            C[u].emplace_back(v, int(G2.edges.size() - xyTrees.size() + i));
            C[v].emplace_back(u, int(G2.edges.size() - xyTrees.size() + i));
        }
    }

    for (int i = 0; i < (int) U.size(); ++i) {
        std::sort(C[i].begin(), C[i].end());
    }
}

// for a given spanning tree T2 and ordered list of children for every vertex
// and a given time limit t, it checks if the T2 with the given children orders
// can be expanded to a valid broadcasting protocol on the whole graph
bool isFeasible(const Graph& G2,
        const std::vector<int>& correspondingEdgeInG,
        const std::vector<bool>& inT2,
        const std::vector<int>& U,
        const std::vector<XYTree>& xyTrees,
        const std::vector<bool>& usedXYTrees,
        const std::vector<int>& timeNeededForXTrees,
        const std::vector<std::vector<int>>& freeChildrenPositions,
        const std::vector<std::vector<std::pair<int, int>>>& C,
        const std::vector<std::vector<std::vector<int>>>& bh, 
        const std::vector<std::vector<std::vector<int>>>& dh,
        const std::vector<std::vector<std::vector<int>>>& dhRemoveEdge,
        std::vector<int>& curRemoveEdges,
        int t) {

    std::vector<std::pair<int, int>> xyTreesStart(xyTrees.size(), std::make_pair(-1, -1));
    std::vector<int> r(U.size(), -1);
    std::function<bool(int)> dfs = [&](int v) {
        if (r[v] > t) return false;
        int timeLeft = t - r[v];
        if (timeLeft < timeNeededForXTrees[v]) return false;
        int haveExtra = timeLeft - timeNeededForXTrees[v];
        int usedExtra = 0;
        int fcPos = 0;
        int addCounter = 1;
        for (auto [u, eid] : C[v]) {
            int addTime = -1;
            if (usedExtra < haveExtra) {
                addTime = usedExtra;
                usedExtra += 1;
            }
            else if (fcPos < (int) freeChildrenPositions[v].size()) {
                addTime = haveExtra + freeChildrenPositions[v][fcPos];
                fcPos += 1;
            }
            else {
                addTime = haveExtra + freeChildrenPositions[v].back() + addCounter;
                addCounter += 1;
            }
            assert(addTime >= 0);
            if (eid >= int(G2.edges.size() - xyTrees.size())) {
                int id = eid - int(G2.edges.size() - xyTrees.size());
                if (!usedXYTrees[id]) {
                    if (U[v] == xyTrees[id].x) {
                        xyTreesStart[id].first = r[v] + addTime;
                    }
                    else {
                        assert(U[v] == xyTrees[id].y);
                        xyTreesStart[id].second = r[v] + addTime;
                    }
                }
                else {
                    int j = 0;
                    if (U[v] != xyTrees[id].x) {
                        assert(U[v] == xyTrees[id].y);
                        j = 1;
                    }
                    if (r[v] + addTime > t) return false;
                    assert(0 <= u && u < (int) r.size());
                    assert(0 <= v && v < (int) r.size());
                    assert(0 <= id && id < (int) bh.size());
                    assert(0 <= j && j < (int) bh[id].size());
                    assert(0 <= t - (r[v] + addTime));
                    if (t - (r[v] + addTime) < (int) bh[id][j].size()) {
                        r[u] = r[v] + addTime + bh[id][j][t - (r[v] + addTime)];
                    }
                    else {
                        r[u] = r[v] + addTime + bh[id][j].back();
                    }
                    if (!dfs(u)) return false;
                }
            }
            else {
                r[u] = r[v] + addTime + 1;
                if (!dfs(u)) return false;
            }
        }
        return true;
    };
    r[G2.s] = 0;
    if (!dfs(G2.s)) return false;
    curRemoveEdges.clear();
    for (int i = 0; i < (int) xyTrees.size(); ++i) {
        if (!usedXYTrees[i]) {
            auto [t1, t2] = xyTreesStart[i];
            assert(t1 != -1 && t2 != -1);
            if (t1 <= t2) {
                int pos = std::min((int) dh[i][0].size() - 1, t2 - t1);
                if (t1 + dh[i][0][pos] > t) return false;
                curRemoveEdges.push_back(dhRemoveEdge[i][0][pos]);
            }
            else {
                int pos = std::min((int) dh[i][1].size() - 1, t1 - t2);
                if (t2 + dh[i][1][pos] > t) return false;
                curRemoveEdges.push_back(dhRemoveEdge[i][1][pos]);
            }
        }
    }
    for (int i = 0; i < int(G2.edges.size() - xyTrees.size()); ++i) {
        if (!inT2[i]) curRemoveEdges.push_back(correspondingEdgeInG[i]);
    }
    return true;
}

// ============= Main Algorithm starts here ==================

BroadcastingProtocol algorithmCyclomatic(const Graph& g, bool verbose) {
    Timer totalTimer;

    if (verbose) {
        printHeader("Running the cyclomatic broadcasting algorithm");
    }

    if ((int) g.edges.size() == g.n - 1) {
        if (verbose) {
            printHeader("Input graph is a tree");
            printHeader("Using linear time tree-specific algorithm");
        }
        BroadcastingProtocol ans = solveOnTree(g);
        if (verbose) {
            double totalTime = totalTimer.elapsed();
            printInfo("Optimal broadcasting rounds", static_cast<unsigned long long>(ans.rounds.size()));
            printEmptyLine();
            printInfo("Total algorithm time", totalTime);
        }
        return ans;
    }

    // === Preprocessing Step ===
    Timer preprocessTimer;

    std::vector<std::vector<std::pair<int, int>>> adj(g.n);
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        const auto& [u, v] = g.edges[i];
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }

    std::vector<int> S = getMinimalFeedbackEdgeSet(g, adj);
    std::vector<int> U = constructU(g, adj, S);

    std::vector<int> positionInU(g.n, -1);
    for (int i = 0; i < (int) U.size(); ++i) {
        positionInU[U[i]] = i;
    }

    std::vector<int> timeNeededForXTrees;
    std::vector<std::vector<int>> freeChildrenPositions;

    std::vector<XYTree> xyTrees;
    std::vector<std::vector<std::vector<int>>> bh;
    std::vector<std::vector<std::vector<int>>> dh;
    std::vector<std::vector<std::vector<int>>> dhRemoveEdge;
    preProcessXTreesAndXYTrees(g, adj, U, positionInU, timeNeededForXTrees,
        freeChildrenPositions, xyTrees, bh, dh, dhRemoveEdge);

    std::vector<int> bestRemoveEdges = S;
    int bestBroadcastingTime = solveOnTree(g, S).rounds.size();

    Graph G2;
    std::vector<int> correspondingEdgeInG;
    constructG2(g, U, positionInU, xyTrees, G2, correspondingEdgeInG);

    double preprocessTime = preprocessTimer.elapsed();

    // === Search Phase ===
    Timer searchTimer;

    long long numberOfSpanningTrees = 0;
    long long numberOfOrderings = 0;

    SpanningTreeIterator spanningTreeIterator(G2);
    while (spanningTreeIterator.hasNext()) {
        std::vector<int> T2 = spanningTreeIterator.next();
        numberOfSpanningTrees += 1;

        std::vector<bool> inT2(G2.edges.size(), false);
        for (int i : T2) inT2[i] = true;

        std::vector<std::vector<std::pair<int, int>>> C;
        std::vector<bool> usedXYTrees;
        computeInitialCAndUsedXYTrees(G2, T2, U, xyTrees, positionInU, C, usedXYTrees);

        std::function<void(int)> enumerateAllOrderings = [&](int i) {
            if (i == (int) C.size()) {
                numberOfOrderings += 1;
                while (bestBroadcastingTime >= 1) {
                    std::vector<int> curRemoveEdges;
                    if (!isFeasible(G2, correspondingEdgeInG, inT2, U, xyTrees,
                        usedXYTrees, timeNeededForXTrees, freeChildrenPositions, C, bh, dh,
                        dhRemoveEdge, curRemoveEdges, bestBroadcastingTime -1)) break;
                    bestBroadcastingTime -= 1;
                    bestRemoveEdges = curRemoveEdges;
                }
            }
            else {
                do {
                    enumerateAllOrderings(i + 1);
                }
                while (std::next_permutation(C[i].begin(), C[i].end()));
            }
        };
        enumerateAllOrderings(0);
    }

    double searchTime = searchTimer.elapsed();
    Timer reconstructTimer;

    // === Constructing protocol ===
    BroadcastingProtocol ans = solveOnTree(g, bestRemoveEdges);
    assert((int) ans.rounds.size() == bestBroadcastingTime);

    if (verbose) {
        double reconstructTime = reconstructTimer.elapsed();
        double totalTime = totalTimer.elapsed();

        printInfo("Size of U", static_cast<unsigned long long>(U.size()));
        printInfo("Spanning trees enumerated", numberOfSpanningTrees);
        printInfo("Child orderings tried", numberOfOrderings);
        printInfo("Optimal broadcasting rounds", static_cast<unsigned long long>(ans.rounds.size()));
        printEmptyLine();
        printInfo("Preprocessing time", preprocessTime);
        printInfo("Search phase time", searchTime);
        printInfo("Protocol construction time", reconstructTime);
        printInfo("Total algorithm time", totalTime);
    }

    return ans;
}

} // namespace broadcast
