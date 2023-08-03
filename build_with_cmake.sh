#!/bin/bash

SOURCE_PATH=.
BUILD_PATH=${SOURCE_PATH}/build

echo "###################################"
echo "# IMPORTANT"
echo "# In order to compile this project you'll need the following libs installed:"
echo "# boost: sudo apt install -y libboost-all-dev"
echo "# cryptopp: sudo apt install -y libcrypto++-dev"
echo "# wxwidgets: sudo apt install -y libwxgtk3.2-dev"
echo "#"
echo "###################################"

echo "Press <ENTER> to continue or <CTRL-C> to abort"
read

if [ ! -d "${BUILD_PATH}" ]
then
  mkdir "${BUILD_PATH}"
fi

echo "#####"
echo "# Creating CMake build files"
echo "#####"
cmake -B "${BUILD_PATH}" -S "${SOURCE_PATH}"

if [ $? -eq 0 ]
then
  echo
  echo "#####"
  echo "# Building NosoWalletCPP"
  echo "#####"
  cmake --build ${BUILD_PATH}
fi
