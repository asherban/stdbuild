#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>

#include "command.h"
#include "git.h"

namespace CPlus {

class InstallCommand : public ICommand
{
  public:
    InstallCommand(const YAML::Node& config);
    ~InstallCommand() {}
    void run();
    std::string name() const { return "install"; };

  private:
    void installPackage(const std::string& packageName);

    YAML::Node config;
};

} // namespace CPlus