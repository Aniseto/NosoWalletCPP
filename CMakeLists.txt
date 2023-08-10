cmake_minimum_required(VERSION 3.20)

# specify the C++ standard
## this should be equal to -std=c++20
SET (CMAKE_CXX_STANDARD             20)
SET (CMAKE_CXX_STANDARD_REQUIRED    True)

# set the project name
project(
  NosoWalletCPP
  VERSION 0.1
  LANGUAGES CXX
  HOMEPAGE_URL "https://github.com/Aniseto/NosoWalletCPP"
)

# Find required packages
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
## WxWidgets
find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

## Botan
find_package(Botan REQUIRED)

# add the executable
add_executable(NosoWalletCPP
  App.cpp
  DataStructures.cpp
  NosoWalletCPP.cpp
)

# add the libs to the linker
target_link_libraries(NosoWalletCPP PRIVATE
  ${wxWidgets_LIBRARIES}
  crypto++
  Botan::Botan
)
