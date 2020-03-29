#!/bin/bash

set -e
set -x

pip install conan --upgrade
pip install conan_package_tools

conan user

conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
