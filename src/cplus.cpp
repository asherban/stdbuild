#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <filesystem>

#include "git.h"
#include "compiler_command.h"

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

    gitClone(config["repo"].as<std::string>(), config["version"].as<std::string>(), packageName);

    std::filesystem::current_path(packagePath);

    if (config["build"]) {
        std::system(config["build"].as<std::string>().c_str());
    }

    std::filesystem::current_path(path);
}

int main() {
    YAML::Node config = YAML::LoadFile("cplus.yaml");

    // for (auto pkg : config["dependencies"]) {
    //     installPackage(pkg.as<std::string>());
    // }

    CPlus::CompilerCommand cmd(config);
    cmd.run();

    return 0;
}