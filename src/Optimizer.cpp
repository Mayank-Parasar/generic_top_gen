//
// Created by Mayank Parasar on 11/26/20.
//

#include "Optimizer.h"


Optimizer::Optimizer(
        const std::vector<std::vector<std::vector<int>>> &applMatrix)
        : m_appl_matrix(applMatrix) {
        // initialize 'm_traffic_percent_mat' in the ctor

        uint64_t row_sum;
        vector<double> normalized_row;
        vector<vector<double>> normalized_matrix;

    for (int app_itr = 0; app_itr < applMatrix.size(); ++app_itr) {
        for (int row_ = 0; row_ < applMatrix[app_itr].size(); ++row_) {
            // sum the row here
            row_sum = 0;
            for (int col_ = 0; col_ < applMatrix[app_itr][row_].size();
                ++col_) {
                row_sum += applMatrix[app_itr][row_][col_];
            }
            // now divide the whole row with row-sum
            for (int col_ = 0; col_ < applMatrix[app_itr][row_].size();
                 ++col_) {
                if (row_sum != 0) { // to avoid divide by 0 error
                    normalized_row.push_back(
                            (double) applMatrix[app_itr][row_][col_] / (double)
                                    row_sum);
                }
                else{
                    normalized_row.push_back(0);
                }
            }
            // push it into the normalized_matrix
            normalized_matrix.push_back(normalized_row);
            normalized_row.clear();
        }
        // push this normalized_matrix into m_traffic_percent_mat
        m_traffic_percent_mat.push_back(normalized_matrix);
        // clear it
        normalized_matrix.clear();
    }

    for (int i = 0; i < m_traffic_percent_mat.size(); ++i) {
        cout << "Application Matrix: " << i << endl;
        print_matrix(m_appl_matrix[i]);
        cout << "Traffic percentage Matrix: " << i << endl;
        print_matrix((m_traffic_percent_mat[i]));
    }

}