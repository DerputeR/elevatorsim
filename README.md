# Dependencies

- SDL 3.1.6 (fetched)
- Dear ImGui 1.91.6-docking (source included)
- GLFW 3.8 (fetched)
- GLM 1.0.1 (fetched)

# Installation

This project uses CMake to build and install.

## Windows

I personally use Ninja as the build system and MSVC 2022 x64 as the compiler, but it should work fine with other build systems and compilers.

To compile using MSVC and Ninja:

1. Download and install CMake 3.12 or above.
2. Download and install [Ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages)
3. Download and install [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false)
    - Ensure that the "Desktop development with C++" workload is selected with your installation.
4. Open a Visual Studio Developer terminal instance in the root directory of the project (here, using the VS Developer Powershell profile added to Windows Terminal) and run the following:

    ```powershell
    mkdir build
    cd build
    cmake -G Ninja ..
    cmake --build .
    cmake --install . --prefix dist
    ```

When all of the above are complete, cd to `dist/bin`. From here, you can run `ElevatorSim.exe`.

Note that the line with `cmake --install` is done for convenience. If you choose to skip this step, you will need to locate `SDL3.dll` (found in `_deps/sdl3-build`) and copy it to the build directory to be adjacent to `ElevatorSim.exe`. If you choose to omit the custom prefix, you will need to run the command in an elevated terminal since the default installation prefix is `C:\Program Files (x86)\ElevatorSim`.

## Linux

GLFW includes Wayland and X11 backends which are enabled by default. These defaults have not been altered in this project. As such, ensure your system has the correct packages installed. See the [GLFW docs](https://www.glfw.org/docs/latest/compile.html) for more detailed info.

For Ubuntu/Debian:

```bash
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```

The remaining process is similar to the Windows build process.

1. Download and setup CMake 3.21 or above (see [here](https://www.linuxfordevices.com/tutorials/linux/install-cmake-on-linux) for a guide)
2. (Optional) Download and setup [Ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).
3. Open a terminal at the root directory of the project and run the following:

    ```bash
    mkdir build
    cd build
    cmake ..
    # cmake -G Ninja .. # Use this if you want to use Ninja
    cmake --build .
    ```

When all of the above are complete, you can run `ElevatorSim`.
Note that unlike the Windows installation, this does not require the installation step. If you do wish to install, note that this will cause RPATH stripping in the final executable, meaning the app will search the default shared library paths (typically `/usr/local/lib` or `/usr/lib`). This means you will need to have installed the SDL3 shared library to your system.

It is possible to change how this behaves in `CMakeList.txt`. The likely candidate for this is around the `MYGAME_VENDORED` option, which could either be changed or extended with another option to change whether or not the installed library should prefer to first look at a specific directory, then look at system defaults if not found. For more info, see this [CMake wiki article](https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling).
