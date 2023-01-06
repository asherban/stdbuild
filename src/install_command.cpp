
#include "install_command.h"

#include <string>
#include <iostream>

namespace CPlus {
InstallCommand::InstallCommand(const YAML::Node& _config)
  : config(_config)
{
}

void
InstallCommand::run()
{
    std::cout << "Running Install:" << std::endl;
    for (auto dep: config["dependencies"]) {
        installPackage(dep.as<std::string>());
    }
}

void
InstallCommand::installPackage(const std::string& packageName)
{
    YAML::Node packageConfig = YAML::LoadFile("packages/" + packageName + ".yaml");

    auto path = std::filesystem::current_path();
    auto packageBaseDir = path / "vendor";
    auto packagePath = packageBaseDir / packageName;

    if (std::filesystem::exists(packagePath)) {
        std::cout << "Package " << packageName << " exists - skipping" << std::endl;
        return;
    }

    std::filesystem::create_directories(packageBaseDir);
    std::filesystem::current_path(packageBaseDir);

    gitClone(packageConfig["repo"].as<std::string>(),
             packageConfig["version"].as<std::string>(),
             packageName);

    std::filesystem::current_path(packagePath);

    if (packageConfig["build"]) {
        std::system(packageConfig["build"].as<std::string>().c_str());
    }

    std::filesystem::current_path(path);
}

}