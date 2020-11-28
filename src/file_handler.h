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
    bool m_debug;
    // This is a vector of matrices
    // It is good to avoid creating pointer to vector as
    // in vector implementation memory is allocated on heap
    // This data-structure will be populated by reading from file
    std::vector<std::vector<std::vector<int>>> m_appl_mat;
    std::vector<int> mat_size;
    std::vector<string> application_names;

public:
    void setMApplMat(const std::vector<std::vector<int>> &mApplMat);
    void setApplicationName(const string &applicationName);
    const vector<std::vector<std::vector<int>>> &getMApplMat() const;
    const vector<string> &getApplicationName() const;
    void setMatSize(const int &matSize);
    const vector<int> &getMatSize() const;

    // initializes that
    // a new object is created with applicaiton name and
    // it should contain the traffic for each application..
    std::fstream& GetHandle(unsigned int id) { return Handles[id]; }
    FileHandler(bool mDebug);
    FileHandler(unsigned int handles, bool mDebug);
    ~FileHandler();

    void OpenHandle(const char* file, std::ios_base::openmode mode);
    void CloseHandle(unsigned int id);
    // class-TopologyUniverse access member function of FileHandler
    friend class TopologyUniverse;
};


#endif //GENERIC_TOPO_GEN_FILE_HANDLER_H
