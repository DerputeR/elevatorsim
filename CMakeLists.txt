cmake_minimum_required(VERSION 3.21...3.31)

project(ElevatorSimProject  VERSION 0.1.0
                            DESCRIPTION "Simulates a single-carriage elevator system"
                            LANGUAGES CXX)

configure_file(src/app_info.h.in ${CMAKE_SOURCE_DIR}/src/app_info.h @ONLY)

# Dependency management, excluding SDL3 (special case handled below)
include(FetchContent)
FetchContent_Declare(
    glfw            
    GIT_REPOSITORY  https://github.com/glfw/glfw
    GIT_TAG         3.4
)
FetchContent_Declare(
    glm            
    GIT_REPOSITORY  https://github.com/g-truc/glm
    GIT_TAG         1.0.1
)

FetchContent_MakeAvailable(glfw glm)


# * Linking SDL instructions are found in SDL/docs/README-cmake.md

# Create an option to switch between a system sdl library and a vendored SDL library
# If this variable exists in the cache, its value will not be overwritten
option(MYGAME_VENDORED "Use vendored libraries" OFF)
set(CMAKE_PREFIX_PATH "/usr/local" CACHE PATH "Custom prefix paths to search for libraries under")

# GLFW sets the variable BUILD_SHARED_LIBS to false before we hit SDL,
# so we need to manually enable SDL_SHARED before we aquire SDL
option(SDL_SHARED "Build a SDL shared library (if available)" ON)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY  https://github.com/libsdl-org/SDL
        GIT_TAG         prerelease-3.1.10
    )
    FetchContent_MakeAvailable(SDL3)
endif()

# Create your executable target as usual
add_executable(ElevatorSim src/elevator.cpp)

# Link to the actual SDL3 library.
target_link_libraries(ElevatorSim PRIVATE SDL3::SDL3)
target_compile_features(ElevatorSim PUBLIC cxx_std_17)
set_target_properties(ElevatorSim PROPERTIES CXX_EXTENSIONS OFF)

# Ensure the library is installed with the executable
if (TARGET SDL3-shared)
    install(TARGETS SDL3-shared
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib/${PROJECT_NAME})
endif()


# Other libraries we can just statically link
target_include_directories(ElevatorSim PRIVATE glfw)
target_include_directories(ElevatorSim PRIVATE glm)

target_link_libraries(ElevatorSim PRIVATE glfw)
target_link_libraries(ElevatorSim PRIVATE glm)

# Ensure the executable is also placed in bin
install(TARGETS ElevatorSim RUNTIME DESTINATION bin)
