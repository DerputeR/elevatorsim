# Dependencies

- SDL release-3.2.16 (fetched)
- Dear ImGui 1.91.9b-docking (fetched)
- glad (included)
- GLM 1.0.1 (fetched)

# Supported systems

This project has been verified to build on:

- Windows 11 (x64)
- Ubuntu 24.04 LTS (amd64, arm64)

This project _should_ work on a variety of other systems, but this has not been tested yet.

# Installation

This project uses CMake to build and install.

## Windows

I recommend using Ninja as the build system and MSVC 2022 x64 as the compiler, but it should work fine with other build systems and compilers.

### To compile using MSVC and Ninja

1. Download and install CMake 3.12 or above.
2. Download and install [Ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages)
3. Download and install [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false)
    - Ensure that the "Desktop development with C++" workload is selected with your installation.
4. Open a Visual Studio Developer terminal instance in the root directory of the project (here, using the VS Developer Powershell profile added to Windows Terminal) and run the following:

    ```powershell
    mkdir build &&
    cd build &&
    cmake -G Ninja .. &&
    cmake --build .
    ```

From here, you can run `ElevatorSim.exe`.

### To generate a Visual Studio 17 2022 solution

Modern versions of Visual Studio actually enable direct integration with CMake projects. If you follow steps 1 and 3 from the previous section, you can open the project root directory as a folder in Visual Studio, and the IDE will recognize the project as a CMake project and let you get started right away.

However, if you prefer to work with the older solution-based approach:

1. Follow steps 1 and 3 from the previous section
2. Open a terminal in the root directory of the project and run the following:

   ```powershell
   mkdir build &&
   cd build &&
   cmake -G "Visual Studio 17 2022" ..
   ```
3. Open `ElevatorSimProject.sln` in Visual Studio Community Edition.

## Linux

This process is similar to the Windows build process.

1. Download and setup CMake 3.21 or above (see [here](https://www.linuxfordevices.com/tutorials/linux/install-cmake-on-linux) for a guide)
2. (Optional) Download and setup [Ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).
3. Open a terminal at the root directory of the project and run the following:

    ```bash
    mkdir build &&
    cd build &&
    cmake -G Ninja .. &&
    cmake --build .
    ```

When all of the above are complete, you can run `ElevatorSim`.
