#include <iostream>
#include "generic_topo_gen.h"
using namespace  std;

int main(int argc, char const *argv[])
{
    std::boolalpha;

	cout << "Hello world" << endl;

    char const *buffer;
    buffer = argv[1];

    uint16_t num_nodes = strtoul(argv[1], nullptr, 10);
    cout << "num-nodes: " << num_nodes << endl;
    uint32_t num_links = strtoul(argv[2], nullptr, 10);
    cout << "num-links: " << num_links << endl;

    if(num_nodes > num_links) {
        cout << "Number of nodes are greater than number of links, " \
                "therefore a SCC topology is not possible" << endl;
    }

	Topology *topo = new Topology(num_nodes, num_links);
//    topo->set_params()
    topo->create_topology();
    topo->is_strongly_connected(topo);

    delete topo;
    
	return 0;
}
