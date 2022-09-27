#pragma once

#include <string>
#include <yaml-cpp/yaml.h>


namespace CPlus {

class ICommand {
   public:
    virtual ~ICommand(){};
    virtual void run() = 0;
    virtual std::string name() const = 0;
};

}  // namespace CPlus