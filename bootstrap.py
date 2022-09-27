#!/usr/bin/env python3

from email.mime import base
import os
import shutil
from os import path
import yaml
from yaml import Loader
from pathlib import Path

def get_dependency(name): 
    config = yaml.load(open(f"packages/{name}.yaml"), Loader=Loader)
    version = config["version"]
    repo = config["repo"]

    base_dir = Path(__file__).resolve().parent
    package_dir = base_dir / "vendor" / name

    if (package_dir.exists()):
        return

    cmd = f"git clone --depth=1 --branch={version} {repo} {package_dir}"
    os.system(cmd)

    os.chdir(package_dir)

    if "build" in config:
        for line in config["build"].split("\n"):
            os.system(line)

    os.chdir(base_dir)

def build():
    sources = [
        "src/cplus.cpp",
        "src/bootstrap/bootstrap_functions.cpp",
        "src/compiler_command.cpp", 
    ]

    compiled_cmd = [
        "g++",
        "--std=c++2a",
        *sources,
        "-Iinclude",
        "-Ivendor/termcolor/include",
        "-Ivendor/yaml-cpp/include", "-Lvendor/yaml-cpp/build", "-lyaml-cpp",
        "--static",
        "-o", "bin/cplus.bootstrap",
    ]

    print(" ".join(compiled_cmd))
    os.system(" ".join(compiled_cmd))


def main():
    os.chdir(path.dirname(__file__))    
    
    if (not os.path.exists("vendor")):
        os.mkdir("vendor")

    get_dependency("yaml-cpp")

    build()

if __name__ == "__main__":
    main()


# set -e

# rm -rf vendor

# ##### termcolor #####
# git clone --depth=1 --branch=master https://github.com/ikalnytskyi/termcolor vendor/termcolor
# rm -rf vendor/termcolor/.git 

# ##### yaml-cpp ######
# git clone --depth=1 --branch=master https://github.com/jbeder/yaml-cpp vendor/yaml-cpp
# rm -rf vendor/yaml-cpp/.git
# mkdir vendor/yaml-cpp/build
# cmake -S vendor/yaml-cpp -B vendor/yaml-cpp/build 
# cmake --build vendor/yaml-cpp/build -j

# g++ \
#     --std=c++2a \
#     cplus.cpp \
#     -Ivendor/termcolor/include \
#     -Ivendor/yaml-cpp/include -Lvendor/yaml-cpp/build -lyaml-cpp \
#     --static \
#     -o bin/cplus.bootstrap

# rm -rf vendor
# ./bin/cplus.bootstrap

# rm -f ./bin/cplus.bootstrap
