//
// Created by Mayank Parasar on 11/26/20.
//

#ifndef GENERIC_TOPO_GEN_OPTIMIZER_H
#define GENERIC_TOPO_GEN_OPTIMIZER_H
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <algorithm>

using namespace  std;

struct elements {
    int src_id_;
    int dest_id_;
    double weight_;

    elements(int rowId, int colId, double weight)
        : src_id_(rowId), dest_id_(colId), weight_(weight)
    { }

    elements() : src_id_(-1), dest_id_(-1), weight_(-0.1)
    { }

    bool operator<(const elements &rhs) const {
        return weight_ < rhs.weight_;
    }


};


class Optimizer{
public:
    Optimizer(const vector<std::vector<std::vector<int>>> &applMatrix);

    // this member contains sorted connections by weight, after
    // reading from file
    // this is a vector. One per each application
    /*deque<pair<double*//*weight*//*, pair<int*//*src*//*,int *//*dest*//*>>>
            sorted_weighted_connections;*/
    vector<deque<elements*>> sorted_weighted_connection;

    void print_sorted_weighted_connection();

    // This is the application matrix
    std::vector<std::vector<std::vector<int>>> m_appl_matrix;

    // Generate a traffic %age matrix
    std::vector<std::vector<std::vector<double>>> m_traffic_percent_mat;

    // populate vector_deque_elements
    void populate_vector_deque_elements();
    template<typename T>
    void printVector(const T& t) {
        std::copy(t.cbegin(), t.cend(),
                  std::ostream_iterator<typename T::value_type>(std::cout, ", "));
    }

//    template<typename T>
//    void printVectorInVector(const T& t) {
//        std::for_each(t.cbegin(), t.cend(),
//                      printVector<typename T::value_type>);
//        cout << endl;
//    }

    template<typename T>
    void print_vector(const T& t) {
        for (int i = 0; i < t.size(); ++i) {
            cout << t[i];
        }
    }

    template<typename T>
    void print_matrix(const T& t) {
        for (int row_ = 0; row_ < t.size(); ++row_) {
            for (int col_ = 0; col_ < t[row_].size(); ++col_) {
                cout << t[row_][col_] << "\t";
            }
            cout << endl;
//            cout << "sum of the row: " << vectorSum(t[row_]);
//            cout << endl;
        }
    }

    template<typename T>
    double vectorSum(const T t) {
        double sum = 0;
        for(auto i : t) {
            sum += i;
        }
        return sum;
    }

    static bool comparePtrElem(elements* a, elements* b) {
        return (a->weight_ > b->weight_);
    }
};

#endif //GENERIC_TOPO_GEN_OPTIMIZER_H
