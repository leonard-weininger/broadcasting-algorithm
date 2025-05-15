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

    std::uniform_int_distribution<unsigned long long> distr_seed(0, (1LL << 60) + 5);

    std::vector<int> vector_type = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 2, 2, 2, 3,
    };
    std::uniform_int_distribution<int> distr_type(0, 19);

    std::vector<std::uniform_int_distribution<int>> distr_n = {
        std::uniform_int_distribution<int>(21, 50),
        std::uniform_int_distribution<int>(51, 100),
        std::uniform_int_distribution<int>(101, 1000),
        std::uniform_int_distribution<int>(1001, 5000),
    };

    std::vector<std::uniform_int_distribution<int>> distr_k = {
        std::uniform_int_distribution<int>(2, 4),
        std::uniform_int_distribution<int>(2, 3),
        std::uniform_int_distribution<int>(1, 2),
        std::uniform_int_distribution<int>(1, 1),
    };

    long long ncas = 0;

    while (timer.elapsed() < TIME_LIMIT) {
        ncas += 1;

        int type = vector_type[distr_type(rng)];
        int n = distr_n[type](rng);
        int k = distr_k[type](rng);
        unsigned long long seed = distr_seed(rng);

        Graph g = generateRandomGraph(n, k, seed);
        BroadcastingProtocol cy = algorithmCyclomatic(g);
        BroadcastingProtocol en = algorithmEnumerateAllTrees(g);
    
        if (cy.rounds.size() != en.rounds.size()) {
            std::cout << "Test case " << ncas << " failed." << '\n';
            std::cout << "Cyclomatic and Enumeration have different number of rounds.\n\n";
            std::cout << "Graph:\n";
            printGraph(g);
            std::cout << '\n';
            std::cout << "Cyclomatic Protocol:\n";
            printBroadcastingProtocol(cy);
            std::cout << "Enumeration Protocol:\n";
            printBroadcastingProtocol(en);
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
            std::cout << "Enumeration Protocol:\n";
            printBroadcastingProtocol(en);
            return 0;
        }
    }

    std::cout << "Passed " << ncas << " test cases in " << timer.elapsed() << " seconds.\n";

    return 0;
}

