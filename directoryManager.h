#ifndef DIRECTORYMANAGER_H
#define DIRECTORYMANAGER_H

#include <string>

class DirectoryManager {
public:
    // Constructor
    DirectoryManager(const std::string& dirName);

    // method create directory
    bool createDirectory() const;

private:
    std::string dirName;  // directory name
};

#endif
