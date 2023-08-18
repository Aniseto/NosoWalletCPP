#!/bin/bash

GENERATE_PRESET=ninja-multi-vcpkg
BUILD_PRESET=ninja-vcpkg-release
BUILDS=builds/ninja-multi-vcpkg

if [ $1 == '--clean' ]
then
  echo "#####"
  echo "# Cleaning builds folder"
  echo "#####"
  if [ -d ${BUILDS} ]
  then
    rm -r ${BUILDS}
  else
    echo
    echo "Nothing to clean"
  fi
fi

echo "#####"
echo "# Creating CMake build files"
echo "#####"
cmake --preset ${GENERATE_PRESET}

if [ $? -eq 0 ]
then
  echo
  echo "#####"
  echo "# Building NosoWalletCPP"
  echo "#####"
  cmake --build --preset ${BUILD_PRESET}
fi
