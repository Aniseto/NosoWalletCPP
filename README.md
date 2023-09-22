# NosoWalletCPP
[![Build Status](https://github.com/gcarreno/NosoWalletCPP/workflows/build/badge.svg?branch=master)](https://github.com/gcarreno/NosoWalletCPP/actions)
[![Supports Windows](https://img.shields.io/badge/support-Windows-blue?logo=Windows)](https://github.com/gcarreno/NosoWalletCPP/releases/latest)
[![Supprts Linux](https://img.shields.io/badge/support-Linux-yellow?logo=Linux)](https://github.com/gcarreno/NosoWalletCPP/releases/latest)
[![Supports macOS](https://img.shields.io/badge/support-macOS-black?logo=macOS)](https://github.com/gcarreno/NosoWalletCPP/releases/latest)
[![License](https://img.shields.io/github/license/gcarreno/NosoWalletCPP)](https://github.com/gcarreno/NosoWalletCPP/blob/master/LICENSE)
[![Latest Release](https://img.shields.io/github/v/release/gcarreno/NosoWalletCPP?label=latest%20release)](https://github.com/gcarreno/NosoWalletCPP/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/gcarreno/NosoWalletCPP/total)](https://github.com/gcarreno/NosoWalletCPP/releases)

c++ Based Noso Wallet

-IDE: Visual Studio 2022 Community Edition

Libraries:

- Boost(https://www.boost.org/)
- Botan (https://botan.randombit.net/)
- WxWidgets(https://www.wxwidgets.org/)

Follow how to install those libraries from respective Websites, if using Visual Studio then you can use VCPKG(https://vcpkg.io/en/) package manager to compile and install libraries.

Remember to add respective Headers and Libs on your Visual studio solution Properties.

## Build with `cmake` and `vcpkg`

We are providing the `CMakeLists.txt`, `CMakePresets.txt` and `vcpkg.json` files that will allow you to build this project with `cmake` and `vcpkg`.

### Building under Linux

We are also providing the file `build_with_cmake_vcpkg.sh` as a quick way of invoking all the necessary commands in order to build with `cmake` and `vcpkg` locally.
