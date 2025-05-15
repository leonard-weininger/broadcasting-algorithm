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
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    };
    std::uniform_int_distribution<int> distr_type(0, 19);

    std::vector<std::uniform_int_distribution<int>> distr_n = {
        std::uniform_int_distribution<int>(3, 35),
        std::uniform_int_distribution<int>(36, 100),
        std::uniform_int_distribution<int>(101, 10000),
    };

    std::vector<std::uniform_int_distribution<int>> distr_k = {
        std::uniform_int_distribution<int>(0, 6),
        std::uniform_int_distribution<int>(0, 5),
        std::uniform_int_distribution<int>(0, 4),
    };

    long long ncas = 0;

    while (timer.elapsed() < TIME_LIMIT) {
        ncas += 1;

        int type = vector_type[distr_type(rng)];
        int n = distr_n[type](rng);
        int k = std::min(distr_k[type](rng), n * (n - 1) / 2 - n + 1);
        unsigned long long seed = distr_seed(rng);

        Graph g = generateRandomPerfectBroadcastingGraph(n, k, seed);
        BroadcastingProtocol cy = algorithmCyclomatic(g);

        int perfectRounds = 0;
        while ((1 << perfectRounds) < n) perfectRounds += 1;
    
        if ((int) cy.rounds.size() != perfectRounds) {
            std::cout << "Test case " << ncas << " failed." << '\n';
            std::cout << "Cyclomatic algorithm does not achieve perfect rounds.\n\n";
            std::cout << "Graph:\n";
            printGraph(g);
            std::cout << '\n';
            std::cout << "Cyclomatic Protocol:\n";
            printBroadcastingProtocol(cy);
            std::cout << "Perfect rounds: " << perfectRounds << '\n';
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
            std::cout << "Perfect rounds: " << perfectRounds << '\n';
            return 0;
        }
    }

    std::cout << "Passed " << ncas << " test cases in " << timer.elapsed() << " seconds.\n";

    return 0;
}

