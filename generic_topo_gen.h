//
// Created by Mayank Parasar on 11/1/20.
//

#ifndef GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
#define GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H

#include <cstdint>
#include <iostream>
class Topology {
public:
    Topology(uint16_t num_nodes, uint32_t num_links)
    : m_num_nodes(num_nodes), m_num_links(num_links)
    {};
    void create_topology();
    void set_params(int nodes, int links);
    bool is_strongly_connected(Topology* );
    uint16_t m_num_nodes;
    uint16_t m_num_links;

};
#endif //GENERIC_TOPO_GEN_GENERIC_TOPO_GEN_H
