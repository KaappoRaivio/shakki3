#!/usr/bin/env zsh

set -e

mkdir tmp
cd tmp || exit
git clone https://github.com/catchorg/Catch2.git
cd Catch2 || exit

cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install