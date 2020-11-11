//
// Created by Mayank Parasar on 11/9/20.
//

#ifndef GENERIC_TOPO_GEN_FILE_HANDLER_H
#define GENERIC_TOPO_GEN_FILE_HANDLER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>

/* [1]: This code is for reference only:
#include <iostream>
#include <vector>

std::vector<std::vector<std::vector<int>>*> mat_ptr;

int main(int argc, char* argv[]) {

    for(int mat_id = 2; mat_id < argc; mat_id++)
    {
        std::vector<std::vector<int>>* tmp;
        tmp = new std::vector<std::vector<int>> (atoi(argv[mat_id]),
                std::vector<int> (atoi(argv[mat_id]), atoi(argv[mat_id])));

        mat_ptr.push_back(tmp);
    }

    std::cout << "Printing the Matrices.." << std::endl;
    std::cout << "Number of Matrices populated: " << mat_ptr.size() <<
    std::endl;
    for (int mat_id = 0; mat_id < mat_ptr.size(); ++mat_id) {
        std::cout << "Printing Matrix: " << mat_id << std::endl;
        for(auto i : *mat_ptr[mat_id]) {
            for(auto k : i) {
                std::cout << k << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
 * */


using namespace  std;
// THis file contains all the data structure related to
// reading the file.
class FileHandler {
private:
    unsigned int MaxHandles;
    unsigned int CurrentHandles;
    std::fstream *Handles;
public:
    // initializes that
    // a new object is created with applicaiton name and
    // it should contain the traffic for each application..
    std::fstream& GetHandle(unsigned int id) { return Handles[id]; }
    // This is a vector of pointers to matrices of different
    // shape and sizes. Refer to the code in comment for usage[1].
    // This datavstructure will be populated by reading from file
    std::vector<std::vector<std::vector<int>>*> mat_ptr;
    std::vector<string> application_name;
    std::vector<int> mat_size;
    FileHandler();
    FileHandler(unsigned int handles);
    ~FileHandler();

    void OpenHandle(const char* file, std::ios_base::openmode mode);
    void CloseHandle(unsigned int id);
};


#endif //GENERIC_TOPO_GEN_FILE_HANDLER_H
