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

    populate_vector_deque_elements();

}

void Optimizer::populate_vector_deque_elements() {
    // traverse through the m_traffic_percent_mat
    for (int app_id = 0; app_id < m_traffic_percent_mat.size(); ++app_id) {
        deque<elements*> tmp;
        for (int row_ = 0; row_ < m_traffic_percent_mat[app_id].size(); ++row_) {
            for (int col_ = 0;
                 col_ < m_traffic_percent_mat[app_id][row_].size(); ++col_) {
                elements* elem = new elements(row_, col_,
                                              m_traffic_percent_mat[app_id][row_][col_]);
                tmp.push_back(elem);  // row-major form
            }
        }
        // sort tmp (in desending order)
        // Sort
        sort(tmp.begin(), tmp.end(), comparePtrElem);
        sorted_weighted_connection.push_back(tmp);
        tmp.clear();
    }
    // print_sorted_weighted_connection();
    return;
}

void Optimizer::print_sorted_weighted_connection() {

    for (int app_id = 0; app_id < sorted_weighted_connection.size(); ++app_id) {
        cout << "Sorted weight for application-id: \t" << app_id << endl;
        cout << "src-id: \t\t dest-id: \t\t  weight:" <<endl;
        for (int id_ = 0;
             id_ < sorted_weighted_connection[app_id].size(); ++id_) {
            cout << "\t" << sorted_weighted_connection[app_id][id_]->src_id_ <<
            "\t\t\t" << sorted_weighted_connection[app_id][id_]->dest_id_ <<
            "\t\t\t\t" << sorted_weighted_connection[app_id][id_]->weight_ <<
            endl;
        }
    }

}


