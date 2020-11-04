//
// Created by Mayank Parasar on 11/1/20.
//

#include "generic_topo_gen.h"

using namespace  std;
// class-Topology member definition
Topology::Topology(uint16_t num_nodes, uint32_t num_links)
: m_num_nodes(num_nodes), m_num_links(num_links)
{};

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


// class-Node member definition
Node::Node() {
    node_id = -1;
//    outgoing_link = nullptr;
//    incoming_link = nullptr;
}

Node::Node(int nodeId, Link* outgoingLink, Link* incomingLink) {
    node_id = nodeId;
    outgoing_link.push_back(outgoingLink);
    incoming_link.push_back(incomingLink);
}

Node::Node(int nodeId, vector<Link*> outgoingLinks, vector<Link*> incomingLinks) {
    node_id = nodeId;
    for(auto i : outgoingLinks)
        outgoing_link.push_back(i);
    for(auto i : incomingLinks)
        incoming_link.push_back(i);
}

// class-Link member definition
Link::Link() {
    link_id = -1;
    src_node = nullptr;
    dest_node = nullptr;
}

Link::Link(int linkId, Node* srcNode, Node* destNode) {
    link_id = linkId;
    src_node = srcNode;
    dest_node = destNode;
}