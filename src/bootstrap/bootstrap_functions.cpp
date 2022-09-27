#include <cstdlib>
#include <string>

#include "../../include/git.h"

void gitClone(const std::string& repo, const std::string& version, const std::string& directory) {
    std::string gitCommand = "git clone --depth=1 --branch=" + version + " " + repo + " " + directory;
    std::system(gitCommand.c_str());
}
