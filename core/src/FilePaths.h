#include <string>
#pragma once
std::string setExecutablePath();

//Paths are relative to the executable path.
class FilePaths {
    public:
        //Paths are from the executable path. By defualt, this assumes the executable is in the build/app directory.
        std::string srcPath = "../../app/src";
        std::string shadersPath = "../../app/shaders";
        std::string fontsPath = "../../app/fonts";
        std::string corePath = "../../core";
        const std::string executablePath = setExecutablePath();
};
