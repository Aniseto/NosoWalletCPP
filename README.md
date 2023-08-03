# NosoWalletCPP

c++ Based Noso Wallet

-IDE: Visual Studio 2022 Community Edition

Libraries:

-WxWidgets(https://www.wxwidgets.org/)
-Boost(https://www.boost.org/)
-CryptoPP(https://www.cryptopp.com/)

Follow how to install those libraries from respective Websites, if using Visual Studio then you can use VCPKG(https://vcpkg.io/en/) package manager to compile and install libraries.

Remember to add respective Headers and Libs on your Visual studio solution Properties.

## Build with CMake

We are providing the `CMakeLists.txt` file to allow you to build this project with CMake

### Building under Linux

In order to compile this project under Linux you'll need to have the above libraries installed in your system

**For Ubuntu**

```console
$ sudo apt install -y libboost-all-dev libcrypto++-dev libwxgtk3.2-dev
```

We are also providing the file `build_with_cmake.sh` as a quick way of invoking all the necessary commands in order to build with CMake.
