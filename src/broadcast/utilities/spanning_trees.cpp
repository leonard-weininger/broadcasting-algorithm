#include <broadcast/utilities/spanning_trees.hpp>

#include <functional>

namespace broadcast {

SpanningTreeIterator::SpanningTreeIterator(const Graph& g) {
    n = g.n;
    m = (int) 2 * g.edges.size();
    r = 0;

    edges.resize(m);

    // convert to directed graph by replacing each {u, v} with (u, v) and (v, u)
    for (int i = 0; i < (int) g.edges.size(); ++i) {
        auto [u, v] = g.edges[i];
        edges[2 * i] = {u, v};
        edges[2 * i + 1] = {v, u};
    }

    outgoing.resize(n);
    incoming.resize(n);

    for (int i = 0; i < m; ++i) {
        auto [u, v] = edges[i];
        if (u != v) {
            outgoing[u].emplace_back(v, i);
            incoming[v].emplace_back(u, i);
        }
    }

    edgeInG.assign(m, true);

    vertexInT.assign(n, false);
    tree.resize(n);

    edgeInF.assign(m, false);
    edgeActiveInF.assign(m, false);

    lastTreeWalkStartTime.resize(n);
    lastTreeWalkEndTime.resize(n);

    vertexInT[r] = true;

    for (auto [v, eid] : outgoing[r]) {
        F.push_back(eid);
        edgeInF[eid] = true;
        edgeActiveInF[eid] = true;
    }

    recursionStack.emplace_back(-1, std::vector<std::pair<int, bool>>());

    done = false;
    advance();
}

bool SpanningTreeIterator::hasNext() const {
    return !done;
}

std::vector<int> SpanningTreeIterator::next() {
    std::vector<int> ans = T;
    for (int& i : ans) i /= 2;
    advance();
    return ans;
}

bool SpanningTreeIterator::isWDescendentOfV(int w, int v) {
    int start_v = lastTreeWalkStartTime[v];
    int end_v = lastTreeWalkEndTime[v];
    int start_w = lastTreeWalkStartTime[w];
    return (start_v <= start_w && start_w <= end_v);
}

void SpanningTreeIterator::performNewTreeWalk() {
    int currentTime = 0;
    std::function<void(int)> dfs = [&](int v) {
        lastTreeWalkStartTime[v] = ++currentTime;
        for (int u : tree[v]) {
            dfs(u);
        }
        lastTreeWalkEndTime[v] = ++currentTime;
    };
    dfs(r);
}

void SpanningTreeIterator::advance() {
    while (!recursionStack.empty()) {
        int& e = recursionStack.back().first;
        std::vector<std::pair<int, bool>>& FF = recursionStack.back().second;

        if (e != -1) {
            int v = edges[e].second;

            while (!F.empty() && edges[F.back()].first == v && !vertexInT[edges[F.back()].second]) {
                edgeInF[F.back()] = false;
                edgeActiveInF[F.back()] = false;
                F.pop_back();
            }

            for (auto [w, eid] : incoming[v]) {
                if (edgeInG[eid] && vertexInT[w] && eid != e) {
                    edgeActiveInF[eid] = true;
                }
            }

            T.pop_back();
            tree[edges[e].first].pop_back();
            vertexInT[v] = false;
            edgeInG[e] = false;
            FF.emplace_back(e, true);

            bool b = true;
            for (auto [w, eid] : incoming[v]) {
                if (edgeInG[eid] && !isWDescendentOfV(w, v)) {
                    b = false;
                    break;
                }
            }

            if (b) {
                while (!FF.empty()) {
                    auto [eid, active] = FF.back();
                    FF.pop_back();
                    F.push_back(eid);
                    edgeInF[eid] = true;
                    edgeActiveInF[eid] = active;
                    edgeInG[eid] = true;
                }
                recursionStack.pop_back();
                continue;
            }
        }

        if ((int) T.size() == n - 1) {
            performNewTreeWalk();
            recursionStack.pop_back();
            return;
        }

        while (!F.empty() && !edgeActiveInF[F.back()]) {
            FF.emplace_back(F.back(), false);
            edgeInF[F.back()] = false;
            F.pop_back();
        }

        e = F.back();
        F.pop_back();
        edgeInF[e] = false;

        int v = edges[e].second;
        vertexInT[v] = true;
        T.push_back(e);
        tree[edges[e].first].push_back(v);

        for (auto [w, eid] : outgoing[v]) {
            if (edgeInG[eid] && !vertexInT[w]) {
                F.push_back(eid);
                edgeInF[eid] = true;
                edgeActiveInF[eid] = true;
            }
        }

        for (auto [w, eid] : incoming[v]) {
            if (edgeInG[eid] && vertexInT[w] && eid != e) {
                edgeActiveInF[eid] = false;
            }
        }

        recursionStack.emplace_back(-1, std::vector<std::pair<int, bool>>());
    }

    done = true;
}

} // namespace broadcast
