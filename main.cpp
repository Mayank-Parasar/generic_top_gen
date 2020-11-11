//
// Created by Mayank Parasar on 11/1/20.
//
#include <iostream>
#include <string>
#include "generic_topo_gen.h"
#include "commandline_parser.h"
#include "file_handler.h"

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
    uint32_t    verbosity_level = 0;
    string      input_file;
    // First configure all possible command line options.
    Parser parser("Customized C++ command line parser.");
    parser.addArgument({"-n", "--nodes"}, &num_nodes, "Number of Nodes in the topology");
    parser.addArgument({"-l", "--links"}, &num_links, "Number of links in the topology");
    parser.addArgument({"-t", "--topologies"}, &num_topology, "Number of topologies");
    parser.addArgument({"-f", "--file"}, &input_file, "input file name for "
                                                      "reading");
    parser.addArgument({"-v", "--verbosity"},
                       &verbosity_level, "Verbosity while running the tool, "
                                         "for debug");
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
    else if(num_links >= (2 * nCr(num_nodes, 2))) { // uni-directional links
        cout << "Number of links are greater than or equal to fully connected"
                " topology: therefore only fully connected topology is "
                "possible. Exiting! ";
        exit(-1);
    }
    else {
        // continue building topology
    }

    /*DO file handling first*/
    FileHandler *file = new FileHandler(1); // only 1 file allowed to open
    file->OpenHandle(("input/"+input_file).c_str(), fstream::in);

    // FIXME: create better API
    // Do file handling inside that API instead of here
    string word;
    bool populate_matrix = false;
    vector<vector<int>>* matrix;
    int row = 0;
    int col = 0;
    while (file->GetHandle(0)>> word) {

        if (populate_matrix) {
            cout << *matrix[row][col];
            *matrix[row][col] = new int(stoi(word));
            col++;
            if (col == file->mat_size.back() &&
                row < file->mat_size.back()){
                col = 0;
                row++;
            }
            if (col == file->mat_size.back() &&
                row == file->mat_size.back()) {
                col = 0;
                row = 0;
                populate_matrix = false; // This matrix has been populated
                file->mat_ptr.push_back(matrix);
            }
        }
        else if (!populate_matrix) {
            if (word == "Application:" ||
                word == "application:") {
                file->GetHandle(0) >> word; // get the next 'word'
                file->application_name.push_back(word);
                file->GetHandle(0) >> word; // get the next 'word'
                file->mat_size.push_back(stoi(word));
                // allocate memory for new incoming matrix
                matrix = new std::vector<std::vector<int>> (stoi(word)),
                        std::vector<int> (stoi(word), 0);
                // Now turn-on populating matrices
                populate_matrix = true;
            }
        }

        cout << word << endl;
    }



    TopologyUniverse *universe = new TopologyUniverse(num_nodes, num_links,
                                                      num_topology);
    universe->init_generic_topo_gen();

    if (verbosity_level >= 1) {
        universe->print_universe();
    }

    /* File handling stuff */
    /* use the universe to output the avg hop count
     * from the application traffic pattern for each generated
     * topology*/

	return 0;
}
