//
// Created by Mayank Parasar on 11/9/20.
//

#include "file_handler.h"
using namespace  std;

FileHandler::FileHandler(bool mDebug)
        : MaxHandles(0), CurrentHandles(0), Handles(nullptr) {
    m_debug = mDebug;
}

FileHandler::FileHandler(unsigned int handles, bool mDebug)
        : MaxHandles(handles), CurrentHandles(0),
        Handles(new std::fstream[handles]), m_debug(mDebug) {}

FileHandler::~FileHandler() { delete[] Handles; }

void FileHandler::OpenHandle(const char* file, std::ios_base::openmode mode) {
    if(CurrentHandles < MaxHandles) { // Check if any handles available
        for(unsigned int i = 0; i < MaxHandles; i++) {
            if(GetHandle(i).is_open()) continue; // Find next free handle
            else {
                Handles[i].open(file, mode);
                char buffer[PATH_MAX];
                if (getcwd(buffer, sizeof(buffer)) != nullptr) {
                    if (m_debug) {
                        printf("Current working directory : %s\n", buffer);
                    }
                } else {
                    perror("getcwd() error");
                }
                if(Handles[i].is_open()) {  // Check if successful
                    if(m_debug) {
                        std::cout << "Success: " << file << " opened\n";
                    }
                    CurrentHandles++;
                }
                else std::cout << "Error: " << file << " could not be opened\n";
                return;
            }
        }
    }
    else {
        // POSSIBLE FEATURE - Expand amount of handles
        std::cout << "Error: All handles occupied\n";
    }
}

void FileHandler::CloseHandle(unsigned int id) {
    if(id < MaxHandles) { // Check if out of bounds
        if(Handles[id].is_open()) {// Check if handle is valid
            Handles[id].close();
            std::cout << "Handle " << id << " closed\n";
            CurrentHandles--;
            return;
        }
        else {
            std::cout << "Invalid closure of handle " << id << "\n";
            return;
        }
    }
    std::cout << "Error: Unknown handle ID " << id << "\n";
}
