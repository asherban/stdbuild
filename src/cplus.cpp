#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <filesystem>

#include "git.h"
#include "compiler_command.h"
#include "install_command.h"

int main() {
    YAML::Node config = YAML::LoadFile("cplus.yaml");

    CPlus::InstallCommand installCommand(config);
    installCommand.run();
    
    CPlus::CompilerCommand buildCommand(config);
    buildCommand.run();

    return 0;
}