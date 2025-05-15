#include <broadcast/utilities/graph.hpp>
#include <broadcast/utilities/protocol.hpp>
#include <broadcast/utilities/timer.hpp>
#include <broadcast/utilities/verbose.hpp>
#include <broadcast/algorithms/cyclomatic.hpp>

#include <iostream>
#include <cstring>
#include <string>

using namespace broadcast;

int main(int argc, char* argv[]) {
    Timer readAndVerifyTimer;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    bool verbose = false;
    bool noOutput = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if (std::strcmp(argv[i], "--no-output") == 0) {
            noOutput = true;
        } else {
            std::cerr << "Error: Unknown argument " << argv[i] << std::endl;
            return 1;
        }
    }

    // Read the graph
    Graph graph = readGraph();

    Timer graphValidationTimer;
    if (!isSimpleAndConnected(graph)) {
        std::cerr << "Error: The given graph is not simple and connected.\n";
        return 1;
    }

    if (verbose) {
        double readAndVerifyTime = readAndVerifyTimer.elapsed();
        printInfo("Read and verify graph time", readAndVerifyTime);
        printInfo("Number of vertices", graph.n);
        printInfo("Cyclomatic number", graph.edges.size() - graph.n + 1);
        printEmptyLine();
    }

    // Run the chosen algorithm
    BroadcastingProtocol protocol = algorithmCyclomatic(graph, verbose);

    Timer validateAndPrintTimer;
    if (!isValidBroadcastingProtocol(protocol, graph)) {
        std::cerr << "Internal Error: The algorithm constructed an invalid broadcasting protocol.\n";
        return 1;
    }

    if (!noOutput) {
        printBroadcastingProtocol(protocol);
    }

    if (verbose) {
        double validateAndPrintTime = validateAndPrintTimer.elapsed();
        printEmptyLine();
        std::string str = std::string("Validate ") + (!noOutput ? "and print " : "") + "protocol time";
        printInfo(str, validateAndPrintTime);
    }

    return 0;
}

