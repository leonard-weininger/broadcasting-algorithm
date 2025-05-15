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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <k>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);
    int k = std::atoi(argv[2]);

    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<unsigned long long> distr_seed(0, (1LL << 60) + 5);

    unsigned long long seed = distr_seed(rng);

    std::cerr << "n:" << n << " k:" << k << " seed:" << seed << '\n';

    Graph graph = generateRandomGraph(n, k, seed);
    
    if (!isSimpleAndConnected(graph)) {
        std::cerr << "Error: Generated random graph is not simple and connected." << '\n';
        return 1;
    }

    BroadcastingProtocol protocol = algorithmCyclomatic(graph, true);
    
    if (!isValidBroadcastingProtocol(protocol, graph)) {
        std::cerr << "Error: Computed protocol is invalid." << '\n';
        return 1;
    }

    std::cerr << "Ok." << '\n' << '\n';

    return 0;
}

