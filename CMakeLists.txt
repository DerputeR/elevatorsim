cmake_minimum_required(VERSION 3.15...3.31)

project(ElevatorSimProject  VERSION 0.1.0
                            DESCRIPTION "Simulates a single-carriage elevator system"
                            LANGUAGES CXX)

# * Linking SDL instructions are found in SDL/docs/README-cmake.md

# Create an option to switch between a system sdl library and a vendored SDL library
# If this variable exists in the cache, its value will not be overwritten
option(MYGAME_VENDORED "Use vendored libraries" OFF)
set(CMAKE_PREFIX_PATH "/usr/local" CACHE PATH "Custom prefix paths to search for libraries under")

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Create your executable target as usual
add_executable(ElevatorSim src/elevator.cpp)

# Link to the actual SDL3 library.
target_link_libraries(ElevatorSim PRIVATE SDL3::SDL3)
target_compile_features(ElevatorSim PUBLIC cxx_std_17)
set_target_properties(ElevatorSim PROPERTIES CXX_EXTENSIONS OFF)



