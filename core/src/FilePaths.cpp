#include "FilePaths.h"
#include <whereami.h>
#include <string>
#include <iostream>
#include <filesystem>

std::string setExecutablePath() {
    int dirname_length;
    int length = wai_getExecutablePath(NULL, 0, NULL);
    auto path = (char*)malloc(length + 1);
    wai_getExecutablePath(path, length, &dirname_length);
    path[length] = '\0';
    std::filesystem::path fPath = path;
    std::cout << "Executable path: " << fPath << std::endl;
    fPath = fPath.parent_path();
    std::cout << "Executable folder: " << fPath << std::endl;
    return fPath;
}
