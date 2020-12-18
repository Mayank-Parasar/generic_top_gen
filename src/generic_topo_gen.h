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
#include "Topology.h"
#include "file_handler.h"
#include "commandline_parser.h"

/* This class contains
 * many automatically generated
 * topologies */
class TopologyUniverse {
public:
    TopologyUniverse(uint32_t mNumNodes, uint32_t mNumLinks,
                     uint32_t mNumTopology, bool debug,
                     bool mUniqueRingsCheck = true);
    void init_generic_topo_gen(Parser& parser, Optimizer* opt_ = nullptr);
    void populate_unique_rings(std::vector<int> node_order);
    void print_universe();
    // function
    Optimizer * init_optimizer(FileHandler *handle);
private:
    uint32_t m_num_nodes;
    uint32_t m_num_links;
    uint32_t m_num_topology;
    bool m_debug;
    std::vector<Topology*> m_topologies;
    bool m_unique_rings_check;
public:
    const std::vector<Topology *> &getMTopologies() const;

    [[maybe_unused]] Optimizer* m_opt_ptr;
private:
    std::vector<std::vector<int>> m_unique_rings;
};
#endif //GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
