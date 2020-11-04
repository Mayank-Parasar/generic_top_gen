//
// Created by Mayank Parasar on 11/1/20.
//

#ifndef GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
#define GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H

#include <cstdint>
#include <iostream>
#include <vector>

class Node;
class Link;

class Topology {
public:
    Topology(); //default ctor
    Topology(uint16_t num_nodes, uint32_t num_links);
    void create_topology();
    void set_params(int nodes, int links);
    bool is_strongly_connected(Topology* );
    uint16_t m_num_nodes;
    uint16_t m_num_links;

    std::vector<Node*> nodes;
    std::vector<Link*> links;
};

class Node {
public:
    Node(); // default ctor
    Node(int nodeId, Link* outgoingLink, Link* incomingLink);
    int node_id;
    Link* outgoing_link;
    Link* incoming_link;
};

class Link {
    Link(); // default ctor
    Link(int linkId, Node* srcNode, Node* destNode);
    int link_id;
    Node* src_node;
    Node* dest_node;
};

#endif //GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
