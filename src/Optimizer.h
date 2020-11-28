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
class Optimizer{
public:
    Optimizer(const vector<std::vector<std::vector<int>>> &applMatrix);

    // this member contains sorted connections by weight, after
    // reading from file
    deque<pair<double/*weight*/, pair<int/*src*/,int /*dest*/>>>
            sorted_weighted_connections;

    // This is the application matrix
    std::vector<std::vector<std::vector<int>>> m_appl_matrix;

    // Generate a traffic %age matrix
    std::vector<std::vector<std::vector<double>>> m_traffic_percent_mat;

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
};

#endif //GENERIC_TOPO_GEN_OPTIMIZER_H
