# Dependencies

- SDL 3.1.6 (git submodule)
- Dear ImGui 1.91.6-docking (source included)
- GLFW 3.8 (fetched)
- GLM 1.0.1 (fetched)

# Installation

This project uses CMake to build and install.

## Windows

I personally use Ninja as the build system and MSVC 2022 x64 as the compiler, but it should work fine with other build systems and compilers.

1. Follow the build instructions for SDL3, found [here](https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md).
    - Note that for Windows, installing without a custom prefix requires elevated privileges, since the default install directory for shared libraries is `C:\Program Files (x86)\<library-name>`
    - If you choose to install with a custom prefix, you'll need to run CMake with and add the custom prefix to your `CMAKE_PREFIX_PATH` variable.
        - If using the CMake Tools extension in VSCode, add this variable as a key-value pair to the setting `cmake: environment`
2. Open a terminal (here, using VS Developer PowerShell) in the root directory of the repo and run the following: 

    ```Powershell
    mkdir build
    cd build
    cmake ..
    # cmake -G Ninja .. # Use this if you want to use Ninja
    # If you installed SDL under a different prefix, add the `-DCMAKE_PREFIX_PATH=<custom-prefix>` flag to this first cmake command
    cmake --build .
    ```
4. After building SDL, copy the SDL.dll file to the build folder of the project so that it is next to ElevatorSim.exe
3. You should now be able to run ElevatorSim.exe

## Linux

Todo, but likely very similar to Windows instructions, minus the default library install prefix. For this, the SDL docs recommends to set a custom prefix of `/usr/local`.