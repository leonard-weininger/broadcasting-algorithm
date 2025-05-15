#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>
#include <broadcast/algorithms/cyclomatic.hpp>
#include <broadcast/algorithms/dp.hpp>
#include <broadcast/algorithms/enumeration.hpp>
#include <broadcast/utilities/generator.hpp>

#include <chrono>
#include <iostream>
#include <random>
#include <cstdlib>

using namespace broadcast;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <integer>" << std::endl;
        return 1;
    }

    const int TIME_LIMIT = std::atoi(argv[1]);

    Timer timer;

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long long> distr(0, (1LL << 60) + 5);

    long long ncas = 0;

    while (timer.elapsed() < TIME_LIMIT) {
        ncas += 1;
        int n = 6 + (ncas % 11);
        int k = (ncas % 9);
        unsigned long long seed = distr(rng);
        Graph g = generateRandomGraph(n, k, seed);
        BroadcastingProtocol cy = algorithmCyclomatic(g);
        BroadcastingProtocol dp = algorithmDP(g);
    
        if (cy.rounds.size() != dp.rounds.size()) {
            std::cout << "Test case " << ncas << " failed." << '\n';
            std::cout << "Cyclomatic and DP have different number of rounds.\n\n";
            std::cout << "Graph:\n";
            printGraph(g);
            std::cout << '\n';
            std::cout << "Cyclomatic Protocol:\n";
            printBroadcastingProtocol(cy);
            std::cout << "DP Protocol:\n";
            printBroadcastingProtocol(dp);
            return 0;
        }

        if (!isValidBroadcastingProtocol(cy, g)) {
            std::cout << "Test case " << ncas << " failed." << '\n';
            std::cout << "Cyclomatic Protocol is invalid.\n\n";
            std::cout << "Graph:\n";
            printGraph(g);
            std::cout << '\n';
            std::cout << "Cyclomatic Protocol:\n";
            printBroadcastingProtocol(cy);
            std::cout << "DP Protocol:\n";
            printBroadcastingProtocol(dp);
            return 0;
        }
    }

    std::cout << "Passed " << ncas << " test cases in " << timer.elapsed() << " seconds.\n";

    return 0;
}

