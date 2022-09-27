#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>

#include "command.h"

namespace CPlus {
class CompilerCommand : public ICommand {
   public:
    CompilerCommand(const YAML::Node& config);
    ~CompilerCommand() {}
    void run();
    std::string name() const { return "compile"; };

   private:
    const std::string compilerOptions(const std::string& packageName) const;
    void executeCompiler(const std::filesystem::path& file);
    void linkOutput() const;

    YAML::Node config;
    std::list<std::filesystem::path> objects;
};

}  // namespace CPlus