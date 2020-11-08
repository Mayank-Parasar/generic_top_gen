//
// Created by Mayank Parasar on 11/1/20.
//
#include <iostream>
#include "generic_topo_gen.h"
#include "commandline_parser.h"

using namespace  std;

/* TODO:
 * have a separate .cpp and .h file for
 * 1. Topology class
 * 2. Node class
 * 3. Link class
 * */

// Utility functions:
int fact(int n) {
    int res = 1;
    for (int i = 2; i <= n; i++)
        res = res * i;
    return res;
}
int nCr(int n, int r) {
    return fact(n) / (fact(r) * fact(n - r));
}

int main(int argc, char *argv[])
{
    std::boolalpha;

	cout << "Hello world" << endl;

    // This variables can be set via the command line.
    bool        oPrintHelp = false;
    uint32_t    num_nodes = 0;
    uint32_t    num_links = 0;
    uint32_t    num_topology = 0;

    // First configure all possible command line options.
    Parser parser("Customized C++ command line parser.");
    parser.addArgument({"-n", "--nodes"}, &num_nodes, "Number of Nodes in the topology");
    parser.addArgument({"-l", "--links"}, &num_links, "Number of links in the topology");
    parser.addArgument({"-t", "--topologies"}, &num_topology, "Number of topologies");
    parser.addArgument({"-h", "--help"}, &oPrintHelp,
                     "Print this help. Help can be printing by "
                     "setting oPrintHelp flag true at compile time! "
                     "Otherwise use --help ot -h option");

    // Then do the actual parsing.
    try {
        parser.parse(argc, argv);
    } catch (std::runtime_error const& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    // When oPrintHelp was set to true, we print a help message.
    if (oPrintHelp) {
        parser.printHelp();
        // return 0;
    }

    // Print the resulting values.
    cout << "num_nodes: " << num_nodes << endl;
    cout << "num_links: " << num_links << endl;
    cout << "num_topologies: " << num_topology << endl;


    if(num_nodes > num_links) {
        cout << "Number of nodes are greater than number of links, " \
                "therefore a SCC topology is not possible" << endl;
        exit(-1);
    }
    else if(num_links >= nCr(num_nodes, 2)) {
        cout << "Number of links are greater than or equal to fully connected"
                " topology: therefore only fully connected topology is "
                "possible. Exiting! ";
        exit(-1);
    }
    else {
        // continue building topology
    }

    /* Create the topolo*/
    /* Do this for each topology in a for loop */

	Topology *topo = new Topology(num_nodes, num_links);
    //    topo->set_params()
    topo->create_ring();
    topo->create_topology();
    topo->is_strongly_connected(topo);

    delete topo;

    /* File handling stuff */

	return 0;
}
