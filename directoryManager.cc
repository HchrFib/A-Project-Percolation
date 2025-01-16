#include "directoryManager.h"
#include <iostream>
#include <filesystem>

// Constructor that initialzes the directory name
DirectoryManager::DirectoryManager(const std::string& dirName) : dirName(dirName) {}

// method create directory
bool DirectoryManager::createDirectory() const {
    if (std::filesystem::create_directory(dirName)) return true;    // create ok
    else  return false;                                             // create failed
}
