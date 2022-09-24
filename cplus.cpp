#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <termcolor/termcolor.hpp>

void installPackage(const std::string& packageName) {
    YAML::Node config = YAML::LoadFile("packages/" + packageName + ".yaml");

    auto path = std::filesystem::current_path();
    auto packageBaseDir = path / "vendor";
    auto packagePath = packageBaseDir / packageName;

    if (std::filesystem::exists(packagePath)) {
        return;
    }

    std::filesystem::create_directories(packageBaseDir);
    std::filesystem::current_path(packageBaseDir);

    std::string gitCommand = "git clone --depth=1 --branch=" + config["version"].as<std::string>() + " " + config["repo"].as<std::string>() + " " + packageName;
    std::system(gitCommand.c_str());

    std::filesystem::current_path(packagePath);

    if (config["build"]) {
        std::system(config["build"].as<std::string>().c_str());
    }

    std::filesystem::current_path(path);
}

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

void executeCompiler(const YAML::Node& config, const std::string& file) {
    std::stringstream oss;
    oss << "g++ --std=c++17 " << file << " ";
    for (auto it : config["dependencies"]) {
        std::string packageName = it.as<std::string>();
        std::string opts = compilerOptions(packageName);
        oss << opts << " ";
    }
    oss << "--static ";
    oss << "-o bin/" << config["name"];
    std::string cmd = oss.str();

    std::system(cmd.c_str());
}

int main(int /*argc*/, char** /*argv*/) {
    std::cout << termcolor::red << "Hello, ";                 // 16 colors
    std::cout << termcolor::color<100> << "Colorful ";        // 256 colors
    std::cout << termcolor::color<211, 54, 130> << "World!";  // true colors
    std::cout << std::endl;

    YAML::Node config = YAML::LoadFile("cplus.yaml");

    for (auto pkg : config["dependencies"]) {
        installPackage(pkg.as<std::string>());
    }

    std::list<std::string> files = {"cplus.cpp"};
    for (auto& file : files) {
        executeCompiler(config, file);
    }

    return 0;
}