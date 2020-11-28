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
uint64_t fact(int n) {
    uint64_t res = 1;
    for (int i = 2; i <= n; i++)
        res = res * i;
    return res;
}
uint64_t nCr(int n, int r) {
    return fact(n) / (fact(r) * fact(n - r));
}

uint64_t nC2(int n) {
    return (n*(n-1)/2);
}

//Utility functions:
vector<vector<int>> dot_product(const vector<vector<int>>& mat1,
                                const vector<vector<int>>& mat2) {
    // Both matrices MUST match in dimensions..
    assert(mat1.size() == mat2.size());
    vector<vector<int>> result;
    vector<int> result_;
    for (int row_ = 0; row_ < mat1.size(); ++row_) {
        for (int col_ = 0; col_ < mat1[row_].size(); ++col_) {
            result_.push_back(mat1[row_][col_] * mat2[row_][col_]);
        }
        result.push_back(result_);
        result_.clear();
    }

    return result;
}

uint64_t mat_sum (const vector<vector<int>>& mat) {
    uint64_t sum = 0;
    for (auto i : mat) {
        for (auto k : i) {
            sum += k;
        }
    }
    return sum;
}

uint64_t mat_diagonal_sum(const vector<vector<int>>& mat) {
    uint64_t sum = 0;
    for (int row_ = 0; row_ < mat.size(); ++row_) {
        for (int col_ = 0; col_ < mat[row_].size(); ++col_) {
            if(row_ == col_) {
                sum += mat[row_][col_];
            }
        }
    }
    return sum;
}

enum topology_name {
    invalid_ = 0,
    mesh_ = 1,
    torus_ = 2,
    flattened_butterfly_ = 3,
    dragonfly_ = 4,
    random_ = 5
};
topology_name str2enum(string topology) {
    if (topology == "Mesh" ||
        topology == "mesh") {
        return mesh_;
    }
    else if (topology == "torus" ||
             topology == "Torus" ||
             topology == "tori" ||
             topology == "Torui") {
        return torus_;
    }
    else if (topology == "FlattenedButterfly" ||
             topology == "FButterfly" ||
             topology == "f-butterfly" ||
             topology == "ffly") {
        return flattened_butterfly_;
    }
    else if (topology == "Dragonfly" ||
             topology == "dragonfly" ||
             topology == "dfly") {
        return dragonfly_;
    }
    else if (topology == "None" ||
             topology == "none") {
        return random_;
    }
    // if there is no name matched
    return invalid_;
}
int main(int argc, char *argv[])
{
    std::boolalpha;

	cout << "Hello world" << endl;

    // This variables can be set via the command line.
    bool        oPrintHelp = false;
    bool        debug_print = false;
    bool        optimizer = true;
    bool        unique_rings = true;
    uint32_t    num_nodes = 0;
    uint32_t    num_links = 0;
    uint32_t    num_rows = 0;   // number of rows in a Mesh
    uint32_t    num_cols = 0;   // number of columns in a Mesh
    uint32_t    num_topology = 0;
    uint32_t    verbosity_level = 0;
    string      input_file;
    string      spl_topology = "None";
    // First configure all possible command line options.
    Parser parser("Customized C++ command line parser.");
    parser.addArgument({"-n", "--nodes"}, &num_nodes, "Number of Nodes in the topology");
    parser.addArgument({"-l", "--links"}, &num_links, "Number of links in the topology");
    parser.addArgument({"-r", "--rows"}, &num_rows, "Number of rows in the"
                                                      " Mesh topology");
    parser.addArgument({"-c", "--cols"}, &num_cols, "Number of columns in the"
                                                     " Mesh topology");
    parser.addArgument({"-st", "--topology"}, &spl_topology,
                       "Special topology such as Mesh, Torus, DragonFly, etc");
    parser.addArgument({"-t", "--topologies"}, &num_topology, "Number of topologies");
    parser.addArgument({"-f", "--file"}, &input_file, "input file name for "
                                                      "reading");
    parser.addArgument({"-d", "--debug"}, &debug_print, "When set to true this "
                                                      "enables debug "
                                                      "messages, by default "
                                                      "it is turned-off");
    parser.addArgument({"-ur", "--uniqueRings"}, &unique_rings,
                       "when set false, it disables the check for unique "
                       "rings");
    parser.addArgument({"-v", "--verbosity"},
                       &verbosity_level, "Verbosity while running the tool, "
                                         "for debug");
    parser.addArgument({"-opt", "--optimizer"}, &optimizer,
                       "this enables the topology generator to generate "
                       "random but optimized topologies based on the traffic "
                       "pattern of the application");
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
    cout << "Special topology: " << spl_topology << endl;
    cout << "num_rows: " << num_rows << endl;
    cout << "num_cols: " << num_cols << endl;

    if (num_rows != 0 && num_cols != 0) {
        num_nodes = num_rows * num_cols;
    }

    cout << "Maximum number of links with given node count for fully "
            "connected topology: " << (2 * nC2(num_nodes)) << endl;

    if((num_nodes > num_links) && (spl_topology == "None")) {
        cout << "Number of nodes are greater than number of links, " \
                "therefore a SCC topology is not possible" << endl;
        exit(-1);
    }
    // it is an independent check
    if(num_links > (2 * nC2(num_nodes))) { // uni-directional links
        cout << "Number of links are greater than fully connected"
                " topology: therefore only fully connected topology is "
                "possible. Exiting! ";
        exit(-1);
    }
    else {
        // continue building topology
    }

    /*DO file handling first*/
    FileHandler *file = new FileHandler(1, debug_print); // only 1 file allowed to open
    file->OpenHandle(("input/"+input_file).c_str(), fstream::in);

    // FIXME: create better API
    // Do file handling inside that API instead of here
    string word;
    bool populate_matrix = false;
    vector<vector<int>> matrix;
    vector<int> vec;
    int row = 0;
    int col = 0;
    while (file->GetHandle(0)>> word) {

        if (populate_matrix) {
            vec.push_back(stoi(word));
            col++;
            if (col == (file->getMatSize().back())) {
                col = 0;
                row++;
                matrix.push_back(vec);
                vec.clear(); // clear vec
            }
            if ((col == 0) &&
                row == (file->getMatSize().back())){
                col = 0;
                row = 0;
                populate_matrix = false; // This matrix has been populated
                file->setMApplMat(matrix);
                matrix.clear();
            }
        }
        else if (!populate_matrix) {
            if (word == "Application:" ||
                word == "application:") {
                file->GetHandle(0) >> word; // get the next 'word'
                file->setApplicationName(word);
                file->GetHandle(0) >> word; // get the next 'word'
                file->setMatSize(stoi(word));
                // Now turn-on populating matrices
                populate_matrix = true;
            }
        }
        // cout << word << endl;
    }

    // Clean up the priting of results later..
    // FIXME: flow of the code needs to be streamlined
    Topology *topology_ = nullptr;
    switch (str2enum(spl_topology)) {

        case invalid_:
            break;
        case mesh_: {
            Mesh *mesh = new Mesh(num_rows, num_cols);
            topology_ = mesh;
            break;
        }
        case torus_: {
            Torus *torus = new Torus(num_rows, num_cols);
            topology_ = torus;
            break;
        }
        case flattened_butterfly_:{
            FlattenedButterfly *flattened_butterfly =
                    new FlattenedButterfly(num_rows, num_cols);
            topology_ = flattened_butterfly;
            break;
        }
        case dragonfly_:
            break;
        case random_:
            break;
    }

    if (topology_) {
        if (verbosity_level >= 1) {
            topology_->print_topology();
        }
        vector<vector<int>> dot_product_mat;
        for (int appMat_itr = 0; appMat_itr < file->getMApplMat().size(); ++appMat_itr) {

            dot_product_mat.clear();

            dot_product_mat = dot_product(topology_->getMHopMatrix(),
                                          file->getMApplMat()[appMat_itr]);

            uint64_t sum_dot_product_mat = mat_sum(dot_product_mat);

            uint64_t sum_app_mat = mat_sum(file->getMApplMat()[appMat_itr]) -
                                   mat_diagonal_sum(file->getMApplMat()[appMat_itr]);

            double average_hop_count = (double)sum_dot_product_mat/((double)
                    sum_app_mat);

            cout << "Application: " << file->getApplicationName()[appMat_itr] <<
                 "\tTopology-" <<spl_topology << ": " << ":\t" <<
                 average_hop_count << endl;
        }

        return 0;
    }

    TopologyUniverse *universe = new TopologyUniverse(num_nodes, num_links,
                                                      num_topology,
                                                      debug_print,
                                                      unique_rings);
    /* invoke optimizer here */
    if (optimizer) {
        universe->init_optimizer(file);
    }
    universe->init_generic_topo_gen();

    if (verbosity_level >= 1) {
        universe->print_universe();
    }

    // output the average hop-count for:
    // 1. each topology AND
    // 2. each traffic pattern.
    vector<vector<int>> dot_product_mat;
    for (int topo_itr = 0;
         topo_itr < universe->getMTopologies().size(); ++topo_itr) {
        for (int appMat_itr = 0; appMat_itr < file->getMApplMat().size();
        ++appMat_itr) {
            
            dot_product_mat.clear();

            dot_product_mat = dot_product(universe->getMTopologies()
                    [topo_itr]->getMHopMatrix(), file->getMApplMat()[appMat_itr]);

            uint64_t sum_dot_product_mat = mat_sum(dot_product_mat);

            uint64_t sum_app_mat = mat_sum(file->getMApplMat()[appMat_itr]) -
                                   mat_diagonal_sum(file->getMApplMat()[appMat_itr]);

            double average_hop_count = (double)sum_dot_product_mat/((double)
            sum_app_mat);

            cout << "Application: " << file->getApplicationName()[appMat_itr] <<
                 "\tTopology-ID: " << topo_itr << ":\t" <<
                 average_hop_count << endl;
        }
    }


    /* File handling stuff */
    /* use the universe to output the avg hop count
     * from the application traffic pattern for each generated
     * topology*/

	return 0;
}
