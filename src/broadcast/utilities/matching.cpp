#include <broadcast/utilities/matching.hpp>

#include <algorithm>

namespace broadcast {

BipartiteMatching::BipartiteMatching(int _n, int _m) : n(_n), m(_m) {
    matchLeft.assign(n, -1);
    matchRight.assign(m, -1);
    adj.resize(n);
    validLeft.assign(n, true);
    validRight.assign(m, true);
    que.resize(n);
    parent.resize(n);
    root.resize(n);
    matchingSize = 0;
}

void BipartiteMatching::addEdge(int a, int b) {
    if (0 <= a && a < n && 0 <= b && b < m) {
        adj[a].push_back(b);
    }
}

void BipartiteMatching::removeVertexA(int a) {
    if (0 <= a && a < n) {
        validLeft[a] = false;
        if (matchLeft[a] != -1) {
            matchRight[matchLeft[a]] = -1;
            matchLeft[a] = -1;
            matchingSize -= 1;
        }
    }
}

void BipartiteMatching::removeVertexB(int b) {
    if (0 <= b && b < m) {
        validRight[b] = false;
        if (matchRight[b] != -1) {
            matchLeft[matchRight[b]] = -1;
            matchRight[b] = -1;
            matchingSize -= 1;
        }
    }
}

void BipartiteMatching::addVertexA(int a) {
    if (0 <= a && a < n) {
        validLeft[a] = true;
    }
}

void BipartiteMatching::addVertexB(int b) {
    if (0 <= b && b < m) {
        validRight[b] = true;
    }
}

int BipartiteMatching::computeMaximumMatching() {
    while (true) {
        bool success = false;
        std::fill(parent.begin(), parent.end(), -1);
        std::fill(root.begin(), root.end(), -1);
        int queEnd = 0;
        for (int a = 0; a < n; ++a) {
            if (validLeft[a] && matchLeft[a] == -1) {
                root[a] = a;
                parent[a] = -2;
                que[queEnd++] = a;
            }
        }
        for (int queBegin = 0; queBegin < queEnd; ++queBegin) {
            int a = que[queBegin];
            if (matchLeft[root[a]] != -1) continue;
            for (int b : adj[a]) {
                if (validRight[b]) {
                    if (matchRight[b] == -1) {
                        ++matchingSize;
                        success = true;
                        while (a != -2) {
                            matchRight[b] = a;
                            std::swap(b, matchLeft[a]);
                            a = parent[a];
                        }
                        break;
                    }
                    else if (parent[matchRight[b]] == -1) {
                        parent[matchRight[b]] = a;
                        que[queEnd++] = matchRight[b];
                        root[matchRight[b]] = root[a];
                    }
                }
            }
        }
        if (!success) break;
    }
    return matchingSize;
}

} // namespace broadcast
