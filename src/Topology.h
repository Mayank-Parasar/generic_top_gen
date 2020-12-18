//
// Created by Mayank Parasar on 11/13/20.
//

#ifndef GENERIC_TOPO_GEN_TOPOLOGY_H
#define GENERIC_TOPO_GEN_TOPOLOGY_H
#include <vector>
#include <iostream>
#include <cassert>
#include "Node.h"
#include "Link.h"
#include "Optimizer.h"

/* One object per topology */
class Topology {

protected:
    uint32_t m_num_nodes;
    uint32_t m_num_links;
    // generate "avg-hop count"
    std::vector<std::vector<int>> m_hop_matrix;

public:
    const std::vector<std::vector<int>> &getMHopMatrix() const;

protected:
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
    Optimizer* m_optimizer;
public:
    Topology(); //default ctor
    Topology(uint32_t mNumNodes, uint32_t mNumLinks);
    Topology(uint32_t mNumNodes, uint32_t mNumLinks,
             std::vector<int> mBaseRing, bool mDebug,
             Optimizer* optimizer = nullptr);
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


class Mesh : public Topology {
private:
    uint32_t m_rows;
    uint32_t m_cols;
public:
    // build the connectivity matrix in the ctor
    Mesh(uint32_t mNumNodes, uint32_t mNumLinks,
         const std::vector<int> &mBaseRing, bool mDebug);

    Mesh(uint32_t mNumNodes, uint32_t mNumLinks,
         uint32_t mNumRows, uint32_t mNumCols);

    Mesh(uint32_t mRows, uint32_t mCols);

    void create_mesh();
};

class Torus : public Topology {
private:
    uint32_t m_rows;
    uint32_t m_cols;
public:
    // build the connectivity matrix in the ctor
    Torus(uint32_t mRows, uint32_t mCols);

    void create_torus();
};

class FlattenedButterfly : public Topology {
private:
    uint32_t m_rows;
    uint32_t m_cols;
public:
    // build the connectivity matrix in the ctor
    FlattenedButterfly(uint32_t mRows, uint32_t mCols);

    void create_flattened_butterfly();
};

#endif //GENERIC_TOPO_GEN_TOPOLOGY_H
