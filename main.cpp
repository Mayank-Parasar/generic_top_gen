#include <iostream>
#include "generic_topo_gen.h"
using namespace  std;

int main(int argc, char const *argv[])
{
    std::boolalpha;

	cout << "Hello world" << endl;
	cout << "num-nodes: " << argv[1];
	uint16_t num_nodes = atoi(argv[1]);
	cout << "num-links: " << argv[2];
    uint32_t num_links = atoi(argv[2]);

	Topology *topo = new Topology(num_nodes, num_links);
//    topo->set_params()
    topo->create_topology();
    topo->is_strongly_connected(topo);

    delete topo;
    
	return 0;
}
