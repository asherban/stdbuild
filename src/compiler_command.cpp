#include "compiler_command.h"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace CPlus {

CompilerCommand::CompilerCommand(const YAML::Node& _config)
  : config(_config)
  , objects()
{
}

const std::string
CompilerCommand::compilerOptions(const std::string& packageName) const
{
    YAML::Node config = YAML::LoadFile("packages/" + packageName + ".yaml");
    std::string baseDir = "vendor/" + packageName + "/";

    std::stringstream oss;
    if (config["headers"]) {
        oss << "-I" << baseDir << config["headers"] << " ";
    }

    return oss.str();
}

void
CompilerCommand::executeCompiler(const fs::path& file)
{
    std::stringstream oss;
    fs::path buildDirectory = config["build_directory"].as<std::string>();

    oss << "g++ --std=c++17 " << file << " ";
    for (auto it : config["dependencies"]) {
        std::string packageName = it.as<std::string>();
        std::string opts = compilerOptions(packageName);
        oss << opts << " ";
    }

    oss << "-I" << config["include"] << " ";

    fs::path outputObject =
      buildDirectory / file.filename().replace_extension(".o");

    objects.push_back(outputObject);

    oss << "-c ";
    oss << "-o " << outputObject;
    std::string cmd = oss.str();

    std::cout << "Compiling " << file << ": " << cmd << std::endl;
    std::system(cmd.c_str());
}

void
CompilerCommand::linkOutput() const
{
    std::stringstream oss;

    oss << "g++ ";

    for (auto obj : objects) {
        oss << obj << " ";
    }

    for (auto dep : config["dependencies"]) {
        YAML::Node depConfig =
          YAML::LoadFile("packages/" + dep.as<std::string>() + ".yaml");

        if (depConfig["libs"]) {
            fs::path libDir = fs::path("vendor") / dep.as<std::string>() /
                              depConfig["libs"].as<std::string>();
            oss << "-L" << libDir << " ";
        }

        for (auto lib : depConfig["link"]) {
            oss << "-l" << lib << " ";
        }
    }

    fs::path buildDirectory = config["build_directory"].as<std::string>();
    fs::path outputFile = fs::path(config["name"].as<std::string>());
    oss << " --static -o " << buildDirectory / outputFile;

    std::string cmd = oss.str();
    std::cout << "Linking : " << cmd << std::endl;
    std::system(cmd.c_str());
}

void
CompilerCommand::run()
{
    for (auto source : config["sources"]) {
        executeCompiler(source.as<std::string>());
    }

    linkOutput();
}

} // namespace CPlus