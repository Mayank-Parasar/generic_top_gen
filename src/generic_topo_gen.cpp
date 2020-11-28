//
// Created by Mayank Parasar on 11/1/20.
//

#include "generic_topo_gen.h"

using namespace  std;

// class-TopologyUniverse member-functions definitions
TopologyUniverse::TopologyUniverse(uint32_t mNumNodes, uint32_t mNumLinks,
                                   uint32_t mNumTopology, bool mDebug,
                                   bool mUniqueRingsCheck)
        : m_num_nodes(mNumNodes), m_num_links(mNumLinks),
          m_num_topology(mNumTopology), m_debug(mDebug),
          m_unique_rings_check(mUniqueRingsCheck)
          {}

void
TopologyUniverse::init_generic_topo_gen() {
    vector<int> node_order;
    for(int i = 0; i < m_num_nodes; i++) {
        node_order.push_back(i);
    }
    populate_unique_rings(node_order);
    // By now all the unique rings have been populated.
    // for 'm_num_topology' distribute one base-ring instance for each
    for (uint32_t mTopology = 0; mTopology < m_num_topology; ++mTopology) {
        Topology *topology_ = new Topology(m_num_nodes, m_num_links,
                                           m_unique_rings[mTopology],
                                           m_debug);
        m_topologies.push_back(topology_);
    }
}

void
TopologyUniverse::populate_unique_rings(std::vector<int> node_order) {
    srand((unsigned) time(0));
    int sizeOfVector = node_order.size();
    // shuffle
    shuffle:
    /* Fisher-Yates shuffle algorithm */
    for (int k = 0; k < sizeOfVector; k++) {
        int r = k + rand() % (sizeOfVector - k); // careful here!
        swap(node_order[k], node_order[r]);
    }
    // check
    // This check can be expensive to generate topologies of larger size
    // say 1k nodes, 10k nodes with higher number of random topoplgies etc:
    // disabling this check based on a command line switch
    if (m_unique_rings_check) {
        for (int i = 0; i < node_order.size(); i++) {
            rotate(node_order.begin(), node_order.begin() + 1,
                   node_order.end());
            for (auto i : m_unique_rings)
                if (i == node_order)
                    goto shuffle;
        }
    }
    //store
    if(m_unique_rings.size() < m_num_topology) {
        m_unique_rings.push_back(node_order);
        goto shuffle;
    }
    else
        return;
}

void TopologyUniverse::print_universe() {
    for (int mTopology = 0; mTopology < m_num_topology; ++mTopology) {
        cout << "Printing Topology:: \t" << mTopology << endl;
        m_topologies[mTopology]->print_topology();
        cout << "\t*********************************" << endl;
    }
    return;
}

const vector<Topology *> &TopologyUniverse::getMTopologies() const {
    return m_topologies;
}

void TopologyUniverse::init_optimizer(FileHandler *handle) {
    cout << "num applications:\t" << handle->m_appl_mat.size() << endl;
    m_opt_ptr = new Optimizer(handle->m_appl_mat);
}


