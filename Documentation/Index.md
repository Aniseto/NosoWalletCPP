## NOSO Dev Documentation
### Version 1.0
#### -Author: Estripa 
#### -Date: 2023-11-21
#### -Status: Draft
##### -Email: nosowalletcpp@outlook.com


## Introduction
This document is intended to be a guide for developers who want to contribute to the NOSO project. It will cover the following topics:
- How to build the NOSO wallet from source code.
- Noso wallet architecture.
- Function definitions.
- How to Test
- How to contribute to the project.
    - Create a new branch.
    - Create a new pull request.
    - create a new issue.
- How to contact the NOSO team.


## How to build the NOSO wallet from source code.
### Windows
#### Requirements
- Visual Studio 2022 Community Edition
- VCPKG (https://vcpkg.io/en/)
    - Package Manager, recommended to install libraries

- Required Libraries:
    - Boost (https://www.boost.org/)
    - Botan (https://botan.randombit.net/)
    - WxWidgets (https://www.wxwidgets.org/)
- Follow how to install those libraries from respective Websites, if using Visual Studio then you can use VCPKG package manager to compile and install libraries.
- Remember to add respective Headers and Libs on your Visual studio solution Properties.
#### Build
- Open Visual Studio
- Open NosoWalletCPP.sln
- Select Release x64
- Build Solution
- The executable file will be located in the folder: NosoWalletCPP\x64\Release
### Linux
#### Requirements
- CMake
- VCPKG (https://vcpkg.io/en/)
    - Package Manager, recommended to install libraries
- Required Libraries:
    - Boost (https://www.boost.org/)
    - Botan (https://botan.randombit.net/)
    - WxWidgets (https://www.wxwidgets.org/)
- Follow how to install those libraries from respective Websites, if using Visual Studio then you can use VCPKG package manager to compile and install libraries.
#### Build
- Open terminal
- Go to the folder: NosoWalletCPP
- Run the following commands:
    - mkdir build
    - cd build
    - cmake ..
    - make
- The executable file will be located in the folder: NosoWalletCPP/bin
### macOS
#### Requirements
- CMake
- VCPKG (https://vcpkg.io/en/)
    - Package Manager, recommended to install libraries
- Required Libraries:
    - Boost (https://www.boost.org/)
    - Botan (https://botan.randombit.net/)
    - WxWidgets (https://www.wxwidgets.org/)
- Follow how to install those libraries from respective Websites, if using Visual Studio then you can use VCPKG package manager to compile and install libraries.
#### Build
- Open terminal
- Go to the folder: NosoWalletCPP
- Run the following commands:
    - mkdir build
    - cd build
    - cmake ..
    - make
- The executable file will be located in the folder: NosoWalletCPP/bin

## Noso wallet architecture.
The Noso wallet is a desktop application developed in C++ using the wxWidgets library for the graphical user interface. The Noso wallet is a multiplatform application.


##NosoWalletCPP Function Definition

### NosoWalletCPPApp Class
#### NosoWalletCPPApp::OnInit()
- Initializes the application.

    


