#include "fileReading.h"
#include <string>
#include <fstream>

//File path is from the app root directory. TODO: Make this configurable through config struct.
std::string getFileContents(const char* filePath) {
    std::string contents;
    //From build dir, exit out to app root, then concatenate passed filePath.
    std::ifstream inputStream(filePath);

    std::string line;
    while (getline (inputStream, line)) {
        contents += line + "\n";
    }

    //std::cout << contents << std::endl;

    inputStream.close();

    return contents;
}
