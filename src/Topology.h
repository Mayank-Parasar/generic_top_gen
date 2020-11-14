//
// Created by Mayank Parasar on 11/13/20.
//

#ifndef GENERIC_TOPO_GEN_TOPOLOGY_H
#define GENERIC_TOPO_GEN_TOPOLOGY_H
#include <vector>
#include <iostream>
#include "Node.h"
#include "Link.h"

/* One object per topology */
class Topology {

private:
    uint32_t m_num_nodes;
    uint32_t m_num_links;
    // generate "avg-hop count"
    std::vector<std::vector<int>> m_hop_matrix;

public:
    const std::vector<std::vector<int>> &getMHopMatrix() const;

private:
    // this contains the information of actual topology
    // column is 'src'-node (sender) and row is 'dest'-node(receiver)
    // 'connectivity matrix' actually contains the link latency information
    // currently assumed to be 1 for every link
    std::vector<std::vector<int>> m_connectivity_matrix;
    // move it to be private members (later)
    std::vector<Node*> nodes;
    std::vector<Link*> links;
    std::vector<int> m_base_ring; // contains the order of nodes
    bool m_debug;
public:
    Topology(); //default ctor
    Topology(uint32_t mNumNodes, uint32_t mNumLinks);
    Topology(uint32_t mNumNodes, uint32_t mNumLinks,
             std::vector<int> mBaseRing, bool mDebug);
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

    void populate_hop_matrix(int source_node_id);
    // helper function
    int minDistance(int dist[], bool sptSet[]);
    void print_topology();
};
#endif //GENERIC_TOPO_GEN_TOPOLOGY_H
