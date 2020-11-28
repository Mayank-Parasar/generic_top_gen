//
// Created by Mayank Parasar on 11/26/20.
//

#ifndef GENERIC_TOPO_GEN_OPTIMIZER_H
#define GENERIC_TOPO_GEN_OPTIMIZER_H
#include <iostream>
#include <vector>
#include <deque>
#include <utility>

using namespace  std;
class [[maybe_unused]] Optimizer{
public:
    Optimizer(const vector<std::vector<std::vector<int>>> &applMatrix);

    // this member contains sorted connections by weight, after
    // reading from file
    deque<pair<double/*weight*/, pair<int/*src*/,int /*dest*/>>>
            sorted_weighted_connections;

    // This is the application matrix
    std::vector<std::vector<std::vector<int>>> m_appl_matrix;

    // Generate a traffic %age matrix
    std::vector<std::vector<std::vector<int>>> traffic_percent_mat;
    // class topology may access the
    // member functions of Optimizer
    friend class Topology;
};

#endif //GENERIC_TOPO_GEN_OPTIMIZER_H
