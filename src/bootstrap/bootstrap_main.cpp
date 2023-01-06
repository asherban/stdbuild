#include <yaml-cpp/yaml.h>

#include "compiler_command.h"

int main() {
    YAML::Node config = YAML::LoadFile("cplus.yaml");

    CPlus::CompilerCommand cmd(config);
    cmd.run();

    return 0;
}