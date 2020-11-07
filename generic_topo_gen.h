//
// Created by Mayank Parasar on 11/1/20.
//

#ifndef GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
#define GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()

class Node;
class Link;
class Topology;

/* This class contains
 * many automatically generated
 * topologies */
class TopologyUniverse {
public:
    TopologyUniverse(uint32_t mNumNodes, uint32_t mNumLinks,
                     uint32_t mNumTopology);
    void populate_unique_rings(std::vector<int> node_order);
private:
    uint32_t m_num_nodes;
    uint32_t m_num_links;
    uint32_t m_num_topology;
    std::vector<Topology*> m_topologies;
    std::vector<std::vector<int>> m_unique_rings;
};

/* One object per topology */
class Topology {

private:
    uint32_t m_num_nodes;
    uint32_t m_num_links;
    // generate "avg-hop count"
    std::vector<std::vector<int>> m_hop_matrix;
    // this contains the information of actual topology
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    std::vector<std::vector<int>> m_connectivity_matrix;
    // move it to be private members (later)
    std::vector<Node*> nodes;
    std::vector<Link*> links;
    std::vector<int> m_base_ring; // contains the order of nodes
    // each instance of Topology contains one underlying distinct ring
    std::vector<std::vector<Node*>> m_ring_topology;
public:
    Topology(); //default ctor
    Topology(uint32_t mNumNodes, uint32_t mNumLinks);
    Topology(uint32_t mNumNodes, uint32_t mNumLinks,
                       std::vector<int> mBaseRing);
    void create_topology();
    void set_params(int nodes, int links);
    bool is_connected(Node* src_node, Node* dest_node);
    bool is_strongly_connected(Topology* );
    // Setter
    void set_num_nodes(int num_nodes) {
        m_num_nodes = num_nodes;
    }
    void set_num_links(int num_links) {
        m_num_links = num_links;
    }
    void create_ring();
    // Getter
    int get_num_nodes() {
        return m_num_nodes;
    }
    int get_num_links(){
        return m_num_links;
    }

    const std::vector<Node *> &getNodes() const;

    void setNodes(const std::vector<Node *> &nodes);

    void populate_hop_matrix();
};

class Node {
public:
    Node(); // default ctor
    Node(int nodeId, Link* outgoingLink, Link* incomingLink);
    Node(int nodeId, std::vector<Link*> outgoingLinks,
         std::vector<Link*> incomingLinks);

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
