//
// Created by Mayank Parasar on 11/1/20.
//

#include "generic_topo_gen.h"

using namespace  std;
void
Topology::create_topology() {
    cout << "This is the template of the function: create_topology()"<< endl;
    cout << "Num-nodes: " << m_num_nodes << endl;
    cout << "Num-links: " << m_num_links << endl;

    return;
}

void
Topology::set_params(int nodes, int links) {
    this->m_num_nodes = nodes;
    this->m_num_links = links;
    return;
}

bool
Topology::is_strongly_connected(Topology * t) {
    cout << "This function checks if the given topology generated is strongly connected" << endl;

    return false;
}