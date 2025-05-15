#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/hash.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace broadcast {

bool BroadcastingProtocol::operator==(const BroadcastingProtocol& other) const {
    return rounds == other.rounds;
}

BroadcastingProtocol readBroadcastingProtocol(std::istream& in) {
    BroadcastingProtocol bp;
    int r;

    if (!(in >> r)) {
        throw std::runtime_error("Failed to read number of rounds.");
    }

    if (r < 0) {
        throw std::runtime_error("Number of rounds must be non-negative.");
    }

    bp.rounds.resize(r);

    for (int i = 0; i < r; ++i) {
        int k;
        if (!(in >> k)) {
            throw std::runtime_error("Failed to read number of messages in round " + std::to_string(i + 1) + ".");
        }

        if (k < 0) {
            throw std::runtime_error("Number of messages in round " + std::to_string(i + 1) + " must be non-negative.");
        }

        bp.rounds[i].resize(k);
        for (int j = 0; j < k; ++j) {
            int from, to;
            if (!(in >> from >> to)) {
                throw std::runtime_error("Failed to read message " + std::to_string(j + 1) +
                    " in round " + std::to_string(i + 1) + ".");
            }
            bp.rounds[i][j] = {from, to};
        }
    }

    return bp;
}


void printBroadcastingProtocol(const BroadcastingProtocol& bp, std::ostream& out) {
    out << bp.rounds.size() << '\n';
    for (const auto& round : bp.rounds) {
        out << round.size() << '\n';
        for (const auto& [u, v] : round) {
            out << u << ' ' << v << '\n';
        }
    }
}

bool isValidBroadcastingProtocol(const BroadcastingProtocol& bp, const Graph& g) {
    std::vector<bool> informed(g.n, false);
    informed[g.s] = true;

    std::unordered_set<std::pair<int, int>, customHash> edgeSet;
    for (auto [u, v] : g.edges) {
        if (u > v) std::swap(u, v); // Normalize edge
        edgeSet.emplace(u, v);
    }

    // lastActiveRound[u] = last round that u informed a neighbour
    std::vector<int> lastActiveRound(g.n, -1);

    for (int i = 0; i < (int) bp.rounds.size(); ++i) {
        if (bp.rounds[i].empty()) {
            return false;
        }

        for (const auto& [u, v] : bp.rounds[i]) {
            if (u < 0 || u >= g.n) return false;
            if (v < 0 || v >= g.n) return false;
            if (!informed[u]) return false;
            if (informed[v]) return false;
            if (edgeSet.find(std::make_pair(std::min(u, v), std::max(u, v))) == edgeSet.end()) return false;
            if (lastActiveRound[u] == i) return false;
            lastActiveRound[u] = i;
        }

        for (const auto& [u, v] : bp.rounds[i]) {
            informed[v] = true;
        }
    }

    return std::all_of(informed.begin(), informed.end(), [](bool v) { return v; });
}

} // namespace broadcast
