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

private:
    uint16_t m_num_nodes;
    uint32_t m_num_links;
    std::vector<std::vector<int>> hop_matrix;
    // move it to be private members (later)
    std::vector<Node*> nodes;
    std::vector<Link*> links;
public:
    Topology(); //default ctor
    Topology(uint16_t num_nodes, uint32_t num_links);
    void create_topology();
    void set_params(int nodes, int links);
    bool is_strongly_connected(Topology* );
    // Setter
    void set_num_nodes(int num_nodes) {
        m_num_nodes = num_nodes;
    }
    void set_num_links(int num_links) {
        m_num_links = num_links;
    }

    // Getter
    int get_num_nodes() {
        return m_num_nodes;
    }
    int get_num_links(){
        return m_num_links;
    }

    void populate_hop_matrix();
};

class Node {
public:
    Node(); // default ctor
    Node(int nodeId, Link* outgoingLink, Link* incomingLink);
    Node(int nodeId, std::vector<Link*> outgoingLinks, std::vector<Link*> incomingLinks);

    Node(int nodeId);

    int node_id;
    std::vector<Link*> outgoing_link;
    std::vector<Link*> incoming_link;
};

class Link {
public:
    Link(); // default ctor
    Link(int linkId, Node* srcNode, Node* destNode);
    int link_id;
    Node* src_node;
    Node* dest_node;
};

#endif //GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
