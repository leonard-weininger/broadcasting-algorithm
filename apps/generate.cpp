#include <broadcast/utilities/generator.hpp>
#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>

#include <chrono>
#include <cstring>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

using namespace broadcast;

int main(int argc, char* argv[]) {
    Timer timer;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 3 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <n> <k> [seed] [--verbose]\n";
        std::cerr << "  n         - number of vertices (n >= 1)\n";
        std::cerr << "  k         - cyclomatic number (0 <= k <= n*(n-1)/2 - n + 1)\n";
        std::cerr << "  seed      - optional integer seed for reproducibility\n";
        std::cerr << "  --verbose - optional flag for verbose output\n";
        return 1;
    }

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long long> distr_seed(0, 1e18 - 1);

    int n, k;
    unsigned long long seed = distr_seed(rng);
    bool verbose = false;

    try {
        n = std::stoi(argv[1]);
        k = std::stoi(argv[2]);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: n and k must be integers.\n";
        return 1;
    }

    // Parse optional arguments
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--verbose") {
            verbose = true;
        } else {
            try {
                seed = std::stoull(arg);
            } catch (const std::invalid_argument&) {
                std::cerr << "Error: seed must be an integer.\n";
                return 1;
            }
        }
    }

    if (verbose) {
        printHeader("Generate random graph");
        printInfo("Number of vertices", n);
        printInfo("Cyclomatic number", k);
        printInfo("Seed", seed);
    }

    Graph g = generateRandomGraph(n, k, seed);
    printGraph(g);

    if (verbose) {
        double totalTime = timer.elapsed();
        printInfo("Generate and print graph time", totalTime);
    }

    return 0;
}

