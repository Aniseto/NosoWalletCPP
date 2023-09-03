# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

# Set the project name
project(
  NosoWalletCPP
  VERSION 0.1
  HOMEPAGE_URL "https://github.com/Aniseto/NosoWalletCPP"
)

# Add the executable
add_executable(NosoWalletCPP)
target_sources(NosoWalletCPP PRIVATE
  App.cpp
  DataStructures.cpp
  NosoWalletCPP.cpp
)

# Main function correction for MSVC or WIN32
if (MSVC)
  target_link_options (NosoWalletCPP PRIVATE "/ENTRY:WinMainCRTStartup")
endif ()

# Add the dependency packets
## Find required packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## Boost
find_package(Boost REQUIRED)
target_link_libraries(NosoWalletCPP PRIVATE Boost::boost)

## Botan
find_package(Botan REQUIRED)
target_link_libraries(NosoWalletCPP PRIVATE Botan::Botan)

## WxWidgets
find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(NosoWalletCPP PRIVATE wx::core wx::base)

# Set properties
set_property(TARGET NosoWalletCPP PROPERTY CXX_STANDARD 20)
