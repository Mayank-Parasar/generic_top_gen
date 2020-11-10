//
// Created by Mayank Parasar on 11/9/20.
//

#ifndef GENERIC_TOPO_GEN_FILE_HANDLER_H
#define GENERIC_TOPO_GEN_FILE_HANDLER_H
#include <fstream>
#include <iostream>

// THis file contains all the data structure related to
// reading the file.
class FileHandler {
private:
    unsigned int CurrentHandles;
    std::fstream *Handles;
    std::fstream& GetHandle(unsigned int id) { return Handles[id]; }
    // it should contain the traffic for each application..
    // a new object is created with applicaiton name and
    // initializes that
};


#endif //GENERIC_TOPO_GEN_FILE_HANDLER_H
