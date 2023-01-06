#include "git.h"
#include "git2/repository.h"
#include "git2/clone.h"

void gitClone(const std::string& repo, const std::string& version, const std::string& targetDirectory)
{
    git_repository* repoObject = nullptr;
    git_clone_options options = GIT_CLONE_OPTIONS_INIT;

    git_clone(&repoObject, repo.c_str(), targetDirectory.c_str(), &options);
}