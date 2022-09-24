#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>

void installPackage(const std::string& packageName) {}

const std::string compilerOptions(const std::string& packageName) {
    YAML::Node config = YAML::LoadFile("packages/" + packageName + ".yaml");
    std::string baseDir = "vendor/" + packageName + "/";

    std::stringstream oss;
    if (config["headers"]) {
        oss << "-I" << baseDir << config["headers"] << " ";
    }
    if (config["libs"]) {
        oss << "-L" << baseDir << config["libs"] << " ";
    }

    for (auto it : config["link"]) {
        oss << "-l" << it << " ";
    }
    return oss.str();
}

void executeCompiler(const YAML::Node & config, const std::string& file) {

    std::stringstream oss;
    oss << "g++ " << file << " ";
    for (auto it : config["dependencies"]) {
        std::string packageName = it.as<std::string>();
        std::string opts = compilerOptions(packageName);
        oss << opts << " ";
    }
    oss << "--static ";
    oss << "-o " << config["name"];
    std::string cmd = oss.str();

    std::system(cmd.c_str());
}

int main(int /*argc*/, char** /*argv*/) {
    std::cout << termcolor::red << "Hello, ";                 // 16 colors
    std::cout << termcolor::color<100> << "Colorful ";        // 256 colors
    std::cout << termcolor::color<211, 54, 130> << "World!";  // true colors
    std::cout << std::endl;

    YAML::Node config = YAML::LoadFile("cplus.yaml");
    std::cout << config["name"] << std::endl;

    std::list<std::string> files = { "cplus.cpp" };
    for (auto & file : files) {
         executeCompiler(config, file);
    }


    return 0;
}