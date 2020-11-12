//
// Created by Mayank Parasar on 11/9/20.
//

#ifndef GENERIC_TOPO_GEN_FILE_HANDLER_H
#define GENERIC_TOPO_GEN_FILE_HANDLER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>

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
    // This is a vector of matrices
    // It is good to avoid creating pointer to vector as
    // in vector implementation memory is allocated on heap
    // This data-structure will be populated by reading from file
    std::vector<std::vector<std::vector<int>>> appl_matrix;
    std::vector<string> application_name;
    std::vector<int> mat_size;
    FileHandler();
    FileHandler(unsigned int handles);
    ~FileHandler();

    void OpenHandle(const char* file, std::ios_base::openmode mode);
    void CloseHandle(unsigned int id);
};


#endif //GENERIC_TOPO_GEN_FILE_HANDLER_H
